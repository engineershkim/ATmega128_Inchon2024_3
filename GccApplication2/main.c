/*
 * GccApplication2.c
 *
 * Created: 2024-10-10 오전 9:39:30
 * Author : user
 */ 
#define __DELAY_BACKWARD_COMPATIBLE__

#define F_CPU			16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define SW1 PORTG4  // 1
//#define SW2 PORTG1
//#define SW3 PORTG2
#define LED1 PORTG0 // 4

volatile int ival = 1000;
ISR(INT0_vect)
{
	ival += 100;
}

ISR(INT1_vect)
{
	ival -= 100;
	if(ival < 10) ival = 100;
}

int TestBit(char *pin, char mask) // PINx 레지스터의 값의 mask bit가 0인지 1인지 판별
{
	if((*pin & mask) != 0) return 1;
	return 0;
}


void StandBy(){
	DDRG &= ~0x10; // PG4 : 입력으로 설정
	PORTG |= 0x10; // PG4 : Pull-up

	//while((PING & (1<<SW1)) == 0); // PING0의 초기값이 1(open)임을 가정 //  전원이 켜지고 활성화 될때(pull-up (1))이 될떄까지 대기
	while(1)
	{
		if(TestBit(PING, 0x10)) break;
	}
	while(1)
	{
		if(!TestBit(PING, 0x10)) break;
	}
	
}


int main(void)
{
    /* Replace with your application code */
	
	//DDRG = 0x10; // G Port 하위 4Bit : 입력, 상위 1bit : 출력
	DDRG &= ~0x0C; // 0b 0000 1100 => 0b 1111 0011 입력 마스크
	DDRG |= 0x03; // 0b xxxx xx11  : 출력 마스크
	
	PORTG |= _BV(LED1); // (1<<LED1); //0x10; //시작되면 LED ON, G0 ~ G3 현상태 유지(Pull-Down)
	StandBy();
	PORTG &= ~_BV(LED1);
	
	
	// Mask Register : EIMSK
	EIMSK |= 0x03; // 0b 0000 0011 : 0x03
	// Create register : EICRA
	EICRA = (EICRA & 0xF0) | 0x0A; // 0b 0000 1111 : 0x0F, 0b xxxx 1010 : 0x0A
	
	sei();
	while(1)
	{
		//if((PING & _BV(SW2)) == 0) ival +=500;
		//else if((PING & _BV(SW2)) == 0) 
		//{
			//ival -=500;
			//if(ival < 10) ival = 100;
		//}
		PORTG |= _BV(LED1); // LED ON
		_delay_ms(ival);		// 1/1000 sec delay
		PORTG &= ~_BV(LED1); //LED OFF
		_delay_ms(ival);
	}
}

