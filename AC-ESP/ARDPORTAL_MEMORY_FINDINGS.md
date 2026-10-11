# ArdPortal: zawieszanie AC-ESP — pomiary 2026-10-08

Biblioteka nie była modyfikowana.

## Potwierdzone
- UART Mega pozostaje połączony; CRC=0, timeout=0, brak resetów ESP.
- Bez HA, ale z MQTT, problem występuje nadal.
- Bez MQTT: 60 kolejnych żądań w pierwszym teście poprawnych. W dłuższym teście pojedyncza odpowiedź LED ucięta, pozostałe nadal obsługiwane przy uptime >220 s.
- Z pełnym HA/MQTT, przy stosie 32 KiB, realna wolna pamięć 8-bit/DMA spada do 2–3 KiB, mimo że ESP.getFreeHeap() pokazuje około 45 KiB i ESP.getMaxAllocHeap() około 43 KiB. Send(MSG_DONTWAIT) zwraca wielokrotnie EAGAIN. Portal następnie przestaje odpowiadać.
- Przy stosie 16 KiB maksymalne zaobserwowane użycie stosu wynosi około 3648 bajtów (high-water mark=12720). Sama redukcja do 16 KiB nie wystarcza podczas testów dużych stron.

## Co poprawić w ArdPortal
1. Na ESP32 kontrolę dostępnej pamięci dla String/JSON/HTTP opierać na heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT) oraz największym bloku z tymi samymi flagami. Osobno pokazywać MALLOC_CAP_DMA. ESP.getFreeHeap() / getMaxAllocHeap() w core 3.3.12 używają MALLOC_CAP_INTERNAL bez MALLOC_CAP_8BIT i doliczają pamięć dostępną tylko dla dostępu 32-bitowego.
2. Dotyczy to m.in. ArdPortalImpl.h: consoleLine (próg 24576), infoJson oraz minimum heap w begin/loop. Obecny próg konsoli nie uruchamia się nawet przy faktycznych 2 KiB RAM dla buforów.
3. Discovery i generowanie stron powinny utrzymywać rezerwę rzeczywistej pamięci 8-bit/DMA dla lwIP; odroczyć pracę przed presją pamięci, a nie dopiero po malloc failure. Próg dobrać pomiarem pod obciążeniem. Sam backoff po OOM nie zapewnia miejsca dla sieci.
4. Przy rejestracji stron raportować zużycie heap8/DMA przed/po oraz stan ukończenia. Zredukować trwałe kopie metadanych/definicji pól. Nie usuwać pól ani encji jako obejścia.

EAGAIN sam w sobie jest poprawną informacją o chwilowym braku miejsca. Powyższy pomiar nie dowodzi oddzielnego błędu implementacji send ani wycieku pamięci. Wyciek wymaga porównania stanu po zakończeniu discovery i wygaśnięciu połączeń TCP TIME_WAIT.

## Zmiana w aplikacji
Test standardowego stosu 8 KiB oraz pomiar jego zapasu; odzyskanie 24 KiB pamięci 8-bit względem poprzednich 32 KiB. Wyniki końcowe wymagają testu pełnego HA/MQTT i zapisu ustawień.

## Wynik pełnego testu i konkretne miejsca naprawy
Stos 8 KiB: 94 żądania, 34 błędy (timeout lub IncompleteRead). Nie jest to naprawa zawieszania. HA i MQTT włączone, wszystkie pola pozostawione. Minimum raportowanego zapasu stosu: 4528 bajtów. Mega przyjęła jasność 122 i potwierdziła trwały zapis po 2800 ms; przywrócenie 121 potwierdzono po 3347 ms. UART CRC=0 i timeout=0. Nie uznajemy ACK po około 3 sekundach za wynik docelowy.

### 1. Nie przechowywać źródeł stron w RAM
AC-ESP ma definicje PROGMEM, ale musi zamienić $device na nazwę urządzenia. Powstaje String z całą stroną. AppConfigPageRegistrationImpl.h:292 przenosi ten String do work.entry->source i przechowuje go przez cały czas życia portalu. Dziewięć stron to dziesiątki KiB trwałego RAM.

Biblioteka ma już overload startAppConfigPageRegistration(const __FlashStringHelper*). Należy umożliwić rozwiązanie $device dla ha.state_topic i ha.command_topic bez kopiowania całej strony. Przykładowa implementacja: resolver nazwy urządzenia w rejestrze, wywoływany dla pojedynczego pola w DynamicPages.h::fieldValue po sparsowaniu jego wycinka. Również walidacja/rejestracja indeksów komend powinna widzieć te same rozwiązane topiki. Źródło strony i offsety pozostają niezmienione we flash. Nazwa powinna pochodzić z ArdDeviceName::mqtt(config.deviceName). Nie zmieniać ID pól, tematów MQTT ani HA unique_id.

Po takim rozszerzeniu w AC-ESP::panelService można zastąpić tworzenie String i definition.replace przez portal.startAppConfigPageRegistration(panelPageDefinition(page)). To jest główna możliwość odzyskania pamięci, bez usuwania encji ani zależności. Nie należy na sztywno wpisywać MAC/nazwy bieżącego ESP do schematu.

### 2. OOM w HTTP nie może udawać końca strumienia
DynamicPortalImpl.h::prepareDynamicHttpPart zwiększa _httpDynamicField przed wygenerowaniem fragmentu, parsuje pole przez fieldValue i przy błędzie/OOM zwraca false. ArdPortalImpl.h:564 interpretuje false jako closeHttp(). Klient otrzymuje 200 z Content-Length, lecz niepełne body (zaobserwowano np. LED 4612/5972 bajtów).

Rozdzielić wyniki Ready / RetryLater / Finished / Failed. Przy przejściowym braku pamięci nie przesuwać indeksu pola, nie zamykać odpowiedzi; ponowić po odciążeniu sieci, z ograniczonym czasem i budżetem. Indeks zwiększać dopiero po utworzeniu kompletnego fragmentu. Najlepiej wysyłać definicje ze źródłowych wycinków JSON we flash bez parse -> DOM -> stringify dla każdego pola. Metadane dodawane przez normalizację i resolver topików muszą nadal być poprawnie emitowane. Przed wysłaniem nagłówków zapewnić rezerwę pamięci; po ich wysłaniu nie da się już zamienić 200 na 503.

### 3. Zabezpieczenie pod obciążeniem
Kontrola rezerwy musi używać pamięci INTERNAL|8BIT oraz DMA i największego bloku 8-bitowego. Nie wystarczy ogólny ESP.getFreeHeap(). Odroczyć discovery i generowanie nowych fragmentów, gdy zabrałyby rezerwę lwIP; nie zatrzymywać obsługi istniejących połączeń. Dodać test wymuszający OOM podczas kolejnego fragmentu oraz test HTTP + discovery + zależności + komenda z zapisem. Nie zakładać stałego progu z powyższego pomiaru bez sprawdzenia buforów i wersji core.

Zmiany aplikacji zostały zapisane i wgrane. ArdPortal nie został zmodyfikowany. Firmware końcowy ma ponownie pełne HA=1 i MQTT=1, standardowy stos 8 KiB, nieblokujące pomiary USB i poprawne metryki Panel heap8/dma/block8. W tej historycznej próbie konfiguracja Mega była jeszcze w NVS ESP; obecny firmware korzysta wyłącznie z natywnych pól ArdPortal w ArdFS.
