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
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include <stdlib.h>
#include <stdio.h>

float getVoltage(float val)
{
    return (val * 4.82 / 255.0);
}

int main(void)
{
    DDRC = 0xFF;
    DDRD = 0xFF;

    ADMUX = 0b01100000;
    ADCSRA = 0b10001001;

    TCCR1A |= 1<<WGM11 | 1<<COM1A1 | 1<<COM1A0|1<<COM1B1|1<<COM1B0;
	TCCR1B |= 1<<WGM13 | 1<<WGM12 | 1<<CS10;
	ICR1 = 19999 ;

    ADCSRA |= (1 << ADSC);


    while (1)
    {
        ADCSRA |= (1 << ADSC);

        while (ADCSRA & (1 << ADSC))
            ;

        char str[15] = {};
        char str2[15] = {};
        int d, t = 0;
        float tem, volt;


        tem = ADCH * 1.0;
        volt = getVoltage(tem);

       
        switch (ADMUX)
        {
        case 0b01100000:

            if (volt >= 1.6)
            {
                OCR1A = ICR1 - 1000;
            }
            else if (volt < 1.6)
            {
                OCR1A = ICR1 - 2500;
            }

           
            ADMUX = 0b01100001;

            break;

        case 0b01100001:

                if (volt >= 1.5)
                {
                    OCR1B = ICR1 - 800;
                }
                else if (volt < 1.5)
                {
                    OCR1B = ICR1 - 2500;
                }
            
          

            ADMUX = 0b01100000;
            break;
        }
         _delay_ms(500);
    }
}
