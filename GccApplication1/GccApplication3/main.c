/*
 * GccApplication3.c
 *
 * Created: 2024-10-11 오후 2:32:51
 * Author : user
 */ 

#define F_CPU			16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#define DDR DDRG
#define PORT PORTG
#define LED0 PORTG0
#define LED1 PORTG1
#define LED2 PORTG2

int cnt = 0, cnt1 = 0, cntEx = 5, st = 0; // 초기값 : 500ms
ISR(TIMER0_OVF_vect)
{
	cnt++;
	
	if(cnt > 25) // (1/16M) * 256 * 256 * 25 = 0.192 sec == 100ms
	{
		
		cnt = 0;
		cnt1++;
		
		if(cnt1 > cntEx){
			if(st){ PORTG &= ~_BV(LED0); st = 0; }
			else{ PORTG |= _BV(LED0); st = 1; }
		}
	}
}

int ocnt = 0, ocnt1 = 0, ocntEx = 5, ost = 0; // 초기값 : 500ms
ISR(TIMER0_COMP_vect)
{
	ocnt++;
	
	if(cnt > 25) // (1/16M) * 256 * 256 * 25 = 0.192 sec == 100ms
	{
		
		ocnt = 0;
		ocnt1++;
		
		if(ocnt1 > ocntEx){
			if(ost){ PORTG &= ~_BV(LED1); ost = 0; }
			else{ PORTG |= _BV(LED2); ost = 1; }
			TCNT0 = 0;
		}
	}
}

ISR(INT0_vect) // slower
{
	cntEx++;
}
ISR(INT1_vect) // faster
{
	cntEx--;
	if(cntEx <1 ) cntEx =1;
}


int main(void)
{
	DDR |= _BV(LED0) | _BV(LED1) | _BV(LED2);
	PORT |= _BV(LED0);
	StandBy();
	PORT &= ~_BV(LED0);
	// Timer Interrupt 0 설정
	TIMSK |= 0x01;
	TCCR0 |= 0x06; //  110 : 256, 111 : 1024 분주비 1024
	// Timer Interrupt 1 설정
	//TIMSK |= 0x02; // TOIE2
	//TCCR0 |= 0x06; //  110 : 256, 111 : 1024 분주비 1024
	
	OCR0 = 128;
	TIMSK |= 0x02; // OCIE0
	//External Interrupt 설정
	EIMSK |= 0x03;
	EICRA |= 0x0F;
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

