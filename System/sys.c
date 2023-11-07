#include "sys.h"

//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}

//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

/**
  * @brief  �ж����ȼ�
  * @param  
  * @retval 
  * @attention  ����MPU6050�ⲿ�ж�Ϊ��ߣ���ΪҪʱ�̲ɼ�����ά��ƽ�⳵
  */
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//4����ռ,4����Ӧ��
	
	//����MPU6050�ⲿ�ж�
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;//�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//ʹ�ܸ�ͨ���ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//�����ȼ�0�����
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;//�����ȼ������
	NVIC_Init(&NVIC_InitStruct);//��ʼ��
	
	//���ô����ж�
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;//�����ж�
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//ʹ�ܸ�ͨ���ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;//�����ȼ�2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;//�����ȼ�2
	NVIC_Init(&NVIC_InitStruct);	//��ʼ��
}
