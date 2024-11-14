/*
 * GccApplication4.c
 *
 * Created: 2024-10-16 오전 11:37:55
 * Author : shkim Engineer
 */ 


#define F_CPU			16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DDR DDRG
#define PORT PORTG
#define LED0 PORTG0
#define LED1 PORTG1
#define LED2 PORTG2


int st = 0;
ISR(TIMER3_OVF_vect)
{
	if(st)
	{
		
		PORT &= ~_BV(LED0);
		st = 0;
	}
	else
	{
		PORT |= _BV(LED0);
		st = 1;
	}
}

int main(void)
{
	DDR |= 0x07; // _BV(LED1) | ...
	
	ETIMSK  |= _BV(TOIE3); // 0x04 : 4b0100
    TCCR3B |= 0x03;			  // (1/16M) * (분주비_64:011:0x03) * (65535) = OVF Timer 인터럽트 주기 (~250ms)
	
	/* Replace with your application code */
	sei();
    while (1) 
    {
    }
}

