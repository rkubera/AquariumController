#pragma once
#include <stdint.h>
#include <math.h>
// Preserve chronological samples; a bounded stack copy replaces heap allocation.
inline float dhtMedian(const float* values,uint8_t count){
 if(!values||!count||count>20)return NAN;
 float sorted[20];
 for(uint8_t i=0;i<count;++i){
  float value=values[i];if(!isfinite(value))return NAN;
  uint8_t j=i;while(j&&sorted[j-1]>value){sorted[j]=sorted[j-1];--j;}sorted[j]=value;
 }
 return count&1 ? sorted[count/2] : (sorted[count/2-1]+sorted[count/2])*0.5f;
}

// DHT11 samples in tenths: half-step medians still retain 0.05 precision.
// The chronological arrays and the sorting scratch buffer use two bytes/sample.
inline float dhtMedianDeci(const uint16_t* values,uint8_t count){
 if(!values||!count||count>20)return NAN;
 uint16_t sorted[20];
 for(uint8_t i=0;i<count;++i){
  uint16_t value=values[i];uint8_t j=i;
  while(j&&sorted[j-1]>value){sorted[j]=sorted[j-1];--j;}sorted[j]=value;
 }
 return count&1 ? sorted[count/2]/10.0f : (uint32_t(sorted[count/2-1])+sorted[count/2])/20.0f;
}
