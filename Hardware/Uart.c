#include "stm32f10x.h"           
#include "Uart.h"
#include <stdio.h>
#include <stdarg.h>

u8 USART1_RX_BUF[64];     //���ջ���,���64���ֽ�.
//����״̬
//bit7��������ɱ�־
//bit6�����յ�0x0d
//bit5~0�����յ�����Ч�ֽ���Ŀ
u16 USART1_RX_STA=0;       //����״̬���

void Usart1_Init(u32 bound)
{
	/*****************************����ṹ�����******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/******************************����ʱ��*****************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/*****************************��������ģʽ******************************/
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/******************************���ô��ڵ�ģʽ*****************************/
	USART_InitStructure.USART_BaudRate = bound;	//����������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//����Ӳ������ģʽ������MCU֪ͨģ�飬����ģ��֪ͨMCU
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		//ͬʱ�������ͺͽ���
	USART_InitStructure.USART_Parity = USART_Parity_No;		//����żУ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//һ��ֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�˸��ֽ�
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1 , USART_IT_RXNE , ENABLE);//ʹ�ܽ����ж�
	
	USART_Cmd(USART1 , ENABLE);//�򿪴���
	USART_ClearFlag(USART1 , USART_FLAG_TC);//�����һ�����ݷ���ʧ�ܵ�����

}


//���Ͱ�λ����λ
void Usart1_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
//��������
void Usart1_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Usart1_SendByte(Array[i]);
	}
}
//�����ַ���
void Usart1_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Usart1_SendByte(String[i]);
	}
}

uint32_t Usart1_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Usart1_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Usart1_SendByte(Number / Usart1_Pow(10, Length - i - 1) % 10 + '0');
	}
}
//���ڵ��ض���----printf('aaaa')  putchar('p')
int fputc(int ch, FILE *f)
{
	Usart1_SendByte(ch);
	/*******************************�ȴ����ڷ������****************************/
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return ch;
}
//���ڵ��ض���----scanf()   getchar()
int fgetc(FILE *f)
{
	/***************************�ȴ�������������********************************/
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return (int )USART_ReceiveData(USART1);
}

void Usart1_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Usart1_SendString(String);
}

//�����жϺ���
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{	
		u8 a;//���ڽ�������
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//�����ж�(���յ������ݱ�����0x0d 0x0a��β)	
		{  	
			a =USART_ReceiveData(USART1);//��ȡ���յ�������
			printf("%c",a); //���յ������ݷ��ͻص���
			
			if((USART1_RX_STA&0x8000)==0)//�жϽ��ձ�־λ���λ�Ƿ�Ϊ1������1���ʾ����δ���
			{
				if(USART1_RX_STA&0x4000)//�жϽ��ձ�־λ��θ�λ�Ƿ�Ϊ1�����յ�0x0d
				{
					if(a!=0x0a)//����
					{
						USART1_RX_STA=0;//���ܴ������¿�ʼ
					}
					else
					{
						USART1_RX_STA |=0x8000;//�������//״̬�����λ��1
					}
				}
				else//��û���յ�0x0d(�س�)
				{
					if(a==0x0d)
					{
						USART1_RX_STA |=0x4000;//�������//״̬�����λ��1
					}
					else
					{
						USART1_RX_BUF[USART1_RX_STA&0x3FFF]=a;//�����ܵ����ݴ����������
						USART1_RX_STA++;//���ݳ��ȼ�����1
						if(USART1_RX_STA>(64-1))
						{
							USART1_RX_STA=0;//�������ݴ������¿�ʼ
						}
					}
					
				}
			}
		} 
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
