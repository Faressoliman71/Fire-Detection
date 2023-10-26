

#ifndef SENSORS_H_
#define SENSORS_H_
#define LM35_SENSOR_CHANNEL    CH_5
#define MPX4115_SENSOR_CHANNEL  CH_4

u16 TEMP_sensor_read(void);

u16 PRESSURE_sensor_read(void);




#endif /* SENSORS_H_ */