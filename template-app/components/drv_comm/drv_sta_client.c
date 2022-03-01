#include "drv_sta_client.h"

char *TCP_TAG = "TCP";
static int connect_socket = 0;         // 连接socket
static struct sockaddr_in server_addr; // server地址
bool tcpRestart = false;               // TCP重连标志位

int GetSocketErrCode(int socket);
int ShowSocketErr(const char *str, int socket);

/**
 * @brief 创建TCP连接
 *
 * @return esp_err_t
 */
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
        //展示错误代码 关闭socket连接
        ShowSocketErr("create client", connect_socket);
        close(connect_socket);
        return ESP_FAIL;
    }

    //配置连接服务器信息
    server_addr.sin_family = AF_INET;                           // IPV4
    server_addr.sin_port = htons(TCP_PORT);                     // TCP端口
    server_addr.sin_addr.s_addr = inet_addr(TCP_SERVER_ADRESS); // TCP服务器地址
    ESP_LOGI(TCP_TAG, "Connect Server .......");

    //连接服务器
    if (connect(connect_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        ShowSocketErr("connect server", connect_socket);
        close(connect_socket);
        return ESP_FAIL;
    }

    ESP_LOGI(TCP_TAG, "Connect Server Success\n");

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

/**
 * @brief TCP任务创建
 *
 * @param pvParameters
 */
void TcpCreateTask_Entry(void *pvParameters)
{
    int socket_result = 0;
    while (1)
    {
        tcpRestart = false;
        xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY); //一直等待WIFI连接
        //创建client
        socket_result = CreateTcpClient();
        if (socket_result == ESP_FAIL)
        {
            ESP_LOGI(TCP_TAG, "socket create Failed");
            continue; //创建失败后继续创建
        }

        //检查TCP是否需要重新连接
        while (1)
        {
            vTaskDelay(3000 / portTICK_PERIOD_MS);
            if (tcpRestart)
            {
                vTaskDelay(3000 / portTICK_PERIOD_MS);
                ESP_LOGI(TCP_TAG, "restart create tcp client");
                socket_result = CreateTcpClient(); //重新创建TCP
                if (socket_result == ESP_FAIL)
                {
                    ESP_LOGI(TCP_TAG, "restart socket create Failed");
                    continue; //继续创建
                }
                else
                {
                    ESP_LOGI(TCP_TAG, "restart socket create Success");
                    tcpRestart = false; //取消重启标志
                }
            }
        }
    }
}

/**
 * @brief TCP发送消息
 *
 * @param message
 * @param len
 */
void TcpSendMessage(char *message)
{
    send(connect_socket, message, strlen(message), 0); //发送数据
}