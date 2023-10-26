#include "DIO_interface.h"
#include "ADC_interface.h"
#include "Keypad_interface.h"
#include "LCD_interface.h"
#include "MOTOR_interface.h"
#include "Sensors.h"
#include "MemMap.h"
#include "StdTypes.h"
#include "Utils.h"
#include "Keypad_cfg.h"
#include <util/delay.h>

#define PASSWORD 1234




#include <avr/io.h>
 u32 Temp ;
 u16 Smoke_per ;
 u16 pass = 0 ;
 u8 k ;


 


 int main(void)
 {
	 DIO_init_all_pins();
	 LCD_init();
	 ADC_init(VREF_AVCC , ADC_SCALER_128);
	 KEYPAD_init();
     LCD_write_string("Fire Detection");
      _delay_ms(100);
       LCD_clear();
	   LCD_write_string("Enter Pass");
	   _delay_ms(50);
	   LCD_clear();
 
 while (1)
 {
      while ( pass != PASSWORD)
	  {
		  k = KEYPAD_get_key();
		  if ( k != NOT_KEY)
		  {
			  LCD_write_number(k-'0');
			  pass = pass*10 + k -'0';
			  
			  if (pass > 10000) // exceeds 4 digits
			  {
				  LCD_clear();
				  pass=0 ;
			  }
			  
		  }
	  }
     
	 Temp = TEMP_sensor_read();
	 if (Temp < 40 )
	 {
		 LCD_clear();
		 CLR_BIT(PORTA ,7);
		 MOTOR_stop(M1);
		 LCD_write_string("Fine");
		 _delay_ms(100);
		 SET_BIT(PORTA,6);
		 
	 }
	 else if (Temp > 40)
	 {
		 Smoke_per = ADC_read(CH_4);
		 Smoke_per =  ((u32) Smoke_per * 100 ) / 1023 ; // get the percent %
		 if (Smoke_per < 50)
		 {
			 MOTOR_stop(M1);
			 LCD_clear();
			 CLR_BIT(PORTA ,6);
			 SET_BIT(PORTA,7);
			 LCD_write_string("HEAT");
			 _delay_ms(100);
			 if (Temp < 40 )
			 {
				 LCD_clear();
				 LCD_write_string("Fine");
				 CLR_BIT(PORTA,7);
				 SET_BIT(PORTA ,6);
				 
			 }
			 
		 }
		 else if ( Smoke_per >= 50 && Temp>=40)
		 {
			 CLR_BIT(PORTA ,6);
			 LCD_clear();
			 LCD_write_string("Fire");
			 _delay_ms(100);
			 SET_BIT(PORTA,7);
			 MOTOR_clockwise(M1);
		 }
		 
		 
	 }
 }



	
 }

