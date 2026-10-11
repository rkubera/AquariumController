#!/usr/bin/env python3
"""Exercise LCD page/channel selection against all valid runtime manifests."""
from pathlib import Path
import subprocess
import tempfile
root=Path(__file__).resolve().parents[1]
source=r'''
#include <cassert>
#include "LcdMenuLayout.h"
int main(){using namespace AcLcdMenu;
 ControllerFeatures all={15,4,6,8};assert(count(all)==9);
 for(unsigned i=0;i<9;++i){assert(at(i,all)==i);assert(number(i,all)==i+1);}
 ControllerFeatures off={0,0,0,0};assert(count(off)==4);assert(at(0,off)==Home&&at(1,off)==Network&&at(2,off)==Clock&&at(3,off)==Led);assert(at(4,off)==PageCount);
 ControllerFeatures dht={ControllerFeatures::Dht,0,0,0};assert(visible(Internal,dht)&&visible(Diagnostics,dht));
 ControllerFeatures limited={0,1,2,3};assert(channels(Relays,limited)==1&&channels(Pwm,limited)==2&&channels(Inputs,limited)==3);
 for(unsigned flags=0;flags<16;++flags)for(unsigned relays=0;relays<=4;++relays)for(unsigned pwm=0;pwm<=6;++pwm)for(unsigned inputs=0;inputs<=8;++inputs){
  ControllerFeatures f={uint8_t(flags),uint8_t(relays),uint8_t(pwm),uint8_t(inputs)};if(!f.valid())continue;
  unsigned expected=4+(relays>0)+(pwm>0)+(inputs>0)+f.has(ControllerFeatures::Dht)+f.diagnostics();assert(count(f)==expected);
  assert(visible(Internal,f)==f.has(ControllerFeatures::Dht));assert(visible(Diagnostics,f)==f.diagnostics());
  for(uint8_t p=0;p<PageCount;++p){
   uint8_t selected=p,channel=255;normalize(selected,channel,f);assert(visible(selected,f));assert(channels(selected,f)>0&&channel<channels(selected,f));
   if(!visible(p,f))assert(selected==Home&&channel==0);
   for(uint8_t ch=0;ch<8;++ch){selected=p;channel=ch;normalize(selected,channel,f);assert(channel<channels(selected,f));if(visible(p,f)&&ch<channels(p,f))assert(channel==ch);}
  }
  for(uint8_t i=0;i<count(f);++i){auto page=at(i,f);assert(visible(page,f)&&number(page,f)==i+1);}
  assert(at(count(f),f)==PageCount);
 }
 // A capability update can remove the current screen or just its selected channel.
 uint8_t page=Pwm,channel=5;normalize(page,channel,limited);assert(page==Pwm&&channel==0);normalize(page,channel,off);assert(page==Home&&channel==0);
}
'''
with tempfile.TemporaryDirectory(prefix='ac-lcd-menu-') as temp:
 temp=Path(temp);(temp/'test.cpp').write_text(source)
 subprocess.run(['c++','-std=c++11','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-I',str(root),str(temp/'test.cpp'),'-o',str(temp/'test')],check=True)
 subprocess.run([str(temp/'test')],check=True)
print('PASS: LCD selection for every valid capability manifest, dynamic shortcuts, channel limits and capability changes')
