void relaysInit() {
  //relay1Up();
  //relay2Up();
}

String relaysGetStringValueFromBool (bool Value) {
  if (Value) {    
      return setBufferFromFlash(charOn);
    }
    else {  
      return setBufferFromFlash(charOff);
    }
}

String relaysGetStringValue (byte Value) {
  if (Value == RELAY_MODE_ON) {    
      return setBufferFromFlash(charOn);
    }
    else if (Value == RELAY_MODE_OFF) {  
      return setBufferFromFlash(charOff);
    }
    return "";
}

void relaysMqttPublishAll() {
  mqttElPublish( setBufferFromFlash(getRelay1MorningMode), relaysGetStringValue(relay1ModeMorning));
  mqttElPublish( setBufferFromFlash(getRelay1AfternoonMode), relaysGetStringValue(relay1ModeAfternoon));
  mqttElPublish( setBufferFromFlash(getRelay1EveningMode), relaysGetStringValue(relay1ModeEvening));
  mqttElPublish( setBufferFromFlash(getRelay1NightMode), relaysGetStringValue(relay1ModeNight));

  mqttElPublish( setBufferFromFlash(getRelay2MorningMode), relaysGetStringValue(relay2ModeMorning));
  mqttElPublish( setBufferFromFlash(getRelay2AfternoonMode), relaysGetStringValue(relay2ModeAfternoon));
  mqttElPublish( setBufferFromFlash(getRelay2EveningMode), relaysGetStringValue(relay2ModeEvening));
  mqttElPublish( setBufferFromFlash(getRelay2NightMode), relaysGetStringValue(relay2ModeNight));

  mqttElPublish( setBufferFromFlash(getRelay1State), relaysGetStringValueFromBool(relayLastRelay1State));
  mqttElPublish( setBufferFromFlash(getRelay2State), relaysGetStringValueFromBool(relayLastRelay2State));

  mqttElPublish(setBufferFromFlash(getRelay1ControlMode), getStringControlModeFromValue(relay1ControlMode));
  mqttElPublish(setBufferFromFlash(getRelay2ControlMode), getStringControlModeFromValue(relay2ControlMode));

  relaysRelay1SetState();
  relaysRelay2SetState();
}

void relaysRelay1SetState() {
  
  byte actualPartOfDay = schedulerGetActualPartOfDay();
  bool relay1UpDown = relayLastRelay1State;
    
  if (relay1ControlMode == CONTROL_MODE_PART_OF_DAY) {
    if (actualPartOfDay==SCHEDULER_MODE_MORNING) {
      if (relay1ModeMorning==RELAY_MODE_ON) {
        relay1Mode = relay1ModeMorning;
      }
      else if (relay1ModeMorning==RELAY_MODE_OFF) {
        relay1Mode = relay1ModeMorning;
      }
    }
    else if (actualPartOfDay==SCHEDULER_MODE_AFTERNOON) {
      if (relay1ModeAfternoon==RELAY_MODE_ON) {
        relay1Mode = relay1ModeAfternoon;
      }
      else if (relay1ModeAfternoon==RELAY_MODE_OFF) {
        relay1Mode = relay1ModeAfternoon;
      }
    }
    else if (actualPartOfDay==SCHEDULER_MODE_EVENING) {
      if (relay1ModeEvening==RELAY_MODE_ON) {
        relay1Mode = relay1ModeEvening;
      }
      else if (relay1ModeEvening==RELAY_MODE_OFF) {
        relay1Mode = relay1ModeEvening;
      }
    }
    else {
      if (relay1ModeNight==RELAY_MODE_ON) {
        relay1Mode = relay1ModeNight;
      }
      else if (relay1ModeNight==RELAY_MODE_OFF) {
        relay1Mode = relay1ModeNight;
      }
    }

    if (relay1Mode == RELAY_MODE_ON) {
      relay1UpDown = true;
    }
    else if (relay1Mode == RELAY_MODE_OFF) {
      relay1UpDown = false;
    }
  }

  if (relay1ControlMode != CONTROL_MODE_MANUAL) {
    if (relayLastPartOfDay1!=actualPartOfDay) {
      relayLastPartOfDay1 = actualPartOfDay;
      relay1ManualOnOff=RELAY_MANUAL_ONOFF_AUTO;
    }
  }
  
  if (relay1ManualOnOff == RELAY_MANUAL_ONOFF_ON) {
    relay1UpDown = true;
  }
  else if (relay1ManualOnOff == RELAY_MANUAL_ONOFF_OFF){
    relay1UpDown = false;
  }
  
  if (relayLastRelay1State!=relay1UpDown) {
    relayLastRelay1State = relay1UpDown;
    if (relay1UpDown == true) { 
      mqttElPublishFull("cmnd/sonoff/POWER2","ON");
      relay1Up();
      mqttElPublish(setBufferFromFlash(getRelay1State), setBufferFromFlash(charOn));
    }
    else {
      mqttElPublishFull("cmnd/sonoff/POWER2","OFF");
      relay1Down();
      mqttElPublish(setBufferFromFlash(getRelay1State), setBufferFromFlash(charOff));
    }
  }
}

