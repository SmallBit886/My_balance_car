#include "Exti.h"
/**
  * @brief  MPU6050�ⲿ�ж�����
  * @param  
  * @retval 
  * @attention  RCC_APB2Periph_AFIO��������ʱ�ӣ�����Ϊ�ⲿ�ж�
  */
void MPU6050_EXTI_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//����ʱ��
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//����ģʽ
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;//PB5����Ϊ��������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB , GPIO_PinSource5);//��GPIO�����ó��ⲿ�ж� 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;//����Ӧ��GPIO�����ӵ��ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�ж��¼����ͣ��½���
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//ѡ��ģʽ���ж���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ�ܸ��ж�
	EXTI_Init(&EXTI_InitStructure);//�����úõĲ���д��Ĵ���
	
}

