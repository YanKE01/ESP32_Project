#ifndef _DRV_SIMIIC_H
#define _DRV_SIMIIC_H

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "drv_gpio.h"


#define SEEKFREE_SCL    GPIO_NUM_5                           //定义SCL引脚  可任意更改为其他IO
#define SEEKFREE_SDA    GPIO_NUM_6                           //定义SDA引脚  可任意更改为其他IO

typedef enum IIC       //DAC模块
{
    SIMIIC,
    SCCB
} IIC_type;




#endif