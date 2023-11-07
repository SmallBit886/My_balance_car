
#if KEY 
#include "sys.h"
unsigned int dis_count = 0;//������ʱ
volatile unsigned int distance_cm = 0;//��ǰ����

void TIM4Init(void)
{	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);		//72 000 000 / 72 = 1 000 000 = 1us
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
}

//�ⲿ�ж����� ������,����û�õ��жϼ�����ʽ
void UltraSound_Init(void)	
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);

	//����һ��GPIO�Ľṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = Echo_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//����GPIO�˿��ٶ�
	GPIO_Init(GPIOB , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//����GPIO�˿��ٶ�
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	//����һ���ⲿ�ж���صĽṹ��
	EXTI_InitTypeDef EXTI_InitStructure;	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB , GPIO_PinSource6);	//��GPIO�����ó��ⲿ�ж� 
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;		//����Ӧ��GPIO�����ӵ��ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//�ж��¼����ͣ��½���
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//ѡ��ģʽ���ж���
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;		//ʹ�ܸ��ж�
	EXTI_Init(&EXTI_InitStructure);		//�����úõĲ���д��Ĵ���
	
	//����һ���жϵĽṹ��
	NVIC_InitTypeDef NVIC_InitStructure; 		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //�׼�Ϊ0������Ƕ��
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//�����ȼ�0�����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	//�����ȼ������
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ�ܸ�ģ���ж�
	NVIC_Init(&NVIC_InitStructure);		//�жϳ�ʼ�������ṹ�嶨�������ִ��
	
	TIM4Init();		//��ʼ��
	Trig_Reset;		//���͵�ƽ
}

//��ʱ�ķ�ʽ��ȡ����ֵ
void GetDistanceDelay(void)
{
		//���ʹ����ź�
		Trig_Set;
		TIM_SetCounter(TIM4, 0);
		TIM_Cmd(TIM4, ENABLE);//�򿪶�ʱ��
		while(TIM_GetCounter(TIM4) < 11);//��ʱ����10us
		Trig_Reset;
		TIM_SetCounter(TIM4, 0);
	
		while(Echo_Read_IO == 0)//�ȴ������ź���ʼλ��
		{
			if(TIM_GetCounter(TIM4) > 60000)//��Ӧʱ�䳬��������Χ 
			{
				distance_cm = 0;//ʧ�ܺ�ͺ���
				TIM_Cmd(TIM4, DISABLE);//�رն�ʱ��
				return;//����ʧ��
			}
		}
		TIM_SetCounter(TIM4, 0);
		while(Echo_Read_IO == 1)//��ʼ���㳤��
		{
			int count = TIM_GetCounter(TIM4);
			if(count > 60000)//��Ӧʱ�䳬��������Χ 
			{
				distance_cm = 0;//ʧ�ܺ�ͺ���
				TIM_Cmd(TIM4, DISABLE);//�رն�ʱ��
				return;//����ʧ��
			}
		}
		dis_count = TIM_GetCounter(TIM4);
		TIM_Cmd(TIM4, DISABLE);//�رն�ʱ��
		distance_cm = (unsigned int)(((long)(dis_count) * 34)/2000);//����340m/s

}
#endif

