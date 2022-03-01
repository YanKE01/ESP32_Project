#ifndef _DRV_STA_CLIENT_H
#define _DRV_STA_CLIENT_H

#define TCP_PORT 9527                     // 电脑端的服务器监听端口
#define TCP_SERVER_ADRESS "192.168.0.252" // 作为client，要连接TCP服务器地址

#include "esp_err.h"
#include <sys/socket.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "drv_wifi.h"

#endif