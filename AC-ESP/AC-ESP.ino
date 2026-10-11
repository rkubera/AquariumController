/* ESP Aquarium Controller bridge. Author: rkubera. License: see ../License.txt. */
// Feature selection belongs only to ESP. LED RGB and the software clock are always enabled.
#ifndef AC_RELAY_COUNT
#define AC_RELAY_COUNT 4
#endif
#ifndef AC_PWM_COUNT
#define AC_PWM_COUNT 0 //4
#endif
#ifndef AC_INPUT_COUNT
#define AC_INPUT_COUNT 0 //8
#endif
#ifndef AC_ENABLE_DHT
#define AC_ENABLE_DHT 1
#endif
#ifndef AC_ENABLE_RTC
#define AC_ENABLE_RTC 1
#endif
#ifndef AC_ENABLE_BUZZER
#define AC_ENABLE_BUZZER 1
#endif
#ifndef AC_ENABLE_FAN
#define AC_ENABLE_FAN 1
#endif
#include "PortalFeatures.h"

#define ARDPORTAL_ENABLE_DYNAMIC_PAGES 1
#define ARDPORTAL_ENABLE_DEPENDENCIES 1
#define ARDPORTAL_ENABLE_HA 1
#define ARDPORTAL_ENABLE_CONSOLE 1
#define ARDPORTAL_ENABLE_CONSOLE_MESSAGES 1
#define ARDPORTAL_ENABLE_MQTT 1
#define ARDPORTAL_ENABLE_MQTT_TLS 0
#define ARDPORTAL_ENABLE_OTA 1
//Compile only the controls used by this panel; library defaults include many
//unneeded entity types and consume ESP8266 RAM and flash.
#define ARDPORTAL_ENABLE_CONTROLS 0
#define ARDPORTAL_ENABLE_CONTROL_SLIDER 1
#define ARDPORTAL_ENABLE_CONTROL_TEXT 1
#define ARDPORTAL_ENABLE_CONTROL_SELECT 1
#define ARDPORTAL_ENABLE_CONTROL_EDIT 1
#define ARDPORTAL_ENABLE_CONTROL_BINARY_SENSOR 0
#define ARDPORTAL_ENABLE_CONTROL_SWITCH 1
#define ARDPORTAL_ENABLE_CONTROL_DATE 1
#define ARDPORTAL_ENABLE_CONTROL_TIME 1
#define ARDPORTAL_ENABLE_CONTROL_LIGHT 1
// UART selection. Diagnostic messages never share the controller UART.
#if defined(ESP32)
#define MEGA_SERIAL Serial2
#define MEGA_SERIAL_NAME "Serial2"
#define DIAGNOSTIC_SERIAL Serial
#define DIAGNOSTIC_SERIAL_NAME "Serial"
#elif defined(ESP8266)
#define MEGA_SERIAL Serial
#define MEGA_SERIAL_NAME "Serial"
#define DIAGNOSTIC_SERIAL Serial1
#define DIAGNOSTIC_SERIAL_NAME "Serial1"
#else
#error "Select an ESP32 or ESP8266 board."
#endif
static const uint32_t MEGA_SERIAL_BAUD=115200,DIAGNOSTIC_SERIAL_BAUD=115200;
static const bool SERIAL_DIAGNOSTICS_ENABLED=true;

#include "NetworkTrace.h"
#include <ArdPortal.h>
// Use the core's default loop stack. Heap reserves are managed by ArdPortal.
#include <time.h>
#include <bitset>

ArdPortal portal;
extern bool panelLinked,panelRefreshWanted,configMegaReady;
extern uint32_t panelLastRx;
extern uint16_t panelPendingIndex;
extern String panelPendingField,panelPendingValue;
#if defined(ESP32)
#include <esp_system.h>
#endif
#include "ReliableUart.h"
#include "WireQueue.h"
#include "WireFields.h"
#include "RestoreFields.h"
#include "FeatureFields.h"
#include "PanelMetadata.h"
#include "PanelWireIds.h"
void espWireReceive(uint8_t,const uint8_t*,uint8_t);
void espWireConnection(bool);
AcWire::Link wireLink(espWireReceive,espWireConnection);
WireQueue<512> bridgeWireQueue;
HardwareSerial& megaSerial=MEGA_SERIAL;
HardwareSerial& diagnosticSerial=DIAGNOSTIC_SERIAL;
#if defined(ESP32)
static const int MEGA_RX_PIN=16,MEGA_TX_PIN=17;
#else
static const int MEGA_RX_PIN=3,MEGA_TX_PIN=1;
#endif
uint32_t bridgeDroppedFrames=0,bridgeRxBytes=0;
bool bridgeWifiWasConnected=false,bridgeMqttWasConnected=false;
String bridgeLastHostname;
String bridgeTelemetry[AC_WIRE_FIELD_COUNT];
std::bitset<AC_WIRE_FIELD_COUNT> bridgeTelemetryDirty,bridgeTelemetrySeen;
bool bridgeDhtSent[2]={};uint32_t bridgeDhtLastSent[2]={};

