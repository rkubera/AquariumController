// Generated shared Portal/HA definitions, filtered by ESP feature defines.
#if 1
static const char PANEL_PAGE_0[] PROGMEM =
R"JSON({"id":"led","name":"LED","order":0,"fields":[)JSON"
R"JSON({"type":"light","name":"LED: Status","id":"M_72085391","persist":false,"default":{"state":"OFF","brightness":0,"color":{"r":255,"g":255,"b":255},"color_mode":"rgb"},"ha":{"state_topic":"stat/$device/CustomColor","command_topic":"cmnd/$device/CustomColor","schema":"json","brightness":true,"brightness_scale":100,"supported_color_modes":["rgb"]},"colorLabel":"Custom Color","controls":[{"key":"state","type":"switch","name":"LED: State","command":"","on":"ON","off":"OFF"},{"key":"brightness","type":"slider","name":"LED: Current brightness (%)","command":"","min":0,"max":100,"step":1},{"key":"color","type":"color","name":"LED: CustomColor","command":""}]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"LED: Mode","id":"M_b2e7d268","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/LedControlMode","command_topic":"cmnd/$device/LedControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"Manual"})JSON"
R"JSON(,{"value":"partofday","name":"Time of day"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"LED: Manual color","id":"M_f35844ff","persist":true,"default":"black","ha":{"state_topic":"stat/$device/LedManualMode","command_topic":"cmnd/$device/LedManualMode","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"manual"},"options":[)JSON"
R"JSON({"value":"black","name":"Black"})JSON"
R"JSON(,{"value":"red","name":"Red"})JSON"
R"JSON(,{"value":"green","name":"Green"})JSON"
R"JSON(,{"value":"blue","name":"Blue"})JSON"
R"JSON(,{"value":"cyan","name":"Cyan"})JSON"
R"JSON(,{"value":"magenta","name":"Magenta"})JSON"
R"JSON(,{"value":"yellow","name":"Yellow"})JSON"
R"JSON(,{"value":"white","name":"White"})JSON"
R"JSON(,{"value":"wave","name":"Wave"})JSON"
R"JSON(,{"value":"custom","name":"Custom"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"LED: Morning — color","id":"M_e4365a67","persist":true,"default":"black","ha":{"state_topic":"stat/$device/LedColorMorning","command_topic":"cmnd/$device/LedColorMorning","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"},"options":[)JSON"
R"JSON({"value":"black","name":"Black"})JSON"
R"JSON(,{"value":"red","name":"Red"})JSON"
R"JSON(,{"value":"green","name":"Green"})JSON"
R"JSON(,{"value":"blue","name":"Blue"})JSON"
R"JSON(,{"value":"cyan","name":"Cyan"})JSON"
R"JSON(,{"value":"magenta","name":"Magenta"})JSON"
R"JSON(,{"value":"yellow","name":"Yellow"})JSON"
R"JSON(,{"value":"white","name":"White"})JSON"
R"JSON(,{"value":"wave","name":"Wave"})JSON"
R"JSON(,{"value":"custom","name":"Custom"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"LED: Morning — brightness (%)","min":0,"max":100,"step":1,"id":"M_80e08794","persist":true,"default":0,"ha":{"state_topic":"stat/$device/MorningBrightness","command_topic":"cmnd/$device/MorningBrightness","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"LED: Afternoon — color","id":"M_71bf461f","persist":true,"default":"black","ha":{"state_topic":"stat/$device/LedColorAfternoon","command_topic":"cmnd/$device/LedColorAfternoon","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"},"options":[)JSON"
R"JSON({"value":"black","name":"Black"})JSON"
R"JSON(,{"value":"red","name":"Red"})JSON"
R"JSON(,{"value":"green","name":"Green"})JSON"
R"JSON(,{"value":"blue","name":"Blue"})JSON"
R"JSON(,{"value":"cyan","name":"Cyan"})JSON"
R"JSON(,{"value":"magenta","name":"Magenta"})JSON"
R"JSON(,{"value":"yellow","name":"Yellow"})JSON"
R"JSON(,{"value":"white","name":"White"})JSON"
R"JSON(,{"value":"wave","name":"Wave"})JSON"
R"JSON(,{"value":"custom","name":"Custom"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"LED: Afternoon — brightness (%)","min":0,"max":100,"step":1,"id":"M_133c350c","persist":true,"default":0,"ha":{"state_topic":"stat/$device/AfternoonBrightness","command_topic":"cmnd/$device/AfternoonBrightness","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"LED: Evening — color","id":"M_c13e66e7","persist":true,"default":"black","ha":{"state_topic":"stat/$device/LedColorEvening","command_topic":"cmnd/$device/LedColorEvening","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"},"options":[)JSON"
R"JSON({"value":"black","name":"Black"})JSON"
R"JSON(,{"value":"red","name":"Red"})JSON"
R"JSON(,{"value":"green","name":"Green"})JSON"
R"JSON(,{"value":"blue","name":"Blue"})JSON"
R"JSON(,{"value":"cyan","name":"Cyan"})JSON"
R"JSON(,{"value":"magenta","name":"Magenta"})JSON"
R"JSON(,{"value":"yellow","name":"Yellow"})JSON"
R"JSON(,{"value":"white","name":"White"})JSON"
R"JSON(,{"value":"wave","name":"Wave"})JSON"
R"JSON(,{"value":"custom","name":"Custom"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"LED: Evening — brightness (%)","min":0,"max":100,"step":1,"id":"M_ba525294","persist":true,"default":0,"ha":{"state_topic":"stat/$device/EveningBrightness","command_topic":"cmnd/$device/EveningBrightness","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"LED: Night — color","id":"M_c09682f9","persist":true,"default":"black","ha":{"state_topic":"stat/$device/LedColorNight","command_topic":"cmnd/$device/LedColorNight","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"},"options":[)JSON"
R"JSON({"value":"black","name":"Black"})JSON"
R"JSON(,{"value":"red","name":"Red"})JSON"
R"JSON(,{"value":"green","name":"Green"})JSON"
R"JSON(,{"value":"blue","name":"Blue"})JSON"
R"JSON(,{"value":"cyan","name":"Cyan"})JSON"
R"JSON(,{"value":"magenta","name":"Magenta"})JSON"
R"JSON(,{"value":"yellow","name":"Yellow"})JSON"
R"JSON(,{"value":"white","name":"White"})JSON"
R"JSON(,{"value":"wave","name":"Wave"})JSON"
R"JSON(,{"value":"custom","name":"Custom"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"LED: Night — brightness (%)","min":0,"max":100,"step":1,"id":"M_03f6f016","persist":true,"default":0,"ha":{"state_topic":"stat/$device/NightBrightness","command_topic":"cmnd/$device/NightBrightness","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"}})JSON"
R"JSON(]})JSON"
;
#endif
#if AC_ACTIVE_RELAYS > 0
static const char PANEL_PAGE_1[] PROGMEM =
R"JSON({"id":"relays","name":"Relays","order":10,"fields":[)JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON({"type":"edit","name":"R1: Name","id":"M_22be3062","persist":true,"default":"","ha":{"state_topic":"stat/$device/Relay1Name","command_topic":"cmnd/$device/Relay1Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1
#if (AC_ACTIVE_RELAYS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"R1: Mode","id":"M_ee1455c9","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/Relay1ControlMode","command_topic":"cmnd/$device/Relay1ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"Manual"})JSON"
R"JSON(,{"value":"partofday","name":"Time of day"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"Threshold"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1
#if (AC_ACTIVE_RELAYS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R1: State","id":"M_9ab64b8a","persist":false,"ha":{"state_topic":"stat/$device/Relay1State","command_topic":"cmnd/$device/Relay1State","payload_on":"on","payload_off":"off"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"R1: Direction","id":"M_b97662b7","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/Relay1ControlDirection","command_topic":"cmnd/$device/Relay1ControlDirection","optimistic":true},"visibleWhen":{"field":"M_ee1455c9","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"direct","name":"Direct"})JSON"
R"JSON(,{"value":"reverse","name":"Reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"R1: Input","id":"M_b5dfdf8f","persist":true,"default":"none","ha":{"state_topic":"stat/$device/Relay1Input","command_topic":"cmnd/$device/Relay1Input","optimistic":true},"visibleWhen":{"field":"M_ee1455c9","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"none","name":"None"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"Sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"Sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"Sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"Sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"Sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"Sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"Sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"Sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"R1: Setpoint","id":"M_805a6e2a","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay1SensorsSetpoint","command_topic":"cmnd/$device/Relay1SensorsSetpoint","optimistic":true},"visibleWhen":{"field":"M_ee1455c9","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"R1: Hysteresis half-width","id":"M_bf8a6d7c","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay1MaxDeviation","command_topic":"cmnd/$device/Relay1MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_ee1455c9","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R1: Morning","id":"M_fbb23b5c","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay1MorningMode","command_topic":"cmnd/$device/Relay1MorningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ee1455c9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R1: Afternoon","id":"M_4718b314","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay1AfternoonMode","command_topic":"cmnd/$device/Relay1AfternoonMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ee1455c9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R1: Evening","id":"M_04365844","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay1EveningMode","command_topic":"cmnd/$device/Relay1EveningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ee1455c9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R1: Night","id":"M_7f32a382","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay1NightMode","command_topic":"cmnd/$device/Relay1NightMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ee1455c9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"R1: SensorsValue","id":"M_05dc2f55","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Relay1SensorsValue"},"visibleWhen":{"field":"M_ee1455c9","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"R2: Name","id":"M_307edc53","persist":true,"default":"","ha":{"state_topic":"stat/$device/Relay2Name","command_topic":"cmnd/$device/Relay2Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"R2: Mode","id":"M_519ed402","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/Relay2ControlMode","command_topic":"cmnd/$device/Relay2ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"Manual"})JSON"
R"JSON(,{"value":"partofday","name":"Time of day"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"Threshold"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R2: State","id":"M_3a4880a5","persist":false,"ha":{"state_topic":"stat/$device/Relay2State","command_topic":"cmnd/$device/Relay2State","payload_on":"on","payload_off":"off"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"R2: Direction","id":"M_ba03507a","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/Relay2ControlDirection","command_topic":"cmnd/$device/Relay2ControlDirection","optimistic":true},"visibleWhen":{"field":"M_519ed402","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"direct","name":"Direct"})JSON"
R"JSON(,{"value":"reverse","name":"Reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"R2: Input","id":"M_16230834","persist":true,"default":"none","ha":{"state_topic":"stat/$device/Relay2Input","command_topic":"cmnd/$device/Relay2Input","optimistic":true},"visibleWhen":{"field":"M_519ed402","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"none","name":"None"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"Sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"Sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"Sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"Sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"Sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"Sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"Sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"Sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"R2: Setpoint","id":"M_f6501d79","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay2SensorsSetpoint","command_topic":"cmnd/$device/Relay2SensorsSetpoint","optimistic":true},"visibleWhen":{"field":"M_519ed402","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"R2: Hysteresis half-width","id":"M_31618a11","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay2MaxDeviation","command_topic":"cmnd/$device/Relay2MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_519ed402","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R2: Morning","id":"M_eb737f4b","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay2MorningMode","command_topic":"cmnd/$device/Relay2MorningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_519ed402","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R2: Afternoon","id":"M_620f21c7","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay2AfternoonMode","command_topic":"cmnd/$device/Relay2AfternoonMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_519ed402","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R2: Evening","id":"M_3007435f","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay2EveningMode","command_topic":"cmnd/$device/Relay2EveningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_519ed402","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R2: Night","id":"M_eafd667d","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay2NightMode","command_topic":"cmnd/$device/Relay2NightMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_519ed402","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"R2: SensorsValue","id":"M_59dde76c","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Relay2SensorsValue"},"visibleWhen":{"field":"M_519ed402","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"R3: Name","id":"M_9de298e8","persist":true,"default":"","ha":{"state_topic":"stat/$device/Relay3Name","command_topic":"cmnd/$device/Relay3Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"R3: Mode","id":"M_bd5e2447","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/Relay3ControlMode","command_topic":"cmnd/$device/Relay3ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"Manual"})JSON"
R"JSON(,{"value":"partofday","name":"Time of day"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"Threshold"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R3: State","id":"M_ae20f4b4","persist":false,"ha":{"state_topic":"stat/$device/Relay3State","command_topic":"cmnd/$device/Relay3State","payload_on":"on","payload_off":"off"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"R3: Direction","id":"M_cd2560e5","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/Relay3ControlDirection","command_topic":"cmnd/$device/Relay3ControlDirection","optimistic":true},"visibleWhen":{"field":"M_bd5e2447","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"direct","name":"Direct"})JSON"
R"JSON(,{"value":"reverse","name":"Reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"R3: Input","id":"M_519c2d81","persist":true,"default":"none","ha":{"state_topic":"stat/$device/Relay3Input","command_topic":"cmnd/$device/Relay3Input","optimistic":true},"visibleWhen":{"field":"M_bd5e2447","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"none","name":"None"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"Sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"Sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"Sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"Sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"Sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"Sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"Sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"Sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"R3: Setpoint","id":"M_2f5c7b1c","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay3SensorsSetpoint","command_topic":"cmnd/$device/Relay3SensorsSetpoint","optimistic":true},"visibleWhen":{"field":"M_bd5e2447","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"R3: Hysteresis half-width","id":"M_a6513792","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay3MaxDeviation","command_topic":"cmnd/$device/Relay3MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_bd5e2447","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R3: Morning","id":"M_038943ca","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay3MorningMode","command_topic":"cmnd/$device/Relay3MorningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_bd5e2447","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R3: Afternoon","id":"M_05ccfab6","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay3AfternoonMode","command_topic":"cmnd/$device/Relay3AfternoonMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_bd5e2447","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R3: Evening","id":"M_5f596df6","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay3EveningMode","command_topic":"cmnd/$device/Relay3EveningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_bd5e2447","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R3: Night","id":"M_e0f03a54","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay3NightMode","command_topic":"cmnd/$device/Relay3NightMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_bd5e2447","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"R3: SensorsValue","id":"M_1cffb733","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Relay3SensorsValue"},"visibleWhen":{"field":"M_bd5e2447","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"R4: Name","id":"M_d424d409","persist":true,"default":"","ha":{"state_topic":"stat/$device/Relay4Name","command_topic":"cmnd/$device/Relay4Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"R4: Mode","id":"M_ad328a90","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/Relay4ControlMode","command_topic":"cmnd/$device/Relay4ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"Manual"})JSON"
R"JSON(,{"value":"partofday","name":"Time of day"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"Threshold"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R4: State","id":"M_3b0814c7","persist":false,"ha":{"state_topic":"stat/$device/Relay4State","command_topic":"cmnd/$device/Relay4State","payload_on":"on","payload_off":"off"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"R4: Direction","id":"M_cdc7b6c0","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/Relay4ControlDirection","command_topic":"cmnd/$device/Relay4ControlDirection","optimistic":true},"visibleWhen":{"field":"M_ad328a90","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"direct","name":"Direct"})JSON"
R"JSON(,{"value":"reverse","name":"Reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 4) || (AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"R4: Input","id":"M_ce562ce6","persist":true,"default":"none","ha":{"state_topic":"stat/$device/Relay4Input","command_topic":"cmnd/$device/Relay4Input","optimistic":true},"visibleWhen":{"field":"M_ad328a90","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"none","name":"None"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"Sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"Sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"Sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"Sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"Sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"Sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"Sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"Sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 4) || (AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"R4: Setpoint","id":"M_ad33f4db","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay4SensorsSetpoint","command_topic":"cmnd/$device/Relay4SensorsSetpoint","optimistic":true},"visibleWhen":{"field":"M_ad328a90","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 4) || (AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"R4: Hysteresis half-width","id":"M_0e147057","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay4MaxDeviation","command_topic":"cmnd/$device/Relay4MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_ad328a90","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 4) || (AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R4: Morning","id":"M_d63d89d1","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay4MorningMode","command_topic":"cmnd/$device/Relay4MorningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ad328a90","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 4) || (AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R4: Afternoon","id":"M_6b4f5a99","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay4AfternoonMode","command_topic":"cmnd/$device/Relay4AfternoonMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ad328a90","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 4) || (AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R4: Evening","id":"M_4d091ad9","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay4EveningMode","command_topic":"cmnd/$device/Relay4EveningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ad328a90","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 4) || (AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"R4: Night","id":"M_4c7c06f7","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay4NightMode","command_topic":"cmnd/$device/Relay4NightMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ad328a90","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_RELAYS >= 1) || (AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 2) || (AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 3) || (AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_RELAYS >= 4) || (AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"R4: SensorsValue","id":"M_0be674fa","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Relay4SensorsValue"},"visibleWhen":{"field":"M_ad328a90","equals":"treshold"}})JSON"
#endif
R"JSON(]})JSON"
;
#endif
#if 1
static const char PANEL_PAGE_2[] PROGMEM =
R"JSON({"id":"schedule","name":"Schedule","order":20,"fields":[)JSON"
R"JSON({"type":"select","name":"Schedule: Mode","id":"M_e9abe86f","persist":true,"default":"daily","ha":{"state_topic":"stat/$device/ScheduleMode","command_topic":"cmnd/$device/ScheduleMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"daily","name":"Every day"})JSON"
R"JSON(,{"value":"weekdayweekend","name":"Mon–Fri and weekend"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Schedule: Every day — Morning","id":"M_04b49b1e","persist":true,"default":"09:00:00","ha":{"state_topic":"stat/$device/MorningTime","command_topic":"cmnd/$device/MorningTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"daily"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Schedule: Every day — Afternoon","id":"M_e46cf686","persist":true,"default":"10:00:00","ha":{"state_topic":"stat/$device/AfternoonTime","command_topic":"cmnd/$device/AfternoonTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"daily"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Schedule: Every day — Evening","id":"M_3bc57e1e","persist":true,"default":"19:00:00","ha":{"state_topic":"stat/$device/EveningTime","command_topic":"cmnd/$device/EveningTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"daily"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Schedule: Every day — Night","id":"M_00f96f58","persist":true,"default":"22:00:00","ha":{"state_topic":"stat/$device/NightTime","command_topic":"cmnd/$device/NightTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"daily"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Schedule: Mon–Fri — Morning","id":"M_343dbe8e","persist":true,"default":"09:00:00","ha":{"state_topic":"stat/$device/WeekdayMorningTime","command_topic":"cmnd/$device/WeekdayMorningTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Schedule: Mon–Fri — Afternoon","id":"M_c1b27a56","persist":true,"default":"10:00:00","ha":{"state_topic":"stat/$device/WeekdayAfternoonTime","command_topic":"cmnd/$device/WeekdayAfternoonTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Schedule: Mon–Fri — Evening","id":"M_ddf379ce","persist":true,"default":"19:00:00","ha":{"state_topic":"stat/$device/WeekdayEveningTime","command_topic":"cmnd/$device/WeekdayEveningTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Schedule: Mon–Fri — Night","id":"M_6464bdc8","persist":true,"default":"22:00:00","ha":{"state_topic":"stat/$device/WeekdayNightTime","command_topic":"cmnd/$device/WeekdayNightTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Schedule: Weekend — Morning","id":"M_3685740b","persist":true,"default":"09:00:00","ha":{"state_topic":"stat/$device/WeekendMorningTime","command_topic":"cmnd/$device/WeekendMorningTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Schedule: Weekend — Afternoon","id":"M_66b258d3","persist":true,"default":"10:00:00","ha":{"state_topic":"stat/$device/WeekendAfternoonTime","command_topic":"cmnd/$device/WeekendAfternoonTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Schedule: Weekend — Evening","id":"M_a13b8513","persist":true,"default":"19:00:00","ha":{"state_topic":"stat/$device/WeekendEveningTime","command_topic":"cmnd/$device/WeekendEveningTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Schedule: Weekend — Night","id":"M_ec17d675","persist":true,"default":"22:00:00","ha":{"state_topic":"stat/$device/WeekendNightTime","command_topic":"cmnd/$device/WeekendNightTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"Schedule: Current phase","id":"M_756f8567","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/ActualPartOfDay"}})JSON"
R"JSON(]})JSON"
;
#endif
#if AC_ACTIVE_INPUTS > 0
static const char PANEL_PAGE_3[] PROGMEM =
R"JSON({"id":"inputs","name":"Inputs","order":30,"fields":[)JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON({"type":"edit","name":"S1: Name","id":"M_3759e61d","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor1Name","command_topic":"cmnd/$device/Sensor1Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S1: Assigned output","id":"M_a5f9eefe","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor1Type","command_topic":"cmnd/$device/Sensor1Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"Not connected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"Relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"Relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"Relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"Relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"Pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"Pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"Pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"Pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"Pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"Pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S1: Calibration: value 1","id":"M_78157d9d","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor1CalibValue1","command_topic":"cmnd/$device/Sensor1CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S1: Calibration: raw value 1","id":"M_400e4039","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor1CalibRawValue1","command_topic":"cmnd/$device/Sensor1CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S1: Calibration: value 2","id":"M_751578e4","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor1CalibValue2","command_topic":"cmnd/$device/Sensor1CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S1: Calibration: raw value 2","id":"M_3d0e3b80","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor1CalibRawValue2","command_topic":"cmnd/$device/Sensor1CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S1: Minimum or off","id":"M_13539af3","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor1MinValue","command_topic":"cmnd/$device/Sensor1MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S1: Maximum or off","id":"M_d0808f51","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor1MaxValue","command_topic":"cmnd/$device/Sensor1MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S1: Critical minimum or off","id":"M_83b82822","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor1CriticalMinValue","command_topic":"cmnd/$device/Sensor1CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S1: Critical maximum or off","id":"M_6de88960","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor1CriticalMaxValue","command_topic":"cmnd/$device/Sensor1CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S1: Value","id":"M_a843477f","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor1Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S1: RawValue","id":"M_a8e306a9","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor1RawValue"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S2: Name","id":"M_c32c1550","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor2Name","command_topic":"cmnd/$device/Sensor2Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S2: Assigned output","id":"M_f3fd17b3","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor2Type","command_topic":"cmnd/$device/Sensor2Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"Not connected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"Relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"Relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"Relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"Relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"Pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"Pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"Pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"Pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"Pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"Pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S2: Calibration: value 1","id":"M_915c41ee","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor2CalibValue1","command_topic":"cmnd/$device/Sensor2CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S2: Calibration: raw value 1","id":"M_9297b8cc","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor2CalibRawValue1","command_topic":"cmnd/$device/Sensor2CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S2: Calibration: value 2","id":"M_905c405b","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor2CalibValue2","command_topic":"cmnd/$device/Sensor2CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S2: Calibration: raw value 2","id":"M_9597bd85","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor2CalibRawValue2","command_topic":"cmnd/$device/Sensor2CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S2: Minimum or off","id":"M_725dac62","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor2MinValue","command_topic":"cmnd/$device/Sensor2MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S2: Maximum or off","id":"M_2b144fa0","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor2MaxValue","command_topic":"cmnd/$device/Sensor2MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S2: Critical minimum or off","id":"M_b24797bf","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor2CriticalMinValue","command_topic":"cmnd/$device/Sensor2CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S2: Critical maximum or off","id":"M_1be960d5","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor2CriticalMaxValue","command_topic":"cmnd/$device/Sensor2CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S2: Value","id":"M_790ef628","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor2Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S2: RawValue","id":"M_84ec1eb8","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor2RawValue"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S3: Name","id":"M_5addecbb","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor3Name","command_topic":"cmnd/$device/Sensor3Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S3: Assigned output","id":"M_c8dbdb90","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor3Type","command_topic":"cmnd/$device/Sensor3Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"Not connected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"Relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"Relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"Relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"Relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"Pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"Pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"Pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"Pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"Pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"Pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S3: Calibration: value 1","id":"M_86d6228f","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor3CalibValue1","command_topic":"cmnd/$device/Sensor3CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S3: Calibration: raw value 1","id":"M_f13c82ff","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor3CalibRawValue1","command_topic":"cmnd/$device/Sensor3CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S3: Calibration: value 2","id":"M_87d62422","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor3CalibValue2","command_topic":"cmnd/$device/Sensor3CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S3: Calibration: raw value 2","id":"M_f23c8492","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor3CalibRawValue2","command_topic":"cmnd/$device/Sensor3CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S3: Minimum or off","id":"M_747247ed","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor3MinValue","command_topic":"cmnd/$device/Sensor3MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S3: Maximum or off","id":"M_b72c9d57","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor3MaxValue","command_topic":"cmnd/$device/Sensor3MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S3: Critical minimum or off","id":"M_986a7160","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor3CriticalMinValue","command_topic":"cmnd/$device/Sensor3CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S3: Critical maximum or off","id":"M_ae3a1022","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor3CriticalMaxValue","command_topic":"cmnd/$device/Sensor3CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S3: Value","id":"M_4fa351e9","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor3Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S3: RawValue","id":"M_5b547cef","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor3RawValue"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S4: Name","id":"M_204eff5e","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor4Name","command_topic":"cmnd/$device/Sensor4Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S4: Assigned output","id":"M_c50272b5","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor4Type","command_topic":"cmnd/$device/Sensor4Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"Not connected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"Relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"Relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"Relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"Relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"Pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"Pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"Pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"Pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"Pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"Pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S4: Calibration: value 1","id":"M_b34c3f38","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor4CalibValue1","command_topic":"cmnd/$device/Sensor4CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S4: Calibration: raw value 1","id":"M_e2aea58a","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor4CalibRawValue1","command_topic":"cmnd/$device/Sensor4CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S4: Calibration: value 2","id":"M_b64c43f1","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor4CalibValue2","command_topic":"cmnd/$device/Sensor4CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S4: Calibration: raw value 2","id":"M_e1aea3f7","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor4CalibRawValue2","command_topic":"cmnd/$device/Sensor4CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S4: Minimum or off","id":"M_c6f065c4","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor4MinValue","command_topic":"cmnd/$device/Sensor4MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S4: Maximum or off","id":"M_205e028e","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor4MaxValue","command_topic":"cmnd/$device/Sensor4MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S4: Critical minimum or off","id":"M_9b860885","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor4CriticalMinValue","command_topic":"cmnd/$device/Sensor4CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S4: Critical maximum or off","id":"M_6ab9063f","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor4CriticalMaxValue","command_topic":"cmnd/$device/Sensor4CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S4: Value","id":"M_ce938de2","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor4Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S4: RawValue","id":"M_0c55dc06","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor4RawValue"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S5: Name","id":"M_f91e9e91","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor5Name","command_topic":"cmnd/$device/Sensor5Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S5: Assigned output","id":"M_e4dbc82a","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor5Type","command_topic":"cmnd/$device/Sensor5Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"Not connected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"Relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"Relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"Relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"Relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"Pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"Pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"Pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"Pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"Pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"Pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S5: Calibration: value 1","id":"M_6614e169","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor5CalibValue1","command_topic":"cmnd/$device/Sensor5CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S5: Calibration: raw value 1","id":"M_9800ecd5","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor5CalibRawValue1","command_topic":"cmnd/$device/Sensor5CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S5: Calibration: value 2","id":"M_6314dcb0","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor5CalibValue2","command_topic":"cmnd/$device/Sensor5CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S5: Calibration: raw value 2","id":"M_9500e81c","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor5CalibRawValue2","command_topic":"cmnd/$device/Sensor5CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S5: Minimum or off","id":"M_e40d3ddf","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor5MinValue","command_topic":"cmnd/$device/Sensor5MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S5: Maximum or off","id":"M_50031e75","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor5MaxValue","command_topic":"cmnd/$device/Sensor5MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S5: Critical minimum or off","id":"M_aca38f9e","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor5CriticalMinValue","command_topic":"cmnd/$device/Sensor5CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S5: Critical maximum or off","id":"M_b8b46354","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor5CriticalMaxValue","command_topic":"cmnd/$device/Sensor5CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S5: Value","id":"M_303387f3","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor5Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S5: RawValue","id":"M_8bfc05ed","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor5RawValue"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S6: Name","id":"M_ad9e3834","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor6Name","command_topic":"cmnd/$device/Sensor6Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S6: Assigned output","id":"M_a21fbcaf","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor6Type","command_topic":"cmnd/$device/Sensor6Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"Not connected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"Relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"Relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"Relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"Relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"Pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"Pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"Pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"Pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"Pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"Pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S6: Calibration: value 1","id":"M_b8d0dfaa","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor6CalibValue1","command_topic":"cmnd/$device/Sensor6CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S6: Calibration: raw value 1","id":"M_41a72f68","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor6CalibRawValue1","command_topic":"cmnd/$device/Sensor6CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S6: Calibration: value 2","id":"M_b7d0de17","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor6CalibValue2","command_topic":"cmnd/$device/Sensor6CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S6: Calibration: raw value 2","id":"M_44a73421","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor6CalibRawValue2","command_topic":"cmnd/$device/Sensor6CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S6: Minimum or off","id":"M_a3d26b5e","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor6MinValue","command_topic":"cmnd/$device/Sensor6MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S6: Maximum or off","id":"M_de9b2614","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor6MaxValue","command_topic":"cmnd/$device/Sensor6MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S6: Critical minimum or off","id":"M_33e5a55b","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor6CriticalMinValue","command_topic":"cmnd/$device/Sensor6CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S6: Critical maximum or off","id":"M_bc24ea89","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor6CriticalMaxValue","command_topic":"cmnd/$device/Sensor6CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S6: Value","id":"M_4d988bec","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor6Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S6: RawValue","id":"M_9fe8972c","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor6RawValue"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S7: Name","id":"M_565a041f","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor7Name","command_topic":"cmnd/$device/Sensor7Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S7: Assigned output","id":"M_4a9a54ec","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor7Type","command_topic":"cmnd/$device/Sensor7Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"Not connected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"Relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"Relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"Relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"Relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"Pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"Pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"Pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"Pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"Pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"Pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S7: Calibration: value 1","id":"M_cb47ec5b","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor7CalibValue1","command_topic":"cmnd/$device/Sensor7CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S7: Calibration: raw value 1","id":"M_2ff172ab","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor7CalibRawValue1","command_topic":"cmnd/$device/Sensor7CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S7: Calibration: value 2","id":"M_cc47edee","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor7CalibValue2","command_topic":"cmnd/$device/Sensor7CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S7: Calibration: raw value 2","id":"M_30f1743e","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor7CalibRawValue2","command_topic":"cmnd/$device/Sensor7CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S7: Minimum or off","id":"M_71be88c9","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor7MinValue","command_topic":"cmnd/$device/Sensor7MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S7: Maximum or off","id":"M_c9f9d83b","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor7MaxValue","command_topic":"cmnd/$device/Sensor7MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S7: Critical minimum or off","id":"M_0400b31c","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor7CriticalMinValue","command_topic":"cmnd/$device/Sensor7CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S7: Critical maximum or off","id":"M_fdcb7c56","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor7CriticalMaxValue","command_topic":"cmnd/$device/Sensor7CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S7: Value","id":"M_1dc7adbd","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor7Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S7: RawValue","id":"M_7cfbebd3","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor7RawValue"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S8: Name","id":"M_0637c052","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor8Name","command_topic":"cmnd/$device/Sensor8Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S8: Assigned output","id":"M_dd82ceb1","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor8Type","command_topic":"cmnd/$device/Sensor8Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"Not connected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"Relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"Relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"Relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"Relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"Pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"Pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"Pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"Pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"Pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"Pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S8: Calibration: value 1","id":"M_71046d54","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor8CalibValue1","command_topic":"cmnd/$device/Sensor8CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S8: Calibration: raw value 1","id":"M_bb4b4d56","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor8CalibRawValue1","command_topic":"cmnd/$device/Sensor8CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S8: Calibration: value 2","id":"M_7404720d","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor8CalibValue2","command_topic":"cmnd/$device/Sensor8CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S8: Calibration: raw value 2","id":"M_ba4b4bc3","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor8CalibRawValue2","command_topic":"cmnd/$device/Sensor8CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S8: Minimum or off","id":"M_dfe1d230","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor8MinValue","command_topic":"cmnd/$device/Sensor8MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S8: Maximum or off","id":"M_a0df6a12","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor8MaxValue","command_topic":"cmnd/$device/Sensor8MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S8: Critical minimum or off","id":"M_040700f1","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor8CriticalMinValue","command_topic":"cmnd/$device/Sensor8CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S8: Critical maximum or off","id":"M_cc206833","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor8CriticalMaxValue","command_topic":"cmnd/$device/Sensor8CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S8: Value","id":"M_a84af346","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor8Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S8: RawValue","id":"M_1a1754ca","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor8RawValue"}})JSON"
#endif
R"JSON(]})JSON"
;
#endif
#if AC_ACTIVE_PWM > 0
static const char PANEL_PAGE_4[] PROGMEM =
R"JSON({"id":"pwm","name":"PWM outputs","order":40,"fields":[)JSON"
#if AC_ACTIVE_PWM >= 1
R"JSON({"type":"edit","name":"PWM1: Name","id":"M_8a6eea92","persist":true,"default":"","ha":{"state_topic":"stat/$device/PWMOutput1Name","command_topic":"cmnd/$device/PWMOutput1Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
#if (AC_ACTIVE_PWM >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM1: Mode","id":"M_cc6519d9","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/PWMOutput1ControlMode","command_topic":"cmnd/$device/PWMOutput1ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"Manual"})JSON"
R"JSON(,{"value":"partofday","name":"Time of day"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"Threshold"})JSON"
#endif
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"pid","name":"PID"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM1: Direction","id":"M_03e2abe7","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/PWMOutput1ControlDirection","command_topic":"cmnd/$device/PWMOutput1ControlDirection","optimistic":true},"visibleWhen":{"or":[{"field":"M_cc6519d9","equals":"treshold"},{"field":"M_cc6519d9","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"direct","name":"Direct"})JSON"
R"JSON(,{"value":"reverse","name":"Reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM1: Input","id":"M_23f03e7f","persist":true,"default":"none","ha":{"state_topic":"stat/$device/PWMOutput1Input","command_topic":"cmnd/$device/PWMOutput1Input","optimistic":true},"visibleWhen":{"or":[{"field":"M_cc6519d9","equals":"treshold"},{"field":"M_cc6519d9","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"none","name":"None"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"Sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"Sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"Sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"Sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"Sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"Sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"Sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"Sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM1: Setpoint","id":"M_c7216cfa","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput1SensorsSetpoint","command_topic":"cmnd/$device/PWMOutput1SensorsSetpoint","optimistic":true},"visibleWhen":{"or":[{"field":"M_cc6519d9","equals":"treshold"},{"field":"M_cc6519d9","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM1: Hysteresis half-width","id":"M_0b6aec8c","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput1MaxDeviation","command_topic":"cmnd/$device/PWMOutput1MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM1: Morning","min":0,"max":65535,"step":1,"id":"M_ea6077cc","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput1MorningMode","command_topic":"cmnd/$device/PWMOutput1MorningMode","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM1: Afternoon","min":0,"max":65535,"step":1,"id":"M_410b2e24","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput1AfternoonMode","command_topic":"cmnd/$device/PWMOutput1AfternoonMode","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM1: Evening","min":0,"max":65535,"step":1,"id":"M_e5ce6434","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput1EveningMode","command_topic":"cmnd/$device/PWMOutput1EveningMode","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM1: Night","min":0,"max":65535,"step":1,"id":"M_624d6cf2","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput1NightMode","command_topic":"cmnd/$device/PWMOutput1NightMode","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM1: Manual PWM","min":0,"max":65535,"step":1,"id":"M_6fc04f90","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput1ManualMode","command_topic":"cmnd/$device/PWMOutput1ManualMode","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"manual"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM1: PidKp","id":"M_8cc3baa5","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput1PidKp","command_topic":"cmnd/$device/PWMOutput1PidKp","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM1: PidKi","id":"M_a3c3deda","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput1PidKi","command_topic":"cmnd/$device/PWMOutput1PidKi","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM1: PidKd","id":"M_98c3cd89","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput1PidKd","command_topic":"cmnd/$device/PWMOutput1PidKd","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"PWM1: Value","id":"M_8b21ac86","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput1Value"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"PWM1: SensorsValue","id":"M_eca25cc5","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput1SensorsValue"},"visibleWhen":{"or":[{"field":"M_cc6519d9","equals":"treshold"},{"field":"M_cc6519d9","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM2: Name","id":"M_cbfea483","persist":true,"default":"","ha":{"state_topic":"stat/$device/PWMOutput2Name","command_topic":"cmnd/$device/PWMOutput2Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM2: Mode","id":"M_dd171d92","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/PWMOutput2ControlMode","command_topic":"cmnd/$device/PWMOutput2ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"Manual"})JSON"
R"JSON(,{"value":"partofday","name":"Time of day"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"Threshold"})JSON"
#endif
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"pid","name":"PID"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM2: Direction","id":"M_f7231fea","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/PWMOutput2ControlDirection","command_topic":"cmnd/$device/PWMOutput2ControlDirection","optimistic":true},"visibleWhen":{"or":[{"field":"M_dd171d92","equals":"treshold"},{"field":"M_dd171d92","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"direct","name":"Direct"})JSON"
R"JSON(,{"value":"reverse","name":"Reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM2: Input","id":"M_12857a64","persist":true,"default":"none","ha":{"state_topic":"stat/$device/PWMOutput2Input","command_topic":"cmnd/$device/PWMOutput2Input","optimistic":true},"visibleWhen":{"or":[{"field":"M_dd171d92","equals":"treshold"},{"field":"M_dd171d92","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"none","name":"None"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"Sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"Sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"Sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"Sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"Sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"Sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"Sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"Sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM2: Setpoint","id":"M_02558409","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput2SensorsSetpoint","command_topic":"cmnd/$device/PWMOutput2SensorsSetpoint","optimistic":true},"visibleWhen":{"or":[{"field":"M_dd171d92","equals":"treshold"},{"field":"M_dd171d92","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM2: Hysteresis half-width","id":"M_6b606161","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput2MaxDeviation","command_topic":"cmnd/$device/PWMOutput2MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM2: Morning","min":0,"max":65535,"step":1,"id":"M_d9f077bb","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput2MorningMode","command_topic":"cmnd/$device/PWMOutput2MorningMode","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM2: Afternoon","min":0,"max":65535,"step":1,"id":"M_3bdd5757","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput2AfternoonMode","command_topic":"cmnd/$device/PWMOutput2AfternoonMode","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM2: Evening","min":0,"max":65535,"step":1,"id":"M_74c7198f","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput2EveningMode","command_topic":"cmnd/$device/PWMOutput2EveningMode","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM2: Night","min":0,"max":65535,"step":1,"id":"M_659d06ad","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput2NightMode","command_topic":"cmnd/$device/PWMOutput2NightMode","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM2: Manual PWM","min":0,"max":65535,"step":1,"id":"M_e3271a65","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput2ManualMode","command_topic":"cmnd/$device/PWMOutput2ManualMode","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"manual"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM2: PidKp","id":"M_6ec780e6","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput2PidKp","command_topic":"cmnd/$device/PWMOutput2PidKp","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM2: PidKi","id":"M_87c7a841","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput2PidKi","command_topic":"cmnd/$device/PWMOutput2PidKi","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM2: PidKd","id":"M_82c7a062","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput2PidKd","command_topic":"cmnd/$device/PWMOutput2PidKd","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"PWM2: Value","id":"M_50a5d211","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput2Value"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"PWM2: SensorsValue","id":"M_3602dd1c","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput2SensorsValue"},"visibleWhen":{"or":[{"field":"M_dd171d92","equals":"treshold"},{"field":"M_dd171d92","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM3: Name","id":"M_e34273d8","persist":true,"default":"","ha":{"state_topic":"stat/$device/PWMOutput3Name","command_topic":"cmnd/$device/PWMOutput3Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM3: Mode","id":"M_7837ccd7","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/PWMOutput3ControlMode","command_topic":"cmnd/$device/PWMOutput3ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"Manual"})JSON"
R"JSON(,{"value":"partofday","name":"Time of day"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"Threshold"})JSON"
#endif
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"pid","name":"PID"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM3: Direction","id":"M_9edc4095","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/PWMOutput3ControlDirection","command_topic":"cmnd/$device/PWMOutput3ControlDirection","optimistic":true},"visibleWhen":{"or":[{"field":"M_7837ccd7","equals":"treshold"},{"field":"M_7837ccd7","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"direct","name":"Direct"})JSON"
R"JSON(,{"value":"reverse","name":"Reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM3: Input","id":"M_269b6231","persist":true,"default":"none","ha":{"state_topic":"stat/$device/PWMOutput3Input","command_topic":"cmnd/$device/PWMOutput3Input","optimistic":true},"visibleWhen":{"or":[{"field":"M_7837ccd7","equals":"treshold"},{"field":"M_7837ccd7","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"none","name":"None"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"Sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"Sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"Sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"Sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"Sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"Sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"Sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"Sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM3: Setpoint","id":"M_b88683ac","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput3SensorsSetpoint","command_topic":"cmnd/$device/PWMOutput3SensorsSetpoint","optimistic":true},"visibleWhen":{"or":[{"field":"M_7837ccd7","equals":"treshold"},{"field":"M_7837ccd7","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM3: Hysteresis half-width","id":"M_e99b5fe2","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput3MaxDeviation","command_topic":"cmnd/$device/PWMOutput3MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM3: Morning","min":0,"max":65535,"step":1,"id":"M_61b98ffa","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput3MorningMode","command_topic":"cmnd/$device/PWMOutput3MorningMode","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM3: Afternoon","min":0,"max":65535,"step":1,"id":"M_0c5a37c6","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput3AfternoonMode","command_topic":"cmnd/$device/PWMOutput3AfternoonMode","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM3: Evening","min":0,"max":65535,"step":1,"id":"M_82483366","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput3EveningMode","command_topic":"cmnd/$device/PWMOutput3EveningMode","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM3: Night","min":0,"max":65535,"step":1,"id":"M_f241ad44","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput3NightMode","command_topic":"cmnd/$device/PWMOutput3NightMode","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM3: Manual PWM","min":0,"max":65535,"step":1,"id":"M_ef37c3c2","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput3ManualMode","command_topic":"cmnd/$device/PWMOutput3ManualMode","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"manual"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM3: PidKp","id":"M_78c90f5f","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput3PidKp","command_topic":"cmnd/$device/PWMOutput3PidKp","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM3: PidKi","id":"M_8fc93394","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput3PidKi","command_topic":"cmnd/$device/PWMOutput3PidKi","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM3: PidKd","id":"M_84c92243","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput3PidKd","command_topic":"cmnd/$device/PWMOutput3PidKd","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"PWM3: Value","id":"M_713c2df0","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput3Value"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"PWM3: SensorsValue","id":"M_35e51963","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput3SensorsValue"},"visibleWhen":{"or":[{"field":"M_7837ccd7","equals":"treshold"},{"field":"M_7837ccd7","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM4: Name","id":"M_67753b79","persist":true,"default":"","ha":{"state_topic":"stat/$device/PWMOutput4Name","command_topic":"cmnd/$device/PWMOutput4Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM4: Mode","id":"M_503e1860","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/PWMOutput4ControlMode","command_topic":"cmnd/$device/PWMOutput4ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"Manual"})JSON"
R"JSON(,{"value":"partofday","name":"Time of day"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"Threshold"})JSON"
#endif
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"pid","name":"PID"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM4: Direction","id":"M_f1d63130","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/PWMOutput4ControlDirection","command_topic":"cmnd/$device/PWMOutput4ControlDirection","optimistic":true},"visibleWhen":{"or":[{"field":"M_503e1860","equals":"treshold"},{"field":"M_503e1860","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"direct","name":"Direct"})JSON"
R"JSON(,{"value":"reverse","name":"Reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM4: Input","id":"M_e406e716","persist":true,"default":"none","ha":{"state_topic":"stat/$device/PWMOutput4Input","command_topic":"cmnd/$device/PWMOutput4Input","optimistic":true},"visibleWhen":{"or":[{"field":"M_503e1860","equals":"treshold"},{"field":"M_503e1860","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"none","name":"None"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"Sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"Sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"Sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"Sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"Sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"Sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"Sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"Sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM4: Setpoint","id":"M_0a210f6b","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput4SensorsSetpoint","command_topic":"cmnd/$device/PWMOutput4SensorsSetpoint","optimistic":true},"visibleWhen":{"or":[{"field":"M_503e1860","equals":"treshold"},{"field":"M_503e1860","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM4: Hysteresis half-width","id":"M_4f6d6867","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput4MaxDeviation","command_topic":"cmnd/$device/PWMOutput4MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM4: Morning","min":0,"max":65535,"step":1,"id":"M_5a687341","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput4MorningMode","command_topic":"cmnd/$device/PWMOutput4MorningMode","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM4: Afternoon","min":0,"max":65535,"step":1,"id":"M_31bcd969","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput4AfternoonMode","command_topic":"cmnd/$device/PWMOutput4AfternoonMode","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM4: Evening","min":0,"max":65535,"step":1,"id":"M_46fd4c89","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput4EveningMode","command_topic":"cmnd/$device/PWMOutput4EveningMode","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM4: Night","min":0,"max":65535,"step":1,"id":"M_c3f542e7","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput4NightMode","command_topic":"cmnd/$device/PWMOutput4NightMode","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM4: Manual PWM","min":0,"max":65535,"step":1,"id":"M_c81a62f7","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput4ManualMode","command_topic":"cmnd/$device/PWMOutput4ManualMode","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"manual"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM4: PidKp","id":"M_e2031320","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput4PidKp","command_topic":"cmnd/$device/PWMOutput4PidKp","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM4: PidKi","id":"M_fb033a7b","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput4PidKi","command_topic":"cmnd/$device/PWMOutput4PidKi","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM4: PidKd","id":"M_f603329c","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput4PidKd","command_topic":"cmnd/$device/PWMOutput4PidKd","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"PWM4: Value","id":"M_888c809b","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput4Value"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"PWM4: SensorsValue","id":"M_978fd22a","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput4SensorsValue"},"visibleWhen":{"or":[{"field":"M_503e1860","equals":"treshold"},{"field":"M_503e1860","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM5: Name","id":"M_916e2426","persist":true,"default":"","ha":{"state_topic":"stat/$device/PWMOutput5Name","command_topic":"cmnd/$device/PWMOutput5Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM5: Mode","id":"M_475ccb45","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/PWMOutput5ControlMode","command_topic":"cmnd/$device/PWMOutput5ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"Manual"})JSON"
R"JSON(,{"value":"partofday","name":"Time of day"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"Threshold"})JSON"
#endif
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"pid","name":"PID"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM5: Direction","id":"M_849c24a3","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/PWMOutput5ControlDirection","command_topic":"cmnd/$device/PWMOutput5ControlDirection","optimistic":true},"visibleWhen":{"or":[{"field":"M_475ccb45","equals":"treshold"},{"field":"M_475ccb45","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"direct","name":"Direct"})JSON"
R"JSON(,{"value":"reverse","name":"Reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM5: Input","id":"M_9de062bb","persist":true,"default":"none","ha":{"state_topic":"stat/$device/PWMOutput5Input","command_topic":"cmnd/$device/PWMOutput5Input","optimistic":true},"visibleWhen":{"or":[{"field":"M_475ccb45","equals":"treshold"},{"field":"M_475ccb45","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"none","name":"None"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"Sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"Sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"Sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"Sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"Sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"Sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"Sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"Sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM5: Setpoint","id":"M_38c42c6e","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput5SensorsSetpoint","command_topic":"cmnd/$device/PWMOutput5SensorsSetpoint","optimistic":true},"visibleWhen":{"or":[{"field":"M_475ccb45","equals":"treshold"},{"field":"M_475ccb45","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM5: Hysteresis half-width","id":"M_1cda4330","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput5MaxDeviation","command_topic":"cmnd/$device/PWMOutput5MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM5: Morning","min":0,"max":65535,"step":1,"id":"M_4c261cc8","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput5MorningMode","command_topic":"cmnd/$device/PWMOutput5MorningMode","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM5: Afternoon","min":0,"max":65535,"step":1,"id":"M_51312d90","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput5AfternoonMode","command_topic":"cmnd/$device/PWMOutput5AfternoonMode","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM5: Evening","min":0,"max":65535,"step":1,"id":"M_8336ffc8","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput5EveningMode","command_topic":"cmnd/$device/PWMOutput5EveningMode","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM5: Night","min":0,"max":65535,"step":1,"id":"M_3edf2c5e","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput5NightMode","command_topic":"cmnd/$device/PWMOutput5NightMode","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM5: Manual PWM","min":0,"max":65535,"step":1,"id":"M_26a0d20c","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput5ManualMode","command_topic":"cmnd/$device/PWMOutput5ManualMode","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"manual"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM5: PidKp","id":"M_2ffab221","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput5PidKp","command_topic":"cmnd/$device/PWMOutput5PidKp","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM5: PidKi","id":"M_16fa8ac6","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput5PidKi","command_topic":"cmnd/$device/PWMOutput5PidKi","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM5: PidKd","id":"M_23fa9f3d","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput5PidKd","command_topic":"cmnd/$device/PWMOutput5PidKd","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"PWM5: Value","id":"M_cf947b0a","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput5Value"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"PWM5: SensorsValue","id":"M_ef572f09","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput5SensorsValue"},"visibleWhen":{"or":[{"field":"M_475ccb45","equals":"treshold"},{"field":"M_475ccb45","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM6: Name","id":"M_1d505e87","persist":true,"default":"","ha":{"state_topic":"stat/$device/PWMOutput6Name","command_topic":"cmnd/$device/PWMOutput6Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM6: Mode","id":"M_7289ca6e","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/PWMOutput6ControlMode","command_topic":"cmnd/$device/PWMOutput6ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"Manual"})JSON"
R"JSON(,{"value":"partofday","name":"Time of day"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"Threshold"})JSON"
#endif
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"pid","name":"PID"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM6: Direction","id":"M_4da79906","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/PWMOutput6ControlDirection","command_topic":"cmnd/$device/PWMOutput6ControlDirection","optimistic":true},"visibleWhen":{"or":[{"field":"M_7289ca6e","equals":"treshold"},{"field":"M_7289ca6e","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"direct","name":"Direct"})JSON"
R"JSON(,{"value":"reverse","name":"Reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 6) || (AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM6: Input","id":"M_cf971640","persist":true,"default":"none","ha":{"state_topic":"stat/$device/PWMOutput6Input","command_topic":"cmnd/$device/PWMOutput6Input","optimistic":true},"visibleWhen":{"or":[{"field":"M_7289ca6e","equals":"treshold"},{"field":"M_7289ca6e","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"none","name":"None"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"Sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"Sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"Sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"Sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"Sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"Sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"Sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"Sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 6) || (AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM6: Setpoint","id":"M_920be09d","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput6SensorsSetpoint","command_topic":"cmnd/$device/PWMOutput6SensorsSetpoint","optimistic":true},"visibleWhen":{"or":[{"field":"M_7289ca6e","equals":"treshold"},{"field":"M_7289ca6e","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 6) || (AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM6: Hysteresis half-width","id":"M_de16a3d5","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput6MaxDeviation","command_topic":"cmnd/$device/PWMOutput6MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 6) || (AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM6: Morning","min":0,"max":65535,"step":1,"id":"M_70002607","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput6MorningMode","command_topic":"cmnd/$device/PWMOutput6MorningMode","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 6) || (AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM6: Afternoon","min":0,"max":65535,"step":1,"id":"M_a900bf53","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput6AfternoonMode","command_topic":"cmnd/$device/PWMOutput6AfternoonMode","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 6) || (AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM6: Evening","min":0,"max":65535,"step":1,"id":"M_9a44e543","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput6EveningMode","command_topic":"cmnd/$device/PWMOutput6EveningMode","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 6) || (AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM6: Night","min":0,"max":65535,"step":1,"id":"M_45e40e39","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput6NightMode","command_topic":"cmnd/$device/PWMOutput6NightMode","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 6) || (AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"slider","name":"PWM6: Manual PWM","min":0,"max":65535,"step":1,"id":"M_19e41b21","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput6ManualMode","command_topic":"cmnd/$device/PWMOutput6ManualMode","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"manual"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 6) || (AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM6: PidKp","id":"M_08f8ec32","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput6PidKp","command_topic":"cmnd/$device/PWMOutput6PidKp","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 6) || (AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM6: PidKi","id":"M_21f9138d","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput6PidKi","command_topic":"cmnd/$device/PWMOutput6PidKi","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 6) || (AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"PWM6: PidKd","id":"M_14f8ff16","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput6PidKd","command_topic":"cmnd/$device/PWMOutput6PidKd","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 6) || (AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"PWM6: Value","id":"M_8c592a25","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput6Value"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
#if (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 5) || (AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0) || (AC_ACTIVE_PWM >= 6) || (AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"PWM6: SensorsValue","id":"M_4528a6a0","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput6SensorsValue"},"visibleWhen":{"or":[{"field":"M_7289ca6e","equals":"treshold"},{"field":"M_7289ca6e","equals":"pid"}]}})JSON"
#endif
R"JSON(]})JSON"
;
#endif
#if 1
static const char PANEL_PAGE_5[] PROGMEM =
R"JSON({"id":"clock","name":"Clock","order":50,"fields":[)JSON"
R"JSON({"type":"date","name":"Local date","id":"M_050550f3","persist":false,"ha":{"state_topic":"stat/$device/ActualDate","command_topic":"cmnd/$device/ActualDate","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Local time","id":"M_8c8e46a6","persist":false,"ha":{"state_topic":"stat/$device/ActualTime","command_topic":"cmnd/$device/ActualTime","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"Timezone: Rule 1 — week","id":"M_28964e47","persist":true,"default":"first","ha":{"state_topic":"stat/$device/TimezoneRule1Week","command_topic":"cmnd/$device/TimezoneRule1Week","optimistic":true},"options":[)JSON"
R"JSON({"value":"first","name":"First"})JSON"
R"JSON(,{"value":"second","name":"Second"})JSON"
R"JSON(,{"value":"third","name":"Third"})JSON"
R"JSON(,{"value":"fourth","name":"Fourth"})JSON"
R"JSON(,{"value":"last","name":"Last"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"Timezone: Rule 1 — day","id":"M_6a205c42","persist":true,"default":"Sunday","ha":{"state_topic":"stat/$device/TimezoneRule1DayOfWeek","command_topic":"cmnd/$device/TimezoneRule1DayOfWeek","optimistic":true},"options":[)JSON"
R"JSON({"value":"Sunday","name":"Sunday"})JSON"
R"JSON(,{"value":"Monday","name":"Monday"})JSON"
R"JSON(,{"value":"Tuesday","name":"Tuesday"})JSON"
R"JSON(,{"value":"Wednesday","name":"Wednesday"})JSON"
R"JSON(,{"value":"Thursday","name":"Thursday"})JSON"
R"JSON(,{"value":"Friday","name":"Friday"})JSON"
R"JSON(,{"value":"Saturday","name":"Saturday"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"Timezone: Rule 1 — month","id":"M_707fef7b","persist":true,"default":"January","ha":{"state_topic":"stat/$device/TimezoneRule1Month","command_topic":"cmnd/$device/TimezoneRule1Month","optimistic":true},"options":[)JSON"
R"JSON({"value":"January","name":"January"})JSON"
R"JSON(,{"value":"February","name":"February"})JSON"
R"JSON(,{"value":"March","name":"March"})JSON"
R"JSON(,{"value":"April","name":"April"})JSON"
R"JSON(,{"value":"May","name":"May"})JSON"
R"JSON(,{"value":"June","name":"June"})JSON"
R"JSON(,{"value":"July","name":"July"})JSON"
R"JSON(,{"value":"August","name":"August"})JSON"
R"JSON(,{"value":"September","name":"September"})JSON"
R"JSON(,{"value":"October","name":"October"})JSON"
R"JSON(,{"value":"November","name":"November"})JSON"
R"JSON(,{"value":"December","name":"December"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"Timezone: Rule 1 — hour","min":0,"max":23,"step":1,"id":"M_3929a5d9","persist":true,"default":0,"ha":{"state_topic":"stat/$device/TimezoneRule1Hour","command_topic":"cmnd/$device/TimezoneRule1Hour","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"Timezone: Rule 1 — UTC offset","min":-12,"max":14,"step":1,"id":"M_f360da94","persist":true,"default":-12,"ha":{"state_topic":"stat/$device/TimezoneRule1Offset","command_topic":"cmnd/$device/TimezoneRule1Offset","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"Timezone: Rule 2 — week","id":"M_070aeefe","persist":true,"default":"first","ha":{"state_topic":"stat/$device/TimezoneRule2Week","command_topic":"cmnd/$device/TimezoneRule2Week","optimistic":true},"options":[)JSON"
R"JSON({"value":"first","name":"First"})JSON"
R"JSON(,{"value":"second","name":"Second"})JSON"
R"JSON(,{"value":"third","name":"Third"})JSON"
R"JSON(,{"value":"fourth","name":"Fourth"})JSON"
R"JSON(,{"value":"last","name":"Last"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"Timezone: Rule 2 — day","id":"M_ba91c141","persist":true,"default":"Sunday","ha":{"state_topic":"stat/$device/TimezoneRule2DayOfWeek","command_topic":"cmnd/$device/TimezoneRule2DayOfWeek","optimistic":true},"options":[)JSON"
R"JSON({"value":"Sunday","name":"Sunday"})JSON"
R"JSON(,{"value":"Monday","name":"Monday"})JSON"
R"JSON(,{"value":"Tuesday","name":"Tuesday"})JSON"
R"JSON(,{"value":"Wednesday","name":"Wednesday"})JSON"
R"JSON(,{"value":"Thursday","name":"Thursday"})JSON"
R"JSON(,{"value":"Friday","name":"Friday"})JSON"
R"JSON(,{"value":"Saturday","name":"Saturday"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"Timezone: Rule 2 — month","id":"M_56cfda54","persist":true,"default":"January","ha":{"state_topic":"stat/$device/TimezoneRule2Month","command_topic":"cmnd/$device/TimezoneRule2Month","optimistic":true},"options":[)JSON"
R"JSON({"value":"January","name":"January"})JSON"
R"JSON(,{"value":"February","name":"February"})JSON"
R"JSON(,{"value":"March","name":"March"})JSON"
R"JSON(,{"value":"April","name":"April"})JSON"
R"JSON(,{"value":"May","name":"May"})JSON"
R"JSON(,{"value":"June","name":"June"})JSON"
R"JSON(,{"value":"July","name":"July"})JSON"
R"JSON(,{"value":"August","name":"August"})JSON"
R"JSON(,{"value":"September","name":"September"})JSON"
R"JSON(,{"value":"October","name":"October"})JSON"
R"JSON(,{"value":"November","name":"November"})JSON"
R"JSON(,{"value":"December","name":"December"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"Timezone: Rule 2 — hour","min":0,"max":23,"step":1,"id":"M_6c3b8950","persist":true,"default":0,"ha":{"state_topic":"stat/$device/TimezoneRule2Hour","command_topic":"cmnd/$device/TimezoneRule2Hour","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"Timezone: Rule 2 — UTC offset","min":-12,"max":14,"step":1,"id":"M_092b22a5","persist":true,"default":-12,"ha":{"state_topic":"stat/$device/TimezoneRule2Offset","command_topic":"cmnd/$device/TimezoneRule2Offset","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"ActualTimezoneOffset","id":"M_f6125e9b","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/ActualTimezoneOffset"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"ActualDayOfWeek","id":"M_e5dc2a8a","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/ActualDayOfWeek"}})JSON"
R"JSON(]})JSON"
;
#endif
#if AC_ACTIVE_FAN
static const char PANEL_PAGE_6[] PROGMEM =
R"JSON({"id":"fan","name":"Fan","order":60,"fields":[)JSON"
R"JSON({"type":"slider","name":"Fan: Start temperature","min":0,"max":100,"step":1,"id":"M_d5224990","persist":true,"default":30,"ha":{"state_topic":"stat/$device/FanStartTemp","command_topic":"cmnd/$device/FanStartTemp","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"Fan: Full-speed temperature","min":0,"max":100,"step":1,"id":"M_74b30ce9","persist":true,"default":35,"ha":{"state_topic":"stat/$device/FanMaxSpeedTemp","command_topic":"cmnd/$device/FanMaxSpeedTemp","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"Fan: Temperature alarm","min":0,"max":100,"step":1,"id":"M_1c6d9e10","persist":true,"default":100,"ha":{"state_topic":"stat/$device/MaxIntTemp","command_topic":"cmnd/$device/MaxIntTemp","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"InternalTemperature","unit_of_measurement":"°C","device_class":"temperature","id":"M_2df5ea3a","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/InternalTemperature"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"InternalHumidity","unit_of_measurement":"%","device_class":"humidity","id":"M_b4497c35","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/InternalHumidity"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"FanPWMValue","id":"M_8cd240b3","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/FanPWMValue"}})JSON"
R"JSON(]})JSON"
;
#endif
#if AC_ENABLE_BUZZER
static const char PANEL_PAGE_7[] PROGMEM =
R"JSON({"id":"buzzer","name":"Sounds","order":70,"fields":[)JSON"
R"JSON({"type":"select","name":"Startup sound","id":"M_6cfafdd6","persist":true,"default":"off","ha":{"state_topic":"stat/$device/BuzzerOnStart","command_topic":"cmnd/$device/BuzzerOnStart","optimistic":true},"options":[)JSON"
R"JSON({"value":"off","name":"Off"})JSON"
R"JSON(,{"value":"on","name":"On"})JSON"
R"JSON(]})JSON"
#if AC_ACTIVE_DIAGNOSTICS
R"JSON(,)JSON"
R"JSON({"type":"select","name":"Alarm sound","id":"M_bcbcfa69","persist":true,"default":"off","ha":{"state_topic":"stat/$device/BuzzerOnErrors","command_topic":"cmnd/$device/BuzzerOnErrors","optimistic":true},"options":[)JSON"
R"JSON({"value":"off","name":"Off"})JSON"
R"JSON(,{"value":"on","name":"On"})JSON"
R"JSON(]})JSON"
#endif
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"Sounds: Morning","id":"M_9dbfaff5","persist":true,"default":true,"ha":{"state_topic":"stat/$device/BuzzerMorning","command_topic":"cmnd/$device/BuzzerMorning","payload_on":"on","payload_off":"off"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"Sounds: Afternoon","id":"M_ec1e6b41","persist":true,"default":true,"ha":{"state_topic":"stat/$device/BuzzerAfternoon","command_topic":"cmnd/$device/BuzzerAfternoon","payload_on":"on","payload_off":"off"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"Sounds: Evening","id":"M_887e9631","persist":true,"default":true,"ha":{"state_topic":"stat/$device/BuzzerEvening","command_topic":"cmnd/$device/BuzzerEvening","payload_on":"on","payload_off":"off"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"Sounds: Night","id":"M_7de5b463","persist":true,"default":false,"ha":{"state_topic":"stat/$device/BuzzerNight","command_topic":"cmnd/$device/BuzzerNight","payload_on":"on","payload_off":"off"}})JSON"
R"JSON(]})JSON"
;
#endif
#if AC_ACTIVE_DIAGNOSTICS
static const char PANEL_PAGE_8[] PROGMEM =
R"JSON({"id":"diagnostics","name":"Diagnostics","order":80,"fields":[)JSON"
R"JSON({"type":"text","name":"Active alarms","id":"M_528b92e8","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/ErrorCount"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"Active alarm 1","id":"M_245a24bd","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/AlarmLast1"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"Active alarm 2","id":"M_215a2004","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/AlarmLast2"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"Active alarm 3","id":"M_225a2197","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/AlarmLast3"}})JSON"
R"JSON(]})JSON"
;
#endif
#if AC_ENABLE_DHT && !AC_ACTIVE_FAN
static const char PANEL_PAGE_9[] PROGMEM =
R"JSON({"id":"dht","name":"DHT","order":60,"fields":[)JSON"
R"JSON({"type":"slider","name":"Fan: Temperature alarm","min":0,"max":100,"step":1,"id":"M_1c6d9e10","persist":true,"default":100,"ha":{"state_topic":"stat/$device/MaxIntTemp","command_topic":"cmnd/$device/MaxIntTemp","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"InternalTemperature","unit_of_measurement":"°C","device_class":"temperature","id":"M_2df5ea3a","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/InternalTemperature"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"InternalHumidity","unit_of_measurement":"%","device_class":"humidity","id":"M_b4497c35","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/InternalHumidity"}})JSON"
R"JSON(]})JSON"
;
#endif
#if AC_ACTIVE_PERIPHERALS
static const char PANEL_PAGE_10[] PROGMEM =
R"JSON({"id":"peripherals","name":"Peripherals","order":90,"fields":[)JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON({"type":"select","name":"S1 (A4/D58): Function","id":"M_2f414012","persist":true,"default":"analog","ha":{"state_topic":"stat/$device/Sensor1Function","command_topic":"cmnd/$device/Sensor1Function","optimistic":true},"options":[)JSON"
R"JSON({"value":"analog","name":"Analog voltage"})JSON"
R"JSON(,{"value":"temperature","name":"Temperature"})JSON"
R"JSON(,{"value":"humidity","name":"Humidity"})JSON"
R"JSON(,{"value":"digital","name":"Digital ON/OFF"})JSON"
R"JSON(,{"value":"level","name":"Water level"})JSON"
R"JSON(,{"value":"leak","name":"Water leak"})JSON"
R"JSON(,{"value":"ph","name":"pH"})JSON"
R"JSON(,{"value":"disabled","name":"Disabled"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S1 (A4/D58): Model — Temperature","id":"M_7c6541a1","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor1TemperatureModel","command_topic":"cmnd/$device/Sensor1TemperatureModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_2f414012","equals":"temperature"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(,{"value":"ds18b20","name":"DS18B20"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S1 (A4/D58): Model — Humidity","id":"M_d062dd98","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor1HumidityModel","command_topic":"cmnd/$device/Sensor1HumidityModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_2f414012","equals":"humidity"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S1 (A4/D58): Invert input","id":"M_1d4ee537","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Sensor1Inverted","command_topic":"cmnd/$device/Sensor1Inverted","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_2f414012","equals":"digital"},{"field":"M_2f414012","equals":"level"},{"field":"M_2f414012","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S1 (A4/D58): Internal pull-up","id":"M_f777e368","persist":true,"default":true,"ha":{"state_topic":"stat/$device/Sensor1Pullup","command_topic":"cmnd/$device/Sensor1Pullup","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_2f414012","equals":"digital"},{"field":"M_2f414012","equals":"level"},{"field":"M_2f414012","equals":"leak"},{"field":"M_2f414012","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S1 (A4/D58): Debounce (ms)","id":"M_68829f0d","persist":true,"default":"50","ha":{"state_topic":"stat/$device/Sensor1DebounceMs","command_topic":"cmnd/$device/Sensor1DebounceMs","optimistic":true},"visibleWhen":{"or":[{"field":"M_2f414012","equals":"digital"},{"field":"M_2f414012","equals":"level"},{"field":"M_2f414012","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S1 (A4/D58): Pulses per litre","id":"M_b10a7a19","persist":true,"default":"450","ha":{"state_topic":"stat/$device/Sensor1PulsesPerLitre","command_topic":"cmnd/$device/Sensor1PulsesPerLitre","optimistic":true},"visibleWhen":{"or":[{"field":"M_2f414012","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S1 (A4/D58): Humidity (%)","id":"M_7ab6d71b","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor1Humidity","unit_of_measurement":"%","device_class":"humidity"},"visibleWhen":{"or":[{"field":"M_2f414012","equals":"humidity"},{"and":[{"field":"M_2f414012","equals":"temperature"},{"or":[{"field":"M_7c6541a1","equals":"dht11"},{"field":"M_7c6541a1","equals":"dht12"},{"field":"M_7c6541a1","equals":"dht21"},{"field":"M_7c6541a1","equals":"dht22"}]}]}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S2 (A5/D59): Function","id":"M_bf3550cb","persist":true,"default":"analog","ha":{"state_topic":"stat/$device/Sensor2Function","command_topic":"cmnd/$device/Sensor2Function","optimistic":true},"options":[)JSON"
R"JSON({"value":"analog","name":"Analog voltage"})JSON"
R"JSON(,{"value":"temperature","name":"Temperature"})JSON"
R"JSON(,{"value":"humidity","name":"Humidity"})JSON"
R"JSON(,{"value":"digital","name":"Digital ON/OFF"})JSON"
R"JSON(,{"value":"level","name":"Water level"})JSON"
R"JSON(,{"value":"leak","name":"Water leak"})JSON"
R"JSON(,{"value":"ph","name":"pH"})JSON"
R"JSON(,{"value":"disabled","name":"Disabled"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S2 (A5/D59): Model — Temperature","id":"M_82b152b4","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor2TemperatureModel","command_topic":"cmnd/$device/Sensor2TemperatureModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_bf3550cb","equals":"temperature"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(,{"value":"ds18b20","name":"DS18B20"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S2 (A5/D59): Model — Humidity","id":"M_1b7e5823","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor2HumidityModel","command_topic":"cmnd/$device/Sensor2HumidityModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_bf3550cb","equals":"humidity"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S2 (A5/D59): Invert input","id":"M_48f5666a","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Sensor2Inverted","command_topic":"cmnd/$device/Sensor2Inverted","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_bf3550cb","equals":"digital"},{"field":"M_bf3550cb","equals":"level"},{"field":"M_bf3550cb","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S2 (A5/D59): Internal pull-up","id":"M_14918319","persist":true,"default":true,"ha":{"state_topic":"stat/$device/Sensor2Pullup","command_topic":"cmnd/$device/Sensor2Pullup","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_bf3550cb","equals":"digital"},{"field":"M_bf3550cb","equals":"level"},{"field":"M_bf3550cb","equals":"leak"},{"field":"M_bf3550cb","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S2 (A5/D59): Debounce (ms)","id":"M_d531324c","persist":true,"default":"50","ha":{"state_topic":"stat/$device/Sensor2DebounceMs","command_topic":"cmnd/$device/Sensor2DebounceMs","optimistic":true},"visibleWhen":{"or":[{"field":"M_bf3550cb","equals":"digital"},{"field":"M_bf3550cb","equals":"level"},{"field":"M_bf3550cb","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S2 (A5/D59): Pulses per litre","id":"M_46c504a0","persist":true,"default":"450","ha":{"state_topic":"stat/$device/Sensor2PulsesPerLitre","command_topic":"cmnd/$device/Sensor2PulsesPerLitre","optimistic":true},"visibleWhen":{"or":[{"field":"M_bf3550cb","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S2 (A5/D59): Humidity (%)","id":"M_830fcef6","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor2Humidity","unit_of_measurement":"%","device_class":"humidity"},"visibleWhen":{"or":[{"field":"M_bf3550cb","equals":"humidity"},{"and":[{"field":"M_bf3550cb","equals":"temperature"},{"or":[{"field":"M_82b152b4","equals":"dht11"},{"field":"M_82b152b4","equals":"dht12"},{"field":"M_82b152b4","equals":"dht21"},{"field":"M_82b152b4","equals":"dht22"}]}]}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S3 (A6/D60): Function","id":"M_09054f00","persist":true,"default":"analog","ha":{"state_topic":"stat/$device/Sensor3Function","command_topic":"cmnd/$device/Sensor3Function","optimistic":true},"options":[)JSON"
R"JSON({"value":"analog","name":"Analog voltage"})JSON"
R"JSON(,{"value":"temperature","name":"Temperature"})JSON"
R"JSON(,{"value":"humidity","name":"Humidity"})JSON"
R"JSON(,{"value":"digital","name":"Digital ON/OFF"})JSON"
R"JSON(,{"value":"level","name":"Water level"})JSON"
R"JSON(,{"value":"leak","name":"Water leak"})JSON"
R"JSON(,{"value":"ph","name":"pH"})JSON"
R"JSON(,{"value":"disabled","name":"Disabled"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S3 (A6/D60): Model — Temperature","id":"M_35c4318f","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor3TemperatureModel","command_topic":"cmnd/$device/Sensor3TemperatureModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_09054f00","equals":"temperature"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(,{"value":"ds18b20","name":"DS18B20"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S3 (A6/D60): Model — Humidity","id":"M_afa11212","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor3HumidityModel","command_topic":"cmnd/$device/Sensor3HumidityModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_09054f00","equals":"humidity"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S3 (A6/D60): Invert input","id":"M_463cb29d","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Sensor3Inverted","command_topic":"cmnd/$device/Sensor3Inverted","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_09054f00","equals":"digital"},{"field":"M_09054f00","equals":"level"},{"field":"M_09054f00","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S3 (A6/D60): Internal pull-up","id":"M_8562d582","persist":true,"default":true,"ha":{"state_topic":"stat/$device/Sensor3Pullup","command_topic":"cmnd/$device/Sensor3Pullup","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_09054f00","equals":"digital"},{"field":"M_09054f00","equals":"level"},{"field":"M_09054f00","equals":"leak"},{"field":"M_09054f00","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S3 (A6/D60): Debounce (ms)","id":"M_f1db6953","persist":true,"default":"50","ha":{"state_topic":"stat/$device/Sensor3DebounceMs","command_topic":"cmnd/$device/Sensor3DebounceMs","optimistic":true},"visibleWhen":{"or":[{"field":"M_09054f00","equals":"digital"},{"field":"M_09054f00","equals":"level"},{"field":"M_09054f00","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S3 (A6/D60): Pulses per litre","id":"M_a8474683","persist":true,"default":"450","ha":{"state_topic":"stat/$device/Sensor3PulsesPerLitre","command_topic":"cmnd/$device/Sensor3PulsesPerLitre","optimistic":true},"visibleWhen":{"or":[{"field":"M_09054f00","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S3 (A6/D60): Humidity (%)","id":"M_5328b83d","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor3Humidity","unit_of_measurement":"%","device_class":"humidity"},"visibleWhen":{"or":[{"field":"M_09054f00","equals":"humidity"},{"and":[{"field":"M_09054f00","equals":"temperature"},{"or":[{"field":"M_35c4318f","equals":"dht11"},{"field":"M_35c4318f","equals":"dht12"},{"field":"M_35c4318f","equals":"dht21"},{"field":"M_35c4318f","equals":"dht22"}]}]}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S4 (A7/D61): Function","id":"M_e7d74181","persist":true,"default":"analog","ha":{"state_topic":"stat/$device/Sensor4Function","command_topic":"cmnd/$device/Sensor4Function","optimistic":true},"options":[)JSON"
R"JSON({"value":"analog","name":"Analog voltage"})JSON"
R"JSON(,{"value":"temperature","name":"Temperature"})JSON"
R"JSON(,{"value":"humidity","name":"Humidity"})JSON"
R"JSON(,{"value":"digital","name":"Digital ON/OFF"})JSON"
R"JSON(,{"value":"level","name":"Water level"})JSON"
R"JSON(,{"value":"leak","name":"Water leak"})JSON"
R"JSON(,{"value":"ph","name":"pH"})JSON"
R"JSON(,{"value":"disabled","name":"Disabled"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S4 (A7/D61): Model — Temperature","id":"M_5c9d1f62","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor4TemperatureModel","command_topic":"cmnd/$device/Sensor4TemperatureModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_e7d74181","equals":"temperature"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(,{"value":"ds18b20","name":"DS18B20"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S4 (A7/D61): Model — Humidity","id":"M_9571f0f5","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor4HumidityModel","command_topic":"cmnd/$device/Sensor4HumidityModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_e7d74181","equals":"humidity"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S4 (A7/D61): Invert input","id":"M_2275a5d0","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Sensor4Inverted","command_topic":"cmnd/$device/Sensor4Inverted","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_e7d74181","equals":"digital"},{"field":"M_e7d74181","equals":"level"},{"field":"M_e7d74181","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S4 (A7/D61): Internal pull-up","id":"M_0e9c1f2b","persist":true,"default":true,"ha":{"state_topic":"stat/$device/Sensor4Pullup","command_topic":"cmnd/$device/Sensor4Pullup","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_e7d74181","equals":"digital"},{"field":"M_e7d74181","equals":"level"},{"field":"M_e7d74181","equals":"leak"},{"field":"M_e7d74181","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S4 (A7/D61): Debounce (ms)","id":"M_ee2df872","persist":true,"default":"50","ha":{"state_topic":"stat/$device/Sensor4DebounceMs","command_topic":"cmnd/$device/Sensor4DebounceMs","optimistic":true},"visibleWhen":{"or":[{"field":"M_e7d74181","equals":"digital"},{"field":"M_e7d74181","equals":"level"},{"field":"M_e7d74181","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S4 (A7/D61): Pulses per litre","id":"M_0ebbdd5a","persist":true,"default":"450","ha":{"state_topic":"stat/$device/Sensor4PulsesPerLitre","command_topic":"cmnd/$device/Sensor4PulsesPerLitre","optimistic":true},"visibleWhen":{"or":[{"field":"M_e7d74181","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S4 (A7/D61): Humidity (%)","id":"M_6ddf5088","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor4Humidity","unit_of_measurement":"%","device_class":"humidity"},"visibleWhen":{"or":[{"field":"M_e7d74181","equals":"humidity"},{"and":[{"field":"M_e7d74181","equals":"temperature"},{"or":[{"field":"M_5c9d1f62","equals":"dht11"},{"field":"M_5c9d1f62","equals":"dht12"},{"field":"M_5c9d1f62","equals":"dht21"},{"field":"M_5c9d1f62","equals":"dht22"}]}]}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S5 (A8/D62): Function","id":"M_3e5b8f7e","persist":true,"default":"analog","ha":{"state_topic":"stat/$device/Sensor5Function","command_topic":"cmnd/$device/Sensor5Function","optimistic":true},"options":[)JSON"
R"JSON({"value":"analog","name":"Analog voltage"})JSON"
R"JSON(,{"value":"temperature","name":"Temperature"})JSON"
R"JSON(,{"value":"humidity","name":"Humidity"})JSON"
R"JSON(,{"value":"digital","name":"Digital ON/OFF"})JSON"
R"JSON(,{"value":"level","name":"Water level"})JSON"
R"JSON(,{"value":"leak","name":"Water leak"})JSON"
R"JSON(,{"value":"flow","name":"Flow rate"})JSON"
R"JSON(,{"value":"ph","name":"pH"})JSON"
R"JSON(,{"value":"disabled","name":"Disabled"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S5 (A8/D62): Model — Temperature","id":"M_f30c6cfd","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor5TemperatureModel","command_topic":"cmnd/$device/Sensor5TemperatureModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_3e5b8f7e","equals":"temperature"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(,{"value":"ds18b20","name":"DS18B20"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S5 (A8/D62): Model — Humidity","id":"M_d260e0c4","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor5HumidityModel","command_topic":"cmnd/$device/Sensor5HumidityModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_3e5b8f7e","equals":"humidity"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S5 (A8/D62): Invert input","id":"M_f09fcdc3","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Sensor5Inverted","command_topic":"cmnd/$device/Sensor5Inverted","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_3e5b8f7e","equals":"digital"},{"field":"M_3e5b8f7e","equals":"level"},{"field":"M_3e5b8f7e","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S5 (A8/D62): Internal pull-up","id":"M_412c4b04","persist":true,"default":true,"ha":{"state_topic":"stat/$device/Sensor5Pullup","command_topic":"cmnd/$device/Sensor5Pullup","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_3e5b8f7e","equals":"digital"},{"field":"M_3e5b8f7e","equals":"level"},{"field":"M_3e5b8f7e","equals":"leak"},{"field":"M_3e5b8f7e","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S5 (A8/D62): Debounce (ms)","id":"M_704553a1","persist":true,"default":"50","ha":{"state_topic":"stat/$device/Sensor5DebounceMs","command_topic":"cmnd/$device/Sensor5DebounceMs","optimistic":true},"visibleWhen":{"or":[{"field":"M_3e5b8f7e","equals":"digital"},{"field":"M_3e5b8f7e","equals":"level"},{"field":"M_3e5b8f7e","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S5 (A8/D62): Pulses per litre","id":"M_29581a9d","persist":true,"default":"450","ha":{"state_topic":"stat/$device/Sensor5PulsesPerLitre","command_topic":"cmnd/$device/Sensor5PulsesPerLitre","optimistic":true},"visibleWhen":{"or":[{"field":"M_3e5b8f7e","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S5 (A8/D62): Humidity (%)","id":"M_05800647","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor5Humidity","unit_of_measurement":"%","device_class":"humidity"},"visibleWhen":{"or":[{"field":"M_3e5b8f7e","equals":"humidity"},{"and":[{"field":"M_3e5b8f7e","equals":"temperature"},{"or":[{"field":"M_f30c6cfd","equals":"dht11"},{"field":"M_f30c6cfd","equals":"dht12"},{"field":"M_f30c6cfd","equals":"dht21"},{"field":"M_f30c6cfd","equals":"dht22"}]}]}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S6 (A9/D63): Function","id":"M_fbedddb7","persist":true,"default":"analog","ha":{"state_topic":"stat/$device/Sensor6Function","command_topic":"cmnd/$device/Sensor6Function","optimistic":true},"options":[)JSON"
R"JSON({"value":"analog","name":"Analog voltage"})JSON"
R"JSON(,{"value":"temperature","name":"Temperature"})JSON"
R"JSON(,{"value":"humidity","name":"Humidity"})JSON"
R"JSON(,{"value":"digital","name":"Digital ON/OFF"})JSON"
R"JSON(,{"value":"level","name":"Water level"})JSON"
R"JSON(,{"value":"leak","name":"Water leak"})JSON"
R"JSON(,{"value":"flow","name":"Flow rate"})JSON"
R"JSON(,{"value":"ph","name":"pH"})JSON"
R"JSON(,{"value":"disabled","name":"Disabled"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S6 (A9/D63): Model — Temperature","id":"M_49e7ea70","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor6TemperatureModel","command_topic":"cmnd/$device/Sensor6TemperatureModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_fbedddb7","equals":"temperature"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(,{"value":"ds18b20","name":"DS18B20"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S6 (A9/D63): Model — Humidity","id":"M_82e17ebf","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor6HumidityModel","command_topic":"cmnd/$device/Sensor6HumidityModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_fbedddb7","equals":"humidity"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S6 (A9/D63): Invert input","id":"M_de7eb9c6","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Sensor6Inverted","command_topic":"cmnd/$device/Sensor6Inverted","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_fbedddb7","equals":"digital"},{"field":"M_fbedddb7","equals":"level"},{"field":"M_fbedddb7","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S6 (A9/D63): Internal pull-up","id":"M_be44f7d5","persist":true,"default":true,"ha":{"state_topic":"stat/$device/Sensor6Pullup","command_topic":"cmnd/$device/Sensor6Pullup","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_fbedddb7","equals":"digital"},{"field":"M_fbedddb7","equals":"level"},{"field":"M_fbedddb7","equals":"leak"},{"field":"M_fbedddb7","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S6 (A9/D63): Debounce (ms)","id":"M_40f14a90","persist":true,"default":"50","ha":{"state_topic":"stat/$device/Sensor6DebounceMs","command_topic":"cmnd/$device/Sensor6DebounceMs","optimistic":true},"visibleWhen":{"or":[{"field":"M_fbedddb7","equals":"digital"},{"field":"M_fbedddb7","equals":"level"},{"field":"M_fbedddb7","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S6 (A9/D63): Pulses per litre","id":"M_7842b824","persist":true,"default":"450","ha":{"state_topic":"stat/$device/Sensor6PulsesPerLitre","command_topic":"cmnd/$device/Sensor6PulsesPerLitre","optimistic":true},"visibleWhen":{"or":[{"field":"M_fbedddb7","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S6 (A9/D63): Humidity (%)","id":"M_5f98c842","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor6Humidity","unit_of_measurement":"%","device_class":"humidity"},"visibleWhen":{"or":[{"field":"M_fbedddb7","equals":"humidity"},{"and":[{"field":"M_fbedddb7","equals":"temperature"},{"or":[{"field":"M_49e7ea70","equals":"dht11"},{"field":"M_49e7ea70","equals":"dht12"},{"field":"M_49e7ea70","equals":"dht21"},{"field":"M_49e7ea70","equals":"dht22"}]}]}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S7 (A10/D64): Function","id":"M_67e9ce6c","persist":true,"default":"analog","ha":{"state_topic":"stat/$device/Sensor7Function","command_topic":"cmnd/$device/Sensor7Function","optimistic":true},"options":[)JSON"
R"JSON({"value":"analog","name":"Analog voltage"})JSON"
R"JSON(,{"value":"temperature","name":"Temperature"})JSON"
R"JSON(,{"value":"humidity","name":"Humidity"})JSON"
R"JSON(,{"value":"digital","name":"Digital ON/OFF"})JSON"
R"JSON(,{"value":"level","name":"Water level"})JSON"
R"JSON(,{"value":"leak","name":"Water leak"})JSON"
R"JSON(,{"value":"flow","name":"Flow rate"})JSON"
R"JSON(,{"value":"ph","name":"pH"})JSON"
R"JSON(,{"value":"disabled","name":"Disabled"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S7 (A10/D64): Model — Temperature","id":"M_b7c7f8bb","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor7TemperatureModel","command_topic":"cmnd/$device/Sensor7TemperatureModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_67e9ce6c","equals":"temperature"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(,{"value":"ds18b20","name":"DS18B20"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S7 (A10/D64): Model — Humidity","id":"M_857c098e","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor7HumidityModel","command_topic":"cmnd/$device/Sensor7HumidityModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_67e9ce6c","equals":"humidity"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S7 (A10/D64): Invert input","id":"M_db4d26e9","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Sensor7Inverted","command_topic":"cmnd/$device/Sensor7Inverted","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_67e9ce6c","equals":"digital"},{"field":"M_67e9ce6c","equals":"level"},{"field":"M_67e9ce6c","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S7 (A10/D64): Internal pull-up","id":"M_a6ce489e","persist":true,"default":true,"ha":{"state_topic":"stat/$device/Sensor7Pullup","command_topic":"cmnd/$device/Sensor7Pullup","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_67e9ce6c","equals":"digital"},{"field":"M_67e9ce6c","equals":"level"},{"field":"M_67e9ce6c","equals":"leak"},{"field":"M_67e9ce6c","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S7 (A10/D64): Debounce (ms)","id":"M_a24e2967","persist":true,"default":"50","ha":{"state_topic":"stat/$device/Sensor7DebounceMs","command_topic":"cmnd/$device/Sensor7DebounceMs","optimistic":true},"visibleWhen":{"or":[{"field":"M_67e9ce6c","equals":"digital"},{"field":"M_67e9ce6c","equals":"level"},{"field":"M_67e9ce6c","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S7 (A10/D64): Pulses per litre","id":"M_513a35b7","persist":true,"default":"450","ha":{"state_topic":"stat/$device/Sensor7PulsesPerLitre","command_topic":"cmnd/$device/Sensor7PulsesPerLitre","optimistic":true},"visibleWhen":{"or":[{"field":"M_67e9ce6c","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S7 (A10/D64): Humidity (%)","id":"M_90223219","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor7Humidity","unit_of_measurement":"%","device_class":"humidity"},"visibleWhen":{"or":[{"field":"M_67e9ce6c","equals":"humidity"},{"and":[{"field":"M_67e9ce6c","equals":"temperature"},{"or":[{"field":"M_b7c7f8bb","equals":"dht11"},{"field":"M_b7c7f8bb","equals":"dht12"},{"field":"M_b7c7f8bb","equals":"dht21"},{"field":"M_b7c7f8bb","equals":"dht22"}]}]}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S8 (A11/D65): Function","id":"M_cee7647d","persist":true,"default":"analog","ha":{"state_topic":"stat/$device/Sensor8Function","command_topic":"cmnd/$device/Sensor8Function","optimistic":true},"options":[)JSON"
R"JSON({"value":"analog","name":"Analog voltage"})JSON"
R"JSON(,{"value":"temperature","name":"Temperature"})JSON"
R"JSON(,{"value":"humidity","name":"Humidity"})JSON"
R"JSON(,{"value":"digital","name":"Digital ON/OFF"})JSON"
R"JSON(,{"value":"level","name":"Water level"})JSON"
R"JSON(,{"value":"leak","name":"Water leak"})JSON"
R"JSON(,{"value":"flow","name":"Flow rate"})JSON"
R"JSON(,{"value":"ph","name":"pH"})JSON"
R"JSON(,{"value":"disabled","name":"Disabled"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S8 (A11/D65): Model — Temperature","id":"M_d15ae6ee","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor8TemperatureModel","command_topic":"cmnd/$device/Sensor8TemperatureModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_cee7647d","equals":"temperature"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(,{"value":"ds18b20","name":"DS18B20"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"S8 (A11/D65): Model — Humidity","id":"M_612ad0e1","persist":true,"default":"dht11","ha":{"state_topic":"stat/$device/Sensor8HumidityModel","command_topic":"cmnd/$device/Sensor8HumidityModel","optimistic":true},"visibleWhen":{"or":[{"field":"M_cee7647d","equals":"humidity"}]},"options":[)JSON"
R"JSON({"value":"dht11","name":"DHT11"})JSON"
R"JSON(,{"value":"dht12","name":"DHT12 (single-wire)"})JSON"
R"JSON(,{"value":"dht21","name":"DHT21 / AM2301"})JSON"
R"JSON(,{"value":"dht22","name":"DHT22 / AM2302"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S8 (A11/D65): Invert input","id":"M_0040ea0c","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Sensor8Inverted","command_topic":"cmnd/$device/Sensor8Inverted","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_cee7647d","equals":"digital"},{"field":"M_cee7647d","equals":"level"},{"field":"M_cee7647d","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"switch","name":"S8 (A11/D65): Internal pull-up","id":"M_566d3227","persist":true,"default":true,"ha":{"state_topic":"stat/$device/Sensor8Pullup","command_topic":"cmnd/$device/Sensor8Pullup","payload_on":"on","payload_off":"off"},"visibleWhen":{"or":[{"field":"M_cee7647d","equals":"digital"},{"field":"M_cee7647d","equals":"level"},{"field":"M_cee7647d","equals":"leak"},{"field":"M_cee7647d","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S8 (A11/D65): Debounce (ms)","id":"M_587dc646","persist":true,"default":"50","ha":{"state_topic":"stat/$device/Sensor8DebounceMs","command_topic":"cmnd/$device/Sensor8DebounceMs","optimistic":true},"visibleWhen":{"or":[{"field":"M_cee7647d","equals":"digital"},{"field":"M_cee7647d","equals":"level"},{"field":"M_cee7647d","equals":"leak"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"edit","name":"S8 (A11/D65): Pulses per litre","id":"M_20bb30fe","persist":true,"default":"450","ha":{"state_topic":"stat/$device/Sensor8PulsesPerLitre","command_topic":"cmnd/$device/Sensor8PulsesPerLitre","optimistic":true},"visibleWhen":{"or":[{"field":"M_cee7647d","equals":"flow"}]}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"text","name":"S8 (A11/D65): Humidity (%)","id":"M_41f3cc34","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor8Humidity","unit_of_measurement":"%","device_class":"humidity"},"visibleWhen":{"or":[{"field":"M_cee7647d","equals":"humidity"},{"and":[{"field":"M_cee7647d","equals":"temperature"},{"or":[{"field":"M_d15ae6ee","equals":"dht11"},{"field":"M_d15ae6ee","equals":"dht12"},{"field":"M_d15ae6ee","equals":"dht21"},{"field":"M_d15ae6ee","equals":"dht22"}]}]}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM1 (D5): Function","id":"M_a4f56877","persist":true,"default":"pwm","ha":{"state_topic":"stat/$device/PWMOutput1Device","command_topic":"cmnd/$device/PWMOutput1Device","optimistic":true},"options":[)JSON"
R"JSON({"value":"pwm","name":"Pwm"})JSON"
R"JSON(,{"value":"servo","name":"Servo"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8) || (AC_ACTIVE_PWM >= 1)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM2 (D44): Function","id":"M_9cfbc76a","persist":true,"default":"pwm","ha":{"state_topic":"stat/$device/PWMOutput2Device","command_topic":"cmnd/$device/PWMOutput2Device","optimistic":true},"options":[)JSON"
R"JSON({"value":"pwm","name":"Pwm"})JSON"
R"JSON(,{"value":"servo","name":"Servo"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8) || (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 2)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM3 (D45): Function","id":"M_64472f39","persist":true,"default":"pwm","ha":{"state_topic":"stat/$device/PWMOutput3Device","command_topic":"cmnd/$device/PWMOutput3Device","optimistic":true},"options":[)JSON"
R"JSON({"value":"pwm","name":"Pwm"})JSON"
R"JSON(,{"value":"servo","name":"Servo"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8) || (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 3)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM4 (D46): Function","id":"M_ec9b2994","persist":true,"default":"pwm","ha":{"state_topic":"stat/$device/PWMOutput4Device","command_topic":"cmnd/$device/PWMOutput4Device","optimistic":true},"options":[)JSON"
R"JSON({"value":"pwm","name":"Pwm"})JSON"
R"JSON(,{"value":"servo","name":"Servo"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8) || (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 4)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM5 (D11): Function","id":"M_cb6f9733","persist":true,"default":"pwm","ha":{"state_topic":"stat/$device/PWMOutput5Device","command_topic":"cmnd/$device/PWMOutput5Device","optimistic":true},"options":[)JSON"
R"JSON({"value":"pwm","name":"Pwm"})JSON"
R"JSON(,{"value":"servo","name":"Servo"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
#if (AC_ACTIVE_INPUTS >= 1) || (AC_ACTIVE_INPUTS >= 2) || (AC_ACTIVE_INPUTS >= 3) || (AC_ACTIVE_INPUTS >= 4) || (AC_ACTIVE_INPUTS >= 5) || (AC_ACTIVE_INPUTS >= 6) || (AC_ACTIVE_INPUTS >= 7) || (AC_ACTIVE_INPUTS >= 8) || (AC_ACTIVE_PWM >= 1) || (AC_ACTIVE_PWM >= 2) || (AC_ACTIVE_PWM >= 3) || (AC_ACTIVE_PWM >= 4) || (AC_ACTIVE_PWM >= 5)
R"JSON(,)JSON"
#endif
R"JSON({"type":"select","name":"PWM6 (D12): Function","id":"M_b847c546","persist":true,"default":"pwm","ha":{"state_topic":"stat/$device/PWMOutput6Device","command_topic":"cmnd/$device/PWMOutput6Device","optimistic":true},"options":[)JSON"
R"JSON({"value":"pwm","name":"Pwm"})JSON"
R"JSON(,{"value":"servo","name":"Servo"})JSON"
R"JSON(]})JSON"
#endif
R"JSON(]})JSON"
;
#endif
static const __FlashStringHelper* panelPageDefinition(byte index) { switch(index) {
#if 1
case 0: return FPSTR(PANEL_PAGE_0);
#endif
#if AC_ACTIVE_RELAYS > 0
case 1: return FPSTR(PANEL_PAGE_1);
#endif
#if 1
case 2: return FPSTR(PANEL_PAGE_2);
#endif
#if AC_ACTIVE_INPUTS > 0
case 3: return FPSTR(PANEL_PAGE_3);
#endif
#if AC_ACTIVE_PWM > 0
case 4: return FPSTR(PANEL_PAGE_4);
#endif
#if 1
case 5: return FPSTR(PANEL_PAGE_5);
#endif
#if AC_ACTIVE_FAN
case 6: return FPSTR(PANEL_PAGE_6);
#endif
#if AC_ENABLE_BUZZER
case 7: return FPSTR(PANEL_PAGE_7);
#endif
#if AC_ACTIVE_DIAGNOSTICS
case 8: return FPSTR(PANEL_PAGE_8);
#endif
#if AC_ENABLE_DHT && !AC_ACTIVE_FAN
case 9: return FPSTR(PANEL_PAGE_9);
#endif
#if AC_ACTIVE_PERIPHERALS
case 10: return FPSTR(PANEL_PAGE_10);
#endif
default: return nullptr; }}
