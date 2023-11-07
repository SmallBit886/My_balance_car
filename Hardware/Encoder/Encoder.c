#include "sys.h"
#include "encoder.h"
/**
  * @brief  ��ʱ��2������ģʽ
  * @param  PA0-ͨ��1 PA1-ͨ��2
  * @retval 
  */
void Encoder_TIM2_Init(void)
{
	/******************************����ʱ��*****************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	/*****************************����ṹ�����******************************/
	GPIO_InitTypeDef GPIO_InitStruct;//GPIO
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//ʱ����Ԫ
	TIM_ICInitTypeDef TIM_ICInitStruct;//��ʱ�����벶��
	/****************************�������ŵ�ģʽ*******************************/
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0 |GPIO_Pin_1;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//�ṹ���ʼ��
	/****************************����ʱ����Ԫ*******************************/		
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);//��ʼ����ʱ����
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=65535;
	TIM_TimeBaseInitStruct.TIM_Prescaler=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);//�ṹ���ʼ��
	/******************************���ñ�����ģʽ*****************************/	
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//���ñ�����ģʽ
	/******************************���벶��Ԫ����*****************************/	
	TIM_ICStructInit(&TIM_ICInitStruct);//��ʼ�����벶��
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter=10;
	TIM_ICInit(TIM2,&TIM_ICInitStruct);//�ṹ���ʼ��
	/*****************************���Լ���ʹ�����涨��Ľṹ�����******************************/	
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter=10;
	TIM_ICInit(TIM2,&TIM_ICInitStruct);//�ṹ���ʼ��
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//������������жϱ�־λ
	
	TIM_SetCounter(TIM2,0);//���㶨ʱ������ֵ
	
	TIM_Cmd(TIM2,ENABLE);//������ʱ��
}

/**
  * @brief  ��ʱ��3������ģʽ
  * @param  PA6-ͨ��1 PA7-ͨ��2
  * @retval 
  */
void Encoder_TIM3_Init(void)
{
	/***************************����ʱ��********************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/****************************�������ŵ�ģʽ*******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/****************************����ʱ����Ԫ*******************************/	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	/******************************���벶��Ԫ����*****************************/
	TIM_ICInitTypeDef TIM_ICInitStructure;//����һ���ṹ�����
	TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ�帳һ����ʼֵ
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//ͨ��1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//
	TIM_ICInit(TIM3, &TIM_ICInitStructure);//�ṹ���ʼ��
	/*****************************���Լ���ʹ�����涨��Ľṹ�����******************************/
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);//�ٶ���һ����ʼ������ֵ����ͨ��2
	/******************************���ñ�����ģʽ*****************************/
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//������������жϱ�־λ
	TIM_SetCounter(TIM3,0);//���㶨ʱ������ֵ
	
	TIM_Cmd(TIM3, ENABLE);//������ʱ��
}
/**
  * @brief  ��ȡ�������ٶ���ں���
  * @param  TIM2   TIM3
  * @retval ��������һ��ʱ��ļ���ֵ
  * @attention  TIM_GetCounter�ķ���ֵΪuint16_t,��Ҫǿ��ת��Ϊ�з�������
  */
int16_t Read_Speed(int TIMx)
{
	int16_t value_1;
	switch(TIMx)
	{
		case 2:value_1=(int16_t)TIM_GetCounter(TIM2);TIM_SetCounter(TIM2,0);break;//IF�Ƕ�ʱ��2��1.�ɼ��������ļ���ֵ�����档2.����ʱ���ļ���ֵ���㡣
		case 3:value_1=(int16_t)TIM_GetCounter(TIM3);TIM_SetCounter(TIM3,0);break;
		default:value_1=0;
	}
	return value_1;
}

int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);//uint16_t����
	TIM_SetCounter(TIM3, 0);//���㶨ʱ������ֵ
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
