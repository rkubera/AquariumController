// Generated shared Portal/HA definitions, filtered by ESP feature defines.
#if 1
static const char PANEL_PAGE_0[] PROGMEM =
R"JSON({"id":"led","name":"LED","order":0,"fields":[)JSON"
R"JSON({"type":"light","name":"LED: Status","id":"M_72085391","persist":false,"default":{"state":"OFF","brightness":0,"color":{"r":255,"g":255,"b":255},"color_mode":"rgb"},"ha":{"state_topic":"stat/$device/CustomColor","command_topic":"cmnd/$device/CustomColor","schema":"json","brightness":true,"brightness_scale":100,"supported_color_modes":["rgb"]},"colorLabel":"Custom Color","controls":[{"key":"state","type":"switch","name":"LED: Stan","command":"","on":"ON","off":"OFF"},{"key":"brightness","type":"slider","name":"LED: Aktualna jasność (%)","command":"","min":0,"max":100,"step":1},{"key":"color","type":"color","name":"LED: CustomColor","command":""}]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"LED: Tryb","id":"M_b2e7d268","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/LedControlMode","command_topic":"cmnd/$device/LedControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"manual"})JSON"
R"JSON(,{"value":"partofday","name":"partofday"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"LED: Kolor ręczny","id":"M_f35844ff","persist":true,"default":"black","ha":{"state_topic":"stat/$device/LedManualMode","command_topic":"cmnd/$device/LedManualMode","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"manual"},"options":[)JSON"
R"JSON({"value":"black","name":"black"})JSON"
R"JSON(,{"value":"red","name":"red"})JSON"
R"JSON(,{"value":"green","name":"green"})JSON"
R"JSON(,{"value":"blue","name":"blue"})JSON"
R"JSON(,{"value":"cyan","name":"cyan"})JSON"
R"JSON(,{"value":"magenta","name":"magenta"})JSON"
R"JSON(,{"value":"yellow","name":"yellow"})JSON"
R"JSON(,{"value":"white","name":"white"})JSON"
R"JSON(,{"value":"wave","name":"wave"})JSON"
R"JSON(,{"value":"custom","name":"custom"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"LED: Rano — kolor","id":"M_e4365a67","persist":true,"default":"black","ha":{"state_topic":"stat/$device/LedColorMorning","command_topic":"cmnd/$device/LedColorMorning","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"},"options":[)JSON"
R"JSON({"value":"black","name":"black"})JSON"
R"JSON(,{"value":"red","name":"red"})JSON"
R"JSON(,{"value":"green","name":"green"})JSON"
R"JSON(,{"value":"blue","name":"blue"})JSON"
R"JSON(,{"value":"cyan","name":"cyan"})JSON"
R"JSON(,{"value":"magenta","name":"magenta"})JSON"
R"JSON(,{"value":"yellow","name":"yellow"})JSON"
R"JSON(,{"value":"white","name":"white"})JSON"
R"JSON(,{"value":"wave","name":"wave"})JSON"
R"JSON(,{"value":"custom","name":"custom"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"LED: Rano — jasność (%)","min":0,"max":100,"step":1,"id":"M_80e08794","persist":true,"default":0,"ha":{"state_topic":"stat/$device/MorningBrightness","command_topic":"cmnd/$device/MorningBrightness","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"LED: Popołudnie — kolor","id":"M_71bf461f","persist":true,"default":"black","ha":{"state_topic":"stat/$device/LedColorAfternoon","command_topic":"cmnd/$device/LedColorAfternoon","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"},"options":[)JSON"
R"JSON({"value":"black","name":"black"})JSON"
R"JSON(,{"value":"red","name":"red"})JSON"
R"JSON(,{"value":"green","name":"green"})JSON"
R"JSON(,{"value":"blue","name":"blue"})JSON"
R"JSON(,{"value":"cyan","name":"cyan"})JSON"
R"JSON(,{"value":"magenta","name":"magenta"})JSON"
R"JSON(,{"value":"yellow","name":"yellow"})JSON"
R"JSON(,{"value":"white","name":"white"})JSON"
R"JSON(,{"value":"wave","name":"wave"})JSON"
R"JSON(,{"value":"custom","name":"custom"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"LED: Popołudnie — jasność (%)","min":0,"max":100,"step":1,"id":"M_133c350c","persist":true,"default":0,"ha":{"state_topic":"stat/$device/AfternoonBrightness","command_topic":"cmnd/$device/AfternoonBrightness","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"LED: Wieczór — kolor","id":"M_c13e66e7","persist":true,"default":"black","ha":{"state_topic":"stat/$device/LedColorEvening","command_topic":"cmnd/$device/LedColorEvening","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"},"options":[)JSON"
R"JSON({"value":"black","name":"black"})JSON"
R"JSON(,{"value":"red","name":"red"})JSON"
R"JSON(,{"value":"green","name":"green"})JSON"
R"JSON(,{"value":"blue","name":"blue"})JSON"
R"JSON(,{"value":"cyan","name":"cyan"})JSON"
R"JSON(,{"value":"magenta","name":"magenta"})JSON"
R"JSON(,{"value":"yellow","name":"yellow"})JSON"
R"JSON(,{"value":"white","name":"white"})JSON"
R"JSON(,{"value":"wave","name":"wave"})JSON"
R"JSON(,{"value":"custom","name":"custom"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"LED: Wieczór — jasność (%)","min":0,"max":100,"step":1,"id":"M_ba525294","persist":true,"default":0,"ha":{"state_topic":"stat/$device/EveningBrightness","command_topic":"cmnd/$device/EveningBrightness","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"LED: Noc — kolor","id":"M_c09682f9","persist":true,"default":"black","ha":{"state_topic":"stat/$device/LedColorNight","command_topic":"cmnd/$device/LedColorNight","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"},"options":[)JSON"
R"JSON({"value":"black","name":"black"})JSON"
R"JSON(,{"value":"red","name":"red"})JSON"
R"JSON(,{"value":"green","name":"green"})JSON"
R"JSON(,{"value":"blue","name":"blue"})JSON"
R"JSON(,{"value":"cyan","name":"cyan"})JSON"
R"JSON(,{"value":"magenta","name":"magenta"})JSON"
R"JSON(,{"value":"yellow","name":"yellow"})JSON"
R"JSON(,{"value":"white","name":"white"})JSON"
R"JSON(,{"value":"wave","name":"wave"})JSON"
R"JSON(,{"value":"custom","name":"custom"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"LED: Noc — jasność (%)","min":0,"max":100,"step":1,"id":"M_03f6f016","persist":true,"default":0,"ha":{"state_topic":"stat/$device/NightBrightness","command_topic":"cmnd/$device/NightBrightness","optimistic":true},"visibleWhen":{"field":"M_b2e7d268","equals":"partofday"}})JSON"
R"JSON(]})JSON"
;
#endif
#if AC_ACTIVE_RELAYS > 0
static const char PANEL_PAGE_1[] PROGMEM =
R"JSON({"id":"relays","name":"Przekaźniki","order":10,"fields":[)JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON({"type":"edit","name":"R1: Nazwa","id":"M_22be3062","persist":true,"default":"","ha":{"state_topic":"stat/$device/Relay1Name","command_topic":"cmnd/$device/Relay1Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,)JSON"
R"JSON({"type":"select","name":"R1: Tryb","id":"M_ee1455c9","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/Relay1ControlMode","command_topic":"cmnd/$device/Relay1ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"manual"})JSON"
R"JSON(,{"value":"partofday","name":"partofday"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"treshold"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R1: Stan","id":"M_9ab64b8a","persist":false,"ha":{"state_topic":"stat/$device/Relay1State","command_topic":"cmnd/$device/Relay1State","payload_on":"on","payload_off":"off"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"R1: Kierunek","id":"M_b97662b7","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/Relay1ControlDirection","command_topic":"cmnd/$device/Relay1ControlDirection","optimistic":true},"visibleWhen":{"field":"M_ee1455c9","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"direct","name":"direct"})JSON"
R"JSON(,{"value":"reverse","name":"reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"R1: Wejście analogowe","id":"M_b5dfdf8f","persist":true,"default":"none","ha":{"state_topic":"stat/$device/Relay1Input","command_topic":"cmnd/$device/Relay1Input","optimistic":true},"visibleWhen":{"field":"M_ee1455c9","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"none","name":"none"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"R1: Wartość zadana","id":"M_805a6e2a","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay1SensorsSetpoint","command_topic":"cmnd/$device/Relay1SensorsSetpoint","optimistic":true},"visibleWhen":{"field":"M_ee1455c9","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"R1: Połowa szerokości histerezy","id":"M_bf8a6d7c","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay1MaxDeviation","command_topic":"cmnd/$device/Relay1MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_ee1455c9","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R1: Rano","id":"M_fbb23b5c","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay1MorningMode","command_topic":"cmnd/$device/Relay1MorningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ee1455c9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R1: Popołudnie","id":"M_4718b314","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay1AfternoonMode","command_topic":"cmnd/$device/Relay1AfternoonMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ee1455c9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R1: Wieczór","id":"M_04365844","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay1EveningMode","command_topic":"cmnd/$device/Relay1EveningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ee1455c9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R1: Noc","id":"M_7f32a382","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay1NightMode","command_topic":"cmnd/$device/Relay1NightMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ee1455c9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"text","name":"R1: SensorsValue","id":"M_05dc2f55","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Relay1SensorsValue"},"visibleWhen":{"field":"M_ee1455c9","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"R2: Nazwa","id":"M_307edc53","persist":true,"default":"","ha":{"state_topic":"stat/$device/Relay2Name","command_topic":"cmnd/$device/Relay2Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,)JSON"
R"JSON({"type":"select","name":"R2: Tryb","id":"M_519ed402","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/Relay2ControlMode","command_topic":"cmnd/$device/Relay2ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"manual"})JSON"
R"JSON(,{"value":"partofday","name":"partofday"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"treshold"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R2: Stan","id":"M_3a4880a5","persist":false,"ha":{"state_topic":"stat/$device/Relay2State","command_topic":"cmnd/$device/Relay2State","payload_on":"on","payload_off":"off"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"R2: Kierunek","id":"M_ba03507a","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/Relay2ControlDirection","command_topic":"cmnd/$device/Relay2ControlDirection","optimistic":true},"visibleWhen":{"field":"M_519ed402","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"direct","name":"direct"})JSON"
R"JSON(,{"value":"reverse","name":"reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"R2: Wejście analogowe","id":"M_16230834","persist":true,"default":"none","ha":{"state_topic":"stat/$device/Relay2Input","command_topic":"cmnd/$device/Relay2Input","optimistic":true},"visibleWhen":{"field":"M_519ed402","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"none","name":"none"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"R2: Wartość zadana","id":"M_f6501d79","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay2SensorsSetpoint","command_topic":"cmnd/$device/Relay2SensorsSetpoint","optimistic":true},"visibleWhen":{"field":"M_519ed402","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"R2: Połowa szerokości histerezy","id":"M_31618a11","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay2MaxDeviation","command_topic":"cmnd/$device/Relay2MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_519ed402","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R2: Rano","id":"M_eb737f4b","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay2MorningMode","command_topic":"cmnd/$device/Relay2MorningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_519ed402","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R2: Popołudnie","id":"M_620f21c7","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay2AfternoonMode","command_topic":"cmnd/$device/Relay2AfternoonMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_519ed402","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R2: Wieczór","id":"M_3007435f","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay2EveningMode","command_topic":"cmnd/$device/Relay2EveningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_519ed402","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R2: Noc","id":"M_eafd667d","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay2NightMode","command_topic":"cmnd/$device/Relay2NightMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_519ed402","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"text","name":"R2: SensorsValue","id":"M_59dde76c","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Relay2SensorsValue"},"visibleWhen":{"field":"M_519ed402","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"R3: Nazwa","id":"M_9de298e8","persist":true,"default":"","ha":{"state_topic":"stat/$device/Relay3Name","command_topic":"cmnd/$device/Relay3Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,)JSON"
R"JSON({"type":"select","name":"R3: Tryb","id":"M_bd5e2447","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/Relay3ControlMode","command_topic":"cmnd/$device/Relay3ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"manual"})JSON"
R"JSON(,{"value":"partofday","name":"partofday"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"treshold"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R3: Stan","id":"M_ae20f4b4","persist":false,"ha":{"state_topic":"stat/$device/Relay3State","command_topic":"cmnd/$device/Relay3State","payload_on":"on","payload_off":"off"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"R3: Kierunek","id":"M_cd2560e5","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/Relay3ControlDirection","command_topic":"cmnd/$device/Relay3ControlDirection","optimistic":true},"visibleWhen":{"field":"M_bd5e2447","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"direct","name":"direct"})JSON"
R"JSON(,{"value":"reverse","name":"reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"R3: Wejście analogowe","id":"M_519c2d81","persist":true,"default":"none","ha":{"state_topic":"stat/$device/Relay3Input","command_topic":"cmnd/$device/Relay3Input","optimistic":true},"visibleWhen":{"field":"M_bd5e2447","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"none","name":"none"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"R3: Wartość zadana","id":"M_2f5c7b1c","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay3SensorsSetpoint","command_topic":"cmnd/$device/Relay3SensorsSetpoint","optimistic":true},"visibleWhen":{"field":"M_bd5e2447","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"R3: Połowa szerokości histerezy","id":"M_a6513792","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay3MaxDeviation","command_topic":"cmnd/$device/Relay3MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_bd5e2447","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R3: Rano","id":"M_038943ca","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay3MorningMode","command_topic":"cmnd/$device/Relay3MorningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_bd5e2447","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R3: Popołudnie","id":"M_05ccfab6","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay3AfternoonMode","command_topic":"cmnd/$device/Relay3AfternoonMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_bd5e2447","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R3: Wieczór","id":"M_5f596df6","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay3EveningMode","command_topic":"cmnd/$device/Relay3EveningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_bd5e2447","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R3: Noc","id":"M_e0f03a54","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay3NightMode","command_topic":"cmnd/$device/Relay3NightMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_bd5e2447","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"text","name":"R3: SensorsValue","id":"M_1cffb733","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Relay3SensorsValue"},"visibleWhen":{"field":"M_bd5e2447","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"R4: Nazwa","id":"M_d424d409","persist":true,"default":"","ha":{"state_topic":"stat/$device/Relay4Name","command_topic":"cmnd/$device/Relay4Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,)JSON"
R"JSON({"type":"select","name":"R4: Tryb","id":"M_ad328a90","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/Relay4ControlMode","command_topic":"cmnd/$device/Relay4ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"manual"})JSON"
R"JSON(,{"value":"partofday","name":"partofday"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"treshold"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R4: Stan","id":"M_3b0814c7","persist":false,"ha":{"state_topic":"stat/$device/Relay4State","command_topic":"cmnd/$device/Relay4State","payload_on":"on","payload_off":"off"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"R4: Kierunek","id":"M_cdc7b6c0","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/Relay4ControlDirection","command_topic":"cmnd/$device/Relay4ControlDirection","optimistic":true},"visibleWhen":{"field":"M_ad328a90","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"direct","name":"direct"})JSON"
R"JSON(,{"value":"reverse","name":"reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"R4: Wejście analogowe","id":"M_ce562ce6","persist":true,"default":"none","ha":{"state_topic":"stat/$device/Relay4Input","command_topic":"cmnd/$device/Relay4Input","optimistic":true},"visibleWhen":{"field":"M_ad328a90","equals":"treshold"},"options":[)JSON"
R"JSON({"value":"none","name":"none"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"R4: Wartość zadana","id":"M_ad33f4db","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay4SensorsSetpoint","command_topic":"cmnd/$device/Relay4SensorsSetpoint","optimistic":true},"visibleWhen":{"field":"M_ad328a90","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"R4: Połowa szerokości histerezy","id":"M_0e147057","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Relay4MaxDeviation","command_topic":"cmnd/$device/Relay4MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_ad328a90","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R4: Rano","id":"M_d63d89d1","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay4MorningMode","command_topic":"cmnd/$device/Relay4MorningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ad328a90","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R4: Popołudnie","id":"M_6b4f5a99","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay4AfternoonMode","command_topic":"cmnd/$device/Relay4AfternoonMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ad328a90","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R4: Wieczór","id":"M_4d091ad9","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay4EveningMode","command_topic":"cmnd/$device/Relay4EveningMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ad328a90","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"R4: Noc","id":"M_4c7c06f7","persist":true,"default":false,"ha":{"state_topic":"stat/$device/Relay4NightMode","command_topic":"cmnd/$device/Relay4NightMode","payload_on":"on","payload_off":"off"},"visibleWhen":{"field":"M_ad328a90","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"text","name":"R4: SensorsValue","id":"M_0be674fa","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Relay4SensorsValue"},"visibleWhen":{"field":"M_ad328a90","equals":"treshold"}})JSON"
#endif
R"JSON(]})JSON"
;
#endif
#if 1
static const char PANEL_PAGE_2[] PROGMEM =
R"JSON({"id":"schedule","name":"Harmonogram","order":20,"fields":[)JSON"
R"JSON({"type":"select","name":"Harmonogram: Tryb","id":"M_e9abe86f","persist":true,"default":"daily","ha":{"state_topic":"stat/$device/ScheduleMode","command_topic":"cmnd/$device/ScheduleMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"daily","name":"Każdego dnia"})JSON"
R"JSON(,{"value":"weekdayweekend","name":"Pn–Pt i Weekend"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Harmonogram: Każdego dnia — Rano","id":"M_04b49b1e","persist":true,"default":"09:00:00","ha":{"state_topic":"stat/$device/MorningTime","command_topic":"cmnd/$device/MorningTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"daily"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Harmonogram: Każdego dnia — Popołudnie","id":"M_e46cf686","persist":true,"default":"10:00:00","ha":{"state_topic":"stat/$device/AfternoonTime","command_topic":"cmnd/$device/AfternoonTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"daily"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Harmonogram: Każdego dnia — Wieczór","id":"M_3bc57e1e","persist":true,"default":"19:00:00","ha":{"state_topic":"stat/$device/EveningTime","command_topic":"cmnd/$device/EveningTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"daily"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Harmonogram: Każdego dnia — Noc","id":"M_00f96f58","persist":true,"default":"22:00:00","ha":{"state_topic":"stat/$device/NightTime","command_topic":"cmnd/$device/NightTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"daily"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Harmonogram: Pn–Pt — Rano","id":"M_343dbe8e","persist":true,"default":"09:00:00","ha":{"state_topic":"stat/$device/WeekdayMorningTime","command_topic":"cmnd/$device/WeekdayMorningTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Harmonogram: Pn–Pt — Popołudnie","id":"M_c1b27a56","persist":true,"default":"10:00:00","ha":{"state_topic":"stat/$device/WeekdayAfternoonTime","command_topic":"cmnd/$device/WeekdayAfternoonTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Harmonogram: Pn–Pt — Wieczór","id":"M_ddf379ce","persist":true,"default":"19:00:00","ha":{"state_topic":"stat/$device/WeekdayEveningTime","command_topic":"cmnd/$device/WeekdayEveningTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Harmonogram: Pn–Pt — Noc","id":"M_6464bdc8","persist":true,"default":"22:00:00","ha":{"state_topic":"stat/$device/WeekdayNightTime","command_topic":"cmnd/$device/WeekdayNightTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Harmonogram: Weekend — Rano","id":"M_3685740b","persist":true,"default":"09:00:00","ha":{"state_topic":"stat/$device/WeekendMorningTime","command_topic":"cmnd/$device/WeekendMorningTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Harmonogram: Weekend — Popołudnie","id":"M_66b258d3","persist":true,"default":"10:00:00","ha":{"state_topic":"stat/$device/WeekendAfternoonTime","command_topic":"cmnd/$device/WeekendAfternoonTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Harmonogram: Weekend — Wieczór","id":"M_a13b8513","persist":true,"default":"19:00:00","ha":{"state_topic":"stat/$device/WeekendEveningTime","command_topic":"cmnd/$device/WeekendEveningTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Harmonogram: Weekend — Noc","id":"M_ec17d675","persist":true,"default":"22:00:00","ha":{"state_topic":"stat/$device/WeekendNightTime","command_topic":"cmnd/$device/WeekendNightTime","optimistic":true},"visibleWhen":{"field":"M_e9abe86f","equals":"weekdayweekend"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"Harmonogram: Aktualna pora","id":"M_756f8567","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/ActualPartOfDay"}})JSON"
R"JSON(]})JSON"
;
#endif
#if AC_ACTIVE_INPUTS > 0
static const char PANEL_PAGE_3[] PROGMEM =
R"JSON({"id":"inputs","name":"Wejścia","order":30,"fields":[)JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON({"type":"edit","name":"S1: Nazwa","id":"M_3759e61d","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor1Name","command_topic":"cmnd/$device/Sensor1Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,)JSON"
R"JSON({"type":"select","name":"S1: Podłączone sterowanie","id":"M_a5f9eefe","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor1Type","command_topic":"cmnd/$device/Sensor1Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"notconnected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S1: Kalibracja: wartość 1","id":"M_78157d9d","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor1CalibValue1","command_topic":"cmnd/$device/Sensor1CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S1: Kalibracja: ADC 1 (0–1023)","id":"M_400e4039","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor1CalibRawValue1","command_topic":"cmnd/$device/Sensor1CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S1: Kalibracja: wartość 2","id":"M_751578e4","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor1CalibValue2","command_topic":"cmnd/$device/Sensor1CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S1: Kalibracja: ADC 2 (0–1023)","id":"M_3d0e3b80","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor1CalibRawValue2","command_topic":"cmnd/$device/Sensor1CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S1: Minimum lub off","id":"M_13539af3","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor1MinValue","command_topic":"cmnd/$device/Sensor1MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S1: Maksimum lub off","id":"M_d0808f51","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor1MaxValue","command_topic":"cmnd/$device/Sensor1MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S1: Minimum krytyczne lub off","id":"M_83b82822","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor1CriticalMinValue","command_topic":"cmnd/$device/Sensor1CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S1: Maksimum krytyczne lub off","id":"M_6de88960","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor1CriticalMaxValue","command_topic":"cmnd/$device/Sensor1CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S1: Value","id":"M_a843477f","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor1Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S1: RawValue","id":"M_a8e306a9","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor1RawValue"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S2: Nazwa","id":"M_c32c1550","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor2Name","command_topic":"cmnd/$device/Sensor2Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,)JSON"
R"JSON({"type":"select","name":"S2: Podłączone sterowanie","id":"M_f3fd17b3","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor2Type","command_topic":"cmnd/$device/Sensor2Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"notconnected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S2: Kalibracja: wartość 1","id":"M_915c41ee","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor2CalibValue1","command_topic":"cmnd/$device/Sensor2CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S2: Kalibracja: ADC 1 (0–1023)","id":"M_9297b8cc","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor2CalibRawValue1","command_topic":"cmnd/$device/Sensor2CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S2: Kalibracja: wartość 2","id":"M_905c405b","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor2CalibValue2","command_topic":"cmnd/$device/Sensor2CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S2: Kalibracja: ADC 2 (0–1023)","id":"M_9597bd85","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor2CalibRawValue2","command_topic":"cmnd/$device/Sensor2CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S2: Minimum lub off","id":"M_725dac62","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor2MinValue","command_topic":"cmnd/$device/Sensor2MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S2: Maksimum lub off","id":"M_2b144fa0","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor2MaxValue","command_topic":"cmnd/$device/Sensor2MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S2: Minimum krytyczne lub off","id":"M_b24797bf","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor2CriticalMinValue","command_topic":"cmnd/$device/Sensor2CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S2: Maksimum krytyczne lub off","id":"M_1be960d5","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor2CriticalMaxValue","command_topic":"cmnd/$device/Sensor2CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S2: Value","id":"M_790ef628","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor2Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S2: RawValue","id":"M_84ec1eb8","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor2RawValue"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S3: Nazwa","id":"M_5addecbb","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor3Name","command_topic":"cmnd/$device/Sensor3Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,)JSON"
R"JSON({"type":"select","name":"S3: Podłączone sterowanie","id":"M_c8dbdb90","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor3Type","command_topic":"cmnd/$device/Sensor3Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"notconnected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S3: Kalibracja: wartość 1","id":"M_86d6228f","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor3CalibValue1","command_topic":"cmnd/$device/Sensor3CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S3: Kalibracja: ADC 1 (0–1023)","id":"M_f13c82ff","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor3CalibRawValue1","command_topic":"cmnd/$device/Sensor3CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S3: Kalibracja: wartość 2","id":"M_87d62422","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor3CalibValue2","command_topic":"cmnd/$device/Sensor3CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S3: Kalibracja: ADC 2 (0–1023)","id":"M_f23c8492","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor3CalibRawValue2","command_topic":"cmnd/$device/Sensor3CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S3: Minimum lub off","id":"M_747247ed","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor3MinValue","command_topic":"cmnd/$device/Sensor3MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S3: Maksimum lub off","id":"M_b72c9d57","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor3MaxValue","command_topic":"cmnd/$device/Sensor3MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S3: Minimum krytyczne lub off","id":"M_986a7160","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor3CriticalMinValue","command_topic":"cmnd/$device/Sensor3CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S3: Maksimum krytyczne lub off","id":"M_ae3a1022","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor3CriticalMaxValue","command_topic":"cmnd/$device/Sensor3CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S3: Value","id":"M_4fa351e9","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor3Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S3: RawValue","id":"M_5b547cef","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor3RawValue"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S4: Nazwa","id":"M_204eff5e","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor4Name","command_topic":"cmnd/$device/Sensor4Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,)JSON"
R"JSON({"type":"select","name":"S4: Podłączone sterowanie","id":"M_c50272b5","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor4Type","command_topic":"cmnd/$device/Sensor4Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"notconnected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S4: Kalibracja: wartość 1","id":"M_b34c3f38","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor4CalibValue1","command_topic":"cmnd/$device/Sensor4CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S4: Kalibracja: ADC 1 (0–1023)","id":"M_e2aea58a","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor4CalibRawValue1","command_topic":"cmnd/$device/Sensor4CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S4: Kalibracja: wartość 2","id":"M_b64c43f1","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor4CalibValue2","command_topic":"cmnd/$device/Sensor4CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S4: Kalibracja: ADC 2 (0–1023)","id":"M_e1aea3f7","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor4CalibRawValue2","command_topic":"cmnd/$device/Sensor4CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S4: Minimum lub off","id":"M_c6f065c4","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor4MinValue","command_topic":"cmnd/$device/Sensor4MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S4: Maksimum lub off","id":"M_205e028e","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor4MaxValue","command_topic":"cmnd/$device/Sensor4MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S4: Minimum krytyczne lub off","id":"M_9b860885","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor4CriticalMinValue","command_topic":"cmnd/$device/Sensor4CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S4: Maksimum krytyczne lub off","id":"M_6ab9063f","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor4CriticalMaxValue","command_topic":"cmnd/$device/Sensor4CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S4: Value","id":"M_ce938de2","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor4Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S4: RawValue","id":"M_0c55dc06","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor4RawValue"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S5: Nazwa","id":"M_f91e9e91","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor5Name","command_topic":"cmnd/$device/Sensor5Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,)JSON"
R"JSON({"type":"select","name":"S5: Podłączone sterowanie","id":"M_e4dbc82a","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor5Type","command_topic":"cmnd/$device/Sensor5Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"notconnected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S5: Kalibracja: wartość 1","id":"M_6614e169","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor5CalibValue1","command_topic":"cmnd/$device/Sensor5CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S5: Kalibracja: ADC 1 (0–1023)","id":"M_9800ecd5","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor5CalibRawValue1","command_topic":"cmnd/$device/Sensor5CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S5: Kalibracja: wartość 2","id":"M_6314dcb0","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor5CalibValue2","command_topic":"cmnd/$device/Sensor5CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S5: Kalibracja: ADC 2 (0–1023)","id":"M_9500e81c","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor5CalibRawValue2","command_topic":"cmnd/$device/Sensor5CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S5: Minimum lub off","id":"M_e40d3ddf","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor5MinValue","command_topic":"cmnd/$device/Sensor5MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S5: Maksimum lub off","id":"M_50031e75","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor5MaxValue","command_topic":"cmnd/$device/Sensor5MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S5: Minimum krytyczne lub off","id":"M_aca38f9e","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor5CriticalMinValue","command_topic":"cmnd/$device/Sensor5CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S5: Maksimum krytyczne lub off","id":"M_b8b46354","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor5CriticalMaxValue","command_topic":"cmnd/$device/Sensor5CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S5: Value","id":"M_303387f3","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor5Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S5: RawValue","id":"M_8bfc05ed","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor5RawValue"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S6: Nazwa","id":"M_ad9e3834","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor6Name","command_topic":"cmnd/$device/Sensor6Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,)JSON"
R"JSON({"type":"select","name":"S6: Podłączone sterowanie","id":"M_a21fbcaf","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor6Type","command_topic":"cmnd/$device/Sensor6Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"notconnected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S6: Kalibracja: wartość 1","id":"M_b8d0dfaa","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor6CalibValue1","command_topic":"cmnd/$device/Sensor6CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S6: Kalibracja: ADC 1 (0–1023)","id":"M_41a72f68","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor6CalibRawValue1","command_topic":"cmnd/$device/Sensor6CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S6: Kalibracja: wartość 2","id":"M_b7d0de17","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor6CalibValue2","command_topic":"cmnd/$device/Sensor6CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S6: Kalibracja: ADC 2 (0–1023)","id":"M_44a73421","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor6CalibRawValue2","command_topic":"cmnd/$device/Sensor6CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S6: Minimum lub off","id":"M_a3d26b5e","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor6MinValue","command_topic":"cmnd/$device/Sensor6MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S6: Maksimum lub off","id":"M_de9b2614","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor6MaxValue","command_topic":"cmnd/$device/Sensor6MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S6: Minimum krytyczne lub off","id":"M_33e5a55b","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor6CriticalMinValue","command_topic":"cmnd/$device/Sensor6CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S6: Maksimum krytyczne lub off","id":"M_bc24ea89","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor6CriticalMaxValue","command_topic":"cmnd/$device/Sensor6CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S6: Value","id":"M_4d988bec","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor6Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S6: RawValue","id":"M_9fe8972c","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor6RawValue"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S7: Nazwa","id":"M_565a041f","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor7Name","command_topic":"cmnd/$device/Sensor7Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,)JSON"
R"JSON({"type":"select","name":"S7: Podłączone sterowanie","id":"M_4a9a54ec","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor7Type","command_topic":"cmnd/$device/Sensor7Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"notconnected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S7: Kalibracja: wartość 1","id":"M_cb47ec5b","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor7CalibValue1","command_topic":"cmnd/$device/Sensor7CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S7: Kalibracja: ADC 1 (0–1023)","id":"M_2ff172ab","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor7CalibRawValue1","command_topic":"cmnd/$device/Sensor7CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S7: Kalibracja: wartość 2","id":"M_cc47edee","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor7CalibValue2","command_topic":"cmnd/$device/Sensor7CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S7: Kalibracja: ADC 2 (0–1023)","id":"M_30f1743e","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor7CalibRawValue2","command_topic":"cmnd/$device/Sensor7CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S7: Minimum lub off","id":"M_71be88c9","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor7MinValue","command_topic":"cmnd/$device/Sensor7MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S7: Maksimum lub off","id":"M_c9f9d83b","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor7MaxValue","command_topic":"cmnd/$device/Sensor7MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S7: Minimum krytyczne lub off","id":"M_0400b31c","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor7CriticalMinValue","command_topic":"cmnd/$device/Sensor7CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S7: Maksimum krytyczne lub off","id":"M_fdcb7c56","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor7CriticalMaxValue","command_topic":"cmnd/$device/Sensor7CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S7: Value","id":"M_1dc7adbd","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor7Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S7: RawValue","id":"M_7cfbebd3","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor7RawValue"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S8: Nazwa","id":"M_0637c052","persist":true,"default":"","ha":{"state_topic":"stat/$device/Sensor8Name","command_topic":"cmnd/$device/Sensor8Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,)JSON"
R"JSON({"type":"select","name":"S8: Podłączone sterowanie","id":"M_dd82ceb1","persist":true,"default":"notconnected","ha":{"state_topic":"stat/$device/Sensor8Type","command_topic":"cmnd/$device/Sensor8Type","optimistic":true},"options":[)JSON"
R"JSON({"value":"notconnected","name":"notconnected"})JSON"
#if AC_ACTIVE_RELAYS >= 1
R"JSON(,{"value":"relay1","name":"relay1"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 2
R"JSON(,{"value":"relay2","name":"relay2"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 3
R"JSON(,{"value":"relay3","name":"relay3"})JSON"
#endif
#if AC_ACTIVE_RELAYS >= 4
R"JSON(,{"value":"relay4","name":"relay4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,{"value":"pwmoutput1","name":"pwmoutput1"})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,{"value":"pwmoutput2","name":"pwmoutput2"})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,{"value":"pwmoutput3","name":"pwmoutput3"})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,{"value":"pwmoutput4","name":"pwmoutput4"})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,{"value":"pwmoutput5","name":"pwmoutput5"})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,{"value":"pwmoutput6","name":"pwmoutput6"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S8: Kalibracja: wartość 1","id":"M_71046d54","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor8CalibValue1","command_topic":"cmnd/$device/Sensor8CalibValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S8: Kalibracja: ADC 1 (0–1023)","id":"M_bb4b4d56","persist":true,"default":"0","ha":{"state_topic":"stat/$device/Sensor8CalibRawValue1","command_topic":"cmnd/$device/Sensor8CalibRawValue1","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S8: Kalibracja: wartość 2","id":"M_7404720d","persist":true,"default":"1","ha":{"state_topic":"stat/$device/Sensor8CalibValue2","command_topic":"cmnd/$device/Sensor8CalibValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S8: Kalibracja: ADC 2 (0–1023)","id":"M_ba4b4bc3","persist":true,"default":"1023","ha":{"state_topic":"stat/$device/Sensor8CalibRawValue2","command_topic":"cmnd/$device/Sensor8CalibRawValue2","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S8: Minimum lub off","id":"M_dfe1d230","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor8MinValue","command_topic":"cmnd/$device/Sensor8MinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S8: Maksimum lub off","id":"M_a0df6a12","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor8MaxValue","command_topic":"cmnd/$device/Sensor8MaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S8: Minimum krytyczne lub off","id":"M_040700f1","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor8CriticalMinValue","command_topic":"cmnd/$device/Sensor8CriticalMinValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"S8: Maksimum krytyczne lub off","id":"M_cc206833","persist":true,"default":"off","ha":{"state_topic":"stat/$device/Sensor8CriticalMaxValue","command_topic":"cmnd/$device/Sensor8CriticalMaxValue","optimistic":true}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S8: Value","id":"M_a84af346","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor8Value"}})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,)JSON"
R"JSON({"type":"text","name":"S8: RawValue","id":"M_1a1754ca","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/Sensor8RawValue"}})JSON"
#endif
R"JSON(]})JSON"
;
#endif
#if AC_ACTIVE_PWM > 0
static const char PANEL_PAGE_4[] PROGMEM =
R"JSON({"id":"pwm","name":"Wyjścia PWM","order":40,"fields":[)JSON"
#if AC_ACTIVE_PWM >= 1
R"JSON({"type":"edit","name":"PWM1: Nazwa","id":"M_8a6eea92","persist":true,"default":"","ha":{"state_topic":"stat/$device/PWMOutput1Name","command_topic":"cmnd/$device/PWMOutput1Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM1: Tryb","id":"M_cc6519d9","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/PWMOutput1ControlMode","command_topic":"cmnd/$device/PWMOutput1ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"manual"})JSON"
R"JSON(,{"value":"partofday","name":"partofday"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"treshold"})JSON"
#endif
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"pid","name":"pid"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM1: Kierunek","id":"M_03e2abe7","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/PWMOutput1ControlDirection","command_topic":"cmnd/$device/PWMOutput1ControlDirection","optimistic":true},"visibleWhen":{"or":[{"field":"M_cc6519d9","equals":"treshold"},{"field":"M_cc6519d9","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"direct","name":"direct"})JSON"
R"JSON(,{"value":"reverse","name":"reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM1: Wejście analogowe","id":"M_23f03e7f","persist":true,"default":"none","ha":{"state_topic":"stat/$device/PWMOutput1Input","command_topic":"cmnd/$device/PWMOutput1Input","optimistic":true},"visibleWhen":{"or":[{"field":"M_cc6519d9","equals":"treshold"},{"field":"M_cc6519d9","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"none","name":"none"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM1: Wartość zadana","id":"M_c7216cfa","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput1SensorsSetpoint","command_topic":"cmnd/$device/PWMOutput1SensorsSetpoint","optimistic":true},"visibleWhen":{"or":[{"field":"M_cc6519d9","equals":"treshold"},{"field":"M_cc6519d9","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM1: Połowa szerokości histerezy","id":"M_0b6aec8c","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput1MaxDeviation","command_topic":"cmnd/$device/PWMOutput1MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM1: Rano","min":0,"max":65535,"step":1,"id":"M_ea6077cc","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput1MorningMode","command_topic":"cmnd/$device/PWMOutput1MorningMode","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM1: Popołudnie","min":0,"max":65535,"step":1,"id":"M_410b2e24","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput1AfternoonMode","command_topic":"cmnd/$device/PWMOutput1AfternoonMode","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM1: Wieczór","min":0,"max":65535,"step":1,"id":"M_e5ce6434","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput1EveningMode","command_topic":"cmnd/$device/PWMOutput1EveningMode","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM1: Noc","min":0,"max":65535,"step":1,"id":"M_624d6cf2","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput1NightMode","command_topic":"cmnd/$device/PWMOutput1NightMode","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM1: Ręczne PWM","min":0,"max":65535,"step":1,"id":"M_6fc04f90","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput1ManualMode","command_topic":"cmnd/$device/PWMOutput1ManualMode","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"manual"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM1: PidKp","id":"M_8cc3baa5","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput1PidKp","command_topic":"cmnd/$device/PWMOutput1PidKp","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM1: PidKi","id":"M_a3c3deda","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput1PidKi","command_topic":"cmnd/$device/PWMOutput1PidKi","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM1: PidKd","id":"M_98c3cd89","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput1PidKd","command_topic":"cmnd/$device/PWMOutput1PidKd","optimistic":true},"visibleWhen":{"field":"M_cc6519d9","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1
R"JSON(,)JSON"
R"JSON({"type":"text","name":"PWM1: Value","id":"M_8b21ac86","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput1Value"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"text","name":"PWM1: SensorsValue","id":"M_eca25cc5","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput1SensorsValue"},"visibleWhen":{"or":[{"field":"M_cc6519d9","equals":"treshold"},{"field":"M_cc6519d9","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM2: Nazwa","id":"M_cbfea483","persist":true,"default":"","ha":{"state_topic":"stat/$device/PWMOutput2Name","command_topic":"cmnd/$device/PWMOutput2Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM2: Tryb","id":"M_dd171d92","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/PWMOutput2ControlMode","command_topic":"cmnd/$device/PWMOutput2ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"manual"})JSON"
R"JSON(,{"value":"partofday","name":"partofday"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"treshold"})JSON"
#endif
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"pid","name":"pid"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM2: Kierunek","id":"M_f7231fea","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/PWMOutput2ControlDirection","command_topic":"cmnd/$device/PWMOutput2ControlDirection","optimistic":true},"visibleWhen":{"or":[{"field":"M_dd171d92","equals":"treshold"},{"field":"M_dd171d92","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"direct","name":"direct"})JSON"
R"JSON(,{"value":"reverse","name":"reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM2: Wejście analogowe","id":"M_12857a64","persist":true,"default":"none","ha":{"state_topic":"stat/$device/PWMOutput2Input","command_topic":"cmnd/$device/PWMOutput2Input","optimistic":true},"visibleWhen":{"or":[{"field":"M_dd171d92","equals":"treshold"},{"field":"M_dd171d92","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"none","name":"none"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM2: Wartość zadana","id":"M_02558409","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput2SensorsSetpoint","command_topic":"cmnd/$device/PWMOutput2SensorsSetpoint","optimistic":true},"visibleWhen":{"or":[{"field":"M_dd171d92","equals":"treshold"},{"field":"M_dd171d92","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM2: Połowa szerokości histerezy","id":"M_6b606161","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput2MaxDeviation","command_topic":"cmnd/$device/PWMOutput2MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM2: Rano","min":0,"max":65535,"step":1,"id":"M_d9f077bb","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput2MorningMode","command_topic":"cmnd/$device/PWMOutput2MorningMode","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM2: Popołudnie","min":0,"max":65535,"step":1,"id":"M_3bdd5757","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput2AfternoonMode","command_topic":"cmnd/$device/PWMOutput2AfternoonMode","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM2: Wieczór","min":0,"max":65535,"step":1,"id":"M_74c7198f","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput2EveningMode","command_topic":"cmnd/$device/PWMOutput2EveningMode","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM2: Noc","min":0,"max":65535,"step":1,"id":"M_659d06ad","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput2NightMode","command_topic":"cmnd/$device/PWMOutput2NightMode","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM2: Ręczne PWM","min":0,"max":65535,"step":1,"id":"M_e3271a65","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput2ManualMode","command_topic":"cmnd/$device/PWMOutput2ManualMode","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"manual"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM2: PidKp","id":"M_6ec780e6","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput2PidKp","command_topic":"cmnd/$device/PWMOutput2PidKp","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM2: PidKi","id":"M_87c7a841","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput2PidKi","command_topic":"cmnd/$device/PWMOutput2PidKi","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM2: PidKd","id":"M_82c7a062","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput2PidKd","command_topic":"cmnd/$device/PWMOutput2PidKd","optimistic":true},"visibleWhen":{"field":"M_dd171d92","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2
R"JSON(,)JSON"
R"JSON({"type":"text","name":"PWM2: Value","id":"M_50a5d211","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput2Value"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"text","name":"PWM2: SensorsValue","id":"M_3602dd1c","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput2SensorsValue"},"visibleWhen":{"or":[{"field":"M_dd171d92","equals":"treshold"},{"field":"M_dd171d92","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM3: Nazwa","id":"M_e34273d8","persist":true,"default":"","ha":{"state_topic":"stat/$device/PWMOutput3Name","command_topic":"cmnd/$device/PWMOutput3Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM3: Tryb","id":"M_7837ccd7","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/PWMOutput3ControlMode","command_topic":"cmnd/$device/PWMOutput3ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"manual"})JSON"
R"JSON(,{"value":"partofday","name":"partofday"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"treshold"})JSON"
#endif
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"pid","name":"pid"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM3: Kierunek","id":"M_9edc4095","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/PWMOutput3ControlDirection","command_topic":"cmnd/$device/PWMOutput3ControlDirection","optimistic":true},"visibleWhen":{"or":[{"field":"M_7837ccd7","equals":"treshold"},{"field":"M_7837ccd7","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"direct","name":"direct"})JSON"
R"JSON(,{"value":"reverse","name":"reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM3: Wejście analogowe","id":"M_269b6231","persist":true,"default":"none","ha":{"state_topic":"stat/$device/PWMOutput3Input","command_topic":"cmnd/$device/PWMOutput3Input","optimistic":true},"visibleWhen":{"or":[{"field":"M_7837ccd7","equals":"treshold"},{"field":"M_7837ccd7","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"none","name":"none"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM3: Wartość zadana","id":"M_b88683ac","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput3SensorsSetpoint","command_topic":"cmnd/$device/PWMOutput3SensorsSetpoint","optimistic":true},"visibleWhen":{"or":[{"field":"M_7837ccd7","equals":"treshold"},{"field":"M_7837ccd7","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM3: Połowa szerokości histerezy","id":"M_e99b5fe2","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput3MaxDeviation","command_topic":"cmnd/$device/PWMOutput3MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM3: Rano","min":0,"max":65535,"step":1,"id":"M_61b98ffa","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput3MorningMode","command_topic":"cmnd/$device/PWMOutput3MorningMode","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM3: Popołudnie","min":0,"max":65535,"step":1,"id":"M_0c5a37c6","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput3AfternoonMode","command_topic":"cmnd/$device/PWMOutput3AfternoonMode","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM3: Wieczór","min":0,"max":65535,"step":1,"id":"M_82483366","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput3EveningMode","command_topic":"cmnd/$device/PWMOutput3EveningMode","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM3: Noc","min":0,"max":65535,"step":1,"id":"M_f241ad44","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput3NightMode","command_topic":"cmnd/$device/PWMOutput3NightMode","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM3: Ręczne PWM","min":0,"max":65535,"step":1,"id":"M_ef37c3c2","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput3ManualMode","command_topic":"cmnd/$device/PWMOutput3ManualMode","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"manual"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM3: PidKp","id":"M_78c90f5f","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput3PidKp","command_topic":"cmnd/$device/PWMOutput3PidKp","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM3: PidKi","id":"M_8fc93394","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput3PidKi","command_topic":"cmnd/$device/PWMOutput3PidKi","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM3: PidKd","id":"M_84c92243","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput3PidKd","command_topic":"cmnd/$device/PWMOutput3PidKd","optimistic":true},"visibleWhen":{"field":"M_7837ccd7","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3
R"JSON(,)JSON"
R"JSON({"type":"text","name":"PWM3: Value","id":"M_713c2df0","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput3Value"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"text","name":"PWM3: SensorsValue","id":"M_35e51963","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput3SensorsValue"},"visibleWhen":{"or":[{"field":"M_7837ccd7","equals":"treshold"},{"field":"M_7837ccd7","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM4: Nazwa","id":"M_67753b79","persist":true,"default":"","ha":{"state_topic":"stat/$device/PWMOutput4Name","command_topic":"cmnd/$device/PWMOutput4Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM4: Tryb","id":"M_503e1860","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/PWMOutput4ControlMode","command_topic":"cmnd/$device/PWMOutput4ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"manual"})JSON"
R"JSON(,{"value":"partofday","name":"partofday"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"treshold"})JSON"
#endif
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"pid","name":"pid"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM4: Kierunek","id":"M_f1d63130","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/PWMOutput4ControlDirection","command_topic":"cmnd/$device/PWMOutput4ControlDirection","optimistic":true},"visibleWhen":{"or":[{"field":"M_503e1860","equals":"treshold"},{"field":"M_503e1860","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"direct","name":"direct"})JSON"
R"JSON(,{"value":"reverse","name":"reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM4: Wejście analogowe","id":"M_e406e716","persist":true,"default":"none","ha":{"state_topic":"stat/$device/PWMOutput4Input","command_topic":"cmnd/$device/PWMOutput4Input","optimistic":true},"visibleWhen":{"or":[{"field":"M_503e1860","equals":"treshold"},{"field":"M_503e1860","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"none","name":"none"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM4: Wartość zadana","id":"M_0a210f6b","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput4SensorsSetpoint","command_topic":"cmnd/$device/PWMOutput4SensorsSetpoint","optimistic":true},"visibleWhen":{"or":[{"field":"M_503e1860","equals":"treshold"},{"field":"M_503e1860","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM4: Połowa szerokości histerezy","id":"M_4f6d6867","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput4MaxDeviation","command_topic":"cmnd/$device/PWMOutput4MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM4: Rano","min":0,"max":65535,"step":1,"id":"M_5a687341","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput4MorningMode","command_topic":"cmnd/$device/PWMOutput4MorningMode","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM4: Popołudnie","min":0,"max":65535,"step":1,"id":"M_31bcd969","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput4AfternoonMode","command_topic":"cmnd/$device/PWMOutput4AfternoonMode","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM4: Wieczór","min":0,"max":65535,"step":1,"id":"M_46fd4c89","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput4EveningMode","command_topic":"cmnd/$device/PWMOutput4EveningMode","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM4: Noc","min":0,"max":65535,"step":1,"id":"M_c3f542e7","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput4NightMode","command_topic":"cmnd/$device/PWMOutput4NightMode","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM4: Ręczne PWM","min":0,"max":65535,"step":1,"id":"M_c81a62f7","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput4ManualMode","command_topic":"cmnd/$device/PWMOutput4ManualMode","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"manual"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM4: PidKp","id":"M_e2031320","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput4PidKp","command_topic":"cmnd/$device/PWMOutput4PidKp","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM4: PidKi","id":"M_fb033a7b","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput4PidKi","command_topic":"cmnd/$device/PWMOutput4PidKi","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM4: PidKd","id":"M_f603329c","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput4PidKd","command_topic":"cmnd/$device/PWMOutput4PidKd","optimistic":true},"visibleWhen":{"field":"M_503e1860","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4
R"JSON(,)JSON"
R"JSON({"type":"text","name":"PWM4: Value","id":"M_888c809b","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput4Value"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"text","name":"PWM4: SensorsValue","id":"M_978fd22a","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput4SensorsValue"},"visibleWhen":{"or":[{"field":"M_503e1860","equals":"treshold"},{"field":"M_503e1860","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM5: Nazwa","id":"M_916e2426","persist":true,"default":"","ha":{"state_topic":"stat/$device/PWMOutput5Name","command_topic":"cmnd/$device/PWMOutput5Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM5: Tryb","id":"M_475ccb45","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/PWMOutput5ControlMode","command_topic":"cmnd/$device/PWMOutput5ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"manual"})JSON"
R"JSON(,{"value":"partofday","name":"partofday"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"treshold"})JSON"
#endif
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"pid","name":"pid"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM5: Kierunek","id":"M_849c24a3","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/PWMOutput5ControlDirection","command_topic":"cmnd/$device/PWMOutput5ControlDirection","optimistic":true},"visibleWhen":{"or":[{"field":"M_475ccb45","equals":"treshold"},{"field":"M_475ccb45","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"direct","name":"direct"})JSON"
R"JSON(,{"value":"reverse","name":"reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM5: Wejście analogowe","id":"M_9de062bb","persist":true,"default":"none","ha":{"state_topic":"stat/$device/PWMOutput5Input","command_topic":"cmnd/$device/PWMOutput5Input","optimistic":true},"visibleWhen":{"or":[{"field":"M_475ccb45","equals":"treshold"},{"field":"M_475ccb45","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"none","name":"none"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM5: Wartość zadana","id":"M_38c42c6e","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput5SensorsSetpoint","command_topic":"cmnd/$device/PWMOutput5SensorsSetpoint","optimistic":true},"visibleWhen":{"or":[{"field":"M_475ccb45","equals":"treshold"},{"field":"M_475ccb45","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM5: Połowa szerokości histerezy","id":"M_1cda4330","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput5MaxDeviation","command_topic":"cmnd/$device/PWMOutput5MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM5: Rano","min":0,"max":65535,"step":1,"id":"M_4c261cc8","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput5MorningMode","command_topic":"cmnd/$device/PWMOutput5MorningMode","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM5: Popołudnie","min":0,"max":65535,"step":1,"id":"M_51312d90","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput5AfternoonMode","command_topic":"cmnd/$device/PWMOutput5AfternoonMode","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM5: Wieczór","min":0,"max":65535,"step":1,"id":"M_8336ffc8","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput5EveningMode","command_topic":"cmnd/$device/PWMOutput5EveningMode","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM5: Noc","min":0,"max":65535,"step":1,"id":"M_3edf2c5e","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput5NightMode","command_topic":"cmnd/$device/PWMOutput5NightMode","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM5: Ręczne PWM","min":0,"max":65535,"step":1,"id":"M_26a0d20c","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput5ManualMode","command_topic":"cmnd/$device/PWMOutput5ManualMode","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"manual"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM5: PidKp","id":"M_2ffab221","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput5PidKp","command_topic":"cmnd/$device/PWMOutput5PidKp","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM5: PidKi","id":"M_16fa8ac6","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput5PidKi","command_topic":"cmnd/$device/PWMOutput5PidKi","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM5: PidKd","id":"M_23fa9f3d","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput5PidKd","command_topic":"cmnd/$device/PWMOutput5PidKd","optimistic":true},"visibleWhen":{"field":"M_475ccb45","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5
R"JSON(,)JSON"
R"JSON({"type":"text","name":"PWM5: Value","id":"M_cf947b0a","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput5Value"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"text","name":"PWM5: SensorsValue","id":"M_ef572f09","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput5SensorsValue"},"visibleWhen":{"or":[{"field":"M_475ccb45","equals":"treshold"},{"field":"M_475ccb45","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM6: Nazwa","id":"M_1d505e87","persist":true,"default":"","ha":{"state_topic":"stat/$device/PWMOutput6Name","command_topic":"cmnd/$device/PWMOutput6Name","optimistic":true,"min":0,"max":15}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM6: Tryb","id":"M_7289ca6e","persist":true,"default":"manual","ha":{"state_topic":"stat/$device/PWMOutput6ControlMode","command_topic":"cmnd/$device/PWMOutput6ControlMode","optimistic":true},"options":[)JSON"
R"JSON({"value":"manual","name":"manual"})JSON"
R"JSON(,{"value":"partofday","name":"partofday"})JSON"
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"treshold","name":"treshold"})JSON"
#endif
#if AC_ACTIVE_INPUTS > 0
R"JSON(,{"value":"pid","name":"pid"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM6: Kierunek","id":"M_4da79906","persist":true,"default":"direct","ha":{"state_topic":"stat/$device/PWMOutput6ControlDirection","command_topic":"cmnd/$device/PWMOutput6ControlDirection","optimistic":true},"visibleWhen":{"or":[{"field":"M_7289ca6e","equals":"treshold"},{"field":"M_7289ca6e","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"direct","name":"direct"})JSON"
R"JSON(,{"value":"reverse","name":"reverse"})JSON"
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"select","name":"PWM6: Wejście analogowe","id":"M_cf971640","persist":true,"default":"none","ha":{"state_topic":"stat/$device/PWMOutput6Input","command_topic":"cmnd/$device/PWMOutput6Input","optimistic":true},"visibleWhen":{"or":[{"field":"M_7289ca6e","equals":"treshold"},{"field":"M_7289ca6e","equals":"pid"}]},"options":[)JSON"
R"JSON({"value":"none","name":"none"})JSON"
#if AC_ACTIVE_INPUTS >= 1
R"JSON(,{"value":"sensor1","name":"sensor1"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 2
R"JSON(,{"value":"sensor2","name":"sensor2"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 3
R"JSON(,{"value":"sensor3","name":"sensor3"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 4
R"JSON(,{"value":"sensor4","name":"sensor4"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 5
R"JSON(,{"value":"sensor5","name":"sensor5"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 6
R"JSON(,{"value":"sensor6","name":"sensor6"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 7
R"JSON(,{"value":"sensor7","name":"sensor7"})JSON"
#endif
#if AC_ACTIVE_INPUTS >= 8
R"JSON(,{"value":"sensor8","name":"sensor8"})JSON"
#endif
R"JSON(]})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM6: Wartość zadana","id":"M_920be09d","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput6SensorsSetpoint","command_topic":"cmnd/$device/PWMOutput6SensorsSetpoint","optimistic":true},"visibleWhen":{"or":[{"field":"M_7289ca6e","equals":"treshold"},{"field":"M_7289ca6e","equals":"pid"}]}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM6: Połowa szerokości histerezy","id":"M_de16a3d5","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput6MaxDeviation","command_topic":"cmnd/$device/PWMOutput6MaxDeviation","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"treshold"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM6: Rano","min":0,"max":65535,"step":1,"id":"M_70002607","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput6MorningMode","command_topic":"cmnd/$device/PWMOutput6MorningMode","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM6: Popołudnie","min":0,"max":65535,"step":1,"id":"M_a900bf53","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput6AfternoonMode","command_topic":"cmnd/$device/PWMOutput6AfternoonMode","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM6: Wieczór","min":0,"max":65535,"step":1,"id":"M_9a44e543","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput6EveningMode","command_topic":"cmnd/$device/PWMOutput6EveningMode","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM6: Noc","min":0,"max":65535,"step":1,"id":"M_45e40e39","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput6NightMode","command_topic":"cmnd/$device/PWMOutput6NightMode","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"partofday"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"PWM6: Ręczne PWM","min":0,"max":65535,"step":1,"id":"M_19e41b21","persist":true,"default":0,"ha":{"state_topic":"stat/$device/PWMOutput6ManualMode","command_topic":"cmnd/$device/PWMOutput6ManualMode","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"manual"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM6: PidKp","id":"M_08f8ec32","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput6PidKp","command_topic":"cmnd/$device/PWMOutput6PidKp","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM6: PidKi","id":"M_21f9138d","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput6PidKi","command_topic":"cmnd/$device/PWMOutput6PidKi","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"edit","name":"PWM6: PidKd","id":"M_14f8ff16","persist":true,"default":"0","ha":{"state_topic":"stat/$device/PWMOutput6PidKd","command_topic":"cmnd/$device/PWMOutput6PidKd","optimistic":true},"visibleWhen":{"field":"M_7289ca6e","equals":"pid"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6
R"JSON(,)JSON"
R"JSON({"type":"text","name":"PWM6: Value","id":"M_8c592a25","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput6Value"}})JSON"
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
R"JSON(,)JSON"
R"JSON({"type":"text","name":"PWM6: SensorsValue","id":"M_4528a6a0","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/PWMOutput6SensorsValue"},"visibleWhen":{"or":[{"field":"M_7289ca6e","equals":"treshold"},{"field":"M_7289ca6e","equals":"pid"}]}})JSON"
#endif
R"JSON(]})JSON"
;
#endif
#if 1
static const char PANEL_PAGE_5[] PROGMEM =
R"JSON({"id":"clock","name":"Zegar","order":50,"fields":[)JSON"
R"JSON({"type":"date","name":"Data lokalna","id":"M_050550f3","persist":false,"ha":{"state_topic":"stat/$device/ActualDate","command_topic":"cmnd/$device/ActualDate","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"time","name":"Czas lokalny","id":"M_8c8e46a6","persist":false,"ha":{"state_topic":"stat/$device/ActualTime","command_topic":"cmnd/$device/ActualTime","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"Strefa czasowa: Reguła 1 — tydzień","id":"M_28964e47","persist":true,"default":"first","ha":{"state_topic":"stat/$device/TimezoneRule1Week","command_topic":"cmnd/$device/TimezoneRule1Week","optimistic":true},"options":[)JSON"
R"JSON({"value":"first","name":"first"})JSON"
R"JSON(,{"value":"second","name":"second"})JSON"
R"JSON(,{"value":"third","name":"third"})JSON"
R"JSON(,{"value":"fourth","name":"fourth"})JSON"
R"JSON(,{"value":"last","name":"last"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"Strefa czasowa: Reguła 1 — dzień","id":"M_6a205c42","persist":true,"default":"Sunday","ha":{"state_topic":"stat/$device/TimezoneRule1DayOfWeek","command_topic":"cmnd/$device/TimezoneRule1DayOfWeek","optimistic":true},"options":[)JSON"
R"JSON({"value":"Sunday","name":"Sunday"})JSON"
R"JSON(,{"value":"Monday","name":"Monday"})JSON"
R"JSON(,{"value":"Tuesday","name":"Tuesday"})JSON"
R"JSON(,{"value":"Wednesday","name":"Wednesday"})JSON"
R"JSON(,{"value":"Thursday","name":"Thursday"})JSON"
R"JSON(,{"value":"Friday","name":"Friday"})JSON"
R"JSON(,{"value":"Saturday","name":"Saturday"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"Strefa czasowa: Reguła 1 — miesiąc","id":"M_707fef7b","persist":true,"default":"January","ha":{"state_topic":"stat/$device/TimezoneRule1Month","command_topic":"cmnd/$device/TimezoneRule1Month","optimistic":true},"options":[)JSON"
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
R"JSON({"type":"slider","name":"Strefa czasowa: Reguła 1 — godzina","min":0,"max":23,"step":1,"id":"M_3929a5d9","persist":true,"default":0,"ha":{"state_topic":"stat/$device/TimezoneRule1Hour","command_topic":"cmnd/$device/TimezoneRule1Hour","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"Strefa czasowa: Reguła 1 — offset UTC","min":-12,"max":14,"step":1,"id":"M_f360da94","persist":true,"default":-12,"ha":{"state_topic":"stat/$device/TimezoneRule1Offset","command_topic":"cmnd/$device/TimezoneRule1Offset","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"Strefa czasowa: Reguła 2 — tydzień","id":"M_070aeefe","persist":true,"default":"first","ha":{"state_topic":"stat/$device/TimezoneRule2Week","command_topic":"cmnd/$device/TimezoneRule2Week","optimistic":true},"options":[)JSON"
R"JSON({"value":"first","name":"first"})JSON"
R"JSON(,{"value":"second","name":"second"})JSON"
R"JSON(,{"value":"third","name":"third"})JSON"
R"JSON(,{"value":"fourth","name":"fourth"})JSON"
R"JSON(,{"value":"last","name":"last"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"Strefa czasowa: Reguła 2 — dzień","id":"M_ba91c141","persist":true,"default":"Sunday","ha":{"state_topic":"stat/$device/TimezoneRule2DayOfWeek","command_topic":"cmnd/$device/TimezoneRule2DayOfWeek","optimistic":true},"options":[)JSON"
R"JSON({"value":"Sunday","name":"Sunday"})JSON"
R"JSON(,{"value":"Monday","name":"Monday"})JSON"
R"JSON(,{"value":"Tuesday","name":"Tuesday"})JSON"
R"JSON(,{"value":"Wednesday","name":"Wednesday"})JSON"
R"JSON(,{"value":"Thursday","name":"Thursday"})JSON"
R"JSON(,{"value":"Friday","name":"Friday"})JSON"
R"JSON(,{"value":"Saturday","name":"Saturday"})JSON"
R"JSON(]})JSON"
R"JSON(,)JSON"
R"JSON({"type":"select","name":"Strefa czasowa: Reguła 2 — miesiąc","id":"M_56cfda54","persist":true,"default":"January","ha":{"state_topic":"stat/$device/TimezoneRule2Month","command_topic":"cmnd/$device/TimezoneRule2Month","optimistic":true},"options":[)JSON"
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
R"JSON({"type":"slider","name":"Strefa czasowa: Reguła 2 — godzina","min":0,"max":23,"step":1,"id":"M_6c3b8950","persist":true,"default":0,"ha":{"state_topic":"stat/$device/TimezoneRule2Hour","command_topic":"cmnd/$device/TimezoneRule2Hour","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"Strefa czasowa: Reguła 2 — offset UTC","min":-12,"max":14,"step":1,"id":"M_092b22a5","persist":true,"default":-12,"ha":{"state_topic":"stat/$device/TimezoneRule2Offset","command_topic":"cmnd/$device/TimezoneRule2Offset","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"ActualTimezoneOffset","id":"M_f6125e9b","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/ActualTimezoneOffset"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"ActualDayOfWeek","id":"M_e5dc2a8a","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/ActualDayOfWeek"}})JSON"
R"JSON(]})JSON"
;
#endif
#if AC_ACTIVE_FAN
static const char PANEL_PAGE_6[] PROGMEM =
R"JSON({"id":"fan","name":"Wentylator","order":60,"fields":[)JSON"
R"JSON({"type":"slider","name":"Fan: Temperatura startu","min":0,"max":100,"step":1,"id":"M_d5224990","persist":true,"default":30,"ha":{"state_topic":"stat/$device/FanStartTemp","command_topic":"cmnd/$device/FanStartTemp","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"Fan: Temperatura pełnej prędkości","min":0,"max":100,"step":1,"id":"M_74b30ce9","persist":true,"default":35,"ha":{"state_topic":"stat/$device/FanMaxSpeedTemp","command_topic":"cmnd/$device/FanMaxSpeedTemp","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"slider","name":"Fan: Alarm temperatury","min":0,"max":100,"step":1,"id":"M_1c6d9e10","persist":true,"default":100,"ha":{"state_topic":"stat/$device/MaxIntTemp","command_topic":"cmnd/$device/MaxIntTemp","optimistic":true}})JSON"
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
R"JSON({"id":"buzzer","name":"Dźwięki","order":70,"fields":[)JSON"
R"JSON({"type":"select","name":"Dźwięk startu","id":"M_6cfafdd6","persist":true,"default":"off","ha":{"state_topic":"stat/$device/BuzzerOnStart","command_topic":"cmnd/$device/BuzzerOnStart","optimistic":true},"options":[)JSON"
R"JSON({"value":"off","name":"off"})JSON"
R"JSON(,{"value":"on","name":"on"})JSON"
R"JSON(]})JSON"
#if AC_ACTIVE_DIAGNOSTICS
R"JSON(,)JSON"
R"JSON({"type":"select","name":"Dźwięk błędów","id":"M_bcbcfa69","persist":true,"default":"off","ha":{"state_topic":"stat/$device/BuzzerOnErrors","command_topic":"cmnd/$device/BuzzerOnErrors","optimistic":true},"options":[)JSON"
R"JSON({"value":"off","name":"off"})JSON"
R"JSON(,{"value":"on","name":"on"})JSON"
R"JSON(]})JSON"
#endif
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"Dźwięki: Rano","id":"M_9dbfaff5","persist":true,"default":true,"ha":{"state_topic":"stat/$device/BuzzerMorning","command_topic":"cmnd/$device/BuzzerMorning","payload_on":"on","payload_off":"off"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"Dźwięki: Popołudnie","id":"M_ec1e6b41","persist":true,"default":true,"ha":{"state_topic":"stat/$device/BuzzerAfternoon","command_topic":"cmnd/$device/BuzzerAfternoon","payload_on":"on","payload_off":"off"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"Dźwięki: Wieczór","id":"M_887e9631","persist":true,"default":true,"ha":{"state_topic":"stat/$device/BuzzerEvening","command_topic":"cmnd/$device/BuzzerEvening","payload_on":"on","payload_off":"off"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"switch","name":"Dźwięki: Noc","id":"M_7de5b463","persist":true,"default":false,"ha":{"state_topic":"stat/$device/BuzzerNight","command_topic":"cmnd/$device/BuzzerNight","payload_on":"on","payload_off":"off"}})JSON"
R"JSON(]})JSON"
;
#endif
#if AC_ACTIVE_DIAGNOSTICS
static const char PANEL_PAGE_8[] PROGMEM =
R"JSON({"id":"diagnostics","name":"Diagnostyka","order":80,"fields":[)JSON"
R"JSON({"type":"text","name":"Aktywne alarmy","id":"M_528b92e8","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/ErrorCount"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"Aktywny alarm 1","id":"M_245a24bd","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/AlarmLast1"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"Aktywny alarm 2","id":"M_215a2004","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/AlarmLast2"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"Aktywny alarm 3","id":"M_225a2197","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/AlarmLast3"}})JSON"
R"JSON(]})JSON"
;
#endif
#if AC_ENABLE_DHT && !AC_ACTIVE_FAN
static const char PANEL_PAGE_9[] PROGMEM =
R"JSON({"id":"dht","name":"DHT","order":60,"fields":[)JSON"
R"JSON({"type":"slider","name":"Fan: Alarm temperatury","min":0,"max":100,"step":1,"id":"M_1c6d9e10","persist":true,"default":100,"ha":{"state_topic":"stat/$device/MaxIntTemp","command_topic":"cmnd/$device/MaxIntTemp","optimistic":true}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"InternalTemperature","unit_of_measurement":"°C","device_class":"temperature","id":"M_2df5ea3a","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/InternalTemperature"}})JSON"
R"JSON(,)JSON"
R"JSON({"type":"text","name":"InternalHumidity","unit_of_measurement":"%","device_class":"humidity","id":"M_b4497c35","persist":false,"readonly":true,"ha":{"state_topic":"stat/$device/InternalHumidity"}})JSON"
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
default: return nullptr; }}
