// Remove the ten retired HA select entities; new Input entities are registered
// through the same panel schema as every other control.
void bridgeRemoveAggregationEntities(){
 static bool wasConnected=false;static byte index=0;static uint32_t nextAt=0;
 if(!portal.mqttConnected()){wasConnected=false;return;}
 if(!wasConnected){wasConnected=true;index=0;nextAt=millis()+1000;}
 static const char retiredIds[][11] PROGMEM={"M_90cacd69","M_7224ced2","M_d0650e9f","M_88ef1870","M_82ad3679","M_3fdb5da2","M_4757d4af","M_42503dc0","M_c0ecbfa5","M_e9363cae"};
 if(index>=10 || int32_t(millis()-nextAt)<0)return;
 String topic="homeassistant/select/ardui_"+portal.chipId()+String(F("/"))+String(FPSTR(retiredIds[index]))+"/config";
 if(portal.publish(topic.c_str(),"",true))++index;
 nextAt=millis()+100;
}
const String& bridgeHostname(){
 static String source,cached;static bool initialized=false;
 const auto& configured=portal.getPortalConfig().deviceName;
 if(!initialized||source!=configured){source=configured;cached=ArdDeviceName::mqtt(source);initialized=true;}
 return cached;
}
void getCommand(String request){
 const auto& c=portal.getPortalConfig();
 if(request=="timestamp"){time_t utc=time(nullptr);uint32_t value=utc>=1577836800LL&&uint64_t(utc)<UINT32_MAX?uint32_t(utc):0;sendCommand(String(F("timestamp "))+String(value));}
 else if(request=="wifistatus")sendCommand(portal.wifiConnected()?"wifi connected":"wifi not connected");
 else if(request=="mqttstatus")sendCommand(portal.mqttConnected()?"mqtt connected":"mqtt not connected");
 else if(request=="hostname")sendCommand("hostname "+bridgeHostname());
 else if(request=="ssid")sendCommand("ssid "+c.ssid);
 else if(request=="mqttserver")sendCommand("mqttserver "+c.host);
 else if(request=="echo")sendCommand("echo");
}
void bridgeConnectionService(){
 bridgeRemovePwmStateTopics();
 bool wifi=portal.wifiConnected(),mqtt=portal.mqttConnected();
 if(wireLink.connected()){
  if(wifi!=bridgeWifiWasConnected&&bridgeTxFree()>64){sendCommand(wifi?"wifi connected":"wifi not connected");bridgeWifiWasConnected=wifi;}
  if(mqtt!=bridgeMqttWasConnected&&bridgeTxFree()>64){sendCommand(mqtt?"mqtt connected":"mqtt not connected");bridgeMqttWasConnected=mqtt;}
  const String& name=bridgeHostname();if(portal.portalAndAppConfigReady()&&name.length()&&name!=bridgeLastHostname&&bridgeTxFree()>96){sendCommand("hostname "+name);sendCommand("ssid "+portal.getPortalConfig().ssid);bridgeLastHostname=name;}
 }
 panelLinked=wireLink.connected()&&configMegaReady;
 if(panelLinked)panelLastRx=millis();
 static String subscribedName;static bool wasMqtt=false;static byte subscriptionStep=0;
 if(!mqtt){wasMqtt=false;subscriptionStep=0;}
 if(mqtt){
  if(!wasMqtt||subscribedName!=bridgeHostname()){wasMqtt=true;subscriptionStep=0;subscribedName=bridgeHostname();for(uint16_t i=0;i<AC_WIRE_FIELD_COUNT;i++)if(bridgeTelemetrySeen[i])bridgeTelemetryDirty[i]=true;}
  if(subscriptionStep<2){String topic="cmnd/"+subscribedName+(subscriptionStep==0?"/ActualTime":"/ActualDate");if(portal.publish(topic.c_str(),"",true))++subscriptionStep;}
  else if(subscriptionStep<4){String topic=(subscriptionStep==2?"cmnd/":"get/")+subscribedName+"/+";if(portal.subscribe(topic.c_str()))++subscriptionStep;}
 }
}
void bridgeMqttService(){
 if(!portal.mqttConnected()||bridgeTelemetryDirty.none())return;
 // Limit telemetry bursts independently of UART and command acknowledgements.
 static uint32_t nextTelemetryAt=0;
 uint32_t now=millis();if(int32_t(now-nextTelemetryAt)<0)return;
 static uint16_t next=0;
 static const uint16_t temperatureId=acFieldId("InternalTemperature"),humidityId=acFieldId("InternalHumidity");
 for(uint16_t checked=0;checked<16&&checked<AC_WIRE_FIELD_COUNT;checked++){
  uint16_t i=next++;if(next>=AC_WIRE_FIELD_COUNT)next=0;if(!bridgeTelemetryDirty[i])continue;if(!acFieldSupported(i+1,portalFeatures)){bridgeTelemetryDirty[i]=false;continue;}
  String key=acFieldName(i+1);
  if(key==F("ClockRtcStatus")){bridgeTelemetryDirty[i]=false;continue;}
  if(key==F("LedState")||key==F("CurrentBrightness")){bridgeTelemetryDirty[i]=false;bridgeTelemetryDirty[acFieldId("CustomColor")-1]=true;return;}
  int dht=i+1==temperatureId?0:i+1==humidityId?1:-1;
  if(dht>=0&&bridgeDhtSent[dht]&&millis()-bridgeDhtLastSent[dht]<60000UL)continue;
  nextTelemetryAt=now+25;
  String topic="stat/"+bridgeHostname()+"/"+key;
  String payload=key==F("CustomColor")?ArdJSON::JSON.stringify(panelLedValue()):bridgeTelemetry[i];
  if(portal.publish(topic.c_str(),payload.c_str(),true)){bridgeTelemetryDirty[i]=false;bridgeTelemetrySeen[i]=true;if(dht>=0){bridgeDhtSent[dht]=true;bridgeDhtLastSent[dht]=millis();}}
  return;
 }
}
void bridgeMqttMessage(const String& topic,const uint8_t* payload,size_t length){
 String commandPrefix="cmnd/"+bridgeHostname()+"/",getPrefix="get/"+bridgeHostname()+"/";
 if(topic.startsWith(getPrefix)){panelRefreshWanted=true;return;}
 if(length>AcWire::Link::MaxMessage-6)return;
 String value=wireText(payload,uint8_t(length));if(value.length()!=length)return;
 if(!topic.startsWith(commandPrefix)||!length)return;
 String key=topic.substring(commandPrefix.length());if(key==F("LedSettings"))return;
 if(key.startsWith("M_"))return;
 if(key=="PublishAll"){value.toLowerCase();if(value==F("on")||value==F("all"))panelRefreshWanted=true;return;}
 panelMqttSetting(key,value);
}

