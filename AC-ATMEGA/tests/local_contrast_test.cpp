#include <cassert>
#include <cstring>
#include "LocalContrastStore.h"
static uint8_t mem[4096];static bool busy=false;static unsigned writes=0;
uint8_t rd(uint16_t a){assert(a>=4064&&a<4096);return mem[a];}
bool ready(){return !busy;}
void wr(uint16_t a,uint8_t v){assert(!busy);assert(a>=4064&&a<4096);mem[a]=v;++writes;}
void commit(LocalContrastStore& s,uint8_t v,uint32_t t){s.changed(v,t);for(unsigned i=0;i<8;i++)s.service(t+1500+i);}
int main(){
 memset(mem,255,sizeof mem);LocalContrastStore s(rd,ready,wr,4064);assert(s.begin(40)==40);
 s.changed(48,100);s.service(1599);assert(writes==0);busy=true;s.service(2000);assert(writes==0);busy=false;
 for(int i=0;i<8;i++)s.service(2000+i);
 LocalContrastStore check(rd,ready,wr,4064);assert(check.begin(40)==48);
 unsigned before=writes;commit(s,48,3000);assert(writes==before);
 uint8_t saved[4096];memcpy(saved,mem,sizeof mem);
 for(int cut=0;cut<7;cut++){
  memcpy(mem,saved,sizeof mem);LocalContrastStore power(rd,ready,wr,4064);assert(power.begin(40)==48);power.changed(64,0);
  for(int i=0;i<cut;i++)power.service(1500+i);
  LocalContrastStore reboot(rd,ready,wr,4064);assert(reboot.begin(40)==(cut>=5?64:48));
 }
 memcpy(mem,saved,sizeof mem);LocalContrastStore wrap(rd,ready,wr,4064);wrap.begin(40);
 for(int i=0;i<300;i++)commit(wrap,uint8_t(i%128),5000+i*2000);
 LocalContrastStore reboot(rd,ready,wr,4064);assert(reboot.begin(40)==299%128);
 for(int i=0;i<4064;i++)assert(mem[i]==255);
 // A corrupt latest record is ignored in favour of its predecessor.
 uint8_t v=reboot.begin(40);assert(v==43);
 for(int i=0;i<8;i++)if(mem[4064+4*i+2]==v)mem[4064+4*i+3]^=1;
 assert(reboot.begin(40)==42);
}
