# Aquarium Controller
Aquarium controller based on Arduino Mega 2560 and ESP8266 microcontrollers.
Alfa release, not for production/test usage

# Features
## main components:
- LCD display
- Keypad
- 2 relays outputs
- 2 12V PWM outputs
- 3 5V PWM outputs
- LED strips controller
- up to 8 5V analog sensors (eg. PH sensor, thermometer, aqua level sensor etc)

## integration and networking:
- MQTT integration
- NTP integration

## control methods for LED, relays and PWM outputs:
- manual
- part of day
- hysteresis
- pid

# ESP8266
ESP866 need esp-link firmware (https://github.com/jeelabs/esp-link), minimal release: 3.2.47 alpha

# License
You can use, modify, fork and redistribute this code free of charge with no restrictions for non-commercial use only.
For commercial use please contact with the author.

# Schema
![Schema](https://github.com/rkubera/AquariumController/blob/master/Fritzing/AquaController.png)