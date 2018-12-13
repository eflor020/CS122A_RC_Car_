/*	Partner(s) Name & E-mail: Edgar Flores eflor020@ucr.edu
 *	Lab Section: 
 *	Assignment: Lab #  Exercise # 
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "usart_ATmega1284.h"
#include "timer.h"
int ADC_Value;
int ADC_ValueLR;
//--------------------------------ADC reference www.electronicwings.com/avr-atmega/analog-joystick-interface-with-atmega-16-32 --------------------------------------------------------------
void ADC_Init();
int ADC_Read(char);
void ADC_Init()
{
	DDRA = 0x00;  /* Make ADC port as input */
	ADCSRA = 0x87;  /* Enable ADC, fr/128  */
	ADMUX = 0x40;  /* Vref: Avcc, ADC channel: 0 */
}
int ADC_Read(char channel)
{
	int ADC_value;
	ADMUX = (0x40) | (channel & 0x07);/* set input channel to read */
	ADCSRA |= (1<<ADSC); /* start conversion */
	while((ADCSRA &(1<<ADIF))== 0); /* monitor end of conversion interrupt flag */
	ADCSRA |= (1<<ADIF); /* clear interrupt flag */
	ADC_value = (int)ADCL; /* read lower byte */
	ADC_value = ADC_value + (int)ADCH*256;/* read higher 2 bits, Multiply with weightage */
	return ADC_value;  /* return digital value */
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

enum JOYS{start, go,reverse, right, left, speed1, speed2, off}input;
	void ComSM()
	{
		switch(input)
		{
			case start:
				ADC_Value = ADC_Read(0);
				ADC_ValueLR = ADC_Read(2);
		
				if(ADC_Value < 520)
				{
					input = go;
				}
				else if(ADC_Value > 700)
				{
					input = reverse;
				}
				else if(ADC_ValueLR < 200)
				{
					input = left;
				}
				else if(ADC_ValueLR > 1000)
				{
					input = right;
				}
				else
				{
					input = off;
				}
				break;
			case go:
				ADC_Value = ADC_Read(0);
				ADC_ValueLR = ADC_Read(2);
		
				if(ADC_Value < 520)
				{
					input = go;
				}
				else if(ADC_Value > 700)
				{
					input = reverse;
				}
				else if(ADC_ValueLR < 200)
				{
					input = left;
				}
				else if(ADC_ValueLR > 1000)
				{
					input = right;
				}
				else
				{
					input = off;
				}
				break;
			case reverse:
				ADC_Value = ADC_Read(0);
				ADC_ValueLR = ADC_Read(2);
		
				if(ADC_Value < 520)
				{
					input = go;
				}
				else if(ADC_Value > 700)
				{
					input = reverse;
				}
				else if(ADC_ValueLR < 200 )
				{
					input = left;
				}
				else if(ADC_ValueLR > 1000)
				{
					input = right;
				}
				else
				{
					input = off;
				}
				break;
			case right:
				ADC_Value = ADC_Read(0);
				ADC_ValueLR = ADC_Read(2);
			
				if(ADC_Value < 520)
				{
					input = go;
				}
				else if(ADC_Value > 700)
				{
					input = reverse;
				}
				else if(ADC_ValueLR < 200)
				{
					input = left;
				}
				else if(ADC_ValueLR > 1000)
				{
					input = right;
				}
				else
				{
					input = off;
				}
				break;
			case left:
				ADC_Value = ADC_Read(0);
				ADC_ValueLR = ADC_Read(2);
			
				if(ADC_Value < 520)
				{
					input = go;
				}
				else if(ADC_Value > 700)
				{
					input = reverse;
				}
				else if(ADC_ValueLR < 200)
				{
					input = left;
				}
				else if(ADC_ValueLR > 1000)
				{
					input = right;
				}
				else
				{
					input = off;
				}
				break;
			case speed1:
				ADC_Value = ADC_Read(0);
				ADC_ValueLR = ADC_Read(2);
		
				if(ADC_Value < 520)
				{
					input = go;
				}
				else if(ADC_Value > 700)
				{
					input = reverse;
				}
				else if(ADC_ValueLR < 200)
				{
					input = left;
				}
				else if(ADC_ValueLR > 1000)
				{
					input = right;
				}
				else
				{
					input = off;
				}
				break;
			case off:
				ADC_Value = ADC_Read(0);
				ADC_ValueLR = ADC_Read(2);
		
				if(ADC_Value < 520)
				{
					input = go;
				}
				else if(ADC_Value > 700)
				{
					input = reverse;
				}
				else if(ADC_ValueLR < 200)
				{
					input = left;
				}
				else if(ADC_ValueLR > 1000)
				{
					input = right;
				}
				else
				{
					input = off;
				}
				break;
			default:
				input = start;
				break;
		}
		switch(input)
		{
			case start:
				
				break;
			case go:
				if(USART_IsSendReady(0))
				{
					USART_Send(0x01,0);
					PORTB = 0x01;
					USART_Flush(0);
				}
				break;
			case reverse:
				if(USART_IsSendReady(0))
				{
					USART_Send(0x02,0);
					PORTB = 0x02;
					USART_Flush(0);
				}
				break;
			case right:
				if(USART_IsSendReady(0))
				{
					USART_Send(0x03,0);
					PORTB = 0x03;
					USART_Flush(0);
				}
				break;
			case left:
				if(USART_IsSendReady(0))
				{
					USART_Send(0x04,0);
					PORTB = 0x04;
					USART_Flush(0);
				}
				break;
			case off:
				if(USART_IsSendReady(0))
				{
					USART_Send(0x00,0);
					PORTB = 0x00;
					USART_Flush(0);
				}
				break;
			default:
				break;
		}
	}


int main(void)
{
	initUSART(0);
	ADC_Init();
	
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PINA = 0xFF;
	input = start;
	TimerSet(50);
	TimerOn();
	
	while (1)
	{
		ComSM();
		while (!TimerFlag) {}
		TimerFlag = 0;
	}
}
