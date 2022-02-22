#ifndef _DRV_I2C_H
#define _DRV_I2C_H


#define I2C_WRITE_MODE 0
#define I2C_READ_MODE 1
#define ACK 0                       /*!< I2C ack value */
#define NACK 1                      /*!< I2C nack value */
#define ACK_CHECK_ENABLE 1
#define ACK_CHECK_DISABLE 0

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"

esp_err_t I2C_Init(int i2c_num, int sda_io, int scl_io);
esp_err_t I2C_WriteData(int i2c_num, uint8_t slaveAddr, uint8_t regAddr, uint8_t *pData, uint8_t len);
esp_err_t I2C_ReadData(int i2c_num, uint8_t slaveAddr, uint8_t regAddr, uint8_t *pData, uint8_t len);


#endif