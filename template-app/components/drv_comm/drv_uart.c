#include "drv_uart.h"



/**
 * @brief TX:IO17 RX:IO16
 * 
 */
void Uart1_Init()
{
    uart_config_t u2_cfg = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(UART_NUM_1, &u2_cfg);
    uart_set_pin(UART_NUM_1, GPIO_NUM_17, GPIO_NUM_16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_1, (2048) * 2, 0, 0, NULL, 0);
}