#include "drv_i2c.h"

/**
  * @brief I2C初始化
  * @param 参数描述
  * @return 返回描述
  * @retval 返回值描述
  */
esp_err_t I2C_Init(int i2c_num, int sda_io, int scl_io)
{
    esp_err_t result = ESP_FAIL;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_io,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = scl_io,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 1000000,
    };

    result = i2c_param_config(i2c_num, &conf);

    if (result != ESP_OK)
    {
        return result;
    }

    return i2c_driver_install(i2c_num, conf.mode, 0, 0, 0);
}

/**
 * @brief I2C写数据
 * 
 * @param i2c_num 
 * @param slaveAddr 
 * @param regAddr 
 * @param pData 
 * @param len 
 * @return esp_err_t 
 */
esp_err_t I2C_WriteData(int i2c_num, uint8_t slaveAddr, uint8_t regAddr, uint8_t *pData, uint8_t len)
{
    esp_err_t result = ESP_FAIL;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (slaveAddr << 1) | I2C_WRITE_MODE, ACK_CHECK_ENABLE);
    if (regAddr != 0)
    {
        i2c_master_write_byte(cmd, regAddr, ACK_CHECK_ENABLE);
    }
    if (len != 0)
    {
        i2c_master_write(cmd, pData, len, ACK_CHECK_ENABLE);
    }
    i2c_master_stop(cmd);
    result = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return result;
}


/**
 * @brief I2C读数据
 * 
 * @param i2c_num 
 * @param slaveAddr 
 * @param regAddr 
 * @param pData 
 * @param len 
 * @return esp_err_t 
 */
esp_err_t I2C_ReadData(int i2c_num, uint8_t slaveAddr, uint8_t regAddr, uint8_t *pData, uint8_t len)
{
    esp_err_t result = ESP_FAIL;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (slaveAddr << 1) | I2C_READ_MODE, ACK_CHECK_ENABLE);
    if (regAddr != 0)
    {
        i2c_master_write_byte(cmd, regAddr, ACK_CHECK_ENABLE);
    }
    if (len != 0)
    {
        i2c_master_read(cmd, pData, len, ACK);
    }
    i2c_master_stop(cmd);
    result = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return result;
}