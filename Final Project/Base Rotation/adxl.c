
#ifndef F_CPU
#define F_CPU 1000000UL // 16 MHz clock speed
#endif

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


#include <avr/io.h>
#include<stdio.h>
#include<math.h>
#include <util/delay.h>
#include "lcd.h"



#define start            0x08
#define repeated_start   0x10
#define ACKM              1
#define NACKM             0

#define dev_write   0xA6
#define dev_read    0xA7
#define x0_address  0x32
#define x1_address  0x33
#define y0_address  0x34
#define y1_address  0x35
#define z0_address  0x36
#define z1_address  0x37
#define sensitivity 282
#define     g        9.8f



void TWI_Ini(void)
{
	TWBR = 32;
	TWSR = 0x00;
}


void TWI_start(void)
{
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!( TWCR & (1<<TWINT) ) );
	
	if ( !(( (TWSR & 0xF8) ==start) || ( (TWSR & 0xF8) == repeated_start)) )
	{
		TWI_start();
	}
}

void TWI_stop(void)
{
	TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN) ;
}



void TWI_write(char data)
{
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while ( !( TWCR & (1<<TWINT) ) );
	
}


char TWI_read(char ACKM_NACKM)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (ACKM_NACKM<<TWEA);
	while ( !( TWCR & (1<<TWINT) ) );
	return TWDR;
}



void ADXL_write(char address,char data)
{
	
	TWI_start();
	TWI_write(dev_write);
	TWI_write(address);
	TWI_write(data);
	TWI_stop();
}


void ADXL_init()
{
	TWI_Ini();
	ADXL_write(0x2D,0x08);
}



void ADXL_Acc(float data_array[])
{
	int x0,x1,y0,y1,z0,z1;
	TWI_start();
	TWI_write(dev_write);
	TWI_write(x0_address);
	TWI_start();
	TWI_write(dev_read);
	x0=TWI_read(ACKM);
	x1=TWI_read(ACKM);
	y0=TWI_read(ACKM);
	y1=TWI_read(ACKM);
	z0=TWI_read(ACKM);
	z1=TWI_read(NACKM);
	TWI_stop();
	data_array[0]=((x1<<8)|x0)*g/sensitivity;
	data_array[1]=((y1<<8)|y0)*g/sensitivity;
	data_array[2]=((z1<<8)|z0)*g/sensitivity ;
	
}



 
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}
 
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }
 
    while (i < d)
        str[i++] = '0';
 
    reverse(str, i);
    str[i] = '\0';
    return i;
}

void ftoa(float n, char *res, int afterpoint)
{
    int ipart = (int)n;
 
    float fpart = n - (float)ipart;
 
    int i = intToStr(ipart, res, 0);
 
    if (afterpoint != 0)
    {
        res[i] = '.'; 
 
        fpart = fpart * pow(10, afterpoint);
 
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

int main(void)
{
    DDRD = 0xFF;
	DDRC = 0xFF;
	int i;
	Lcd8_Init();
    Lcd8_Clear();
	ADXL_init();

	float acc_data[3] = {0};

	while(1)
	{
		Lcd8_Clear();
		ADXL_Acc(acc_data);


/******************* X - Axis *********************/ 

        char signX[2] = {}; 

        if(acc_data[0] < 0.0)
        {
            acc_data[0] = -1 * acc_data[0]; 
            signX[0] = '-'; 
        }
        else 
        {
            signX[0] = '+';
        }
        
        Lcd8_Set_Cursor(1,0); 
		Lcd8_Write_String("x:");
        Lcd8_Write_String(signX);

        char chX[15]={};
        int data = acc_data[0] ; 
        itoa(data , chX , 10);
        Lcd8_Write_String(chX);
        Lcd8_Write_Char('.');


        acc_data[0] *= 100 ; 
        data = acc_data[0] ; 
        data = data % 100 ; 
        itoa(data , chX , 10);
        Lcd8_Write_String(chX);

/******************** Y-Axis *********************/

         char signY[2] = {}; 

        if(acc_data[1] < 0.0)
        {
            acc_data[1] = -1 * acc_data[1]; 
            signY[0] = '-'; 
        }
        else 
        {
            signY[0] = '+';
        }
        
        Lcd8_Set_Cursor(1,9); 
		Lcd8_Write_String("Y:");
        Lcd8_Write_String(signY);

        char chY[15]={};
        data = acc_data[1] ; 
        itoa(data , chY , 10);
        Lcd8_Write_String(chY);
        Lcd8_Write_Char('.');


        acc_data[1] *= 100 ; 
        data = acc_data[1] ; 
        data = data % 100 ; 
        itoa(data , chY , 10);
        Lcd8_Write_String(chY);

/******************* Z-Axis ****************/ 

         char signZ[2] = {}; 

        if(acc_data[2] < 0.0)
        {
            acc_data[2] = -1 * acc_data[2]; 
            signZ[0] = '-'; 
        }
        else 
        {
            signZ[0] = '+';
        }
        
        Lcd8_Set_Cursor(2,4); 
		Lcd8_Write_String("Z:");
        Lcd8_Write_String(signZ);

        char chZ[15]={};
        data = acc_data[2] ; 
        itoa(data , chZ , 10);
        Lcd8_Write_String(chZ);
        Lcd8_Write_Char('.');


        acc_data[2] *= 100 ; 
        data = acc_data[2] ; 
        data = data % 100 ; 
        itoa(data , chZ , 10);
        Lcd8_Write_String(chZ);
		
		_delay_ms(200);
        Lcd8_Clear();
		
	}

	}