void relaysRelay2SetState() {
  
  byte actualPartOfDay = schedulerGetActualPartOfDay();
  bool relay2UpDown = relayLastRelay2State;
    
  if (relay2ControlMode == CONTROL_MODE_PART_OF_DAY) {
    if (actualPartOfDay==SCHEDULER_MODE_MORNING) {
      if (relay2ModeMorning==RELAY_MODE_ON) {
        relay2Mode = relay2ModeMorning;
      }
      else if (relay2ModeMorning==RELAY_MODE_OFF) {
        relay2Mode = relay2ModeMorning;
      }
    }
    else if (actualPartOfDay==SCHEDULER_MODE_AFTERNOON) {
      if (relay2ModeAfternoon==RELAY_MODE_ON) {
        relay2Mode = relay2ModeAfternoon;
      }
      else if (relay2ModeAfternoon==RELAY_MODE_OFF) {
        relay2Mode = relay2ModeAfternoon;
      }
    }
    else if (actualPartOfDay==SCHEDULER_MODE_EVENING) {
      if (relay2ModeEvening==RELAY_MODE_ON) {
        relay2Mode = relay2ModeEvening;
      }
      else if (relay2ModeEvening==RELAY_MODE_OFF) {
        relay2Mode = relay2ModeEvening;
      }
    }
    else {
      if (relay2ModeNight==RELAY_MODE_ON) {
        relay2Mode = relay2ModeNight;
      }
      else if (relay2ModeNight==RELAY_MODE_OFF) {
        relay2Mode = relay2ModeNight;
      }
    }

    if (relay2Mode == RELAY_MODE_ON) {
      relay2UpDown = true;
    }
    else if (relay2Mode == RELAY_MODE_OFF) {
      relay2UpDown = false;
    }
  }

  if (relay2ControlMode != CONTROL_MODE_MANUAL) {
    if (relayLastPartOfDay2!=actualPartOfDay) {
      relayLastPartOfDay2 = actualPartOfDay;
      relay2ManualOnOff=RELAY_MANUAL_ONOFF_AUTO;
    }
  }
  
  if (relay2ManualOnOff == RELAY_MANUAL_ONOFF_ON) {
    relay2UpDown = true;
  }
  else if (relay2ManualOnOff == RELAY_MANUAL_ONOFF_OFF){
    relay2UpDown = false;
  }
  
  if (relayLastRelay2State!=relay2UpDown) {
    relayLastRelay2State = relay2UpDown;
    if (relay2UpDown == true) { 
      relay2Up();
      mqttElPublish(setBufferFromFlash(getRelay2State), setBufferFromFlash(charOn));
    }
    else {
      relay2Down();
      mqttElPublish(setBufferFromFlash(getRelay2State), setBufferFromFlash(charOff));
    }
  }
}


void relaysMillisEvent() {
  relaysRelay1SetState();
  relaysRelay2SetState();
}

void relay1Up() {
  pinMode(RELAY_PIN_1,HIGH);
}

void relay1Down() {
  pinMode(RELAY_PIN_1,LOW);
}

void relay2Up() {
  pinMode(RELAY_PIN_2,HIGH);
}

void relay2Down() {
  pinMode(RELAY_PIN_2,LOW);
}


