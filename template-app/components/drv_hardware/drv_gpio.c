#include "drv_gpio.h"

/**
 * @brief GPIO初始化
 * 
 * @param gpio_num 
 * @param mode 
 */
void GpioInit(gpio_num_t gpio_num,gpio_mode_t mode)
{
    gpio_pad_select_gpio(gpio_num);
    gpio_set_direction(gpio_num,mode);
}
