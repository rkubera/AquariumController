#!/usr/bin/env python3
"""Verify compact cache indexing against the shared runtime capability filter."""
from pathlib import Path
import subprocess,tempfile,re
root=Path(__file__).resolve().parents[1]
count=int(re.search(r'AC_WIRE_FIELD_COUNT=(\d+)',(root.parent/'AC-ATMEGA/WireFields.h').read_text()).group(1))
for relays,pwm,inputs,dht,rtc,buzzer,fan,peripherals in [(4,6,8,1,1,1,1,0),(4,6,8,1,1,1,1,1),(4,0,0,1,1,1,1,1),(0,0,0,0,0,0,0,1),(2,3,2,0,1,0,1,1),(0,6,8,0,0,1,0,1)]:
 code='\n'.join('#define '+k+' '+str(v)for k,v in dict(AC_ENABLE_PERIPHERALS=peripherals,AC_RELAY_COUNT=relays,AC_PWM_COUNT=pwm,AC_INPUT_COUNT=inputs,AC_ENABLE_DHT=dht,AC_ENABLE_RTC=rtc,AC_ENABLE_BUZZER=buzzer,AC_ENABLE_FAN=fan).items())
 code+='''
#include <cassert>
#include <string>
#include <cstdint>
using String=std::string;
#define PROGMEM
#define pgm_read_word(p) (*(p))
#include "PortalFeatures.h"
#include "RestoreFields.h"
#include "FeatureFields.h"
'''+f'constexpr uint16_t AC_WIRE_FIELD_COUNT={count};\n'+'''#include "ActiveTelemetryCache.h"
int main(){ActiveTelemetryCache cache;unsigned enabled=0;
 for(uint16_t i=0;i<AC_WIRE_FIELD_COUNT;i++){
  assert((acTelemetryIndex(i)!=UINT16_MAX)==acFieldSupported(i+1,portalFeatures));
  if(acFieldSupported(i+1,portalFeatures)){cache[i]="value "+std::to_string(i);++enabled;}
 }
 assert(enabled==AC_TELEMETRY_COUNT);
 for(uint16_t i=0;i<AC_WIRE_FIELD_COUNT;i++)if(acFieldSupported(i+1,portalFeatures))assert(cache[i]=="value "+std::to_string(i));
 if(enabled<AC_WIRE_FIELD_COUNT)assert(sizeof(cache)<AC_WIRE_FIELD_COUNT*sizeof(String));
 else assert(sizeof(cache)==AC_WIRE_FIELD_COUNT*sizeof(String));
}
'''
 with tempfile.TemporaryDirectory(prefix='ac-active-cache-')as tmp:
  p=Path(tmp)/'test.cpp';p.write_text(code)
  subprocess.run(['c++','-std=c++11','-fsanitize=address,undefined','-I',str(root),str(p),'-o',tmp+'/test'],check=True)
  subprocess.run([tmp+'/test'],check=True)
print('PASS: compact active cache, every numeric ID, feature gating, index isolation and smaller disabled-hardware profiles.')
