#!/usr/bin/env python3
"""Run the actual controller classes on a host with simulated ADC, GPIO and CONFIG.
The AVR build remains the authority for ABI and full-sketch integration.
"""
from pathlib import Path
import re
import subprocess
import tempfile
import sys

ROOT = Path(__file__).resolve().parents[1]
for test in ('run_peripheral_tests.py','run_peripheral_driver_tests.py'):
    subprocess.run([sys.executable,str(ROOT/'tests'/test)],check=True)
main = (ROOT / 'AC-ATMEGA.ino').read_text()
constants = '\n'.join(re.findall(r'^#define .+$', main, re.M))
strings = '\n'.join(re.findall(r'^const char .+$', main, re.M))
files = ['PwmHardware.ino','Tools.ino', 'Control.ino', 'Sensors.ino', 'PWMOutputs.ino', 'Relays.ino','PeripheralConfig.ino']
prototypes = []
for file in files:
    prototypes += [line.rstrip(' {') + ';' for line in re.findall(
        r'^(?:void|bool|float|double|byte|int|String|unsigned long|uint16_t) \w+\s*\([^\n]*\)\s*\{',
        (ROOT / file).read_text(), re.M)]
config = (ROOT / 'Config.ino').read_text()
name_save = config[config.index('void configSaveString '):config.index('void configSaveLedBrightness')]
with tempfile.TemporaryDirectory(prefix='aquarium-tests-') as work:
    work = Path(work)
    (work / 'Arduino.h').write_text('#pragma once\n')
    source = '\n'.join([
        (ROOT / 'tests/host_support.hpp').read_text()+'\n#include "TemporaryState.h"\n', '#include "PeripheralDevices.h"\n', (ROOT/'WireFields.h').read_text(), 'bool megaConfigReady=true; void wdt_reset() {}', constants, strings,
        '\n'.join(prototypes), 'void pwmWrite16(byte,uint16_t);', (ROOT/'tests/peripheral_support.hpp').read_text(),
        *[(ROOT / file).read_text() for file in files],
        name_save,
        (ROOT / 'tests/acquisition_tests.cpp').read_text(),
        (ROOT/'tests/peripheral_tests.cpp').read_text(),
        (ROOT / 'tests/control_tests.cpp').read_text(),
    ])
    (work / 'test.cpp').write_text(source)
    subprocess.run(['c++', '-std=c++11', '-DARDUINO=100', '-I', str(work), '-I', str(ROOT),
                    '-fsanitize=address,undefined', '-fno-omit-frame-pointer',
                    '-g', str(work / 'test.cpp'), '-o', str(work / 'tests')], check=True)
    subprocess.run([str(work / 'tests')], check=True)
