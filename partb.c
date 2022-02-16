/*
 * GccApplication3.c
 *
 * Created: 2/10/2022 10:28:56 PM
 * Author : jbspr
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 16000000UL
volatile int count = 0;

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
   sei();
}

ISR(TIMER1_CAPT_vect){
	if (PINB & (1<<PINB0)){
	if(count == 0){
		PORTB |= (1 << PORTB1);
		PORTB &= ~(1 << PORTB4);
		count=1;
		_delay_ms(20000);
		
	}
	else if(count == 1){
		PORTB &= ~(1 << PORTB1);
		PORTB |= (1 << PORTB2);
		count=2;
		_delay_ms(20000);
		
	}
	else if(count == 2){
		PORTB |= (1 << PORTB3);
		PORTB &= ~(1 << PORTB2);
		count=3;
		_delay_ms(20000);
	}
	else if(count == 3){
		PORTB |= (1 << PORTB4);
		PORTB &= ~(1 << PORTB3);
		count=0;
		_delay_ms(20000);
	}
	TCCR1B &= ~(1<<ICES1); //look for falling
	}
	else{
		PORTB &= ~(1<<PORTB1);
		PORTB &= ~(1<<PORTB2);
		PORTB &= ~(1<<PORTB3);
		PORTB &= ~(1<<PORTB4);
		TCCR1B |= (1<<ICES1); //look for rising edge
	}

}

int main(void)
{
   initialize();
   
   while (1)
   {
	   
   }
}


