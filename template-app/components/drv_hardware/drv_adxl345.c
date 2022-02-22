#include "drv_adxl345.h"

uint8_t Get_AdxlID(int i2c_num);
void ADXL345_WR_Reg(int i2c_num, uint8_t addr, uint8_t val);

static const char *TestTag = "test";
/*
* SCL:IO6  SDA:IO7
* I2C_NUM_1
*/
void ADXL_Init(int i2c_num, int sda_io, int scl_io)
{
    I2C_Init(i2c_num, sda_io, scl_io);

    //设备自检
    if (Get_AdxlID(i2c_num) == 0xe5)
    {
        ADXL345_WR_Reg(i2c_num, INT_ENABLE, 0x00);  //关闭中断
        ADXL345_WR_Reg(i2c_num, DATA_FORMAT, 0x0B); //13位全分辨率
        ADXL345_WR_Reg(i2c_num, BW_RATE, 0x1A);     //正常模式 1600hz采样
        ADXL345_WR_Reg(i2c_num, POWER_CTL, 0x08);   // 自动休眠模式 休眠时以8HZ频率采样 INT_ENABLE 38 08
        ADXL345_WR_Reg(i2c_num,INT_ENABLE,0x14);
    }
}

/*
* 与drv_i2c略有不一致，故重写
*/
uint8_t Get_AdxlID(int i2c_num)
{
    uint8_t data = 0;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, SensorAdd << 1 | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, DEVICE_ID, ACK_CHECK_EN);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SensorAdd << 1) | I2C_MASTER_READ, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, &data, NACK_VAL);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return data;
}

uint8_t ADXL345_RD_Reg(int i2c_num, uint8_t addr)
{
    uint8_t data = 0;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, SensorAdd << 1 | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, addr, ACK_CHECK_EN);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, SensorAdd << 1 | I2C_MASTER_READ, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, &data, NACK_VAL);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return data;
}

void ADXL345_WR_Reg(int i2c_num, uint8_t addr, uint8_t val)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, SensorAdd << 1 | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, addr, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, val, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
}

void Get_AdxlXYZ(int i2c_num)
{
    short x, y, z;
    uint8_t buf[6];
    for (int i = 0; i < 6; i++)
    {
        buf[i] = ADXL345_RD_Reg(i2c_num, 0x32 + i);
    }

    x = (short)(((uint16_t)buf[1] << 8) + buf[0]);
    y = (short)(((uint16_t)buf[3] << 8) + buf[2]);
    z = (short)(((uint16_t)buf[5] << 8) + buf[4]);

    ESP_LOGI(TestTag, "XYZ:%d\t %d\t %d", x, y, z);
}