void setup() {
  // Both ports are selected at the top of this sketch; reject accidental sharing.
  if(SERIAL_DIAGNOSTICS_ENABLED && &diagnosticSerial!=&megaSerial)diagnosticSerial.begin(DIAGNOSTIC_SERIAL_BAUD);
#if defined(ESP32)
  megaSerial.begin(MEGA_SERIAL_BAUD,SERIAL_8N1,MEGA_RX_PIN,MEGA_TX_PIN);
  wireLink.begin(esp_random(),AC_WIRE_SCHEMA,millis());
#ifdef LED_BUILTIN
  pinMode(LED_BUILTIN,OUTPUT);digitalWrite(LED_BUILTIN,HIGH);
#endif
#else
  megaSerial.setDebugOutput(false);
  megaSerial.begin(MEGA_SERIAL_BAUD,SERIAL_8N1);
  wireLink.begin(ESP.random(),AC_WIRE_SCHEMA,millis());
  // GPIO2 is Serial1 TX: do not change its pin mode for the onboard LED.
#endif
  panelSetup();
  portal.onMqttMessage(bridgeMqttMessage);
  ArdPortal::Options options;
  static String defaultName="mqtt-clnt-"+ArdPortal::chipId();
  options.deviceName=defaultName.c_str();
  options.haWorkBudgetMs=1;
  options.appConfigRegistrationWorkBudgetMs=1;
  options.appConfigRegistrationMaxOperationsPerLoop=1;
  options.haMaxOperationsPerLoop=1;
  options.discoveryIntervalMs=0; //Retained discovery: reconnect/HA birth only.
  options.appStateIntervalMs=0; //Mega owns ongoing state publications.
  if (!portal.begin(options)) portal.log(F("Portal initialization failed"));
#if defined(ESP32)
  WiFi.setSleep(false); // Permanently powered controller: avoid modem sleep latency.
  portal.log(String(F("ESP boot: resetReason="))+String(int(esp_reset_reason()))+" (1=power, 3=software, 4=panic, 5/6/7=watchdog, 9=brownout)");
#else
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  portal.log("ESP boot: "+ESP.getResetReason());
#endif
  espConfigSetup();
  portal.log(String(F("Serial: transport="))+String(MEGA_SERIAL_NAME)+String(F(", diagnostics="))+String(SERIAL_DIAGNOSTICS_ENABLED && &diagnosticSerial!=&megaSerial ? DIAGNOSTIC_SERIAL_NAME : "off"));
  portal.log(String(F("UART v2: "))+String(MEGA_SERIAL_NAME)+String(F(" baud="))+String(MEGA_SERIAL_BAUD)+String(F(" RX="))+String(MEGA_RX_PIN)+String(F(" TX="))+String(MEGA_TX_PIN)+String(F(" schema="))+String(AC_WIRE_SCHEMA,HEX));
}

