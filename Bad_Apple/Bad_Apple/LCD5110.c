#pragma once

#define COMMAND 0
#define DATA	1

void LCD5110_GPIO_Init() {
	LCD5510_RST_DDR	|= 1<<LCD5510_RST_PIN;
	LCD5510_CE_DDR	|= 1<<LCD5510_CE_PIN;
	LCD5510_DC_DDR	|= 1<<LCD5510_DC_PIN;
	LCD5510_DIN_DDR	|= 1<<LCD5510_DIN_PIN;
	LCD5510_CLK_DDR	|= 1<<LCD5510_CLK_PIN;
}

void LCD5110_Init() {
	LCD5110_GPIO_Init();
	
	LCD5510_RST_PORT |= 1<<LCD5510_RST_PIN;
	LCD5110_Write(0x21, COMMAND);
	LCD5110_Write(0x13, COMMAND);
	LCD5110_Write(0x04, COMMAND);
	LCD5110_Write(0x80 + LCD5510_VOLTAGE, COMMAND);
	LCD5110_Write(0x20, COMMAND);
	LCD5110_Write(0x0C, COMMAND);
	LCD5110_Clear();
}

void LCD5110_Send(uint8_t data) {
	LCD5110_Write(data, DATA);
}

void LCD5110_Write(uint8_t data, uint8_t mode) {
	if(mode) 
		LCD5510_DC_PORT |= 1<<LCD5510_DC_PIN; // Выбор команда/данные
	else 
		LCD5510_DC_PORT &= ~(1<<LCD5510_DC_PIN);
	
	LCD5510_CE_PORT &= ~(1<<LCD5510_CE_PIN);
	for(uint8_t i = 0; i < 8; i++)
	{
		LCD5510_CLK_PORT &= ~(1<<LCD5510_CLK_PIN);

		if(data & 0x80) 
			LCD5510_DIN_PORT |= 1<<LCD5510_DIN_PIN;
		else 
			LCD5510_DIN_PORT &= ~(1<<LCD5510_DIN_PIN);

		data <<= 1;

		LCD5510_CLK_PORT |= 1<<LCD5510_CLK_PIN;
	}
	LCD5510_CE_PORT |= 1<<LCD5510_CE_PIN;
}

void LCD5110_Clear() {
	LCD5110_Setpos(0,0);
	for(uint16_t i = 0; i < LCD5510_X * LCD5510_Y; i++)
		LCD5110_Write(0, DATA);
	LCD5110_Setpos(0,0);
}

void LCD5110_Setpos(uint8_t X, uint8_t Y) {
	LCD5110_Write(0x80 + X, COMMAND);
	LCD5110_Write(0x40 + Y, COMMAND);
}

void LCD5110_Array_Send(uint8_t arr[504]) {
	LCD5110_Setpos(0,0);
	for(uint16_t i = 0; i < LCD5510_X * LCD5510_Y; i++)
		LCD5110_Write(arr[i], DATA);
	LCD5110_Setpos(0,0);
}