/*
 * GccApplication1.c
 *
 * Created: 2024-10-02 오후 3:09:05
 * Author : Engineer shkim
 */ 
#define __DELAY_BACKWARD_COMPATIBLE__

#define F_CPU			16000000UL
#define SPEED_DOWN		3
#define SPEED_UP		2
#define TOGGLE			1
#define NOT_WORK		0

#define SWITCH3			0x08
#define SWITCH2			0x04
#define SWITCH1			0x02

#define GPortL			0x0E
#define G4_Pin_address	0x08
#define G5_Pin_address	0x10
#include <avr/io.h>
#include <util/delay.h>
//  SW : PG3, Input
// LED : PG4, Output

int checking_switch_num(void){
	// TODO : Error Checking num
	char c = PING | GPortL;
	int n = NOT_WORK;

	if(c == 0){
		n = TOGGLE;
	}

	//switch(c){
		//case SWITCH1 :
		//n = TOGGLE;
		//case SWITCH2 :
		//n = SPEED_UP;
		//case SWITCH3 :
		//n = SPEED_DOWN;
	//}
	return n;
}



int main(void)
{

    /* Replace with your application code */
	// printf("Hello World");
	DDRG |=  0x10; // xxxx xxxx => xxx1 0000 : 1=출력
	DDRG &= ~0x0E; // xxxx xxxx => xxxx 000x : 0=입력
	//DDG3 = 1; // bit단위의 접근, but const 때문에 입력불가..?
	
	char v = NOT_WORK;
	int n = NOT_WORK;
	int delay = 200;
	
    while (1) 
    {		

		if(v == TOGGLE) {
			PORTG |= G5_Pin_address;
			_delay_ms(delay);
				
			PORTG &= ~G5_Pin_address;
			_delay_ms(delay);
		}
		n = checking_switch_num();

		if(n == TOGGLE){
			if(v == NOT_WORK) {v = TOGGLE;   _delay_ms(200);}
			else			  {v = NOT_WORK; _delay_ms(200);}
		}
		else if ( n == SPEED_UP ){
			delay -= 10;
		}
		else if ( n == SPEED_DOWN ){
			delay += 10;
		}
    }
}
