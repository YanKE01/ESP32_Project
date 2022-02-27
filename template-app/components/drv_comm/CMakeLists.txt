idf_component_register(SRCS "drv_uart.c" "drv_mqtt.c" "drv_wifi.c" "drv_i2c.c" "drv_comm.c" 
                    "D:/IDE/ESP32_IDF/Main/components/mqtt/esp-mqtt/mqtt_client.c" 
                    "D:/IDE/ESP32_IDF/Main/components/mqtt/esp-mqtt/lib/mqtt_msg.c" 
                    "D:/IDE/ESP32_IDF/Main/components/mqtt/esp-mqtt/lib/mqtt_outbox.c" 
                    "D:/IDE/ESP32_IDF/Main/components/mqtt/esp-mqtt/lib/platform_esp32_idf.c"
                    INCLUDE_DIRS "include" "D:/IDE/ESP32_IDF/Main/components/mqtt/esp-mqtt/include"
                    PRIV_INCLUDE_DIRS  "D:/IDE/ESP32_IDF/Main/components/mqtt/esp-mqtt/lib/include"
                    REQUIRES lwip nghttp mbedtls tcp_transport
                    )
