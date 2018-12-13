/*	Partner(s) Name & E-mail: Edgar Flores eflor020@ucr.edu
 *	Lab Section: 
 *	Assignment: Lab #  Exercise # 
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
 

#define F_CPU 8000000UL		/* Define CPU Frequency e.g. here its 8MHz */
#include <util/delay.h>		/* Include Delay header file */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart_ATmega1284.h"
#include "timer.h"
unsigned char i;
unsigned char Test;

enum Move{start}state;
void MoveSM()
{
 switch(state)
 {
  unsigned char tmp;
  
  case start:
   tmp = 0x00;
   if ( USART_HasReceived(0) )
   {
    
    tmp = USART_Receive(0);
    PORTA = tmp;
    USART_Flush(0);
    if(tmp == 0x01)
    {
     PORTB = 0x01;
     state = start;
     break;
    }
    else if(tmp == 0x02)
    {
     PORTB = 0x02;
     state = start;
     break;
    }
	
    else if(tmp == 0x00)
    {
     PORTB = 0x00;
     state = start;
     break;
    }
   }
   
 
int main(void)
{
DDRA = 0xFF; PORTA = 0x00;
DDRB = 0xFF; PORTB = 0x00;

initUSART(0);
unsigned char temp = 0;
TimerSet(15);
TimerOn();

    while (1) 
    {
 
 MoveSM();
 while(!TimerFlag){}
 TimerFlag = 0;
    }
}
