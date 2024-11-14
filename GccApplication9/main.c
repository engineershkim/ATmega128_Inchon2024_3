/*
 * GccApplication9.c
 *
 * Created: 2024-10-25 오전 9:45:36
 * Author : user
 */ 
#define __DELAY_BACKWARD_COMPATIBLE__

#define F_CPU			16000000UL


#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

void UART0_init(){
	// UART Register setting 1 - Buad Rate : 9600
	UCSR0A |= (1<<U2X0);	// 2배속 설정
	UBRR0H = 0;				// Baud rate : 9600
	UBRR0L = 207;
	
	UCSR0C &= ~((1<<UPM01)|(1<<UPM00)); // Parity - 00:None 10:Even 11:Odd
	
	UCSR0B &= ~(1<<UCSZ02);
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00); // Data bits - 011:8  111:9  010:7
	
	UCSR0C &= ~(1<<USBS0);				// Stop bit - 0:1(default) 1:2
	
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);	// UART Open
	
}
void UART0_putc(char c)
{
	while(1)
	{
		if(UCSR0A & (1<<UDRE0)) break; // 입출력 버퍼가 준비될 때까지 대기
	}
	UDR0 = c; // send	
}


void UART0_puts(char *str)
{
	while(*str) UART0_putc(*str++);
}

char buf[256];
char *str = "Hello\r\n";
int n = 0;
ISR(INT0_vect)
{
	//n++;
	sprintf(buf, "Button Pushed %d times.\r\n", n++);
	UART0_puts(buf);
}

int main(void)
{
    /* Replace with your application code */
	UART0_init();
	UART0_puts("\033[23"); // screen clear - ESC[23
	UART0_puts("\033[0;0H"); // move cursor to left top - ESC[0;0H
	EIMSK |= (1<<INT0);
	EICRA |= (0x02); // falling edge
	
	sei();
    while (1) 
    {
    }
}