void loop() {
  uint32_t loopStarted=millis();
  static uint32_t maxLoopMs=0,maxPortalMs=0,maxPanelMs=0,maxMqttMs=0,lastTimingReport=0;
  bridgeSerialService();
  static uint32_t lastUsbDiagnostic=0;
  bool usbDiagnosticDue=SERIAL_DIAGNOSTICS_ENABLED && &diagnosticSerial!=&megaSerial && millis()-lastUsbDiagnostic>=5000UL;
  if(usbDiagnosticDue){
    lastUsbDiagnostic=millis();char line[128];
    int net=snprintf(line,sizeof(line),"TCP calls=%lu errors=%lu errno=%d HTTP=%lu\n",(unsigned long)acSendCalls,(unsigned long)acSendErrors,acSendLastErrno,(unsigned long)acHttpSendCalls);
    if(net>0&&net<int(sizeof(line))&&diagnosticSerial.availableForWrite()>=net)diagnosticSerial.write((const uint8_t*)line,net);
#if defined(ESP32)
    const auto memory=ArdHeap::sample();
    int n=snprintf(line,sizeof(line),"ESP before portal up=%lu heap8=%lu dma=%lu stackFree=%lu link=%u config=%u\n",(unsigned long)(millis()/1000),(unsigned long)memory.free8,(unsigned long)memory.freeDma,(unsigned long)uxTaskGetStackHighWaterMark(nullptr),wireLink.connected(),configMegaReady);
    if(n>0&&n<int(sizeof(line))&&diagnosticSerial.availableForWrite()>=n)diagnosticSerial.write((const uint8_t*)line,n);
#else
    const auto memory=ArdHeap::sample();
    int n=snprintf(line,sizeof(line),"ESP up=%lu heap=%lu block=%lu link=%u config=%u\n",(unsigned long)(millis()/1000),(unsigned long)memory.free8,(unsigned long)memory.block8,wireLink.connected(),configMegaReady);
    if(n>0&&n<int(sizeof(line))&&diagnosticSerial.availableForWrite()>=n)diagnosticSerial.write((const uint8_t*)line,n);
#endif
  }
  uint32_t portalStarted=millis();
  portal.loop();
  uint32_t portalMs=millis()-portalStarted;
  if(usbDiagnosticDue){
    const auto memory=ArdHeap::sample();
    char line[128];int n=snprintf(line,sizeof(line),"ESP after portal ms=%lu heap=%lu block=%lu\n",(unsigned long)portalMs,(unsigned long)memory.free8,(unsigned long)memory.block8);
    if(n>0&&n<int(sizeof(line))&&diagnosticSerial.availableForWrite()>=n)diagnosticSerial.write((const uint8_t*)line,n);
  }
  if (portalMs>maxPortalMs) maxPortalMs=portalMs;
  // Drain commands accepted by HTTP/MQTT before background storage and rendering.
  bridgeWireQueue.drain(wireLink);
  bridgeSerialService();
  espConfigService();
  bridgeSerialService();
  bridgeWireQueue.drain(wireLink);
  bridgeConnectionService();
  uint32_t panelStarted=millis();
  panelService();
  uint32_t panelMs=millis()-panelStarted;
  if (panelMs>maxPanelMs) maxPanelMs=panelMs;
  bridgeWireQueue.drain(wireLink);
  bridgeSerialService();
  uint32_t mqttStarted=millis();
  bridgeMqttService();
  uint32_t mqttMs=millis()-mqttStarted;
  if(mqttMs>maxMqttMs)maxMqttMs=mqttMs;
  bridgeSerialService();
#if defined(ESP32)
  vTaskDelay(1); // Let lower-priority network/storage tasks run for one RTOS tick.
#else
  yield();
#endif
  uint32_t elapsed=millis()-loopStarted;
  if (elapsed>maxLoopMs) maxLoopMs=elapsed;
  if (millis()-lastTimingReport>=(wireLink.connected()?30000UL:10000UL)) {
    portal.log(String(F("Loop timing: uptime="))+String(millis()/1000)+String(F(" s, max="))+String(maxLoopMs)+String(F(" ms, portal="))+String(maxPortalMs)+String(F(" ms, panel="))+String(maxPanelMs)+String(F(" ms, mqtt="))+String(maxMqttMs)+" ms"+panelMemoryDiagnostic());
    portal.log(String(F("UART v2: link="))+String(wireLink.connected()?"on":"off")+String(F(" bytes="))+String(bridgeRxBytes)+String(F(" frames="))+String(wireLink.stats.good)+String(F(" malformed="))+String(wireLink.stats.malformed)+String(F(" timeout="))+String(wireLink.stats.timeouts)+String(F(" crc="))+String(wireLink.stats.crcErrors)+String(F(" retry="))+String(wireLink.stats.retries)+String(F(" duplicate="))+String(wireLink.stats.duplicates)+String(F(" schema="))+String(wireLink.stats.schemaErrors)+" config="+(configMegaReady?"ready":"waiting"));
    lastTimingReport=millis(); maxLoopMs=maxPortalMs=maxPanelMs=maxMqttMs=0;
  }
}
