#include <stdio.h>
#include "drv_task.h"

const char *TASK_TAG = "TASK";
/**
 * @brief Wifi重连Task，即使Wifi Handler内有重连机制
 *        但是一旦WIFI创建完成后，我们还是需要一直检测WIFI状态
 * 
 */
void Wifi_Reconncet_Task()
{
    EventBits_t bits;

    while (1)
    {
        bits = xEventGroupWaitBits(s_wifi_event_group,
                                   WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                   pdFALSE,
                                   pdFALSE,
                                   100 / portTICK_PERIOD_MS);

        if (bits & WIFI_FAIL_BIT)
        {
            ESP_LOGI(TASK_TAG, "Retry Connect Task");
            esp_wifi_connect();
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void Test_Task()
{
    while (1)
    {
        ESP_LOGI(TASK_TAG, "Test Task Running\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void Task_Startup()
{
    xTaskCreate(Test_Task, "Test", 2048, NULL, 1, NULL);
    xTaskCreate(Wifi_Reconncet_Task, "Reconnect", 2048, NULL, 5, NULL);
}