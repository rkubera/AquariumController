#pragma once
#include <stdint.h>
#include <stddef.h>
#include <string.h>

namespace AcWire {
inline uint16_t u16(const uint8_t* p){return uint16_t(p[0])|(uint16_t(p[1])<<8);}
inline uint32_t u32(const uint8_t* p){return uint32_t(p[0])|(uint32_t(p[1])<<8)|(uint32_t(p[2])<<16)|(uint32_t(p[3])<<24);}
inline void put16(uint8_t* p,uint16_t v){p[0]=v;p[1]=v>>8;}
inline void put32(uint8_t* p,uint32_t v){for(uint8_t i=0;i<4;i++)p[i]=v>>(i*8);}
inline uint32_t crcAccumulate(uint32_t c,const uint8_t* p,size_t n){while(n--){c^=*p++;for(uint8_t b=0;b<8;b++)c=(c>>1)^((c&1)?0xedb88320UL:0);}return c;}
inline uint32_t crc(const uint8_t* p,size_t n){return ~crcAccumulate(0xffffffffUL,p,n);}
inline uint32_t configDigest(uint32_t previous,const uint8_t* p,uint8_t n){if(n>160)return 0;uint8_t prefix[4];put32(prefix,previous);return ~crcAccumulate(crcAccumulate(0xffffffffUL,prefix,4),p,n);}
enum Message:uint8_t {Value=1,Setting=2,Result=3,Info=4,Request=5,Diagnostic=6,Reserved7=7,ConfigBegin=8,ConfigChunk=9,ConfigCommit=10,Reserved11=11,ConfigRequest=12,ConfigReady=13,Capabilities=14};
enum Query:uint8_t {Timestamp=1,Wifi=2,Broker=3,Hostname=4,Ssid=5,BrokerServer=6,Ping=7,Snapshot=8};
// One DATA frame (43 bytes maximum) plus its ACK (19 bytes) fits the stock
// 63-byte usable AVR RX ring. Long messages are fragmented, with one frame in flight.
class Link {
public:
 static const uint8_t MaxMessage=160,Part=21;
 typedef void (*Receive)(uint8_t,const uint8_t*,uint8_t);
 typedef void (*Connection)(bool);
 struct Stats {uint32_t good=0,crcErrors=0,malformed=0,retries=0,duplicates=0,timeouts=0,oversized=0,resets=0,schemaErrors=0;} stats;
 Link(Receive receive,Connection changed):receive(receive),changed(changed){}
 void begin(uint32_t session,uint32_t schema,uint32_t now){if(peer&&changed)changed(false);peer=0;txSeq=1;lastSeq=0;rxLength=txLength=txPosition=0;messagePending=waiting=ackDue=discard=false;inOffset=0;own=session?session:1;fingerprint=schema;lastRx=lastTx=now;helloDue=true;}
 bool connected()const{return peer!=0;}
 bool idle()const{return !messagePending;}
 bool send(uint8_t kind,const uint8_t* bytes,uint8_t length){
  if(!peer||messagePending||length>MaxMessage)return false;
  if(length)memcpy(out,bytes,length);outKind=kind;outLength=length;outOffset=0;messagePending=true;waiting=false;attempts=0;return true;
 }
 void input(uint8_t b,uint32_t now){
  if((rxLength||discard)&&uint32_t(now-byteAt)>100){rxLength=0;discard=false;stats.timeouts++;}
  byteAt=now;
  if(!b){if(!discard&&rxLength)decode(now);rxLength=0;discard=false;return;}
  if(discard)return;
  if(rxLength>=sizeof(rx)){discard=true;rxLength=0;stats.oversized++;return;}
  rx[rxLength++]=b;
 }
 template<class SerialPort> void service(SerialPort& port,uint32_t now){
  if((rxLength||discard)&&uint32_t(now-byteAt)>100){rxLength=0;discard=false;stats.timeouts++;}
  if(peer&&uint32_t(now-lastRx)>8000)restart(now);
  if(!txLength){
   if(ackDue){frame(2,ackSeq,nullptr,0);ackDue=false;}
   else if(helloDue||(!messagePending&&uint32_t(now-lastTx)>=2000)){uint8_t p[4];put32(p,fingerprint);frame(1,0,p,4);helloDue=false;}
   else if(messagePending){
    if(waiting&&uint32_t(now-sentAt)>=200){if(++attempts>=10){stats.timeouts++;restart(now);return;}stats.retries++;waiting=false;}
    if(!waiting){uint8_t p[24];p[0]=outKind;p[1]=outOffset;p[2]=outLength;chunk=uint8_t(outLength-outOffset);if(chunk>Part)chunk=Part;if(chunk)memcpy(p+3,out+outOffset,chunk);frame(3,txSeq,p,uint8_t(chunk+3));waiting=true;sentAt=now;}
   }
  }
  uint8_t budget=32;
  while(budget--&&txPosition<txLength&&port.availableForWrite()>0)port.write(tx[txPosition++]);
  if(txLength&&txPosition==txLength){if(txType==3)sentAt=now;txLength=txPosition=0;lastTx=now;}
 }
private:
 Receive receive;Connection changed;
 uint32_t own=1,peer=0,fingerprint=0,lastRx=0,lastTx=0,byteAt=0,sentAt=0;
 uint16_t txSeq=1,lastSeq=0,ackSeq=0;
 uint8_t rx[43],tx[44],out[MaxMessage],in[MaxMessage];
 uint8_t txType=0,rxLength=0,txLength=0,txPosition=0,outKind=0,outLength=0,outOffset=0,chunk=0,attempts=0,inKind=0,inLength=0,inOffset=0;
 bool discard=false,messagePending=false,waiting=false,ackDue=false,helloDue=false;
 void restart(uint32_t now){if(peer&&changed)changed(false);peer=0;own+=0x9e3779b9UL;if(!own)own=1;lastRx=lastTx=now;txSeq=1;lastSeq=0;messagePending=waiting=ackDue=false;inOffset=0;helloDue=true;stats.resets++;}
 void frame(uint8_t type,uint16_t seq,const uint8_t* p,uint8_t n){
  txType=type;uint8_t raw[41];raw[0]=2;raw[1]=type;put32(raw+2,own);put32(raw+6,type==1?0:peer);put16(raw+10,seq);raw[12]=n;if(n)memcpy(raw+13,p,n);put32(raw+13+n,crc(raw,13+n));
  uint8_t length=uint8_t(17+n),code=1,codeAt=0,at=1;for(uint8_t i=0;i<length;i++){if(!raw[i]){tx[codeAt]=code;code=1;codeAt=at++;}else{tx[at++]=raw[i];++code;}}tx[codeAt]=code;tx[at++]=0;txLength=at;txPosition=0;
 }
 void decode(uint32_t now){
  uint8_t raw[41],at=0,pos=0;
  while(pos<rxLength){uint8_t code=rx[pos++];if(!code||uint16_t(pos)+code-1>rxLength){stats.malformed++;return;}for(uint8_t i=1;i<code;i++){if(at>=sizeof(raw)){stats.oversized++;return;}raw[at++]=rx[pos++];}if(code!=255&&pos<rxLength){if(at>=sizeof(raw)){stats.oversized++;return;}raw[at++]=0;}}
  if(at<17||raw[0]!=2||raw[12]>24||at!=uint8_t(17+raw[12])){stats.malformed++;return;}
  if(u32(raw+at-4)!=crc(raw,at-4)){stats.crcErrors++;return;}
  uint8_t type=raw[1],n=raw[12];uint32_t sender=u32(raw+2);uint16_t seq=u16(raw+10);const uint8_t* p=raw+13;
  if(type==1){
   if(!sender||n!=4){stats.malformed++;return;}if(u32(p)!=fingerprint){stats.schemaErrors++;return;}
   lastRx=now;stats.good++;
   if(sender!=peer){bool was=peer!=0;if(was&&changed)changed(false);peer=sender;txSeq=1;lastSeq=0;inOffset=0;messagePending=waiting=ackDue=false;helloDue=true;stats.resets++;if(changed)changed(true);}return;
  }
  if(!peer||sender!=peer||u32(raw+6)!=own)return;
  if(type==2){if(n||!messagePending||!waiting||seq!=txSeq)return;lastRx=now;stats.good++;waiting=false;attempts=0;outOffset+=chunk;++txSeq;if(!txSeq)txSeq=1;if(outOffset==outLength)messagePending=false;return;}
  if(type!=3||n<3||!seq){stats.malformed++;return;}
  if(seq==lastSeq){lastRx=now;ackSeq=seq;ackDue=true;stats.duplicates++;return;}
  uint16_t expected=uint16_t(lastSeq+1);if(!expected)expected=1;
  if(seq!=expected)return;
  uint8_t kind=p[0],offset=p[1],total=p[2],bytes=n-3;
  if(total>MaxMessage||uint16_t(offset)+bytes>total||(!bytes&&total)){stats.malformed++;return;}
  if(offset==0){inKind=kind;inLength=total;inOffset=0;}
  if(kind!=inKind||total!=inLength||offset!=inOffset){stats.malformed++;return;}
  if(bytes)memcpy(in+offset,p+3,bytes);inOffset+=bytes;lastSeq=seq;lastRx=now;ackSeq=seq;ackDue=true;stats.good++;
  if(inOffset==inLength){inOffset=0;if(receive)receive(kind,in,inLength);}
 }
};
}
