#include "stm32f10x.h"               
#include "MyI2C.h"
#include "MPU6050_Soft_Reg.h"
#include "MPU6050_Soft.h"

#define MPU6050_ADDRESS		0xD0	//写地址
#define soft 1	//置0为硬件IIC，置1为软件IIC
#if soft
/***********************************************************
*@fuction	:MPU6050_WriteReg
*@brief		:指定地址写一个字节
*@param		:--
*@return	:void
*@author	:--
*@date		:2023-07-24
***********************************************************/

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);	//发送从机地址
	MyI2C_ReceiveAck();					//接收应答；返回应答位；可判断
	MyI2C_SendByte(RegAddress);			//发送指定寄存器里的内容；MPU6050当前地址指针里
	MyI2C_ReceiveAck();					//接收应答
	/****************************
	后两句套起来循环，可以写多个字节
	****************************/
	MyI2C_SendByte(Data);				//发送字节：写入指定寄存器地址下的数据
	MyI2C_ReceiveAck();					//接收应答
	
	MyI2C_Stop();						//终止时序
}

/***********************************************************
*@fuction	:MPU6050_ReadReg
*@brief		:指定地址读一个字节
*@param		:--
*@return	:uint8_t Data
*@author	:--
*@date		:2023-07-24
***********************************************************/

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;		//存储读的字节
	/****************************
	指定地址
	****************************/
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	/****************************
	重新指定读写位
	****************************/
	MyI2C_Start();								//重复起始位
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);		//或上0x01 将写地址 改为 读地址
	MyI2C_ReceiveAck();
	/****************************
	后两句套起来循环，可以读多个字节
	****************************/
	Data = MyI2C_ReceiveByte();					//接收字节
	MyI2C_SendAck(1);							//不给从机应答
	
	MyI2C_Stop();								//终止时序
	
	return Data;
}
#else
/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : MPU6050_WaitEvent
  ** @brief         : 防止死循环程序卡死：超时退出机制
  ** @param         : None
  ** @retval        : None
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : None
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;//可加入错误处理功能：如打印等
		}
	}
}

/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : MPU6050_WriteReg
  ** @brief         : 指定地址，写入寄存器函数
  ** @param         : None
  ** @retval        : None
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : 复合格式指定地址部分
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)//写寄存器
{	

	I2C_GenerateSTART(I2C2, ENABLE);  						//起始条件；(写完就结束，退出函数【非阻塞式的函数】)
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);	//等待EV5事件；【确保上面函数执行到位】
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter); //发送从机地址
					/***（发送和接收数据自带了应答）***/
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);   //等待EV6事件
	
	I2C_SendData(I2C2, RegAddress);								//写入DR；发送数据（自动转到移位寄存器）
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);//等待EV8事件
	
	I2C_SendData(I2C2, Data);									//发送最后一个字节Data
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//等待EV8_2事件（移位完成了，不再接收数据了）：字节已经发送完毕
	
	I2C_GenerateSTOP(I2C2, ENABLE);		//终止时序
}

/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : MPU6050_ReadReg
  ** @brief         : 指定地址，读出寄存器函数
  ** @param         : None
  ** @retval        : None
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : 复合格式指定地址部分
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
uint8_t MPU6050_ReadReg(uint8_t RegAddress)//读寄存器
{
	uint8_t Data;

	I2C_GenerateSTART(I2C2, ENABLE);  //起始条件
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);//等待EV5事件
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter); //发送从机地址
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	//等待EV6事件
	
	I2C_SendData(I2C2, RegAddress);//发送数据
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//等待EV8事件
/******************************************************************************************
	
******************************************************************************************/
	I2C_GenerateSTART(I2C2, ENABLE);//重复起始条件
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);//等待EV5事件
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver); //发送从机地址
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED); //等待EV6事件
/******************************************************************************************
	接收最后一个字节之前，提前配置ACK和STOP
******************************************************************************************/
	I2C_AcknowledgeConfig(I2C2, DISABLE);//提前给ACK置0；（不给应答）
	I2C_GenerateSTOP(I2C2, ENABLE);//提前申请产生终止条件：STOP=1
	
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);//等待EV7事件；（产生后，一个数据已经存到DR中）
	Data = I2C_ReceiveData(I2C2);//读取DR的数据：存到Data中
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);//ACK回复默认的1；（为了方便指定地址收多个字节）
	
	return Data;
}
#endif
/***********************************************************
*@fuction	:MPU6050_Init
*@brief		:初始化
*@param		:--
*@return	:void
*@author	:--
*@date		:2023-07-24
***********************************************************/

void MPU6050_Init(void)		//建立在MyI2C之上
{
#if soft  //软件IIC
	MyI2C_Init();			//继承一下底层
#else //硬件IIC
	/******************************开启时钟*****************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	
	//GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);
	/*****************************定义结构体变量******************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	/****************************配置引脚模式*******************************/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;		//复用开漏输出：硬件I2C协议的设计要求；硬件控制
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/****************************配置IIC2外设*******************************/
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;			//模式
	I2C_InitStructure.I2C_ClockSpeed = 50000;			//50KHZ
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;	//时钟占空比
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;			//默认给应答
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	//STM32作为从机模式时；可以相应7位地址
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;	//STM32作为从机模式时；用于指定STM32自身地址，方便别的主机呼叫（不要和其他设备重复）
	I2C_Init(I2C2, &I2C_InitStructure);
	
	I2C_Cmd(I2C2, ENABLE);	//使能
#endif
	/*********************************
			MPU6050寄存器的配置
	*********************************/
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);//配置电源管理寄存器1
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);//配置电源管理寄存器1
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);//采样率分频：10分频
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);		//数字低通滤波器
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);//陀螺仪
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);//加速度计
}

/***********************************************************
*@fuction	:MPU6050_GetID
*@brief		:读取设备ID号
*@param		:--
*@return	:void
*@author	:--
*@date		:2023-07-24
***********************************************************/

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/**
  ************************************************************************** 
  ** -------------------------------------------------------------------- **
  ** @name          : MPU6050_GetData
  ** @brief         : 获取数据寄存器
  ** @param         : None
  ** @retval        : int16_t *AccX, int16_t *AccY, int16_t *AccZ,
					  int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ
  ** @author        : XRaccoon
  ** -------------------------------------------------------------------- **
  ** @attention     : None
  ** -------------------------------------------------------------------- **
  ************************************************************************** 
**/
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;//高8位；低8位
	/*******************************
	            加速度值
	*******************************/
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);		//读取高八位的值
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);		//读取第八位的值
	*AccX = (DataH << 8) | DataL;		//高八位左移8位，再或上第八位：加速度计X轴的16位数据；（通过指针变量返回）
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	/*******************************
	            陀螺仪值
	*******************************/	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}
