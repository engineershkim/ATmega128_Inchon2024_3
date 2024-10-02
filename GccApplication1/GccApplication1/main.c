/*
 * GccApplication1.c
 *
 * Created: 2024-10-02 오후 3:09:05
 * Author : Engineer shkim
 */ 

#include <avr/io.h>
// LED Port : PG4
int main(void)
{
    /* Replace with your application code */
	// printf("Hello World");
	DDRG |= 0x10; // xxx1 0000 : 1=출력, 0=입력
	
    while (1) 
    {
		PORTG |= 0x10;
    }
}

