


#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_
typedef enum
{
	VREF_AREF,
	VREF_AVCC, // 5
	VREF_256   //2.5
}ADC_VREF_TYPE;

typedef enum
{
	CH_0=0,
	CH_1,
	CH_2,
	CH_3,
	CH_4,
	CH_5,
	CH_6,
	CH_7
}ADC_CHANNEL_TYPE;

typedef enum
{
	ADC_SCALER_2=1,
	ADC_SCALER_4,
	ADC_SCALER_8,
	ADC_SCALER_16,
	ADC_SCALER_32,
	ADC_SCALER_64,
	ADC_SCALER_128
}ADC_PRESCALER_TYPE;
#define AREF ((u16)3000)

void ADC_init(ADC_VREF_TYPE Vref,ADC_PRESCALER_TYPE scaler);
//read ADCH and ADCL registers
u16 ADC_read(ADC_CHANNEL_TYPE channel);
//read analog volt and return it in (mV)
u16 ADC_read_analog_volt(ADC_CHANNEL_TYPE channel);

//Polling
void ADC_start_conversion(ADC_CHANNEL_TYPE channel);
u16 ADC_get_read_busy_wait(void);
u8 ADC_get_read_periodic(u16* data);
/***********/
u16 ADC_read_running(ADC_CHANNEL_TYPE channel);
void ADC_init_running(ADC_VREF_TYPE Vref, ADC_PRESCALER_TYPE scaler);


#endif /* ADC_INTERFACE_H_ */