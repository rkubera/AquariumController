#!/usr/bin/env python3
"""Create a deterministic gzip OTA image for ESP8266 (not ESP32).

The ESP8266 core/eboot and ArdPortal OTA must support gzip firmware uploads.
Use a .bin compiled for the target board and its existing flash/FS layout.
"""
import argparse
import gzip
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('firmware', type=Path, help='ESP8266 application .bin')
    parser.add_argument('--output', type=Path, help='Default: <firmware>.gz')
    args = parser.parse_args()
    data = args.firmware.read_bytes()
    if len(data) < 8 or data[0] != 0xE9:
        parser.error('Expected an uncompressed application firmware .bin')
    output = args.output or args.firmware.with_suffix(args.firmware.suffix + '.gz')
    if output.resolve() == args.firmware.resolve():
        parser.error('Output must differ from the original firmware')
    packed = gzip.compress(data, compresslevel=9, mtime=0)
    if gzip.decompress(packed) != data:
        raise RuntimeError('OTA compression verification failed')
    output.write_bytes(packed)
    print(f'{output}: {len(data)} -> {len(packed)} bytes '
          f'({100 * (1 - len(packed) / len(data)):.1f}% smaller); round-trip verified')


if __name__ == '__main__':
    main()