void bridgeRemoveRetiredStateEntities(){
 static bool connected=false;static byte index=0;static uint32_t nextAt=0;
 if(!portal.mqttConnected()){connected=false;return;}
 if(!connected){connected=true;index=0;nextAt=millis()+2500;}
 static const char ids[][25] PROGMEM={"binary_sensor/M_9ab64b8a","select/M_42e0e837","binary_sensor/M_3a4880a5","select/M_756e63ae","binary_sensor/M_ae20f4b4","select/M_2a61cbdd","binary_sensor/M_3b0814c7","select/M_8684b82c","binary_sensor/M_ec2402fa","select/M_ac1849e7","binary_sensor/M_ac9ab415","select/M_02ef3fde","binary_sensor/M_2622f1e4","select/M_0f232d8d","binary_sensor/M_a2000db7","select/M_c3de379c","binary_sensor/M_d6ad6036","select/M_90d36b4b","binary_sensor/M_3ab00671","select/M_8a2f5652","binary_sensor/M_14e7f87f","select/M_11516c44","select/M_fbb23b5c","select/M_4718b314","select/M_04365844","select/M_7f32a382","select/M_eb737f4b","select/M_620f21c7","select/M_3007435f","select/M_eafd667d","select/M_038943ca","select/M_05ccfab6","select/M_5f596df6","select/M_e0f03a54","select/M_d63d89d1","select/M_6b4f5a99","select/M_4d091ad9","select/M_4c7c06f7","text/M_04b49b1e","text/M_e46cf686","text/M_3bc57e1e","text/M_00f96f58","text/M_050550f3","text/M_8c8e46a6","switch/M_ec2402fa","switch/M_ac9ab415","switch/M_2622f1e4","switch/M_a2000db7","switch/M_d6ad6036","switch/M_3ab00671"};
 if(index>=sizeof(ids)/sizeof(ids[0])||int32_t(millis()-nextAt)<0)return;
 String item=String(FPSTR(ids[index])),component=item.substring(0,item.indexOf('/')),id=item.substring(item.indexOf('/')+1);
 String topic="homeassistant/"+component+"/ardui_"+portal.chipId()+"/"+id+"/config";
 if(portal.publish(topic.c_str(),"",true))++index;nextAt=millis()+100;
}

void bridgeRemovePwmStateTopics(){
 static bool connected=false;static byte index=0;static uint32_t nextAt=0;
 if(!portal.mqttConnected()){connected=false;return;}
 if(!connected){connected=true;index=0;nextAt=millis()+2500;}
 if(index>=12 || int32_t(millis()-nextAt)<0)return;
 String topic=(index<6?"stat/":"cmnd/")+bridgeHostname()+String(F("/PWMOutput"))+String(index%6+1)+"State";
 if(portal.publish(topic.c_str(),"",true))++index;
 nextAt=millis()+100;
}

