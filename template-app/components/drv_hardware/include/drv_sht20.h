#ifndef _SHT20_H
#define _SHT20_H

#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "drv_i2c.h"

#define SHT20_ADDRESS 0x40
#define SHT20_HUMIDITY_WAIT_TICK 20
#define SHT20_TEMPERATURE_WAIT_TICK 40

#define SHT20_T_HOLD 0xE3
#define SHT20_RH_HOLD 0xE5
#define SHT20_T_NO_HOLD 0xF3
#define SHT20_RH_NO_HOLD 0xF5
#define SHT20_SOFT_RESET 0xFE

void Sht20_Init(int i2c_num, int sda_io, int scl_io);
float Sht20_GetTemper(int i2c_num);
float Sht20_GetHumidity(int i2c_num);

#endif