#pragma once
// Select before evaluating value expressions: snapshot/readback must not
// repeatedly format all the other channel values on the AVR.
bool linkFieldWanted(uint16_t field);
void reportSelectedField(uint16_t field,String value);
void reportSelectedField(uint16_t field,String value,bool callCriticalEvent);
#define reportField(field,...) do { const uint16_t acReportFieldId=(field); if(linkFieldWanted(acReportFieldId)) reportSelectedField(acReportFieldId,__VA_ARGS__); } while(0)
