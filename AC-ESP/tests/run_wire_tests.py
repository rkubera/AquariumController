#!/usr/bin/env python3
from pathlib import Path
import subprocess,tempfile,re
root=Path(__file__).resolve().parents[1]
assert (root/'ReliableUart.h').read_bytes()==(root.parent/'AC-ATMEGA/ReliableUart.h').read_bytes()
esp=(root/'WireFields.h').read_text();mega=(root.parent/'AC-ATMEGA/WireFields.h').read_text()
assert re.search(r'AC_WIRE_SCHEMA=(\d+)',esp).group(1)==re.search(r'AC_WIRE_SCHEMA=(\d+)',mega).group(1)
keys=re.findall(r'AC_WIRE_KEY_\d+\[\] PROGMEM = "([^"]+)";',esp)
for i,k in enumerate(keys):
 assert re.search(r'\b'+k.replace('Mqtt','Broker')+'='+str(i+1)+r'(?:,|})',mega)
assert not re.search(r'PROGMEM|acFieldName|acFieldId|"',mega)
assert not re.search(r'(?i)mqtt|stat/|cmnd/',mega)
with tempfile.TemporaryDirectory(prefix='ac-wire-tests-') as tmp:
 out=str(Path(tmp)/'test')
 subprocess.run(['c++','-std=c++17','-fsanitize=address,undefined','-fno-omit-frame-pointer','-g',str(root/'tests/wire_tests.cpp'),'-o',out],check=True)
 subprocess.run([out],check=True)
