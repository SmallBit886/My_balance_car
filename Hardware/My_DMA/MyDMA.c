#include "MyDMA.h"

uint16_t MyDMA_Size;
/**
  * @brief  DMA1_Channel1��ʼ�����洢�����洢��ת��
  * @param  
  * @retval 3������������DMA����
  * @attention  1.�������������0  2.����Դ�д����ź�  3.DMAʹ��
  */
void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
	MyDMA_Size = Size;
	/***************************����ʱ��********************************/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/***************************����ṹ�����********************************/
	DMA_InitTypeDef DMA_InitStructure;
	/***************************DMA��������********************************/
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;//����վ�����ʼ��ַ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//���ݿ�ȣ��ֽ�
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//�Ƿ�����
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;//�洢��վ�����ʼ��ַ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//���ݿ�ȣ��ֽ�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�Ƿ�����
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//���䷽������վ�㵽�洢��
	DMA_InitStructure.DMA_BufferSize = Size;//��������С����ʵ���Ǵ��������
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//����ģʽ����ʵ�����Ƿ�ʹ���Զ���װ
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;//�Ƿ��Ǵ洢�����洢������ʵ�����Ƿ���Ӳ�������������������
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//���ȼ�
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//��ʼ��
	
	DMA_Cmd(DMA1_Channel1,ENABLE);//ʹ��
}
/**
  * @brief  DMA���亯��
  * @param  ����һ�δ���һ��
  * @retval 
  * @attention  ��DMA��ֵҪ������**ʧ��**
  */
void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);//ʧ��
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);//�������������ֵ
	DMA_Cmd(DMA1_Channel1, ENABLE);//ʹ��
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//�ȴ�ת����ɱ�־λ��1
	DMA_ClearFlag(DMA1_FLAG_TC1);//���ת����ɱ�־λ
}
