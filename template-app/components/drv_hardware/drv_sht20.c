#include "drv_sht20.h"

/**
 * @brief SHT20初始化
 *
 * @param i2c_num
 * @param sda_io
 * @param scl_io
 */
void Sht20_Init(int i2c_num, int sda_io, int scl_io)
{
    I2C_Init(i2c_num, sda_io, scl_io);
}

/**
 * @brief SHT20获取温度
 *
 * @return float
 */
float Sht20_GetTemper(int i2c_num)
{
    uint8_t temperature[3] = {0};
    uint16_t result = 0;

    I2C_WriteData(i2c_num, SHT20_ADDRESS, SHT20_T_NO_HOLD, 0, 0); //地址+指令
    vTaskDelay(40 / portTICK_PERIOD_MS);
    I2C_ReadData(i2c_num, SHT20_ADDRESS, 0, 0, 0);
    vTaskDelay(40 / portTICK_PERIOD_MS);
    I2C_ReadData(i2c_num, SHT20_ADDRESS, 0, temperature, 3);
    result = ((uint16_t)temperature[0] << 8) + ((uint16_t)(temperature[1] & 0xFC));
    if (((temperature[1] >> 1) & 0x00) != 0)
    {
        return 0;
    }
    return (175.72 * (result / 65536.0) - 46.85);
}

/**
 * @brief SHT20获取湿度
 *
 * @return float
 */
float Sht20_GetHumidity(int i2c_num)
{
    uint8_t humidity[3] = {0};
    uint16_t result = 0;

    I2C_WriteData(i2c_num, SHT20_ADDRESS, SHT20_RH_NO_HOLD, 0, 0); //地址+指令
    vTaskDelay(20 / portTICK_PERIOD_MS);
    I2C_ReadData(i2c_num, SHT20_ADDRESS, 0, 0, 0);
    vTaskDelay(20 / portTICK_PERIOD_MS);
    I2C_ReadData(i2c_num, SHT20_ADDRESS, 0, humidity, 3);
    result = ((uint16_t)humidity[0] << 8) + ((uint16_t)(humidity[1] & 0xFC));

    /* Check the measurement type by bit 1 (temperature=0, humidity=1)*/
    if (((humidity[1] >> 1) & 0x01) != 1)
    {
        return 0;
    }

    return (125.0 * (result / 65536.0) - 6.0);
}