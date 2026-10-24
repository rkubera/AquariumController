# Memory and runtime audit — 2026-10-24

This audit covers AquariumController source and offline tests. ArdPortal was not modified. No firmware was uploaded and no runtime device heap measurement was taken.

## Changes

- Mega: pack input validity/debounce flags and store peripheral DHT humidity as integer tenths. Static RAM falls from 6012 to 5959 bytes, saving 53 bytes net without reducing precision or channel counts.
- Mega: do not touch the GPIO latch when detaching an already detached servo. This removes needless critical sections and prevents a short LOW glitch on a normal PWM output previously used by a servo.
- Mega: limit each flow input to 4096 rising edges per measurement window, normally one second. On excess edges, suspend that channel's pin-change interrupts until the next window and invalidate the measurement. A separate two-byte counter caps all shared pin-change IRQs at 32768 per window, including aliased noise that does not appear as a sampled rising edge. This bounds interrupt storms from noisy wiring.
- Mega: editing a flow input preserves counts, pending edges, overflow lockouts and the measurement deadline of the other inputs. The edited input discards its partial first window; the warmup bitmap costs one byte.
- Mega: remove cross-slot servo interrupt masking from OneWire. Its short callbacks already run atomically inside Timer2; ordinary interrupt latency still requires hardware timing validation. The production short-callback path normally did not enter the removed masking branch, so no measured servo-jitter improvement is claimed.
- ESP/Mega: builds include the new optional-peripherals feature manifest; saved assignments and existing Home Assistant entities are preserved.
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
| Mega | 102,822 | 253,952 | 151,130 | 5,959 | 2,233 |
| ESP32 current | 1,238,608 | 1,966,080 | 727,472 | 59,932 | 267,748 |
| ESP32 full | 1,329,152 | 1,966,080 | 636,928 | 64,684 | 262,996 |
| ESP8266 current | 527,264 | 962,560 | 435,296 | 46,324 | 33,868 |
| ESP8266 full | 617,344 | 962,560 | 345,216 | 49,900 | 30,292 |

ESP32 uses `PartitionScheme=min_spiffs`: each OTA application slot is 1,966,080 bytes. "Remaining image area" is firmware growth room inside one slot, not the total flash chip's unused storage. The second OTA slot and filesystem are separate.

ESP8266 uses the generic 1 MiB / 64 KiB FS layout. Its filesystem begins at flash offset `0xEB000` (962,560), used here as the raw image-area boundary. Remaining image bytes are not a measurement of the device's available OTA space; OTA also needs staging space and sector alignment. A different board/flash layout changes these figures.

ESP8266 instruction RAM uses 60,815 / 65,536 bytes, including the instruction-cache reservation; 4,721 bytes remain. This is separate from data RAM/heap.

## Cache RAM savings with the current disabled-channel profile

- ESP32 current: static RAM 64,684 → 59,932; saved 4,752 bytes.
- ESP8266 current: static RAM 49,884 → 46,324; saved 3,560 bytes.

## Validation

Controller/input/servo/flow tests (including flow-channel edit isolation, pending-edge preservation, overflow lockouts and servo interrupt availability throughout OneWire), UART and native persistence tests, all 128 feature combinations, compact-cache mapping tests, LED/optimistic command tests and actual ArdPortal/ArdFS persistence tests passed. Host tests use address/undefined-behavior sanitizers. Allocation-failure tests exercise the actual ESP decoding paths, and 20,000 repeated decoding/lookup iterations passed. The dictionary lookup test also asserts that an existing-key lookup constructs no temporary String.

No physical restart-free duration or runtime heap minimum is claimed by these offline checks.
