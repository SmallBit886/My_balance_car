#include "Exti.h"
/**
  * @brief  MPU6050外部中断引脚
  * @param  
  * @retval 
  * @attention  RCC_APB2Periph_AFIO开启复用时钟，复用为外部中断
  */
void MPU6050_EXTI_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//开启时钟
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//上拉模式
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;//PB5配置为上拉输入
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB , GPIO_PinSource5);//将GPIO口配置成外部中断 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;//将对应的GPIO口连接到中断线上
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//中断事件类型，下降沿
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//选择模式，中断型
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能该中断
	EXTI_Init(&EXTI_InitStructure);//将配置好的参数写入寄存器
	
}

