/*
 * GccApplication6.c
 *
 * Created: 2024-10-16 오후 3:14:12
 * Author : user
 */ 

#define __DELAY_BACKWARD_COMPATIBLE__

#define F_CPU			16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#define FND_TYPE_CATHOD
#define CDDR		DDRA
#define IDDR		DDRB
#define CPORT		PORTA
#define IPORT		PORTB
unsigned char img[] = { 0x3f, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67 }; // 0123456789
char data[5];
int Mode = 0; // 0:Ready, 1:up-Count, 2:Stop, ...

int digit(int num){
	if(num > 9999) return 0;
	data[3] = num % 10; //1
	data[2] = (num / 10) % 10; //10
	data[1] = (num / 100) % 10; //100
	data[0] = num / 1000;	//1000
	return 1;
}

int tcnt = 0, dp = 1; // dp : 점찍을 자리
int i;
ISR(TIMER2_OVF_vect) // FND refresh
{

	//CPORT = 0x0F; // 모든 자리수 선택
	//IPORT = ~0xFF; // 전체 세그먼트 ON ---> [8.]
	if(++tcnt > 2){
		tcnt = 0;
		for(i = 0; i < 4; i++)
		{
//#ifdef FND_TYPE_CATHOD
			if(i == dp)
			{
				CPORT = _BV(i); // 1 << i
				IPORT = ~(img[data[i]] | 0x80);
				_delay_ms(5);
			}
			else
			{
				CPORT = _BV(i); // 1 << i
				IPORT = ~img[data[i]];
				_delay_ms(5);
			}
//#else
//			CPORT = ~_BV(i); // 1 << i
//				IPORT = img[data[i]];
//#endif
				//_delay_ms(5);
		}
			//TCNT0 =0;
	}

}
volatile int tick = 0;
int sTick = 0;
ISR(TIMER0_COMP_vect)
{
	tick++;
	TCNT0 = 0;
}
ISR(INT0_vect)
{
	//Mode++;
	switch(++Mode)
	{
		case 0:
		case 1:			// Start up-count
			tick = 0;
			break;
		case 2:			// Stop
			sTick = tick;
			break;
		default:		// Mode > 2
			tick = sTick = 0;
			Mode = 0;
	}
	if(Mode > 2) Mode = 0;
}

int main(void)
{
	// Timer0: 100ms stop-watch, timer2: FND refresh

	//TIMSK |= 0x40; // 0b 01xx xxxx OCIE0
	DDRG &= ~0x10; // PG4 : 입력으로 설정
	PORTG |= 0x10; // PG4 : Pull-up
	
	//TIMSK |= 0x40; // //0b 01xx xxxx TOIE2
	TIMSK |= 0x02; // TOIE0
	TIMSK |= 0x40; // TOIE2
	TCCR0 |= 0x07; // 분주비 110:256, (1/16M) * 256 * 256 =0.004xxx (~=4ms)
	TCCR2 |= 0x05; // 분주비 110:256, 100:256 : (1/16M) * 256 * 1024 = 0.016(sec)
	OCR0 = 84;// (1/16M) * 84 * 1024 = 0.005376 == 0.01 (~=10ms)

	//OCR0 = 224; // (1/16M) * 224 * 1024 * 7 = 0.1003 ( ~= 100ms)
	
	// Ext-Intr : Mode Change 0:Ready, 1:upCount, 2:Stop, ...
	EIMSK |= 0x01; // D0 pin, EINT0,
	EICRA |= 0x03;
	//EICRA &= ~0x03;

	CDDR |= 0x0F; // 하위 4bit를 출력으로 설정
	IDDR |= 0xFF; // 8bit 전체를 출력으로 설정
    /* Replace with your application code */
    
	//Mode = 0;
	sei();
	//CPORT = 0x0F; // 모든 자리수 선택
	//IPORT = ~0xFF; // 전체 세그먼트 ON ---> [8.]
	int num = 0000;
	digit(num);
	//StandBy(); 
	_delay_ms(100);
	while (1) 
    {
		switch(Mode)
		{
			case 0: // Ready
				digit(0);
				break;
			case 1: // Up-Count
				digit(tick);
				break;
			case 2: // Stop
				break;
				digit(sTick);
			default:
				break;
		}
		
		/*
		for(num = 0; num < 100; num++)
		{
			digit(num);
			//for(int i = 0; i < 4; i++){
				//CPORT = _BV(i); // 1 << i
				////for(int j = 0; j < 4; j++){
				//IPORT = ~img[data[i]];
				////tmp_num %= _BV(i);
				_delay_ms(10);
				////}
			//}
						//if(TestBit(PING, 0x10) == 0){
							//_delay_ms(1000);
							//break;
						//}
						//else{
										//
							//_delay_ms(10);
							//continue;
						//}
			//if(!TestBit(PING, 0x10))
			//{
				//num = 9999;
				//digit(num);
				//break;
			//}

		}
		*/
		
		
	
		
    }
}