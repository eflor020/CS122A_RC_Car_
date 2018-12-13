 /*	Partner(s) Name & E-mail: Edgar Flores eflor020@ucr.edu
 *	Lab Section: 
 *	Assignment: Lab #  Exercise # 
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

 #define BUT ((~PINB) & 0x03)

 #define F_CPU 8000000UL		/* Define CPU Frequency e.g. here its 8MHz */
 #include <util/delay.h>		/* Include Delay header file */
 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include "usart_ATmega1284.h"
 #include "timer.h"
 #include "Bit.h"

 //-----------------------------------------------------Shared Variables------------------------------------------------------------------------------------------
 unsigned char i;
 unsigned char Test;
 unsigned char Forward;
 unsigned char Reverse;
 unsigned char UPPERBOUND;
 //---------------------------------------------------------------------------------------------------------------------------------------------------------------

 //----------------------------------------------------PWM Forward-------------------------------------------------------------------------------------------------
 enum Speed{init, on, off}speed;
 void PWM()
 {
	 switch(speed)
	 {
		 case init:
		 if(Forward)
		 {
			 speed = on;
			 i = 0;
		 }
		 else
		 {
			 speed = init;
		 }
		 break;
		 case on:
		 if(!Forward || (GetBit(~PINA, 0) == 1) )
		 {
			 speed = init;
		 }
		 else if(i < UPPERBOUND && Forward)
		 {
			 speed = on;
			 i++;
		 }
		 else if(Forward)
		 {
			 speed = off;
			 i = 0;
		 }
		 break;
		 case off:
		 if(!Forward || (GetBit(~PINA, 0) == 1))
		 {
			 speed = init;
		 }
		 else if(i < 4 && Forward)
		 {
			 speed = off;
			 i++;
		 }
		 else if(Forward)
		 {
			 speed = on;
			 i = 0;
		 }
		 break;
		 default:
		 speed = init;
		 break;
	 }
	 switch(speed)
	 {
		 case init:
		 break;
		 case on:
		 PORTC = 0x01;
		 break;
		 case off:
		 PORTC = 0x00;
		 break;
		 default:
		 break;
	 }
 }
 //---------------------------------------------------------------------------------------------------------------------------------------------

 //--------------------------------------------PWM Reverse--------------------------------------------------------------------------------------
 enum Speed2{init2, on2, off2}speed2;
 void PWM2()
 {
	 switch(speed2)
	 {
		 case init2:
		 if(Reverse)
		 {
			 speed2 = on2;
			 i = 0;
		 }
		 else
		 {
			 speed2 = init2;
		 }
		 break;
		 case on2:
		 if(!Reverse || (GetBit(~PINA, 1) == 1) )
		 {
			 speed2 = init2;
		 }
		 else if(i < UPPERBOUND && Reverse)
		 {
			 speed2 = on2;
			 i++;
		 }
		 else if(Reverse)
		 {
			 speed2 = off2;
			 i = 0;
		 }
		 break;
		 case off2:
		 if(!Reverse || (GetBit(~PINA, 1) == 1) )
		 {
			 speed2 = init2;
		 }
		 else if(i < 4 && Reverse )
		 {
			 speed2 = off2;
			 i++;
		 }
		 else if(Reverse)
		 {
			 speed2 = on2;
			 i = 0;
		 }
		 break;
		 default:
		 speed2 = init2;
		 break;
	 }
	 switch(speed2)
	 {
		 case init2:
		 break;
		 case on2:
		 PORTC = 0x02;
		 break;
		 case off2:
		 PORTC = 0x00;
		 break;
		 default:
		 break;
	 }
 }
 //-----------------------------------------------------------------------------------------------------------------------------------------

 //-------------------------------------------------------Receiver for car------------------------------------------------------------------
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
			 
			 if(tmp == 0x01 && (GetBit(~PINA, 0) == 0))
			 {
				 
				 Forward = 1;
				 
				 state = start;
				 break;
			 }
			 else if(tmp == 0x02 && (GetBit(~PINA, 1) == 0))
			 {
				 
				 Reverse = 1;
				 state = start;
				 break;
			 }
			 else if(tmp == 0x03)
			 {

				 OCR1A = 105;  // left turn
				 state = start;
				 break;
			 }
			 else if(tmp == 0x04)
			 {
				 
				 OCR1A = 225; // right turn
				 state = start;
				 break;
			 }
			 else if(tmp != 0x01 || tmp != 0x02)
			 {
				 PORTC = 0x00;
				 Forward = 0;
				 Reverse = 0;
				 state = start;
				 break;
			 }
			 else
			 {
				 PORTC = 0x00;
				 Forward = 0;
				 Reverse = 0;
				 OCR1A = 165; // straight
				 state = start;
				 break;
			 }
		 }
		
	 }
 }

 //--------------------------------------------------------------------------------------------------------------------------------
 int main(void)
 {
	 DDRA = 0x00; PINA = 0xFF;
	 DDRB = 0x00; PINB = 0xFF;
	 DDRC = 0xFF; PORTC = 0x00;

	 //---------------------code reference interface www.electronicwings.com/avr-atmega/servo-motor-interfacing-with-atmega16 --------------------------------------
	 DDRD |= (1<<PD5);	/* Make OC1A pin as output */
	 TCNT1 = 0;		/* Set timer1 count zero */
	 ICR1 = 2499;		/* Set TOP count for timer1 in ICR1 register */

	 /* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
	 TCCR1A = (1<<WGM11)|(1<<COM1A1);
	 TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
	 //-------------------------------------------------------------------------------------------------------------------------------

	 initUSART(0);
	 UPPERBOUND = 2;
	 TimerSet(1);
	 TimerOn();

	 while (1)
	 {
		 
		 if(BUT == 0x01)
		 {
			 UPPERBOUND = 2;
			 
		 }
		 if(BUT == 0x02)
		 {
			 UPPERBOUND = 50;
		 }
		 
		 MoveSM();
		 PWM();
		 PWM2();
		 while(!TimerFlag){}
		 TimerFlag = 0;
	 }
 }
