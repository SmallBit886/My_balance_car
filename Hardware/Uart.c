#include "stm32f10x.h"           
#include "Uart.h"
#include <stdio.h>
#include <stdarg.h>

u8 USART1_RX_BUF[64];     //接收缓冲,最大64个字节.
//接收状态
//bit7，接收完成标志
//bit6，接收到0x0d
//bit5~0，接收到的有效字节数目
u16 USART1_RX_STA=0;       //接收状态标记

void Usart1_Init(u32 bound)
{
	/*****************************定义结构体变量******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/******************************开启时钟*****************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/*****************************配置引脚模式******************************/
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/******************************配置串口的模式*****************************/
	USART_InitStructure.USART_BaudRate = bound;	//波特率配置
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//控制硬件流控模式，用于MCU通知模块，或者模块通知MCU
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		//同时开启发送和接收
	USART_InitStructure.USART_Parity = USART_Parity_No;		//无奇偶校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//一个停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//八个字节
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1 , USART_IT_RXNE , ENABLE);//使能接收中断
	
	USART_Cmd(USART1 , ENABLE);//打开串口
	USART_ClearFlag(USART1 , USART_FLAG_TC);//解决第一个数据发送失败的问题

}


//发送八位比特位
void Usart1_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
//发送数组
void Usart1_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Usart1_SendByte(Array[i]);
	}
}
//发送字符串
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
//串口的重定义----printf('aaaa')  putchar('p')
int fputc(int ch, FILE *f)
{
	Usart1_SendByte(ch);
	/*******************************等待串口发送完成****************************/
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return ch;
}
//串口的重定义----scanf()   getchar()
int fgetc(FILE *f)
{
	/***************************等待串口输入数据********************************/
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

//串口中断函数
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{	
		u8 a;//用于接收数据
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收中断(接收到的数据必须是0x0d 0x0a结尾)	
		{  	
			a =USART_ReceiveData(USART1);//读取接收到的数据
			printf("%c",a); //把收到的数据发送回电脑
			
			if((USART1_RX_STA&0x8000)==0)//判断接收标志位最高位是否为1；不是1则表示接收未完成
			{
				if(USART1_RX_STA&0x4000)//判断接收标志位最次高位是否为1；接收到0x0d
				{
					if(a!=0x0a)//换行
					{
						USART1_RX_STA=0;//接受错误，重新开始
					}
					else
					{
						USART1_RX_STA |=0x8000;//接收完成//状态的最高位置1
					}
				}
				else//还没有收到0x0d(回车)
				{
					if(a==0x0d)
					{
						USART1_RX_STA |=0x4000;//接收完成//状态的最高位置1
					}
					else
					{
						USART1_RX_BUF[USART1_RX_STA&0x3FFF]=a;//将接受的数据存放入数组中
						USART1_RX_STA++;//数据长度计数加1
						if(USART1_RX_STA>(64-1))
						{
							USART1_RX_STA=0;//接收数据错误，重新开始
						}
					}
					
				}
			}
		} 
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
