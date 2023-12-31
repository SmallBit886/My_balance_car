/**
  ******************************************************************************
  **--------------------------------------------------------------------------**
  ** @FileName      : MPU6050_Reg.h
  ** @Description   : None
  **--------------------------------------------------------------------------**
  ** @Author Date   : XRaccoon  2023-07-24
  ** @Version       : v1.0
  **--------------------------------------------------------------------------**
  ** @Function List : None
  **--------------------------------------------------------------------------**
  ** @Attention     : None
  **--------------------------------------------------------------------------**
  ******************************************************************************
  *
 **/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __M_P_U6050__REG_H_
#define __M_P_U6050__REG_H_


#endif
/******************************** END OF FILE *********************************/

#ifndef __MPU6050_REG_H
#define __MPU6050_REG_H

#define	MPU6050_SMPLRT_DIV		0x19
#define	MPU6050_CONFIG			0x1A
#define	MPU6050_GYRO_CONFIG		0x1B
#define	MPU6050_ACCEL_CONFIG	0x1C

#define	MPU6050_ACCEL_XOUT_H	0x3B
#define	MPU6050_ACCEL_XOUT_L	0x3C
#define	MPU6050_ACCEL_YOUT_H	0x3D
#define	MPU6050_ACCEL_YOUT_L	0x3E
#define	MPU6050_ACCEL_ZOUT_H	0x3F
#define	MPU6050_ACCEL_ZOUT_L	0x40
#define	MPU6050_TEMP_OUT_H		0x41
#define	MPU6050_TEMP_OUT_L		0x42
#define	MPU6050_GYRO_XOUT_H		0x43
#define	MPU6050_GYRO_XOUT_L		0x44
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48	

#define	MPU6050_PWR_MGMT_1		0x6B	//电源管理寄存器1
#define	MPU6050_PWR_MGMT_2		0x6C	//电源管理寄存器2
#define	MPU6050_WHO_AM_I		0x75	

#endif
