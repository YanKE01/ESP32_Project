#include <stdio.h>
#include "drv_task.h"

const char *TASK_TAG = "TASK";
SemaphoreHandle_t testSem = NULL; //二值信号量

/**
 * @brief 呼吸灯检测
 *
 * @param pvParameters
 */
void LedTask_Entry(void *pvParameters)
{
    uint8_t status = 0;
    uint8_t count = 0;
    while (1)
    {
        count++;
        status = !status;
        gpio_set_level(GPIO_NUM_12, status);
        if (count >= 10)
        {
            count = 0;
            xSemaphoreGive(testSem);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief 信号量测试线程
 *
 * @param pvParameters
 */
void SemTest_Entry(void *pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(testSem, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGI(TASK_TAG, "Take Sem\n");
        }
    }
}

/**
 * @brief 线程启动函数
 *
 */
void Task_Startup()
{
    testSem = xSemaphoreCreateBinary(); //创建二值信号量
    xTaskCreate(&LedTask_Entry, "LED", 1024*2, NULL, 1, NULL);
    xTaskCreate(&SemTest_Entry, "SEM", 1024*2, NULL, 2, NULL);
}