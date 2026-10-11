#pragma once
#include "ControllerFeatures.h"
#if AC_RELAY_COUNT<0 || AC_RELAY_COUNT>4 || AC_PWM_COUNT<0 || AC_PWM_COUNT>6 || AC_INPUT_COUNT<0 || AC_INPUT_COUNT>8
#error "Feature channel count exceeds Mega hardware capacity (4 relays, 6 PWM, 8 inputs)."
#endif
#if AC_ENABLE_DHT!=0 && AC_ENABLE_DHT!=1
#error "AC_ENABLE_DHT must be 0 or 1."
#endif
#if AC_ENABLE_RTC!=0 && AC_ENABLE_RTC!=1
#error "AC_ENABLE_RTC must be 0 or 1."
#endif
#if AC_ENABLE_BUZZER!=0 && AC_ENABLE_BUZZER!=1
#error "AC_ENABLE_BUZZER must be 0 or 1."
#endif
#if AC_ENABLE_FAN!=0 && AC_ENABLE_FAN!=1
#error "AC_ENABLE_FAN must be 0 or 1."
#endif
#define AC_ACTIVE_RELAYS AC_RELAY_COUNT
#define AC_ACTIVE_PWM AC_PWM_COUNT
#define AC_ACTIVE_INPUTS AC_INPUT_COUNT
#define AC_ACTIVE_FAN (AC_ENABLE_FAN && AC_ENABLE_DHT)
#define AC_ACTIVE_DIAGNOSTICS (AC_ACTIVE_INPUTS || AC_ENABLE_DHT || AC_ENABLE_RTC)
static const ControllerFeatures portalFeatures={uint8_t((AC_ENABLE_DHT?ControllerFeatures::Dht:0)|(AC_ENABLE_RTC?ControllerFeatures::Rtc:0)|(AC_ENABLE_BUZZER?ControllerFeatures::Buzzer:0)|(AC_ACTIVE_FAN?ControllerFeatures::Fan:0)),AC_ACTIVE_RELAYS,AC_ACTIVE_PWM,AC_ACTIVE_INPUTS};
