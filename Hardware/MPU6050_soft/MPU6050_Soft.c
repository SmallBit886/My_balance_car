#include "stm32f10x.h"               
#include "MyI2C.h"
#include "MPU6050_Soft_Reg.h"
#include "MPU6050_Soft.h"

#define MPU6050_ADDRESS		0xD0	//写地址

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
	MyI2C_Init();			//继承一下底层
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
