#!/usr/bin/env python3
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
source=r'''
#include <cassert>
#include <algorithm>
#include <cstring>
#include "DhtMedian.h"
int main(){
 float values[20],original[20],ordered[20];
 for(unsigned round=0;round<30;++round)for(unsigned count=1;count<=20;++count){
  for(unsigned i=0;i<count;++i)values[i]=float((i*7+round*13)%31)-10.5f;
  memcpy(original,values,sizeof(values));memcpy(ordered,values,sizeof(values));std::sort(ordered,ordered+count);
  float expected=count%2?ordered[count/2]:(ordered[count/2-1]+ordered[count/2])/2;
  assert(dhtMedian(values,count)==expected);assert(!memcmp(original,values,sizeof(values)));
 }
 assert(isnan(dhtMedian(nullptr,1)));assert(isnan(dhtMedian(values,0)));assert(isnan(dhtMedian(values,21)));
 values[0]=NAN;assert(isnan(dhtMedian(values,1)));
 float outlier[]={25,26,99,24,25};assert(dhtMedian(outlier,5)==25);
 uint16_t deci[20],copy[20],sorted[20];
 for(unsigned round=0;round<30;++round)for(unsigned n=1;n<=20;++n){
  for(unsigned i=0;i<20;++i)deci[i]=(i*73+round*127)%1001;
  memcpy(copy,deci,sizeof(deci));memcpy(sorted,deci,sizeof(deci));std::sort(sorted,sorted+n);
  float expected=n%2?sorted[n/2]/10.0f:(uint32_t(sorted[n/2-1])+sorted[n/2])/20.0f;
  assert(fabs(dhtMedianDeci(deci,n)-expected)<0.0001f);assert(!memcmp(copy,deci,sizeof(deci)));
 }
 assert(isnan(dhtMedianDeci(nullptr,1)));assert(isnan(dhtMedianDeci(deci,0)));assert(isnan(dhtMedianDeci(deci,21)));
 uint16_t half[]={241,242};assert(fabs(dhtMedianDeci(half,2)-24.15f)<0.0001f);
 uint16_t maximum[]={65535,65535};assert(fabs(dhtMedianDeci(maximum,2)-6553.5f)<0.0001f);

}
'''
with tempfile.TemporaryDirectory(prefix='ac-dht-median-') as t:
 t=Path(t);(t/'test.cpp').write_text(source)
 subprocess.run(['c++','-std=c++11','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-I',str(root),str(t/'test.cpp'),'-o',str(t/'test')],check=True)
 subprocess.run([str(t/'test')],check=True)
print('PASS: DHT median, odd/even windows 1–20, outliers, preserved sample order and invalid input')
