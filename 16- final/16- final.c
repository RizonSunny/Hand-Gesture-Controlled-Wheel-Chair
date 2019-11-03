#include<avr/io.h>
/*Includes io.h header file where all the Input/Output Registers and its Bits are defined for all AVR microcontrollers*/

#define	F_CPU	1000000
/*Defines a macro for the delay.h header file. F_CPU is the microcontroller frequency value for the delay.h header file. Default value of F_CPU in delay.h header file is 1000000(1MHz)*/

#include<util/delay.h>
/*Includes delay.h header file which defines two functions, _delay_ms (millisecond delay) and _delay_us (microsecond delay)*/

#define		LCD_DATA_PORT		PORTB
/*LCD_DATA_PORT is the microcontroller PORT Register to which the data pins of the LCD are connected. Here it is connected to PORTB*/

#define 	LCD_CONT_PORT		PORTD
/*LCD_CONT_PORT is the microcontroller PORT Register to which the control pins of the LCD are connected. Here it is connected to PORTD*/

#define 	LCD_RS 		PD0
/*LCD_RS is the microcontroller Port pin to which the RS pin of the LCD is connected. Here it is connected to PD0*/

#define 	LCD_RW 		PD1
/*LCD_RW is the microcontroller Port pin to which the RW pin of the LCD is connected. Here it is connected to PD1*/

#define 	LCD_EN 		PD2
/*LCD_EN is the microcontroller Port pin to which the EN pin of the LCD is connected. Here it is connected to PD2*/

#include<stdlib.h>
/*Includes stdlib.h header file which defines different standard library functions.*/

/*ADC Function Declarations*/
void adc_init(void);
int read_adc_channel(unsigned char channel);


/*ADXL335 Accelerometer Sensor Function Declarations*/
int read_adxl335_x_value(unsigned char channel);
int read_adxl335_y_value(unsigned char channel);
int read_adxl335_z_value(unsigned char channel);


/*Alphanumeric LCD Function Declarations*/
void lcd_data_write(char data);
void lcd_command_write( char command);
void lcd_init();
void lcd_string_write( char *string);
void lcd_number_write(int number,unsigned char radix);
void motor_fwd();
void motor_bck();


  
int main(void)
{
	DDRB=0xff;
	/*All pins of PortB are declared output (data pins of LCD are connected)*/

	DDRD=0x07;
	DDRC=0x0f;
	/*PD0, PD1 and PD2 pins of PortD are declared output (control pins of LCD connected)*/
        
	int x_axis,y_axis,z_axis;
	/*Variable declarations*/
 
	adc_init();
	/*ADC initialization*/
   
	lcd_init();
	/*LCD initialization*/
 	
	lcd_string_write("Microprocessor ");
	/*String display in 1st row of LCD*/

	lcd_command_write(0xc0);
	/*Cursor moves to 2nd row 1st column of LCD*/

	lcd_string_write("project");
	/*String display in 2nd row of LCD*/	

	_delay_ms(500);
	_delay_ms(500);
	_delay_ms(500);
	_delay_ms(500);
	/*Display stays for 2 second*/

	/*Start of infinite loop*/
	while(1)
	{
        x_axis=read_adxl335_x_value(0);
		y_axis=read_adxl335_y_value(1);
        z_axis=read_adxl335_z_value(2);
        
	if(x_axis<=370 && y_axis>=450)
		{
				
			lcd_command_write(0x01);
			lcd_string_write("back");
			lcd_command_write(0xc0);
			lcd_string_write("left");
			motor_bck();
		
			
		}			
		else if(x_axis<=370 && y_axis<=370)
		{
			lcd_command_write(0x01);
			lcd_string_write("back");
			lcd_command_write(0xc0);
			lcd_string_write("right");
			motor_bck();
		
		}			
			
		else if(x_axis<=370)
		{
			lcd_command_write(0x01);
			lcd_command_write(0x01);
			lcd_string_write("back");
			lcd_command_write(0xc0);
			lcd_string_write(". . . .");
			motor_bck();
		}
		else if (x_axis>=460 && y_axis>=450)
		{
			lcd_command_write(0x01);
			lcd_string_write("forward");
			lcd_command_write(0xc0);
			lcd_string_write("left");
			motor_fwd();	
		}
		else if(x_axis>=460 && y_axis<=380)
		{
			lcd_command_write(0x01);
			lcd_string_write("forward");
			lcd_command_write(0xc0);
			lcd_string_write("right");
			motor_fwd();
								
		}
		else if(x_axis>=460)
		{
			lcd_command_write(0x01);
			lcd_string_write("forward");
			lcd_command_write(0xc0);
			lcd_string_write(". . . .");
			motor_fwd();
			
		}
		else 
		{
			PORTC=0b00000000;
			lcd_command_write(0x01);
			lcd_string_write("no move");
			lcd_command_write(0xc0);
			lcd_string_write("for 2 second");
			_delay_ms(500);
			_delay_ms(500);
			_delay_ms(500);
			_delay_ms(500);
			
		}	
		
	}                                                                                                                                                                                                                     
}                              
/*End of Program*/

void motor_fwd()
{
	PORTC=0b00000010;
	_delay_ms(50);

		
		PORTC=0b00000000;
}

void motor_bck()
{
	PORTC=0b00000001;
_delay_ms(500);
		
		PORTC=0b00000000;
}

                                                                                                                  
                                                                                     
/*ADC Function Definitions*/
void adc_init(void)
{                    
	ADCSRA=(1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(1<<ADPS2);
	SFIOR=0x00;
}


int read_adc_channel(unsigned char channel)
{
	int adc_value;
	unsigned char temp;
	ADMUX=(1<<REFS0)|channel;
  	_delay_ms(1);
	temp=ADCL;
	adc_value=ADCH;
	adc_value=(adc_value<<8)|temp;
  	return adc_value;
}

/*ADXL335 Accelerometer Sensor Function Definitions*/
int read_adxl335_x_value(unsigned char channel)
{
	int x_value;
	x_value=read_adc_channel(channel);
	return x_value;
}

int read_adxl335_y_value(unsigned char channel)
{
	int y_value;
	y_value=read_adc_channel(channel);
	return y_value;
}

int read_adxl335_z_value(unsigned char channel)
{
	int z_value;
	z_value=read_adc_channel(channel);
	return z_value;
}  


/*Alphanumeric LCD Function Definitions*/
void lcd_data_write(char data)
{
	LCD_CONT_PORT=_BV(LCD_EN)|_BV(LCD_RS);
	LCD_DATA_PORT=data;
	_delay_ms(1);
	LCD_CONT_PORT=_BV(LCD_RS);
	_delay_ms(1);
}


void lcd_command_write(char command)
{
	LCD_CONT_PORT=_BV(LCD_EN);
	LCD_DATA_PORT=command;
	_delay_ms(1);
	LCD_CONT_PORT=0x00;
	_delay_ms(1);
}

void lcd_init()
{
	lcd_command_write(0x38);
	lcd_command_write(0x01);
	lcd_command_write(0x06);
	lcd_command_write(0x0e);	
}

void lcd_string_write(char *string)
{
	while (*string)
	lcd_data_write(*string++);
}

void lcd_number_write(int number,unsigned char radix)
{
	char *number_string="00000";
	itoa(number,number_string,radix);
	lcd_string_write(number_string);
}
