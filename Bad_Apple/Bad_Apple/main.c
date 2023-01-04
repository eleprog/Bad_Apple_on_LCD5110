﻿#define F_CPU 16000000 //Δ

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "LCD5110.h"

#define BAUDRATE 250000
#define FPS 24

uint8_t lcd_buff[504];
uint8_t flag_update = 0;

volatile uint16_t lcd_buff_count = 0;

void UART_Init();
void Timer1_Init();


void UART_Init() {
	UBRRL = F_CPU / 16 / BAUDRATE - 1;			// настройка baudrate
	UCSRA = 0;									// сброс флагов завершения приема, передачи
	UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);		// разрешение прерывания по приему данных, разрешение приема и передачи
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);	// асинхронный режим, без битов четности, 1 стоп-бит, 8 бит данных
}

void Timer1_Init() {
	TCCR1B = (0<<CS22)|(1<<CS21)|(1<<CS20);	// установка предделителя (64) таймера 1
	TCNT1 = 0;								// обнуление счетного регистра таймера 1
	OCR1A = F_CPU / 64 / FPS;				// установка регистра сравнения таймера 1
	TIMSK |= (1<<OCIE1A);					// разрешение прерываний по совпадению
}

// генерация посылок синхронизации
ISR(TIMER1_COMPA_vect) {
	TCNT1 = 0;
	lcd_buff_count = 0;
	UDR = 0xAA;
}

// обработчик приемника UART
ISR(USART_RXC_vect)		
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