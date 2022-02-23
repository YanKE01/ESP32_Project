/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "nvs_flash.h"
#include "esp_log.h"

/*------USER INCLUDE-----*/
#include "drv_sht20.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "drv_bh1750.h"
/*------SYSTEM TAG-----*/
char *TAG = "TAG";

/*------USER CODE-----*/
void LedFlashTask(void *pvParameters);



void app_main(void)
{
   /*-----------------------------NVS初始化-------------------------------------*/
   esp_err_t ret = nvs_flash_init();
   if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
   {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
   }
   ESP_ERROR_CHECK(ret);

   /*-----------------------------用户代码起始-------------------------------------*/
   Bh1750_Init(I2C_NUM_0, GPIO_NUM_5, GPIO_NUM_4);

   gpio_pad_select_gpio(GPIO_NUM_12);
   gpio_set_direction(GPIO_NUM_12, GPIO_MODE_OUTPUT);

   xTaskCreate(&LedFlashTask,"LED",1024*2,NULL,1,NULL);
}



void LedFlashTask(void *pvParameters)
{
   uint8_t flag=0;

   while(1)
   {
      flag = !flag;
      gpio_set_level(GPIO_NUM_12,flag);
      vTaskDelay(1000/portTICK_PERIOD_MS);
   }
}