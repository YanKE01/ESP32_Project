#include "drv_sta_client.h"

char *TCP_TAG = "TCP";
static int connect_socket = 0; // 连接socket

int GetSocketErrCode(int socket);
int ShowSocketErr(const char *str, int socket);

esp_err_t CreateTcpClient()
{
    ESP_LOGI(TCP_TAG, "Connect IP:%s,Port:%d", TCP_SERVER_ADRESS, TCP_PORT);

    /**
     * 新建Socket
     * 形参1: AF_INET:IPV4网络通讯 AF_INET6:IPV6网络通讯
     * 形参2: SOCK_STREAM:字节流套接字
     * 形参3: 0:TCP传输协议
     * */
    connect_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect_socket < 0)
    {
        ShowSocketErr("create client", connect_socket);
        close(connect_socket);
    }

    return ESP_OK;
}

/**
 * @brief 获取Socket操作代码
 *
 * @param socket
 * @return int
 */
int GetSocketErrCode(int socket)
{
    int result;
    u32_t optlen = sizeof(int);
    int err = getsockopt(socket, SOL_SOCKET, SO_ERROR, &result, &optlen);
    if (err == -1)
    {
        ESP_LOGE(TCP_TAG, "socket error code:%d", err);
        ESP_LOGE(TCP_TAG, "socket error code:%s", strerror(err));
        return -1;
    }
    return result;
}

/**
 * @brief socket连接错误
 *
 * @param str
 * @param socket
 * @return int
 */
int ShowSocketErr(const char *str, int socket)
{
    int err = GetSocketErrCode(socket);
    if (err != 0)
    {
        ESP_LOGW(TCP_TAG, "%s socket error reason %d %s", str, err, strerror(err));
    }
    return err;
}