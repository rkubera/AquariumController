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
You can upload firmware for mqtt 3.1 version (eg. for mosquitto mqtt) from subfolder:esp8266/mosquitto_31_firmware_esp-link v3.2.50

# License
You can use, modify, fork and redistribute this code free of charge with no restrictions for non-commercial use only.
For commercial use please contact with the author.

# Schema
![Schema](https://github.com/rkubera/AquariumController/blob/master/Fritzing/AquaController.png)