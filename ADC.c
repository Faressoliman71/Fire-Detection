#include "StdTypes.h"
#include "Utils.h"
#include "MemMap.h"
#include "ADC_interface.h"

u16 volt_ref;
static u8  ADC_FLAG=1;
u16 adc_read;


void ADC_init(ADC_VREF_TYPE Vref,ADC_PRESCALER_TYPE scaler)
{
	//clock
	scaler=scaler&0x07;  //00000111
	ADCSRA=ADCSRA&0XF8;  //11111000
	ADCSRA=ADCSRA|scaler;
	
	//(Volt)ref
	switch(Vref)
	{
		case VREF_AREF:
		CLR_BIT(ADMUX,REFS0);
		CLR_BIT(ADMUX,REFS1);
		volt_ref=AREF;
		break;
		
		case VREF_AVCC:
		SET_BIT(ADMUX,REFS0);
		CLR_BIT(ADMUX,REFS1);
		volt_ref=5000;
		break;
		
		case VREF_256:     //2.56 volt
		SET_BIT(ADMUX,REFS0);
		SET_BIT(ADMUX,REFS1);
		volt_ref=2560;
		break;
	}
	
	CLR_BIT(ADMUX,ADLAR);
	
	//ADC Enable
	SET_BIT(ADCSRA,ADEN);
}
/*************************************************/

u16 ADC_read(ADC_CHANNEL_TYPE channel)
{
	u16 adc_value=0;
	/*select channel*/
	ADMUX=ADMUX & 0XE0; 
	ADMUX=ADMUX | channel;
	/*start conversion*/
	SET_BIT(ADCSRA,ADSC);
	/*wait until finish*/
	while(READ_BIT(ADCSRA,ADSC));
	/*get read*/
	adc_value=ADC;   //ADC is a pointer to short and carry address of the adc_low register.
	/*get read with another method
	u8 adc_high,adc_low;
	adc_low=ADCL;
	adc_high=ADCH;
	u16 adc_value=(u16)(adc_high<<8)|(u16)(adc_low);*/
	return adc_value;
}
//read analog volt and return it in (mV)
u16 ADC_read_analog_volt(ADC_CHANNEL_TYPE channel)
{
	u16 adc=ADC_read(channel);
	u16 analog_volt=(adc*(u32)volt_ref*1000)/(1024);
	return analog_volt;
}
/*****************************INT*****************************************/


void ADC_start_conversion(ADC_CHANNEL_TYPE channel)
{
	if(ADC_FLAG==1)
	{
		/*select channel*/
		ADMUX=ADMUX & 0XE0;
		ADMUX=ADMUX | channel;
		/*start conversion*/
		SET_BIT(ADCSRA,ADSC);
		ADC_FLAG=0;
	}
}

//method 1 code busy wait
u16 ADC_get_read_busy_wait(void)
{
	while(READ_BIT(ADCSRA,ADSC));
	ADC_FLAG=1;
	return ADC;
}
//method 2 code periodic checking
u8 ADC_get_read_periodic(u16* data)
{
	if(!READ_BIT(ADCSRA,ADSC))
	{
		*data=ADC;
		ADC_FLAG=1;
		return 1;
	}
	else
	{
		return 0;
	}
}


/***********************************************************************************/

u16 ADC_read_running(ADC_CHANNEL_TYPE channel)
{
	u16 adc_value = 0;
	
	/* Select channel */
	ADMUX = (ADMUX & 0xE0) | (channel & 0x07);

	/* Start conversion */
	SET_BIT(ADCSRA, ADSC);

	/* Wait for conversion to complete */
	while (!READ_BIT(ADCSRA, 4)); // Wait for the ADC interrupt flag (conversion complete)

	/* Clear the ADIF flag by writing 1 to it (optional, but good practice) */
	SET_BIT(ADCSRA, 4);

	/* Get the result */
	adc_value = ADC;

	return adc_value;
}

void ADC_init_running(ADC_VREF_TYPE Vref, ADC_PRESCALER_TYPE scaler)
{
	// Set ADC prescaler
	scaler = scaler & 0x07;
	ADCSRA = ADCSRA & 0xF8;
	ADCSRA = ADCSRA | scaler;

	// Set reference voltage
	switch (Vref)
	{
		case VREF_AREF:
		CLR_BIT(ADMUX, REFS0);
		CLR_BIT(ADMUX, REFS1);
		volt_ref = AREF;
		break;
		case VREF_AVCC:
		SET_BIT(ADMUX, REFS0);
		CLR_BIT(ADMUX, REFS1);
		volt_ref = 5000;  // Assuming AVCC reference voltage
		break;
		case VREF_256:
		SET_BIT(ADMUX, REFS0);
		SET_BIT(ADMUX, REFS1);
		volt_ref = 2560;  // Assuming 2.56V reference voltage
		break;
	}

	CLR_BIT(ADMUX, ADLAR);

	// Enable ADC, enable free running mode (ADATE bit)
	SET_BIT(ADCSRA, ADEN);
	SET_BIT(ADCSRA, ADATE);
}