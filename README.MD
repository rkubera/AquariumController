# Aquarium Controller
Aquarium controller based on Arduino Mega 2560 and ESP8266 microcontrollers.
Alfa release, not for production/test usage

# Features
## Main components:
- LCD display
- keypad
- up to 8 5V analog sensors (eg. PH sensor, thermometer, aqua level sensor etc)
- watchdog
- 2 x phisical relays outputs
- 3 x 5V PWM outputs
- 2 x 12V PWM outputs (optional)
- LED strips controller (optional)
- internal fan with pwm and temperature control (optional)
- buzzer (optional)
- real time clock (optional)

## Integration and networking:
- MQTT integration
- NTP integration
- OTA update

## Control methods for LED, relays and PWM outputs:
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