/*
 * MPU-6050: Rotating Hand-base
 *
 * Created: 29-07-2018 8:30 PM
 *  Author: Shamiul Hasan Rumman
 */ 




#include "mpu6050_twi.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


#define D0 eS_PORTD0
#define D1 eS_PORTD1
#define D2 eS_PORTD2
#define D3 eS_PORTD3
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7



#include "lcd.h"



float Acc_x,Acc_y,Acc_z,Temperature,Gyro_x,Gyro_y,Gyro_z;


void ftoap(float val)
{

    char fullNo[30] = {};

    if(val < 0.0)
    {
        val = -1 * val;
        fullNo[0] = '-';
    }
    else
    {
        fullNo[0] = '+';
    }

    char ch[15]={};
    int data = val;
	itoa(data , ch , 10);

    strcat(fullNo , ch);
    strcat(fullNo , ".");

    val = val * 100.0 ;
    data = val ;
    data = data % 100 ;

 	char ch2[15]={};
	itoa(data , ch2 , 10);
    strcat(fullNo , ch2);

    Lcd8_Write_String(fullNo);
}




int main()
{

	DDRD = 0xFF;
	DDRC = 0xFF; 
	DDRA = 0xFF; 

	//Lcd8_Init();


	char buffer[20], float_[10];
	float Xa,Ya,Za,t;
	float Xg=0.0,Yg=0.0,Zg=0.0;

	DDRD = 0xFF;
    	TCCR1A |= 1<<WGM11 | 1<<COM1A1 | 1<<COM1A0;
	TCCR1B |= 1<<WGM12 | 1<<WGM13 | 1<<CS10;
	ICR1 = 19999 ;

	ADMUX = 0b01100000;
    	ADCSRA = 0b10001001;


	I2C_Init();	
	Gyro_Init();

	int right = 0 , left = 1 , middle = 2; 
	int position = right ; 


	
	while(1)
	{

		Read_RawValue(&Acc_x, &Acc_y, &Acc_z, &Temperature, &Gyro_x, &Gyro_y, &Gyro_z);

		/* Divide raw value by sensitivity scale factor */ 
		//16384.0 
		Xa = Acc_x * 9.8 / 16384.0 ;										
		Ya = Acc_y * 9.8 / 16384.0 ;
		Za = Acc_z * 9.8 / 16384.0 ;
		
		//16.4
		Xg = Gyro_x/16.4;
		Yg = Gyro_y/16.4;
		Zg = Gyro_z/16.4;

		
		t = (Temperature/340.00)+36.53;

		/*Lcd8_Clear();

		Lcd8_Set_Cursor(1,0); 
		Lcd8_Write_String("x:");
		ftoap(Xg);

		Lcd8_Set_Cursor(1,9); 
		Lcd8_Write_String("y:");
		ftoap(Yg);

		Lcd8_Set_Cursor(2,4); 
		Lcd8_Write_String("z:");
		ftoap(Zg);*/

		

		if(position == right)
		{
			if(Ya <= -3.0)
			{
				
				OCR1A = ICR1 - 800;
				position = left ; 
			}
		}
		else if(position == left)
		{
			if(Ya >= 3.0)
			{
				 
				OCR1A = ICR1 - 1200;
				position = right ; 
			}
		}

		
		_delay_ms(200);
		

	}
}

