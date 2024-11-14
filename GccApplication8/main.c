/*
 * GccApplication8.c
 *
 * Created: 2024-10-23 오전 9:47:39
 * Author : user
 */ 


#define __DELAY_BACKWARD_COMPATIBLE__

#define F_CPU			16000000UL

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define LED_R PORTB5
#define LED_G PORTB6
#define LED_Y PORTB7
#define BUZZER PORTB6
#define DDR DDRB
#define PORT PORTB

volatile int period = 10;
volatile int DutyRate = 50;

int CS = 1024; // 분주비, Prescaler

//enum { DO, RE, ME, FA, SL, LA, SI};
enum { DO=130, Do=138, RE=146, Re=155, MI=164, FA=174, Fa=185, SL=196, Sl=207, LA=220, La=233, SI=246 };
int song[] = {SL,SL,LA,LA,SL,SL,MI,SL,SL,MI,MI, RE,SL,SL,LA,LA,SL,SL,MI,SL,MI,RE,MI,DO};

int song1[100];

void conv()
{
	for(int i=0; i<35 ; i++)
	{
		song1[i] = F_CPU / (long)((long)CS * (long)(song[i] * 4));
	}
}


ISR(INT0_vect)
{
	//period -= 50;
	//if(period < 0) period = 500;
}
ISR(INT1_vect)
{
	DutyRate += 10;
	if(DutyRate > 90) DutyRate = 10;
}

int led_y_st = 0;
int cnt=0, cnt_ref = 1;
ISR(TIMER0_COMP_vect)
{
	if(++cnt > cnt_ref ){
		if(led_y_st) { PORT &= ~(1<<LED_Y); led_y_st = 0; }
		else		 { PORT |= (1<<LED_Y);  led_y_st = 1; }
		cnt = 0;
	}
	TCNT0 = 0;
}
ISR(INT2_vect)
{
	//cnt_ref--;
	//if(cnt_ref < 0) cnt_ref = 50;
	OCR0 +=10;
	//if (OCR0 > 200) OCR0 = 1;
}

int main(void)
{
    /* Replace with your application code */
    EIMSK = (1<<INT0) | (1<<INT1) | (1 <<INT2); // 0x07;
	EICRA = 0x2A; // xx10 1010 => INT0, INT1, INT2 Falling edge interrupt generate
	
	
	
	//TIMSK = (1<<OCIE1A) | (1<<OCIE1B);
	//ETIMSK = (1<<OCIE1C) ;
	
	TCCR1A = 0xAA; // COMx : 10b : Normal, WGM : xx10
	TCCR1B = 0x1D; // WGM : 11xx, cs : 101 : 1024
	ICR1 = 5000; // WGM 1110b ==>> max
	OCR1A = 157; // 10ms
	OCR1B = 157;
	OCR1C = 157;
	
	TCCR0 |= 0x07; // PSC 1024 // 6uS
	TCCR0 |= (1<<WGM01)|(1<<WGM00); // 11: 고속  PWM
	TCCR0 |= (1<<COM01); // | (1<<COM00) // 10 : Normal Wave
	OCR0 = 157; // ~~10ms = 6us * 157
	DDRB |= 0xF0; // PB4~7 :Output
	DDR |= (1 << LED_R)|(1 << LED_G)|(1 << LED_Y); // 0x07;
	sei();
	while (1) 
    {
		PORT |= (1 << LED_R)|(1 << LED_G); _delay_ms(period * DutyRate / 100);
		PORT &= ~((1 << LED_R)|(1 << LED_G)); _delay_ms(period * (100 - DutyRate) / 100);
	}
}

