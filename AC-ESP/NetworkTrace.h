#pragma once
// Sketch-only observation of nonblocking transport writes; library stays untouched.
#ifdef ESP32
#include <WiFi.h>
#include <lwip/sockets.h>
#include <errno.h>

#endif
static uint32_t acSendCalls=0,acSendErrors=0,acHttpSendCalls=0;
static int acSendLastErrno=0;
#ifdef ESP32
static int acObserveSend(int fd,const void* bytes,size_t length,int flags){
  int result=::send(fd,bytes,length,flags);++acSendCalls;
  if(length>=5&&!memcmp(bytes,"HTTP/",5))++acHttpSendCalls;
  if(result<0){
    int saved=errno;acSendLastErrno=saved;++acSendErrors;
    if(saved!=EAGAIN&&saved!=EWOULDBLOCK&&saved!=EINTR){
      char line[96];int n=snprintf(line,sizeof(line),"TCP send error fd=%d errno=%d bytes=%u up=%lu\n",fd,saved,unsigned(length),(unsigned long)(millis()/1000));
      if(SERIAL_DIAGNOSTICS_ENABLED && &DIAGNOSTIC_SERIAL!=&MEGA_SERIAL && n>0&&n<int(sizeof(line))&&DIAGNOSTIC_SERIAL.availableForWrite()>=n)DIAGNOSTIC_SERIAL.write((const uint8_t*)line,n);
    }
    errno=saved;
  }
  return result;
}
#define send acObserveSend
#include <NetworkIo.h>
#undef send

#endif
