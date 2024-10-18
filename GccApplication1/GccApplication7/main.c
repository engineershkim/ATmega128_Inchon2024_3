/*
 * GccApplication7.c
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

enum User_Mode {
	Ready = 0,
	Clock_Ready,
	Clock,
	Clock_Setting,
	Timer_Ready,
	Timer,
	Timer_Setting,
	};
	
enum Admin_Mode{
	Clock_Senario = 0,
	Timer_Senario,
	};


int Mode = Clock; // 0:Ready, 1:Clock, 2: Clock Setting, 3:Timer, 4: Timer Setting ...
int time_Mode = Clock_Senario; // 0: Clock, 1: Timer

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
		}
	}
}
volatile int sec_tick = 0, min_tick = 0;      // clock tick
volatile int set_tick = 0, set_min_tick = 0;  // Timer tick
int sTick = 0;
ISR(TIMER0_COMP_vect)
{
	sec_tick++;
	TCNT0 = 0;
}
ISR(INT0_vect) // BTN1
{
	if(time_Mode == Clock_Senario) 
	{
		if(Mode == Clock){
			sTick = sec_tick;
			Mode = Clock_Setting;
		}
		else if(Mode == Clock_Setting)
		{			
			sec_tick = sTick;
			Mode = Clock;
		}
	}
	else if(time_Mode == Timer_Senario )
	{
		if(Mode == Timer_Ready){
			Mode = Timer_Setting;
			sTick = set_tick;
			set_tick = 0;
		}
		else if(Mode == Timer_Setting)
		{
			Mode = Timer_Ready;
			//set_tick = sTick;
		}
	}
}
ISR(INT1_vect) // BTN2
{
	if(Mode == Clock_Setting)
	{

		++min_tick;
		digit(min_tick);
	}
	else if(Mode == Timer_Setting)
	{
		
		++set_tick;
		digit(set_tick);
	}
}
ISR(INT2_vect) // BTN3
{
	if(time_Mode == Clock_Senario)
	{
		time_Mode == Timer_Senario;
	}
	else if(time_Mode == Timer_Senario )
	{
		time_Mode == Clock_Senario;
	}
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
	OCR0 = 200;// (1/16M) * 84 * 1024 = 0.005376 == 0.01 (~=10ms)

	//OCR0 = 224; // (1/16M) * 224 * 1024 * 7 = 0.1003 ( ~= 100ms)
	
	// Ext-Intr : Mode Change 0:Ready, 1:upCount, 2:Stop, ...
	EIMSK |= 0x07; // D0 pin, EINT0,
	EICRA |= 0x07;
	//EICRA &= ~0x03;

	CDDR |= 0x0F; // 하위 4bit를 출력으로 설정
	IDDR |= 0xFF; // 8bit 전체를 출력으로 설정
    /* Replace with your application code */
    
	//Mode = 0;
	sei();
	//CPORT = 0x0F; // 모든 자리수 선택
	//IPORT = ~0xFF; // 전체 세그먼트 ON ---> [8.]

	//StandBy(); 
	_delay_ms(100);
	while (1) 
    {
		switch(Mode)
		{
			case Ready: // Ready
				sec_tick = sTick;
				digit(sec_tick);
				_delay_ms(5);
				break;
			case Clock_Ready:	// Clock_Idle
				digit(sTick);
				break;
			case Clock:			// Clock_Running
				digit(sec_tick);
				break;
			case Clock_Setting: // Clock_Setting
				break;
			case Timer_Ready:	// Timer_Idle
				digit(set_tick);
				break;
			case Timer:			// Timer_Running
				if(sTick - sec_tick > 0){
					sec_tick = 0;
					digit(set_tick - sec_tick);
				}
				else{
					digit(0);
					//TODO : Buzzer Run
				}
				break;
			case Timer_Setting: // Timer_Setting
				break;
			default:
				break;
		}
    }
}