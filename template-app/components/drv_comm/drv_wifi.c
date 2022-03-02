#include "drv_wifi.h"

const char *WIFI_TAG = "wifi_log";
int MAX_Reconnect = 10; //最大重新连接次数

#define WIFI_SSID "D-403"
#define WIFI_PSWD "hy123456"

EventGroupHandle_t s_wifi_event_group; // WIFI事件组

/**
 * @brief Wifi事件函数，在WIFI初始化时会将事件注册于此
 *
 * @param arg
 * @param event_base
 * @param event_id
 * @param event_data
 */
void Wifi_Event_Handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        // WIFI事件并且WIFI初始化结束走到STA
        esp_wifi_connect(); //连接WIFI
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        esp_wifi_connect();
        ESP_LOGI(WIFI_TAG, "Retry Connecting");
        xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        tcpRestart = true; //需要重连TCP
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        //连接成功
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(WIFI_TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        MAX_Reconnect = 10; //复位
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

/**
 * @brief: WIfic初始化程序
 * @other: IP地址动态分配
 *
 */
void Wifi_Init()
{
    // Step1 创建LWIP核心任务，初始化LWIP相关工作
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());    //创建系统事件任务
    esp_netif_create_default_wifi_sta();                 //网络接口绑定STA(连接他人WIFI)
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); //生成默认配置参数
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));                //创建WIFI驱动任务，初始化WIFI驱动程序

    // Step2 创建事件
    s_wifi_event_group = xEventGroupCreate(); // 创建事件组
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &Wifi_Event_Handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &Wifi_Event_Handler,
                                                        NULL,
                                                        &instance_got_ip));
    // Step3 WIFI配置阶段
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PSWD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false},
        }};
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));               // WIFI配置为STA模式
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config)); //写入WIFI配置

    // Step4 启动Wifi 至此初始化结束 WIFI连接由事件函数操作
    ESP_ERROR_CHECK(esp_wifi_start());

    // Step5 等待WIfi事件运行 获取标志位
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(WIFI_TAG, "WIFI Connect Success");
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI(WIFI_TAG, "WIFI Connect Failed");
    }
}

/**
 * @brief 静态分配IP地址
 *
 */
void Wifi_InitStatic()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());       //创建系统事件任务
    esp_netif_t *sta = esp_netif_create_default_wifi_sta(); //采用STA方式
    esp_netif_dhcpc_stop(sta);                              //禁用DHCP
    esp_netif_ip_info_t ip_info;                            //手动分配IP地址
    ip_info.ip.addr = ipaddr_addr("192.168.10.121");
    ip_info.gw.addr = ipaddr_addr("192.168.10.1");
    ip_info.netmask.addr = ipaddr_addr("255.255.255.0");
    esp_netif_set_ip_info(sta, &ip_info);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); //生成默认配置参数
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));                //创建WIFI驱动任务，初始化WIFI驱动程序

    // Step2 创建事件
    s_wifi_event_group = xEventGroupCreate(); // 创建事件组
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &Wifi_Event_Handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &Wifi_Event_Handler,
                                                        NULL,
                                                        &instance_got_ip));
    // Step3 WIFI配置阶段
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PSWD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false},
        }};
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));               // WIFI配置为STA模式
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config)); //写入WIFI配置

    // Step4 启动Wifi 至此初始化结束 WIFI连接由事件函数操作
    ESP_ERROR_CHECK(esp_wifi_start());

    // Step5 等待WIfi事件运行 获取标志位
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(WIFI_TAG, "WIFI Connect Success");
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI(WIFI_TAG, "WIFI Connect Failed");
    }
}