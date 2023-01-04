#pragma once 
//Δ
/////////////* display pinout connection */////////////
#define LCD5510_RST_DDR		DDRB
#define LCD5510_RST_PORT	PORTB
#define LCD5510_RST_PIN		1

#define LCD5510_CE_DDR		DDRD
#define LCD5510_CE_PORT		PORTD
#define LCD5510_CE_PIN		5

#define LCD5510_DC_DDR		DDRD
#define LCD5510_DC_PORT		PORTD
#define LCD5510_DC_PIN		6

#define LCD5510_DIN_DDR		DDRD
#define LCD5510_DIN_PORT	PORTD
#define LCD5510_DIN_PIN		7

#define LCD5510_CLK_DDR		DDRB
#define LCD5510_CLK_PORT	PORTB
#define LCD5510_CLK_PIN		0

#define LCD5510_X	84  // Количество столбцов на дисплее
#define LCD5510_Y	6   // Количество строк на дисплее


// напряжение питания дисплея (VLCD = 3.06 В + v * 0.06 В) 
// (не рекомендуется значение больше 85)
#define LCD5510_VOLTAGE 69


void LCD5110_Init();
void LCD5110_Send(uint8_t data);
void LCD5110_Write(uint8_t data, uint8_t mode);
void LCD5110_Clear();
void LCD5110_Setpos(uint8_t X, uint8_t Y);


#include "LCD5110.c"