# Wyświetlacz i klawiatura

PCD8544 (84×48 pikseli) mieści 12 znaków w wierszu i 6 wierszy. Teksty są
przycinane i dopełniane spacjami, więc po zmianie strony nie zostają stare cyfry.
Zawartość ekranu jest przeliczana co 200 ms i po klawiszu. Do LCD wysyłany jest
najwyżej jeden zmieniony wiersz w jednym wywołaniu obsługi menu; między wierszami
pętla obsługuje RS i sterowanie. Niezmienione wiersze nie są przesyłane ponownie.
Zmiana strony zastępuje oczekującą zawartość poprzedniej strony. Nazwy na ekranie
mogą być krótsze niż w LINK.

## Ekrany

LCD używa manifestu funkcji odebranego z ESP, tak samo jak sterowanie Mega.
Poniższa numeracja dotyczy konfiguracji ze wszystkimi funkcjami. Po wyłączeniu
funkcji ekrany są usuwane z listy, a numery klawiszy `1…9` i licznik stron
są przeliczane bez luk. Główna, Sieć, Zegar programowy i LED są zawsze dostępne.
Przed odebraniem manifestu widoczne są tylko te cztery ekrany.

- Wejścia, Przekaźniki i PWM pojawiają się tylko dla liczby kanałów większej od zera.
  A/B przechodzą wyłącznie po aktywnych kanałach 1–N.
- Wyłączone RTC usuwa wiersz RTC; czas programowy i synchronizacja pozostają.
- Wyłączone DHT usuwa ekran Wewnętrzny. Przy DHT bez wentylatora ekran nazywa się
  DHT i pokazuje tylko temperaturę oraz wilgotność.
- Błędy pojawiają się według tego samego warunku co Diagnostyka w ESP: aktywne
  wejścia analogowe, DHT lub RTC. Bez wejść znika wiersz `In:` na wyjściach.
- Po zmianie manifestu usunięty bieżący ekran wraca do Głównej; wybrany kanał
  poza nowym limitem wraca do kanału 1. Niedostępny numer ekranu jest ignorowany.


| Strona | Zawartość |
| --- | --- |
| 1. Główna | Data `RRRR/MM/DD`, czas `GG:MM:SS`, `WiFi:on/off`, `LINK:on/off`. Dolny wiersz: `A/B kontrast`. |
| 2. Sieć | Nazwa kontrolera, SSID, serwer LINK, status LINK. |
| 3. Zegar | Data, czas lokalny, stan RTC (`waiting/ok/missing/invalid`), stan synchronizacji. |
| 4. Czujniki | Numer i nazwa jednego z aktywnych wejść (maks. 8), wartość skalibrowana, surowy odczyt ADC, przypisane wyjście (`Relay1…4`, `PWM1…6` lub `none`). Brak poprawnego pomiaru: `invalid`. |
| 5. Przekaźniki | Numer i nazwa jednego z aktywnych wyjść (maks. 4), tryb i ręczne nadpisanie, rzeczywisty stan on/off, połączona wartość wejść lub `invalid`. |
| 6. PWM | Numer i nazwa jednego z aktywnych wyjść (maks. 6), tryb i ręczne nadpisanie, rzeczywiste wysterowanie 0…65535, połączona wartość wejść lub `invalid`. |
| 7. LED | Zadany stan, jasność 0…100%, aktualne składowe R/G/B podczas przejścia. |
| 8. Wewnętrzny | Temperatura, wilgotność DHT11, limit temperatury, PWM wentylatora 0…65535. |
| 9. Błędy | Liczba błędów, maska kategorii, licznik błędów CRC i zbyt długich ramek RS. Szczegóły są w polu Status. |

Poza stroną główną dolny wiersz podaje numer strony i kanału, np.
`4/9 2/8 A/B`: strona 4 z 9, czujnik 2 z 8. Tryby wyjść: `manual`, `day`,
`hyst` (histereza), `pid` (tylko PWM). Sufiks `auto/on/off` oznacza ręczne
nadpisanie; np. `hyst:auto` to aktywna regulacja histerezą.

Maska błędów: 1 — nieważny czujnik analogowy; 2 — regulator bez poprawnego
wejścia; 4 — za wysoka temperatura wewnętrzna; 8 — nieważny DHT; 16 — błąd obliczeń PID.
Kategorie mogą się sumować. Licznik CRC nie zwiększa liczby błędów pomiarów.

