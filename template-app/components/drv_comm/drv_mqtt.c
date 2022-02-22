#include "drv_mqtt.h"

//MQTT客户端句柄
esp_mqtt_client_handle_t client;

//MQTT客户端连接参数
#define USER_NAME "YANKE"
#define USER_PSWD "123456"
#define MQTT_HOST "101.43.5.182"
#define MQTT_PORT 1883

//函数声明区域
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

/**
 * @brief mqtt client 参数初始化
 * 
 */
void selfMqtt_init()
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .host = MQTT_HOST,
        .port = MQTT_PORT,
        .username = USER_NAME,
        .password = USER_PSWD};
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    ESP_ERROR_CHECK(esp_mqtt_client_start(client));
}

/**
 * @brief mqtt事件句柄
 * @param handler_args 
 * @param base 
 * @param event_id 
 * @param event_data 
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    // 获取MQTT客户端结构体指针
    esp_mqtt_event_handle_t event = event_data;
    // 通过事件ID来分别处理对应的事件
    switch (event->event_id)
    {
    // 建立连接成功
    case MQTT_EVENT_CONNECTED:
        printf("MQTT_client cnnnect to EMQ ok. \n");
        break;
    // 客户端断开连接
    case MQTT_EVENT_DISCONNECTED:
        printf("MQTT_client have disconnected. \n");
        break;
    // 主题订阅成功
    case MQTT_EVENT_SUBSCRIBED:
        printf("mqtt subscribe ok. msg_id = %d \n", event->msg_id);
        break;
    // 取消订阅
    case MQTT_EVENT_UNSUBSCRIBED:
        printf("mqtt unsubscribe ok. msg_id = %d \n", event->msg_id);
        break;
    //  主题发布成功
    case MQTT_EVENT_PUBLISHED:
        printf("mqtt published ok. msg_id = %d \n", event->msg_id);
        break;
    // 已收到订阅的主题消息
    case MQTT_EVENT_DATA:
        printf("mqtt received topic: %.*s \n", event->topic_len, event->topic);
        printf("topic data: %.*s\r\n", event->data_len, event->data);
        break;
    // 客户端遇到错误
    case MQTT_EVENT_ERROR:
        printf("MQTT_EVENT_ERROR \n");
        break;
    default:
        printf("Other event id:%d \n", event->event_id);
        break;
    }
}

/**
 * @brief 消息发送函数
 * 
 * @param topic 
 * @param send_data 
 * @param len 
 */
void selfMqtt_Publsh(char *topic, char *send_data, int len)
{
    esp_mqtt_client_publish(client, topic, send_data, len, 0, 0);
}