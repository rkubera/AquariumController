# Aquarium Controller
Aquarium controller based on Arduino Mega 2560 and ESP8266 microcontrollers.
Alfa release, not for production/test usage

# Features
## Main components:
- LCD display
- keypad
- watchdog
- timezones support
- 8 x 5V analog sensors (eg. PH sensor, thermometer, aqua level sensor etc)
- 2 x relays outputs
- 4 x 5V PWM outputs
- 2 x 12V PWM outputs (optional)
- LED strips controller (optional)
- internal fan with pwm and temperature controller (optional)
- buzzer (optional)

## Integration and networking:
- MQTT integration
- NTP integration

## Control methods for LED, relays and PWM outputs:
- manual
- part of day
- treshold (hysteresis)
- pid

# ESP8266
ESP866 need ESP8266-MQTT-to-Serial-Bridge firmware (https://github.com/rkubera/ESP8266-MQTT-to-Serial-Bridge)

# License
You can use, modify, fork and redistribute this code free of charge with no restrictions for non-commercial use only.
For commercial use please contact with the author.

# Schema
![Schema](https://github.com/rkubera/AquariumController/blob/master/Fritzing/AquaController.png)
