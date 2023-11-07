#include "sys.h"
#include "encoder.h"
/**
  * @brief  定时器2编码器模式
  * @param  PA0-通道1 PA1-通道2
  * @retval 
  */
void Encoder_TIM2_Init(void)
{
	/******************************开启时钟*****************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	/*****************************定义结构体变量******************************/
	GPIO_InitTypeDef GPIO_InitStruct;//GPIO
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//时基单元
	TIM_ICInitTypeDef TIM_ICInitStruct;//定时器输入捕获
	/****************************配置引脚的模式*******************************/
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0 |GPIO_Pin_1;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//结构体初始化
	/****************************配置时基单元*******************************/		
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);//初始化定时器。
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=65535;
	TIM_TimeBaseInitStruct.TIM_Prescaler=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);//结构体初始化
	/******************************配置编码器模式*****************************/	
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//配置编码器模式
	/******************************输入捕获单元配置*****************************/	
	TIM_ICStructInit(&TIM_ICInitStruct);//初始化输入捕获
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter=10;
	TIM_ICInit(TIM2,&TIM_ICInitStruct);//结构体初始化
	/*****************************可以继续使用上面定义的结构体变量******************************/	
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter=10;
	TIM_ICInit(TIM2,&TIM_ICInitStruct);//结构体初始化
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//配置溢出更新中断标志位
	
	TIM_SetCounter(TIM2,0);//清零定时器计数值
	
	TIM_Cmd(TIM2,ENABLE);//开启定时器
}

/**
  * @brief  定时器3编码器模式
  * @param  PA6-通道1 PA7-通道2
  * @retval 
  */
void Encoder_TIM3_Init(void)
{
	/***************************开启时钟********************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/****************************配置引脚的模式*******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/****************************配置时基单元*******************************/	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	/******************************输入捕获单元配置*****************************/
	TIM_ICInitTypeDef TIM_ICInitStructure;//定义一个结构体变量
	TIM_ICStructInit(&TIM_ICInitStructure);//给结构体赋一个初始值
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//通道1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//
	TIM_ICInit(TIM3, &TIM_ICInitStructure);//结构体初始化
	/*****************************可以继续使用上面定义的结构体变量******************************/
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);//再定义一个初始化，把值赋给通道2
	/******************************配置编码器模式*****************************/
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//配置溢出更新中断标志位
	TIM_SetCounter(TIM3,0);//清零定时器计数值
	
	TIM_Cmd(TIM3, ENABLE);//启动定时器
}
/**
  * @brief  读取编码器速度入口函数
  * @param  TIM2   TIM3
  * @retval 计数器在一定时间的计数值
  * @attention  TIM_GetCounter的返回值为uint16_t,需要强制转换为有符号类型
  */
int16_t Read_Speed(int TIMx)
{
	int16_t value_1;
	switch(TIMx)
	{
		case 2:value_1=(int16_t)TIM_GetCounter(TIM2);TIM_SetCounter(TIM2,0);break;//IF是定时器2，1.采集编码器的计数值并保存。2.将定时器的计数值清零。
		case 3:value_1=(int16_t)TIM_GetCounter(TIM3);TIM_SetCounter(TIM3,0);break;
		default:value_1=0;
	}
	return value_1;
}

int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);//uint16_t类型
	TIM_SetCounter(TIM3, 0);//清零定时器计数值
	return Temp;
}
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=0)
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}		
}
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=0)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}		
}
