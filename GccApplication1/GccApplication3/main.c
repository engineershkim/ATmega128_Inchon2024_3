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
#define LED PORTG1


int cnt = 0, cnt1 = 0, cntEx = 5, st = 0; // 초기값 : 500ms
ISR(TIMER0_OVF_vect)
{
	cnt++;
	
	if(cnt > 25) // (1/16M) * 256 * 256 * 25 = 0.192 sec == 100ms
	{
		
		cnt = 0;
		cnt1++;
		
		if(cnt1 > cntEx){
			if(st){ PORTG &= ~_BV(LED); st = 0; }
			else{ PORTG |= _BV(LED); st = 1; }
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

int TestBit(char *pin, char mask) // PINx 레지스터의 값의 mask bit가 0인지 1인지 판별
{
	if((*pin & mask) != 0) return 1;
	return 0;
}


void StandBy(){
	DDR &= ~0x10; // PG4 : 입력으로 설정
	PORT |= 0x10; // PG4 : Pull-up

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
	DDR |= _BV(LED);
	PORT |= _BV(LED);
	StandBy();
	PORT &= ~_BV(LED);
	// Timer Interrupt 0 설정
	TIMSK |= 0x01;
	TCCR0 |= 0x06; //  110 : 256, 111 : 1024 분주비 1024
	// Timer Interrupt 1 설정
	TIMSK |= 0x02; // TOIE2
	TCCR0 |= 0x06; //  110 : 256, 111 : 1024 분주비 1024
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

