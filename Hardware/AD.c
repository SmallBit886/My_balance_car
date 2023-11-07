#include "sys.h"
/**
  * @brief  ADC1����ת������ɨ��ת��ģʽ
  * @param  
  * @retval 
  * @attention  
  */
void AD_Init(void)
{
	/*****************************����ʱ��******************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*****************************�ṹ�����******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	/****************************ADCCLK��Ƶ��*******************************/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//6��Ƶ��72MHZ/6=12MHZ
	
	/*****************************����ģʽ����******************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*****************************����������ͨ������******************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);	//����������ͨ��
	
	/****************************ADC�ṹ���ʼ��*******************************/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//����ģʽ������ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//���ݶ��룺�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�ⲿ����ת��ѡ���ڲ��������
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//����ת��	//��Ҫ���ϴ���
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//��ɨ��ת��ģʽ
	ADC_InitStructure.ADC_NbrOfChannel = 1;					//ͨ����ĿΪ1~16֮��
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);		//����ADC��Դ
	/*****************************ADC����У׼******************************/
	ADC_ResetCalibration(ADC1);		//��λУ׼
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);		//�ж�У׼�Ƿ���ɣ�׼���´�У׼��
	ADC_StartCalibration(ADC1);		//����У׼
	while (ADC_GetCalibrationStatus(ADC1) == SET);			//��ȡУ׼״̬
}

uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//�����������ת��
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	//��ȡ��־λ״̬��������ת����ɱ�־λ��55.5+12.5=68T/12M=5.6um��
	return ADC_GetConversionValue(ADC1);		//��ȡת���Ľ��
}

/**
  * @brief  ADC1����ת������ɨ��ת��ģʽ
  * @param  
  * @retval 
  * @attention  
  */
#if 0
void AD_Init(void)
{
	/*****************************����ʱ��******************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*****************************�ṹ�����******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	/****************************ADCCLK��Ƶ��*******************************/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//6��Ƶ��72MHZ/6=12MHZ
	
	/*****************************����ģʽ����******************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*****************************����������ͨ������******************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);	//����������ͨ��
	
	/****************************ADC�ṹ���ʼ��*******************************/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//����ģʽ������ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//���ݶ��룺�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�ⲿ����ת��ѡ���ڲ��������
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;		//����ת��	//��Ҫ���ϴ���
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//��ɨ��ת��ģʽ
	ADC_InitStructure.ADC_NbrOfChannel = 1;					//ͨ����ĿΪ1~16֮��
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);		//����ADC��Դ
	/*****************************ADC����У׼******************************/
	ADC_ResetCalibration(ADC1);		//��λУ׼
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);		//�ж�У׼�Ƿ���ɣ�׼���´�У׼��
	ADC_StartCalibration(ADC1);		//����У׼
	while (ADC_GetCalibrationStatus(ADC1) == SET);			//��ȡУ׼״̬
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//����һ���������ת������
}

uint16_t AD_GetValue(void)
{
	return ADC_GetConversionValue(ADC1);		//��ȡת���Ľ��
}
#endif

/**
  * @brief  ADC1����ת������ɨ��ת��ģʽ��ʵ�ֶ�ͨ��ת��
  * @param  
  * @retval 
  * @attention  
  */
#if 0
void AD_Init(void)
{
	/*****************************����ʱ��******************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	/*****************************�ṹ�����******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	/****************************ADCCLK��Ƶ��*******************************/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//6��Ƶ��72MHZ/6=12MHZ
	
	/*****************************����ģʽ����******************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*****************************����������ͨ�����÷ŵ���ȡ������******************************/
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);	//����������ͨ��
	
	/****************************ADC�ṹ���ʼ��*******************************/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//����ģʽ������ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//���ݶ��룺�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�ⲿ����ת��ѡ���ڲ��������
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//����ת��	//��Ҫ���ϴ���
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//��ɨ��ת��ģʽ
	ADC_InitStructure.ADC_NbrOfChannel = 1;					//ͨ����ĿΪ1~16֮��
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);		//����ADC��Դ
	/*****************************ADC����У׼******************************/
	ADC_ResetCalibration(ADC1);		//��λУ׼
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);		//�ж�У׼�Ƿ���ɣ�׼���´�У׼��
	ADC_StartCalibration(ADC1);		//����У׼
	while (ADC_GetCalibrationStatus(ADC1) == SET);			//��ȡУ׼״̬
	
}

uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	/*****************************����������ͨ�����÷ŵ���ȡ������******************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);	//����������ͨ��
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//�����������ת��
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	//��ȡ��־λ״̬��������ת����ɱ�־λ��55.5+12.5=68T/12M=5.6um��
	return ADC_GetConversionValue(ADC1);		//��ȡת���Ľ��
}
#endif

/**
  * @brief  ADC1����ɨ��ģʽ��DMA����ת��ģʽ
  * @param  
  * @retval 
  * @attention  
  */
