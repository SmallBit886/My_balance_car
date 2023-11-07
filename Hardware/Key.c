#include "stm32f10x.h"         
#include "Delay.h"
#include "Key.h"

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN | KEY2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(KEY1_GPIO, &GPIO_InitStructure);

}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if (KEY1_Read_IO == 0)
	{
		Delay_ms(20);
		while (KEY1_Read_IO == 0);
		Delay_ms(20);
		KeyNum = 1;
	}
	if (KEY2_Read_IO == 0)
	{
		Delay_ms(20);
		while (KEY2_Read_IO == 0);
		Delay_ms(20);
		KeyNum = 2;
	}

	return KeyNum;
}
