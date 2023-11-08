#include "stm32f10x.h"               
#include "MyI2C.h"
#include "MPU6050_Soft_Reg.h"
#include "MPU6050_Soft.h"

#define MPU6050_ADDRESS		0xD0	//д��ַ

/***********************************************************
*@fuction	:MPU6050_WriteReg
*@brief		:ָ����ַдһ���ֽ�
*@param		:--
*@return	:void
*@author	:--
*@date		:2023-07-24
***********************************************************/

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);	//���ʹӻ���ַ
	MyI2C_ReceiveAck();					//����Ӧ�𣻷���Ӧ��λ�����ж�
	MyI2C_SendByte(RegAddress);			//����ָ���Ĵ���������ݣ�MPU6050��ǰ��ַָ����
	MyI2C_ReceiveAck();					//����Ӧ��
	/****************************
	������������ѭ��������д����ֽ�
	****************************/
	MyI2C_SendByte(Data);				//�����ֽڣ�д��ָ���Ĵ�����ַ�µ�����
	MyI2C_ReceiveAck();					//����Ӧ��
	
	MyI2C_Stop();						//��ֹʱ��
}

/***********************************************************
*@fuction	:MPU6050_ReadReg
*@brief		:ָ����ַ��һ���ֽ�
*@param		:--
*@return	:uint8_t Data
*@author	:--
*@date		:2023-07-24
***********************************************************/

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;		//�洢�����ֽ�
	/****************************
	ָ����ַ
	****************************/
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	/****************************
	����ָ����дλ
	****************************/
	MyI2C_Start();								//�ظ���ʼλ
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);		//����0x01 ��д��ַ ��Ϊ ����ַ
	MyI2C_ReceiveAck();
	/****************************
	������������ѭ�������Զ�����ֽ�
	****************************/
	Data = MyI2C_ReceiveByte();					//�����ֽ�
	MyI2C_SendAck(1);							//�����ӻ�Ӧ��
	
	MyI2C_Stop();								//��ֹʱ��
	
	return Data;
}

/***********************************************************
*@fuction	:MPU6050_Init
*@brief		:��ʼ��
*@param		:--
*@return	:void
*@author	:--
*@date		:2023-07-24
***********************************************************/

void MPU6050_Init(void)		//������MyI2C֮��
{
	MyI2C_Init();			//�̳�һ�µײ�
	/*********************************
			MPU6050�Ĵ���������
	*********************************/
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);//���õ�Դ����Ĵ���1
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);//���õ�Դ����Ĵ���1
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);//�����ʷ�Ƶ��10��Ƶ
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);		//���ֵ�ͨ�˲���
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);//������
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);//���ٶȼ�
}

/***********************************************************
*@fuction	:MPU6050_GetID
*@brief		:��ȡ�豸ID��
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
  ** @brief         : ��ȡ���ݼĴ���
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
	uint8_t DataH, DataL;//��8λ����8λ
	/*******************************
	            ���ٶ�ֵ
	*******************************/
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);		//��ȡ�߰�λ��ֵ
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);		//��ȡ�ڰ�λ��ֵ
	*AccX = (DataH << 8) | DataL;		//�߰�λ����8λ���ٻ��ϵڰ�λ�����ٶȼ�X���16λ���ݣ���ͨ��ָ��������أ�
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	/*******************************
	            ������ֵ
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
