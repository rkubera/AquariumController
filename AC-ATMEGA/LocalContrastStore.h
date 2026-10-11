#pragma once
#include <stdint.h>
// Eight journal records, four bytes each: commit marker, sequence, Vop, CRC8.
// The marker is written last, so an interrupted write preserves the old record.
class LocalContrastStore {
public:
 typedef uint8_t (*Read)(uint16_t);typedef bool (*Ready)();typedef void (*Write)(uint16_t,uint8_t);
 LocalContrastStore(Read r,Ready b,Write w,uint16_t base):read(r),ready(b),write(w),base(base){}
 uint8_t begin(uint8_t fallback){
  saved=fallback;slot=7;sequence=0;valid=false;phase=0;pending=false;
  for(uint8_t i=0;i<8;i++){uint16_t a=base+4*i;uint8_t s=read(a+1),v=read(a+2);
   if(read(a)==0xa7&&v<=127&&read(a+3)==checksum(s,v)&&(!valid||int8_t(s-sequence)>0)){valid=true;slot=i;sequence=s;saved=v;}
  }
  desired=saved;return saved;
 }
 void changed(uint8_t value,uint32_t now){desired=value>127?127:value;changedAt=now;pending=true;}
 void service(uint32_t now){
  if(!ready())return;
  if(!phase){if(!pending||uint32_t(now-changedAt)<1500)return;pending=false;if(valid&&desired==saved)return;
   targetSlot=(slot+1)%8;targetSeq=sequence+1;targetValue=desired;phase=1;
  }
  uint16_t a=base+4*targetSlot;uint8_t value;
  switch(phase){case 1:value=0;break;case 2:a++;value=targetSeq;break;case 3:a+=2;value=targetValue;break;case 4:a+=3;value=checksum(targetSeq,targetValue);break;case 5:value=0xa7;break;
   default:slot=targetSlot;sequence=targetSeq;saved=targetValue;valid=true;phase=0;return;
  }
  if(read(a)!=value)write(a,value);++phase;
 }
private:
 static uint8_t checksum(uint8_t s,uint8_t v){uint8_t c=0x5d;uint8_t b[2]={s,v};for(uint8_t x:b){c^=x;for(uint8_t i=0;i<8;i++)c=c&0x80?uint8_t((c<<1)^0x07):uint8_t(c<<1);}return c;}
 Read read;Ready ready;Write write;uint16_t base;
 uint32_t changedAt=0;uint8_t saved=40,desired=40,slot=7,sequence=0,phase=0,targetSlot=0,targetSeq=0,targetValue=0;bool valid=false,pending=false;
};
