#include "drv_bh1750.h"

/**
 * @brief Bh1750初始化 ADDR=LOW
 *
 * @param i2c_num
 * @param sda_io
 * @param scl_io
 */
void Bh1750_Init(int i2c_num, int sda_io, int scl_io)
{
    uint8_t data;
    I2C_Init(i2c_num, sda_io, scl_io);
    data = BH1750_PWR_ON;
    I2C_WriteData(i2c_num, BH1750_SLAVE_ADDR, 0, &data, 1); //启动
    data = BH1750_CON_H;
    I2C_WriteData(i2c_num, BH1750_SLAVE_ADDR, 0, &data, 1); //设置分辨率
}


float BH1750_ReadLightIntensity(int i2c_num)
{
    float lux = 0.0;
    uint8_t sensorData[2] = {0};
    I2C_ReadData(i2c_num,BH1750_SLAVE_ADDR, 0, sensorData, 2);
    lux = (sensorData[0] << 8 | sensorData[1]) / 1.2;
    return lux;
}
