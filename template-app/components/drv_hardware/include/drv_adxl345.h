#ifndef _DRV_ADXL345_H
#define _DRV_ADXL345_H

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "stdint.h"
#include "stdbool.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "math.h"
#include "string.h"
#include "drv_i2c.h"


#define DEVICE_ID		0X00 	// 器件ID,0XE5
#define THRESH_TAP		0X1D   	// 敲击阀值寄存器
#define OFSX			0X1E
#define OFSY			0X1F
#define OFSZ			0X20
#define DUR				0X21
#define Latent			0X22
#define Window  		0X23 
#define THRESH_ACT		0X24	// 运动阈值寄存器
#define THRESH_INACT	0X25 	// 静止阈值寄存器
#define TIME_INACT		0X26	// 静止时间			比例1 sec /LSB
#define ACT_INACT_CTL	0X27	// 启用运动/静止检测
#define THRESH_FF		0X28	// 自由下落阈值	建议采用300 mg与600 mg(0x05至0x09)之间的值 比例62.5 mg/LSB
#define TIME_FF			0X29 	// 自由下落时间	建议采用100 ms与350 ms(0x14至0x46)之间的值 比例5ms/LSB
#define TAP_AXES		0X2A  
#define ACT_TAP_STATUS  0X2B 
#define BW_RATE			0X2C 
#define POWER_CTL		0X2D 

#define INT_ENABLE		0X2E		// 设置中断配置
//DATA_READY	SINGLE_TAP		DOUBLE_TAP		Activity	Inactivity 		FREE_FALL Watermark Overrun
//新数据		加速度报警		两次加速度		活动		加速度值小于	自由下落
#define INT_MAP			0X2F
#define INT_SOURCE  	0X30
#define DATA_FORMAT	    0X31
#define DATA_X0			0X32
#define DATA_X1			0X33
#define DATA_Y0			0X34
#define DATA_Y1			0X35
#define DATA_Z0			0X36
#define DATA_Z1			0X37
#define FIFO_CTL		0X38
#define FIFO_STATUS		0X39

#define SensorAdd 0x53
#define ACK_CHECK_EN 0x1  /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0 /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0       /*!< I2C ack value */
#define NACK_VAL 0x1      /*!< I2C nack value */

void ADXL_Init(int i2c_num, int sda_io, int scl_io);
uint8_t Get_AdxlID(int i2c_num);
void Get_AdxlXYZ(int i2c_num);

#endif