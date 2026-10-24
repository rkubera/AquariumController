#include <cassert>
#include <vector>
#include <deque>
#include <cstdio>
#include <random>
#include "../ReliableUart.h"
#include "../../AC-ATMEGA/ConfigStorage.h"
using namespace AcWire;
std::vector<std::vector<uint8_t>> receivedA,receivedB;
int connectionsA=0,connectionsB=0;
void recvA(uint8_t k,const uint8_t* p,uint8_t n){assert(k==77);receivedA.emplace_back(p,p+n);}
void recvB(uint8_t k,const uint8_t* p,uint8_t n){assert(k==77);receivedB.emplace_back(p,p+n);}
void changedA(bool c){if(c)connectionsA++;}
void changedB(bool c){if(c)connectionsB++;}
struct Port{std::deque<uint8_t> bytes;int availableForWrite(){return 64;}void write(uint8_t v){bytes.push_back(v);}};
void transfer(Port& p,Link& l,uint32_t now,bool& drop,bool& damage,std::vector<uint8_t>& packet){while(!p.bytes.empty()){uint8_t b=p.bytes.front();p.bytes.pop_front();packet.push_back(b);if(!b){assert(packet.size()<=43);if(drop)drop=false;else{if(damage&&packet.size()>20){packet[packet.size()-6]^=4;damage=false;}for(uint8_t v:packet)l.input(v,now);}packet.clear();}}}
void pump(Link& a,Link& b,Port& pa,Port& pb,uint32_t& now,unsigned count,bool dropA=false,bool dropB=false,bool damageA=false){std::vector<uint8_t> xa,xb;bool damageB=false;for(unsigned i=0;i<count;i++){now++;a.service(pa,now);b.service(pb,now);transfer(pa,b,now,dropA,damageA,xa);transfer(pb,a,now,dropB,damageB,xb);}assert(xa.empty()&&xb.empty());}
int main(){
 const uint8_t check[]={'1','2','3','4','5','6','7','8','9'};assert(crc(check,9)==0xcbf43926);
 Link a(recvA,changedA),b(recvB,changedB);Port pa,pb;uint32_t now=0;a.begin(111,123,now);b.begin(222,123,now);pump(a,b,pa,pb,now,20);assert(a.connected()&&b.connected());
 for(int length=0;length<=160;length++){std::vector<uint8_t> v(length);for(int i=0;i<length;i++)v[i]=uint8_t(i*37);assert(a.send(77,v.data(),v.size()));pump(a,b,pa,pb,now,200);assert(a.idle());assert(receivedB.back()==v);}
 size_t before=receivedB.size();uint8_t data[160];for(int i=0;i<160;i++)data[i]=i;
 for(int n=0;n<=160;n++){uint8_t reference[164];uint32_t previous=uint32_t(n)*0xabcdef13UL;put32(reference,previous);memcpy(reference+4,data,n);assert(configDigest(previous,data,n)==crc(reference,n+4));}
 assert(a.send(77,data,160));pump(a,b,pa,pb,now,600,false,true);assert(a.idle()&&receivedB.size()==before+1&&b.stats.duplicates>0);
 assert(a.send(77,data,160));pump(a,b,pa,pb,now,600,true,false);assert(a.idle());
 assert(a.send(77,data,160));pump(a,b,pa,pb,now,600,false,false,true);assert(a.idle()&&b.stats.crcErrors>0);
 assert(a.send(77,data,160)&&b.send(77,data,160));pump(a,b,pa,pb,now,300);assert(a.idle()&&b.idle());
 for(int i=0;i<66000;i++){assert(a.send(77,data,1));pump(a,b,pa,pb,now,4);}assert(a.idle());
 // Stale packets from a previous boot must not execute after peer re-handshake.
 assert(a.send(77,data,1));a.service(pa,++now);std::vector<uint8_t> stale(pa.bytes.begin(),pa.bytes.end());pa.bytes.clear();a.begin(333,123,now);pump(a,b,pa,pb,now,2200);before=receivedB.size();for(uint8_t v:stale)b.input(v,++now);pump(a,b,pa,pb,now,20);assert(receivedB.size()==before);
 std::mt19937 rng(42);for(int i=0;i<50000;i++)b.input(uint8_t(rng()),++now);b.input(0,now);assert(receivedB.size()==before);
 // Header/schema mismatches never establish a session.
 Link incompatible(recvA,changedA);Port pc;incompatible.begin(999,456,now);pump(incompatible,b,pc,pb,now,100);assert(!incompatible.connected());
 // Runtime settings never survive reconstruction and cannot write outside RAM.
 {ConfigStorage ram;ram.begin();for(int address:{0,111,112,431,432,551,552,815,816,943,944,955}){ram.update(address,42);assert(ram.read(address)==42);}ram.update(-1,7);ram.update(ConfigStorage::PAYLOAD_SIZE,7);assert(ram.read(-1)==255&&ram.read(ConfigStorage::PAYLOAD_SIZE)==255);ram.update(59,1);ConfigStorage cold;cold.begin();assert(cold.read(59)==255);}
 puts("PASS: COBS/CRC, fragmentation, dropped DATA/ACK, corruption, duplicates, bidirectional traffic, sequence wrap, sessions, noise/schema mismatch; RAM-only configuration.");
}