## Klawisze

| Klawisz | Działanie |
| --- | --- |
| `A`, `B` | Poprzedni/następny kanał czujnika, przekaźnika lub PWM; wybór zapętla się. Na stronie głównej: A zwiększa kontrast o 2, B zmniejsza o 2 i rozjaśnia tło. |
| `C` | Na stronie przekaźników lub LED przełącza wybrany kanał on/off przez tymczasowe nadpisanie. |
| `D` | Na stronie przekaźników lub LED usuwa ręczne nadpisanie i przywraca skonfigurowane sterowanie. |
| `*` | Na każdej stronie przełącza LED on/off. |
| `#` | Na każdej stronie przywraca skonfigurowane sterowanie LED. |
| `1…9` | Przechodzi od razu do strony o tym numerze w aktualnej liście widocznych ekranów. Wybiera pierwszy kanał. Przy wszystkich funkcjach obowiązuje numeracja tabeli powyżej. |
| `0` | Wraca na stronę główną. |

Ręczne włączenie regulatora nie omija zabezpieczenia braku poprawnych wejść:
wyjście z histerezą/PID pozostaje wyłączone, jeśli którykolwiek przypisany czujnik
jest nieważny. W trybie dnia nadpisanie kończy się na granicy kolejnej części dnia;
w innych trybach obowiązuje do `D`/`#`, zmiany trybu lub restartu.

Klawiatura służy do podglądu i ręcznego sterowania. Nazwy, kalibracje, przypisania,
limity, parametry PID, harmonogram i reguły strefy czasowej ustawia się przez LINK;
nie ma lokalnego edytora liczbowego. Zmiany z LINK są widoczne na następnym
odświeżeniu, a sterowanie z klawiatury publikuje ten sam stan LINK co polecenia sieciowe.

Podświetlenie pracuje z pełną jasnością przez 10 sekund po starcie i 5 sekund po klawiszu, następnie wygasa przez 3 sekundy. Pin LIGHT jest sterowany przez PWM na D2. `LCD_BACKLIGHT_ACTIVE_LOW=1` oznacza włączenie stanem niskim; dla modułu włączanego stanem wysokim ustaw 0.
Pierwszy klawisz po wygaszeniu wykonuje również swoją funkcję. Zegar przy starcie odczytuje Tiny RTC (DS1307). Moduł ze sprawną baterią podtrzymuje czas po odłączeniu zasilania; bez poprawnego odczytu RTC zapis CONFIG nie uwzględnia czasu wyłączenia.
`waiting/pending/rejected/manual/synchronized/rtc` na stronie zegara pomaga rozpoznać
stan czasu zapisanego lub odebranego z mostka ESP.

Kontrast PCD8544 przy starcie wynosi 40 zamiast wcześniejszego 57. Gdy całe tło jest czarne, naciśnij `1` lub `0`, a potem kilka razy `B`, aby obniżyć kontrast. Gdy litery są za blade, użyj `A`. Regulacja obowiązuje do restartu i nie zapisuje CONFIG. Podświetlenie i kontrast to osobne ustawienia.

Klawiatura 4×4: rzędy `1 2 3 A`, `4 5 6 B`, `7 8 9 C`, `* 0 # D`. Piny wierszy: 30, 29, 28, 27; kolumn: 22, 23, 24, 25. Ekrany wybiera się cyframi; klawisze A/B nie przełączają ekranów.

Wszystkie wyjścia używają sprzętowego PWM 16-bitowego 0–65535 przy 244,140625 Hz. Timery1/3/4/5: RGB D11/D12/D13, LCD D2, wentylator D3, PWM kanały 1–6: D7/D8/D45/D46/D44/D5. Przepięcia: D4→D44, D9→D45, D10→D46. Przy 0 i 65535 wyjście ma stały poziom. Jasność RGB pozostaje 0–100%; CustomColor pozostaje RGB 24-bitowym. Fade i wave zachowują czas. DHT D32 i buzzer D6 używają kanałów Timer2.

Kontrast zmieniany A/B jest zapisywany tylko w EEPROM Mega, po 1,5 s bez zmian. Osiem rekordów z CRC zajmuje ostatnie 32 bajty EEPROM; zapis pojedynczych bajtów jest asynchroniczny. ESP i obraz konfiguracji UART nie zawierają kontrastu. Brak poprawnego rekordu oznacza domyślny kontrast 40.
