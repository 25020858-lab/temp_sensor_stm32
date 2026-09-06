#include "midea.h"
#include "stm32f1xx_hal.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
void Midea_Delay_us(uint16_t time) 
{
	__HAL_TIM_SetCounter(&htim4, 0);
	HAL_TIM_Base_Start(&htim4);
	while (__HAL_TIM_GET_COUNTER(&htim4) < time) 
	{
		// do nothing
	}
	HAL_TIM_Base_Stop(&htim4);
}
void Midea_Send_mark_us(uint16_t time) 
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	htim3.Instance->CCR1 = 577;
	HAL_TIM_Base_Start(&htim4);
	uint16_t start = __HAL_TIM_GetCounter(&htim4);
	while (__HAL_TIM_GetCounter(&htim4) - start < time) 
	{
	}
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_TIM_Base_Stop(&htim4);
}
void Midea_Function (Midea_FunctionEnumTypeDef function) 
{
	//header
	Midea_Send_mark_us(9000);
	Midea_Delay_us(4500);
	// 48 bit
	for (uint16_t i = 0; i < 48; i++) 
	{
		Midea_Send_mark_us(560);
		if (function__bit_arr[function][i] == 0) Midea_Delay_us(560);
		else Midea_Delay_us(1690);
	}
	Midea_Send_mark_us(560);
}













