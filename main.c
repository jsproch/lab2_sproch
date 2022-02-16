/*
 * morse.c
 *
 * Created: 2/15/2022 6:27:39 PM
 * Author : jbspr
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include <avr/interrupt.h>
#include <stdbool.h>
volatile int overcount = 0;
volatile int edge1 = 0;
volatile int edge2 = 0;
volatile int tim = 0;
volatile int test = 0;
volatile int timsincelast = 0;
char String[25];
bool pressed = true;


void initialize(){
	cli();
	DDRB &= ~(1<<DDB0);
	
	TCCR1B &= ~(1<<CS10);
	TCCR1B |= (1<<CS11);
	TCCR1B &= ~(1<<CS12); //two meg Hz clock instead of 16
	
	TCCR1A &= ~(1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12);
	TCCR1B &= ~(1<<WGM13); //clock in normal mode
	
	TCCR1B |= (1<<ICES1); //look for rising edge
	
	TIFR1 |= (1<<ICF1); //clear flag
	
	TIMSK1 |= (1<<ICIE1); //enable interrupt
	TIMSK1 |= (1<<TOIE1); // enable overflow interrupt
	
	sei();

}

ISR(TIMER1_CAPT_vect){
	PORTB ^= (1<<PORTB2);
	PORTB ^= (1<<PORTB3);
	PORTB ^= (1<<PORTB4);
	if (pressed){
		
		edge1 = ICR1;
	}
	if (!(pressed)){
		
		edge2 = ICR1;
		if (overcount < 1){
			int ticks = edge2 - edge1;
			tim = ticks/2000;//ticks divided by two megahertz
		}
		else if(overcount > 0){
			tim = (edge2/2000) + (overcount * 32);
			overcount=0;
		}
		
	}
	//sprintf(String,"time is %u\n",tim);
	//UART_putstring(String);
	pressed=1-pressed;
	TCCR1B ^= (1<<ICES1); //toggle edge
	timsincelast = 0;
}

ISR(TIMER1_OVF_vect){
	overcount++;
}


int main(void)
{
	initialize();
	UART_init(BAUD_PRESCALER);
	int morse = 0;
	int morsecount = 1;
	int morseword = 0;
	bool side = true;
	
	while (1)
	{
		
		if (overcount < 1){
			int ticks = edge2 - ICR1;
			timsincelast = ticks/2000;//ticks divided by two megahertz
		}
		else if(overcount > 0){
			timsincelast = (edge2/2000) + (overcount * 32);
		}
		
		if((tim>30)&(tim<400)){
			//sprintf(String,"DOT\n");
			//UART_putstring(String);
			morse = 3;
			if(morsecount==1){
				side = true;
			}
			morseword += (morse * morsecount);
			morsecount++;
			tim = 0;
		}
		else if((tim>400)&(tim<800)) {
			//sprintf(String,"DASH\n");
			//UART_putstring(String);
			morse = 10;
			if(morsecount==1){
				side = false;
			}
			morseword += (morse * morsecount);
			morsecount++;
			tim = 0;
		}
		else if((timsincelast>800)) {
			//sprintf(String,"SPACE\n");
			//UART_putstring(String);
			morse = 0;
			timsincelast = 0;
		}

		if(morse==0){
			if(morseword==23){
				sprintf(String,"A");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 37){
				sprintf(String,"B");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 58){
				if(side){
					sprintf(String,"V");
					UART_putstring(String);
				}
				else{
					sprintf(String,"C");
					UART_putstring(String);
				}
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 25){
				sprintf(String,"D");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 3){
				sprintf(String,"E");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 51){
				if(side){
					sprintf(String,"F");
					UART_putstring(String);
				}
				else{
					sprintf(String,"Z");
					UART_putstring(String);
				}
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 39){
				if(side){
					sprintf(String,"U");
					UART_putstring(String);
				}
				else{
					sprintf(String,"G");
					UART_putstring(String);
				}
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 30){
				if(side){
					sprintf(String,"H");
					UART_putstring(String);
				}
				else{
					sprintf(String,"M");
					UART_putstring(String);
				}
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 9){
				sprintf(String,"I");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 93){
				sprintf(String,"J");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 46){
				sprintf(String,"K");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 44){
				sprintf(String,"L");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 16){
				sprintf(String,"N");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 60){
				sprintf(String,"O");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 65){
				if(side){
					sprintf(String,"P");
					UART_putstring(String);
				}
				else{
					sprintf(String,"X");
					UART_putstring(String);
				}
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 79){
				sprintf(String,"Q");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 32){
				sprintf(String,"R");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 18){
				sprintf(String,"S");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 10){
				sprintf(String,"T");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 53){
				sprintf(String,"W");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else if(morseword == 86){
				sprintf(String,"Y");
				UART_putstring(String);
				morseword = 0;
				morsecount = 1;
			}
			else{
				morseword=0;
				timsincelast = 0;
				morsecount = 1;
			}
		}
		
	}
}
