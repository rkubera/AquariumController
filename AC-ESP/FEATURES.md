# Wybór funkcji

Definicje znajdują się na początku `AC-ESP.ino`. Domyślnie wszystkie funkcje są włączone.
LED RGB, Harmonogram i zegar programowy pozostają zawsze dostępne.

| Definicja | Domyślnie | Znaczenie |
|---|---:|---|
| AC_RELAY_COUNT | 4 | Aktywne kanały 1–N, zakres 0–4 |
| AC_PWM_COUNT | 6 | Aktywne kanały 1–N, zakres 0–6 |
| AC_INPUT_COUNT | 8 | Aktywne kanały 1–N, zakres 0–8 |
| AC_ENABLE_DHT | 1 | Czujnik DHT |
| AC_ENABLE_RTC | 1 | Fizyczny moduł RTC |
| AC_ENABLE_BUZZER | 1 | Dźwięki |
| AC_ENABLE_FAN | 1 | Wentylator, wymaga DHT |

Przełączniki DHT, RTC, buzzera i wentylatora przyjmują 0 lub 1.
Dla przekaźników, PWM i wejść liczba kanałów 0 wyłącza całą grupę; osobnych przełączników nie ma.
Nie trzeba uruchamiać generatora po zmianie definicji: filtr działa podczas kompilacji.
Te same definicje pól służą Portalowi i HA. Firmware nie usuwa wcześniejszych encji HA.

Bez wejść analogowych znikają PID, Threshold i pola regulacji. Listy przypisań zawierają
wyłącznie aktywne wejścia i wyjścia. Bez DHT wentylator jest automatycznie wyłączony.
Gdy DHT jest aktywny, a wentylator wyłączony, pojawia się strona DHT z temperaturą,
wilgotnością i alarmem temperatury. Diagnostyka jest obecna przy wejściach, DHT lub RTC;
wyłączenie buzzera nie usuwa diagnostyki. Gdy nie ma diagnostyki, znika ustawienie
sygnalizacji błędów buzzerem. Wyłączenie RTC pozostawia zegar programowy i synchronizację
UTC z ESP oraz reguły strefy czasowej potrzebne harmonogramom.

Mega nie ma tych definicji. ESP wysyła manifest funkcji przez niezawodny UART przed
przywróceniem ustawień. Manifest i uporządkowana lista aktywnych ustawień są sprawdzane
przed odblokowaniem wyjść. Wyłączone kanały pozostają wyłączone, nie są mierzone ani
publikowane. Zmiana manifestu zamyka wyjścia na czas nowej konfiguracji. Odłączenie ESP
po poprawnym starcie pozwala Mega kontynuować pracę z ostatnią konfiguracją RAM.

Po tej zmianie protokołu należy wgrać nowe firmware obu urządzeń. Następne zmiany
wyboru funkcji wymagają już tylko kompilacji i wgrania ESP. Ustawienia nadal zapisuje
wyłącznie ArdPortal w ArdFS; nie dodano kopii NVS ani zapisu konfiguracji do EEPROM Mega.
