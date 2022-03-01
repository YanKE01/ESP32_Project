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
 * @brief TCP发送线程
 *
 * @param pvParameters
 */
void TcpSendTask_Entry(void *pvParameters)
{
    EventBits_t bits;
    while (1)
    {
        //无需清除标志位
        bits = xEventGroupWaitBits(s_wifi_event_group,
                                   WIFI_CONNECTED_BIT,
                                   pdFALSE,
                                   pdFALSE,
                                   1);
        
        if ((bits & WIFI_CONNECTED_BIT) && tcpRestart == false)
        {
            //只有在WIFI连接状态下，且Tcp无需重启状态下
            ESP_LOGI(TASK_TAG, "Send Success");
            TcpSendMessage("hello world\n");
        }
        else
        {
            ESP_LOGI(TASK_TAG, "Wait Wifi And Tcp");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief SHT20采集线程
 * 
 * @param pvParameters 
 */
void Sht20RequireTask_Entry(void *pvParameters)
{
    
}

/**
 * @brief 线程启动函数
 *
 */
void Task_Startup()
{
    testSem = xSemaphoreCreateBinary(); //创建二值信号量

    xTaskCreate(&LedTask_Entry, "LED", 1024 * 2, NULL, 1, NULL);
    xTaskCreate(&SemTest_Entry, "SEM", 1024 * 2, NULL, 2, NULL);
    // xTaskCreate(&TcpSendTask_Entry, "Send", 1024 * 2, NULL, 3, NULL);
    // xTaskCreate(&TcpCreateTask_Entry, "Tcp Create", 1024 * 4, NULL, 5, NULL); // TCP连接任务
}