#if 0
uint16_t AD_Value[5];//�洢��λ��
void AD_DMA_Init(void)
{
	/*****************************����ʱ��******************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/*****************************�ṹ�����******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	/****************************ADCCLK��Ƶ��*******************************/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//6��Ƶ��72MHZ/6=12MHZ
	
	/*****************************����ģʽ����******************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*****************************����������ͨ�����÷ŵ���ȡ������******************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);	//����������ͨ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);	//����������ͨ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);	//����������ͨ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);	//����������ͨ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_55Cycles5);	//����������ͨ��
	/****************************ADC�ṹ���ʼ��*******************************/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//����ģʽ������ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//���ݶ��룺�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�ⲿ����ת��ѡ���ڲ��������
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//����ת��	//��Ҫ���ϴ���
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;			//ɨ��ת��ģʽ
	ADC_InitStructure.ADC_NbrOfChannel = 5;					//ͨ����ĿΪ1~16֮��
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_DMACmd(ADC1,ENABLE);	//ADC����DMAʹ��
	ADC_Cmd(ADC1, ENABLE);		//����ADC��Դ

	/***************************DMA��������********************************/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//����վ�����ʼ��ַ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ�ȣ����֣�ȡDR�Ĵ����ĵ�16λ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�Ƿ�������������
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;//�洢��վ�����ʼ��ַ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ�ȣ��ֽ�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�Ƿ�����������
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//���䷽������վ�㵽�洢��
	DMA_InitStructure.DMA_BufferSize = 5;//��������С����ʵ���Ǵ��������
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//����ģʽ����ʵ�����Ƿ�ʹ���Զ���װ
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//�Ƿ��Ǵ洢�����洢������ʵ�����Ƿ���Ӳ�������������������
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//���ȼ�
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//��ʼ��
	
	DMA_Cmd(DMA1_Channel1,ENABLE);//ʹ��
	/*****************************ADC����У׼******************************/
	ADC_ResetCalibration(ADC1);		//��λУ׼
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);		//�ж�У׼�Ƿ���ɣ�׼���´�У׼��
	ADC_StartCalibration(ADC1);		//����У׼
	while (ADC_GetCalibrationStatus(ADC1) == SET);			//��ȡУ׼״̬
	
}

void  AD_DMA_GetValue(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);//ʧ��
	DMA_SetCurrDataCounter(DMA1_Channel1, 5);//�������������ֵ
	DMA_Cmd(DMA1_Channel1, ENABLE);//ʹ��
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//�����������ת��
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//�ȴ�ת����ɱ�־λ��1
	DMA_ClearFlag(DMA1_FLAG_TC1);//���ת����ɱ�־λ
}
#endif
/**
  * @brief  ADC1����ɨ��ģʽ��DMA����ѭ��ת��ģʽ
  * @param  
  * @retval 
  * @attention  
  */
#if 1
uint16_t AD_Value[5];//�洢��λ��
void AD_DMA_Init(void)
{
	/*****************************����ʱ��******************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/*****************************�ṹ�����******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	/****************************ADCCLK��Ƶ��*******************************/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//6��Ƶ��72MHZ/6=12MHZ
	
	/*****************************����ģʽ����******************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*****************************����������ͨ�����÷ŵ���ȡ������******************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);	//����������ͨ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);	//����������ͨ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);	//����������ͨ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);	//����������ͨ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_55Cycles5);	//����������ͨ��
	/****************************ADC�ṹ���ʼ��*******************************/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//����ģʽ������ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//���ݶ��룺�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�ⲿ����ת��ѡ���ڲ��������
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;		//����ת��	//����Ҫ���ϴ���
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;			//ɨ��ת��ģʽ
	ADC_InitStructure.ADC_NbrOfChannel = 5;					//ͨ����ĿΪ1~16֮��
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_DMACmd(ADC1,ENABLE);	//ADC����DMAʹ��
	ADC_Cmd(ADC1, ENABLE);		//����ADC��Դ

	/***************************DMA��������********************************/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//����վ�����ʼ��ַ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ�ȣ����֣�ȡDR�Ĵ����ĵ�16λ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�Ƿ�������������
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;//�洢��վ�����ʼ��ַ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ�ȣ��ֽ�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�Ƿ�����������
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//���䷽������վ�㵽�洢��
	DMA_InitStructure.DMA_BufferSize = 5;//��������С����ʵ���Ǵ��������
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//����ģʽ��ѭ��ģʽ��ʹ���Զ���װ
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//�Ƿ��Ǵ洢�����洢������ʵ�����Ƿ���Ӳ�������������������
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//���ȼ�
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//��ʼ��
	
	DMA_Cmd(DMA1_Channel1,ENABLE);//ʹ��
	/*****************************ADC����У׼******************************/
	ADC_ResetCalibration(ADC1);		//��λУ׼
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);		//�ж�У׼�Ƿ���ɣ�׼���´�У׼��
	ADC_StartCalibration(ADC1);		//����У׼
	while (ADC_GetCalibrationStatus(ADC1) == SET);			//��ȡУ׼״̬
	/***************************ֱ�ӷ��ڳ�ʼ�����һ��********************************/
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//�����������ת��
}
/*******ֱ�Ӳ���Ҫ��
void  AD_DMA_GetValue(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);//ʧ��
	DMA_SetCurrDataCounter(DMA1_Channel1, 5);//�������������ֵ
	DMA_Cmd(DMA1_Channel1, ENABLE);//ʹ��
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//�ȴ�ת����ɱ�־λ��1
	DMA_ClearFlag(DMA1_FLAG_TC1);//���ת����ɱ�־λ
}*/
#endif
