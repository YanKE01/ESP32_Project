#ifndef _DRV_STA_SERVER_H
#define _DRV_STA_SERVER_H

#define TCP_SERVER_PORT 8896

#include "esp_err.h"
#include <sys/socket.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "drv_wifi.h"
#include "drv_sta_client.h"

#define MAX_Client 10  //控制服务器的最大连接数量

typedef struct MySocketInfo
{
    int socketCon;        // 套接字描述符
    char *ipaddr;         // 客户端IP地址
    uint16_t port;        // 客户端端口号
    TaskHandle_t xHandle; //任务句柄

} _MySocketInfo;

struct MySocketInfo clientSockArryInfo[MAX_Client]; //客户端的sockfd的保存

void TcpServerCreateTask_Entry(void *pvParameters);
esp_err_t TcpServerCreateTest();
void TcpServerAcceptTask_Entry(void *pvParameters);
void ReceiveClientInfoTask_Entry(void *clientinfo);//


#endif