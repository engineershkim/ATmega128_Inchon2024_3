/*
 * MyLib.c
 *
 * Created: 2024-10-16 오전 11:40:27
 *  Author: user
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
#define SW1 PORTG4  // 1

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