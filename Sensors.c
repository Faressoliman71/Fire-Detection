#include "StdTypes.h"
#include "ADC_interface.h"
#include "Sensors.h"

//return temp in (10*c)
u16 TEMP_sensor_read(void)
{
	u16 volt=ADC_read(LM35_SENSOR_CHANNEL);
	u16 temp=(volt * 512) / 1024 - 1;
	return temp;
}

//return pressure in (10*k_p)
u16 PRESSURE_sensor_read(void)    //MPX4115 pressure sensor.
{
	u16 adc=ADC_read(MPX4115_SENSOR_CHANNEL);
	u16 pressure=(((u32)(1000)*(adc-55))/921+150);
	return pressure;
}
