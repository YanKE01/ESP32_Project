#ifndef _DRV_WIFI_H
#define _DRV_WIFI_H

#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "drv_sta_client.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

extern int MAX_Reconnect;
extern EventGroupHandle_t s_wifi_event_group; // WIFI事件组

void Wifi_Init();       //动态分配IP地址
void Wifi_InitStatic(); //静态分配IP地址

#endif