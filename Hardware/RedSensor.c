#if KEY
#include "sys.h"
void RedSensor_Init(void)
{	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //开启SW-DP
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //上拉输入
	GPIO_InitStructure.GPIO_Pin = HW_L1_PIN | HW_L2_PIN | HW_R2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HW_L1_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //上拉输入
	GPIO_InitStructure.GPIO_Pin = HW_R1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HW_R1_GPIO, &GPIO_InitStructure);	
	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3 | GPIO_PinSource4 | GPIO_PinSource5);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);
//	
//	EXTI_InitTypeDef EXTI_InitStructure;
//	EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line4 | EXTI_Line5 | EXTI_Line15;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿触发
//	EXTI_Init(&EXTI_InitStructure);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	NVIC_Init(&NVIC_InitStructure);
//		
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Init(&NVIC_InitStructure);
//		
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	NVIC_Init(&NVIC_InitStructure);

//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Init(&NVIC_InitStructure);
//}

//void EXTI3_IRQHandler(void)
//{
//	if (EXTI_GetITStatus(EXTI_Line3) == SET)
//	{
//		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
//		if (HW_L1 == 0)
//		{
//			CarLeft();
//			Delay_ms(200);
//		}
//		EXTI_ClearITPendingBit(EXTI_Line3);
//	}

//}
//void EXTI4_IRQHandler(void)
//{
//	if (EXTI_GetITStatus(EXTI_Line4) == SET)
//	{
//		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
//		if (HW_L2 == 0)
//		{
//			CarLeft();
//			Delay_ms(300);
//		}
//		EXTI_ClearITPendingBit(EXTI_Line4);
//	}

//}
//void EXTI9_5_IRQHandler(void)
//{
//	if (EXTI_GetITStatus(EXTI_Line5) == SET)
//	{
//		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
//		if (HW_R2 == 0)
//		{
//			CarRight();
//			Delay_ms(200);
//		}
//		EXTI_ClearITPendingBit(EXTI_Line5);
//	}

//}
//void EXTI15_10_IRQHandler(void)
//{
//	if (EXTI_GetITStatus(EXTI_Line15) == SET)
//	{
//		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
//		if (HW_R1 == 0)
//		{
//			CarRight();
//			Delay_ms(300);
//		}
//		EXTI_ClearITPendingBit(EXTI_Line15);
//	}
}
void HW_Trailing(void)
{
	CarGo();
	
	if(HW_R1_Read_IO == 0 && HW_R2_Read_IO == 1 && HW_L1_Read_IO == 0 && HW_L2_Read_IO == 0)
	{
		CarRight();
		Delay_ms(50);
	}
	if(HW_R1_Read_IO == 1 && HW_R2_Read_IO == 0 && HW_L1_Read_IO == 0 && HW_L2_Read_IO == 0)
	{
		CarRight();
		Delay_ms(100);
	}
	if(HW_R1_Read_IO == 1 && HW_R2_Read_IO == 1 && HW_L1_Read_IO == 0 && HW_L2_Read_IO == 0)
	{
		CarRight();
		Delay_ms(150);
	}			
	if(HW_R1_Read_IO == 0 && HW_R2_Read_IO == 0 && HW_L1_Read_IO == 1 && HW_L2_Read_IO == 0)
	{
		CarLeft();
		Delay_ms(50);
	}
	if(HW_R1_Read_IO == 0 && HW_R2_Read_IO == 0 && HW_L1_Read_IO == 0 && HW_L2_Read_IO == 1)
	{
		CarLeft();
		Delay_ms(100);
	}
	if(HW_R1_Read_IO == 0 && HW_R2_Read_IO == 0 && HW_L1_Read_IO == 1 && HW_L2_Read_IO == 1)
	{
		CarLeft();
		Delay_ms(150);
	}	
}
#endif
