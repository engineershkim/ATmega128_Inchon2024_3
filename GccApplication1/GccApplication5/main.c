/*
 * GccApplication4.c
 *
 * Created: 2024-10-15 오후 3:36:23
 * Author : user
 */ 


#define __DELAY_BACKWARD_COMPATIBLE__

#define F_CPU			16000000UL

#include <avr/io.h>
#include <util/delay.h>


#define SW1 PORTG4  // 1
//#define SW2 PORTG1
//#define SW3 PORTG2
#define LED1 PORTG0 // 4

int main(void)
{
    /* Replace with your application code */
	
	//DDRG = 0x10; // G Port 하위 4Bit : 입력, 상위 1bit : 출력
	DDRG &= ~0x0C; // 0b 0000 1100 => 0b 1111 0011 입력 마스크
	DDRG |= 0x03; // 0b xxxx xx11  : 출력 마스크
    while (1) 
    {
		
		PORTG |= _BV(LED1); // LED ON
		_delay_ms(500);		// 1/1000 sec delay
		PORTG &= ~_BV(LED1); //LED OFF
		_delay_ms(500);
    }
}

