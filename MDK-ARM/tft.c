#include "tft.h"
static void TFT_WriteCMD(uint8_t cmd) 
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
}

static void TFT_WriteData(uint8_t data) 
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1); //A0 data
	HAL_SPI_Transmit(&hspi1, &data, 1, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
}

static void TFT_SetPos(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) 
{
	//col
	TFT_WriteCMD(0x2A);
	TFT_WriteData(0x00);
	TFT_WriteData(x1); //x start
	TFT_WriteData(0x00);
	TFT_WriteData(x2); // x end
	TFT_WriteCMD(0x2B);
	TFT_WriteData(0x00);
	TFT_WriteData(y1); //y start
	TFT_WriteData(0x00);
	TFT_WriteData(y2); // y end
}

void TFT_Init(const uint8_t* cmdList) 
{
	// Hardware reset
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_Delay(20);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_Delay(120);
	while (*cmdList != TFT_CMD_EOF) 
	{
		TFT_WriteCMD(*cmdList++);
		uint8_t len = *cmdList;
		cmdList++;
		for (uint8_t i = 0; i < len; i++) TFT_WriteData(*cmdList++);
	}
	HAL_Delay(100);
}
void TFT_FillScreen (uint16_t color) 
{
	TFT_SetPos(0, 0, 127, 159);
	TFT_WriteCMD(0x2C);
	for (uint32_t i = 0; i < 128 * 160; i++)
	{
			TFT_WriteData(color >> 8);
			TFT_WriteData(color & 0xFF);
	}
}
void TFT_WriteChar(uint8_t ch) 
{
	if (ch < 32 || ch >=126) return;
	uint8_t index = ch - 32;
	TFT_SetPos(x, y, x + 6, y + 9);
	TFT_WriteCMD(0x2C);
	for (uint8_t i = 0; i < 10; i++) 
	{
		for (uint8_t j = 0; j < 7; j++) 
		{
			uint16_t color = ((Font7x10[index*10+i]>>(15-j))&1) ? WHITE : BLACK; // set color
			
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0); //en pulse
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1); //data
			
			uint8_t data[2] = {color >> 8, color & 0xFF}; // msb
			HAL_SPI_Transmit(&hspi1, data, 2, 100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
		}
	}
}
void TFT_WriteString(const char* str) 
{
	while (*str) 
	{
		TFT_WriteChar((uint8_t)*str);
		str++;
		x+=7;
	}
}

















