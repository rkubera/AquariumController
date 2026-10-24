static_assert(ConfigStorage::PAYLOAD_SIZE==AcPeripheral::ConfigEnd,"Runtime settings size mismatch");
static_assert(SENSORS_COUNT==8 && SENSORS_SENSOR_CONFIG_BYTES==40 && CONFIG_sensors_addr==112,"Update runtime RAM layout after changing sensors");
static_assert(RELAYS_COUNT==4 && RELAYS_RELAY_CONFIG_BYTES==30 && CONFIG_relays_addr==432,"Update runtime RAM layout after changing relays");
static_assert(PWMOUTPUTS_COUNT==6 && PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES==44 && CONFIG_pwm_outputs_addr==552,"Update runtime RAM layout after changing PWM");
static_assert(CONFIG_sensor_limits_addr==816 && SENSOR_LIMITS_CONFIG_BYTES==16,"Update runtime RAM layout after changing limits");
ConfigStorage configStorage;
void configStorageMillisEvent(){configRestoreService();}
