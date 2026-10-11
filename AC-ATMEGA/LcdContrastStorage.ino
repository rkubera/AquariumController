#include <avr/eeprom.h>
#include <util/atomic.h>
#include "LocalContrastStore.h"
// Local-only storage: the final 32 EEPROM bytes never enter the ESP config image.
uint8_t lcdContrastRead(uint16_t address){return eeprom_read_byte((const uint8_t*)address);}
bool lcdContrastWriteReady(){return !(EECR&_BV(EEPE));}
void lcdContrastWriteStart(uint16_t address,uint8_t value){
 ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
  EEAR=address;EEDR=value;
  EECR&=~(_BV(EEPM0)|_BV(EEPM1));
  EECR|=_BV(EEMPE);EECR|=_BV(EEPE);
 }
}
LocalContrastStore lcdContrastStore(lcdContrastRead,lcdContrastWriteReady,lcdContrastWriteStart,E2END+1-32);
void lcdContrastLoad(){lcdContrast=lcdContrastStore.begin(40);}
void lcdContrastChanged(){lcdContrastStore.changed(lcdContrast,millis());}
void lcdContrastStorageService(){lcdContrastStore.service(millis());}
