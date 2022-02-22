#ifndef _DRV_BH1750_H
#define _DRV_BH1750_H

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "stdint.h"
#include "stdbool.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "math.h"
#include "string.h"
#include "drv_i2c.h"

#define BH1750_PWR_DOWN     0x00 // 关闭模块
#define BH1750_PWR_ON       0x01 // 打开模块等待测量指令
#define BH1750_SLAVE_ADDR   0x23 // 从机地址
#define BH1750_CON_H        0x10 // 连续高分辨率模式，1lx，120ms


float BH1750_ReadLightIntensity(int i2c_num);
void Bh1750_Init(int i2c_num, int sda_io, int scl_io);



#endif