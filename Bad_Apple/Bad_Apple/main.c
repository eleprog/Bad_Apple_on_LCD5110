#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "LCD5110.h"

#define FPS 24
#define BAUDRATE 250000

uint8_t flag_update = 0;

uint8_t lcd_buff[504];
volatile uint16_t lcd_buff_count = 0;

void UART_Init() {
	UBRRL = F_CPU / 16 / BAUDRATE - 1;			// ��������� baudrate
	UCSRA = 0;									// ����� ������ ���������� ������, ��������
	UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);				// ���������� ���������� �� ������ ������, ���������� ������ � ��������
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);	// ����������� �����, ��� ����� ��������, 1 ����-���, 8 ��� ������
}

void Timer1_Init() {
	TCCR1B = (0<<CS22)|(1<<CS21)|(1<<CS20);	// ��������� ������������ (64) ������� 1
	TCNT1 = 0;								// ��������� �������� �������� ������� 1
	OCR1A = F_CPU / 64 / FPS;				// ��������� �������� ��������� ������� 1
	TIMSK |= (1<<OCIE1A);					// ���������� ���������� �� ����������
}

ISR(TIMER1_COMPA_vect) {
	TCNT1 = 0;
	lcd_buff_count = 0;
	UDR = 0xAA;
}

ISR(USART_RXC_vect)		// ��������� ��������� UART
{
	lcd_buff[lcd_buff_count] = UDR;
	lcd_buff_count++;
	if(lcd_buff_count == 504) {
		lcd_buff_count = 0;
		flag_update = 1;
	}
}

int main(void)
{
	LCD5110_Init();
	UART_Init();
	Timer1_Init();
	
	asm("sei");
	
	while (1) {
		if(flag_update) {
			LCD5110_Array_Send(lcd_buff);
			flag_update = 0;
		}
	}
}