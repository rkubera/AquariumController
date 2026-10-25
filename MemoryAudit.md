# Memory and runtime audit — 2026-10-25

This audit covers AquariumController source and offline tests. ArdPortal was not modified. No firmware was uploaded and no runtime device heap measurement was taken.

## Changes

- Mega: pack input validity/debounce flags and store peripheral DHT humidity as integer tenths. The packed representation preserves precision and channel counts; the current figures below also include the boot-date feature.
- Mega: do not touch the GPIO latch when detaching an already detached servo. This removes needless critical sections and prevents a short LOW glitch on a normal PWM output previously used by a servo.
- Mega: limit each flow input to 4096 rising edges per measurement window, normally one second. On excess edges, suspend that channel's pin-change interrupts until the next window and invalidate the measurement. A separate two-byte counter caps all shared pin-change IRQs at 32768 per window, including aliased noise that does not appear as a sampled rising edge. This bounds interrupt storms from noisy wiring.
- Mega: editing a flow input preserves counts, pending edges, overflow lockouts and the measurement deadline of the other inputs. The edited input discards its partial first window; the warmup bitmap costs one byte.
- Mega: remove cross-slot servo interrupt masking from OneWire. Its short callbacks already run atomically inside Timer2; ordinary interrupt latency still requires hardware timing validation. The production short-callback path normally did not enter the removed masking branch, so no measured servo-jitter improvement is claimed.
- ESP/Mega: builds include the new optional-peripherals feature manifest; saved assignments and existing Home Assistant entities are preserved.
- Mega: track elapsed time across millis wraps and estimate the last firmware-start UTC instant only from trusted RTC/NTP/manual time. Diagnostics shows milliseconds before synchronization and local date/time with the historical UTC offset afterwards. The boot tracker and fallback-publication timer add 16 bytes of static SRAM; no boot record is written to EEPROM/ArdFS. Small clock jitter is ignored; larger trusted corrections may refine the estimate.
- Mega/ESP: add SEN0204, Grove 101020018, SEN0217, SEN0161-V2 and SEN0169-V2 model profiles. Selections occupy previously unused bits in each input options byte; the runtime configuration buffer remains 1050 bytes. Named pH profiles seed the nominal two-point conversion, preserve custom calibration and invalidate values outside pH 0–14.
- Portal: 398 native fields / 401 bridge metadata fields / 429 UART fields in the full build. The 43,810-byte Peripherals page registers from flash incrementally; the real ArdPortal/ArdFS host test covers registration, profile saving and reboot restore above the old 32 KiB generator guard.
- ESP: omit telemetry String objects for disabled hardware. The compact cache preserves the numeric UART/MQTT field IDs. Fully enabled builds retain direct indexing and the original cache footprint.
- ESP: replace whole-panel name scans and temporary dictionary Strings with direct numeric mapping and flash-string comparison.
- ESP: send settings and process acknowledgements as typed fields rather than assembling and reparsing text frames. Move queued String buffers instead of copying them.
- ESP: skip empty persistence/render/MQTT work and scan at most 16 telemetry IDs per MQTT service call. Existing UART-first handling and MQTT publication pacing remain in place.
- ESP: reject an entire UART/MQTT value when reserve or concatenation fails, preventing a truncated numeric prefix from becoming a valid setting.
- ESP: preserve native Boolean types when rendering switches, including input inversion and pull-up, and normalize their Boolean commands to on/off.

## Leak findings and limits

No unbounded project-owned allocation or missing matching free was found. Transport queues, cached field counts and command queues are bounded. String/JSON values use managed lifetimes. Retaining a bounded String buffer's peak capacity is not itself a leak, but repeated temporary allocation can still fragment a small heap.

These findings do not prove that the complete running system never leaks or restarts. ArdPortal, networking, SDK allocations, real interrupts, supply conditions and actual stack high-water marks require a prolonged device run. Existing Mega USB diagnostics expose `RAM gap`, `minGap`, loop timing and watchdog/reset breadcrumbs; ESP diagnostics expose heap, largest block and reset reason. Build-time RAM headroom below is not measured free heap after Wi-Fi/HA/HTTP startup.

## Build profiles

- **Current:** the sketch's existing defines: four relays, zero PWM channels, zero configurable inputs; DHT, RTC, buzzer and fan enabled. These user settings were preserved.
- **Full:** four relays, six PWM channels, eight inputs, all optional modules enabled. The full sketch was compiled from a temporary copy with those counts; project defaults were not changed.
- Mega remains a full-capability firmware in both profiles.
- Toolchains: AVR core 1.8.8, ESP32 core 3.3.12, ESP8266 core 3.1.2; installed ArdPortal 0.8.1.

## Compiled sizes

All sizes below are bytes. Mega firmware means loadable program size, not the ASCII Intel HEX file length. ESP firmware means the actual `.bin` file size.

| Build | Firmware | Image-area budget | Remaining image area | Static RAM | RAM after globals |
| --- | ---: | ---: | ---: | ---: | ---: |
| Mega | 105,776 | 253,952 | 148,176 | 5,977 | 2,215 |
| ESP32 current | 1,240,016 | 1,966,080 | 726,064 | 59,972 | 267,708 |
| ESP32 full | 1,343,872 | 1,966,080 | 622,208 | 65,236 | 262,444 |
| ESP8266 current | 528,816 | 962,560 | 433,744 | 46,348 | 33,844 |
| ESP8266 full | 632,624 | 962,560 | 329,936 | 50,316 | 29,876 |

ESP32 uses `PartitionScheme=min_spiffs`: each OTA application slot is 1,966,080 bytes. "Remaining image area" is firmware growth room inside one slot, not the total flash chip's unused storage. The second OTA slot and filesystem are separate.

ESP8266 uses the generic 1 MiB / 64 KiB FS layout. Its filesystem begins at flash offset `0xEB000` (962,560), used here as the raw image-area boundary. Remaining image bytes are not a measurement of the device's available OTA space; OTA also needs staging space and sector alignment. A different board/flash layout changes these figures.

ESP8266 instruction RAM uses 60,815 / 65,536 bytes, including the instruction-cache reservation; 4,721 bytes remain. This is separate from data RAM/heap.

## RAM differences between hardware profiles

- ESP32: full 65,236 bytes; current 59,972 bytes; difference 5,264 bytes.
- ESP8266: full 50,316 bytes; current 46,348 bytes; difference 3,968 bytes.

These are measured static-RAM differences between hardware profiles, including the compact telemetry cache and other compiled feature data. They are not an isolated before/after cache benchmark or a runtime heap measurement.

## Validation

Boot-date tests (unknown-time ms, 200 days across millis wraps, trusted/invalid time, jitter/correction, reset and historical DST offsets), local clock-cache tests, controller/input/servo/flow tests (including flow-channel edit isolation, pending-edge preservation, overflow lockouts and servo interrupt availability throughout OneWire), UART and native persistence tests, all 128 feature combinations, concrete profile defaults/option-bit preservation/pH bounds/custom-calibration tests, compact-cache mapping tests, LED/optimistic command tests and actual ArdPortal/ArdFS persistence tests passed. Host tests use address/undefined-behavior sanitizers. Allocation-failure tests exercise the actual ESP decoding paths, and 20,000 repeated decoding/lookup iterations passed. The dictionary lookup test also asserts that an existing-key lookup constructs no temporary String.

No physical restart-free duration or runtime heap minimum is claimed by these offline checks.
