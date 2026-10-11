# UART v2 and ESP-owned configuration

Both firmwares must be updated together. The old decimal-CRC/text UART protocol is
not supported. Mega uses Serial3 (TX3 D14, RX3 D15); ESP32 uses UART2 at 115200, 8N1: RX2 GPIO16, TX2 GPIO17.
Mega TX3 D14 connects through a 5-to-3.3 V level converter to ESP GPIO16;
ESP GPIO17 connects to Mega RX3 D15. UART0 remains available for USB.
Connect TX to the other board's RX and use a common ground. Mega TX is 5 V; the path
to ESP RX needs a divider/level converter to 3.3 V. UART core buffers are not patched.

The architecture uses typed messages and numeric data IDs, similar in purpose to
Tuya MCU datapoints, but is not compatible with Tuya's wire format. MQTT topics do
not cross the serial link. Existing MQTT names, Portal fields and HA entities remain.
ESP translates numeric data IDs to `stat/<device>/<field>` and applies the same
setting flow for Portal and HA. External POWER commands remain non-retained.

## Frames

COBS encoding followed by a zero delimiter allows recovery after noise or lost bytes.
The decoded frame is little endian:

| Offset | Bytes | Meaning |
| --- | --- | --- |
| 0 | 1 | Protocol version: 2 |
| 1 | 1 | HELLO=1, ACK=2, DATA=3 |
| 2 | 4 | Sender boot/session identifier |
| 6 | 4 | Target session; zero for HELLO |
| 10 | 2 | Frame sequence, 1..65535, then wraps to 1 |
| 12 | 1 | Payload length, at most 24 |
| 13 | variable | Payload |
| final | 4 | IEEE CRC32 of the entire header and payload |

A HELLO payload contains the shared dictionary/layout fingerprint. Both peers must
agree. Schema mismatches appear in ESP Console → Messages. Boot sessions reject
old packets after reset. A DATA payload starts with message type, byte offset and
complete message length; remaining bytes contain up to 21 bytes of the message.
Application messages may contain at most 160 bytes. Fragments are accepted only
in order, with consistent type/length. Only complete messages reach the controller.

A DATA frame is at most 43 bytes including its zero delimiter. Its ACK is 19 bytes.
Their total of 62 bytes fits the stock AVR UART's 63-byte usable receive ring.
Only one DATA frame per direction is in flight. Whole messages enter bounded
512-byte queues atomically. Oversized/full queues are rejected, not partially sent.
The telemetry snapshot restarts if a producer could not enqueue a value.

ACK confirms CRC-checked receipt. ACK loss retransmits the same sequence and does
not execute the message twice. A frame is retried after 200 ms; ten attempts trigger
a new session/synchronization. Partial input expires after 100 ms. Idle HELLO traffic
keeps the link alive every two seconds; no valid peer traffic for eight seconds
invalidates the connection. Each service call reads at most 64 and writes at most
32 bytes, without waiting for UART hardware. CRC provides error detection, not
cryptographic authentication.

## Application messages

| ID | Message | Payload |
| --- | --- | --- |
| 1 | Value | uint16 field ID, text value |
| 2 | Setting | uint32 operation ID, uint16 command field ID, text value |
| 3 | Result | uint32 operation ID, uint16 readback field ID, actual text value |
| 4 | Info | uint8 query ID, text value |
| 5 | Request | uint8 query ID |
| 6 | Diagnostic | text, shown only in Messages |
| 7 | Power | uint8 channel (0/1), uint8 state (0/1) |
| 8 | ConfigBegin | uint32 transaction, uint16 field count, uint32 schema |
| 9 | ConfigChunk | uint16 index, uint16 field ID, text value |
| 10 | ConfigCommit | uint32 transaction, uint32 ordered-record digest |
| 11 | reserved | unused |
| 12 | ConfigRequest | 0=restore native fields at startup |
| 13 | ConfigReady | uint32 transaction, uint32 ordered-record digest, uint32 manifest CRC |
| 14 | Capabilities | version=1, flags (DHT=1/RTC=2/buzzer=4/fan=8), relay/PWM/input counts; five uint8 values |

Query IDs: timestamp=1, WiFi=2, MQTT=3, hostname=4, SSID=5, MQTT server=6,
ping=7, complete snapshot=8. UTC still comes from ESP on Mega's request;
RTC and time validation remain in Mega. Mega uses numeric `FieldId` constants and channel arithmetic without a field-name
dictionary. ESP alone translates field names. `WireFields.h` is generated for both
sketches by `tests/generate_wire.py`; `generate_panel.py` also refreshes it.
IDs are not MQTT/HA entity IDs. A changed dictionary requires flashing both peers.

## Native configuration and boot restoration

ArdPortal fields with `persist=true` are the sole persistent settings source on
both ESP32 and ESP8266. ArdPortal owns ArdFS and all debounce, atomic storage and
save callbacks. The application does not open another filesystem, create hidden
records, use Preferences, or upload a binary configuration image. There is no
migration from the previous store. Reconfigure native fields after upgrading.

Up to 249 active settings are restored through the existing validated Mega handlers. The
records have a generated deterministic order: fan full-speed temperature
before start temperature, calibration before sensor assignments, and control modes
last. A record is acknowledged through Result with operation `UINT32_MAX` and its
actual value. Normal UI operations must not use this reserved operation ID.

Each UART frame has CRC32. The transaction additionally chains CRC32 over the
previous digest, index, field ID and value. The initial digest is the CRC32 of the capability manifest.
The reliable Capabilities message precedes restoration; Mega validates counts and
fan/DHT dependencies. Only enabled settings are restored. ConfigReady includes
the manifest CRC, preventing a stale readiness announcement for a different selection. Mega verifies the schema, expected
count, exact generated order, token and final digest before opening output
interlocks. Missing, duplicate, out-of-order or corrupted records cannot complete
startup. A timed-out transaction restarts from safe defaults. The startup sound
uses the restored configuration. ArdPortal storage and all nine pages must finish
loading/registering before restoration starts.

Settings changed in Portal or HA use native persistence. Validated corrections,
rejections and dependent sensor assignments update the same fields through
`setAppConfigValue`; no second persistent representation exists. Latest queued
optimistic changes take precedence over older readbacks. Confirmation of application
is separate from completion of ArdPortal's asynchronous storage save.

Telemetry, active alarms, actual output states and clock-setting actions have
`persist=false`. They are not part of the restoration transaction. RTC and NTP
supply time; there is no persisted timestamp fallback. The explicitly requested
local LCD contrast journal remains in Mega EEPROM and is not sent to ESP.

An ESP-only restart does not reset a running Mega. Mega announces readiness and
publishes its current values; confirmed settings reconcile native fields if
necessary. Losing ESP after initialization leaves control running autonomously.
Flash both firmwares together because the boot transaction schema has changed.

## Verification

`python3 tests/run_tests.py` in AC-ESP covers real frame code, both actual restoration
state machines, transaction CRC/count/order checks, boot/reconnect, registration and
shared schemas. `python3 tests/run_tests.py` and `run_link_tests.py` in AC-ATMEGA cover
control logic and typed application adapters; the latter simulates delivery rather
than a broker. `run_rtc_tests.py` exercises the RTC driver. MCU compilation remains
the authority for the AVR ABI and complete Arduino integration.

Feature selection and dependencies: [FEATURES.md](FEATURES.md).
