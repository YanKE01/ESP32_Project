#include "drv_sta_server.h"

char *TCP_SERVER_TAG = "TCP";

static int server_socket = 0;                       //服务器socket
static int server_connect_socket = 0;               //服务器accept后的socket
static struct sockaddr_in server_addr, client_addr; // server地址 client地址
static unsigned int socklen = sizeof(client_addr);  // 地址长度

//接收相关
char serverRecvData[256];
int serverRecvLength;

void ReceiveClientInfoTask_Entry(void *clientinfo);

/**
 * @brief 创建TCP SERVER
 *
 * @param isFirstCreateServer
 * @return esp_err_t
 */
esp_err_t CreateTcpServer(bool isFirstCreateServer)
{
    if (isFirstCreateServer)
    {
        //首次创建TCP_TAG
        server_socket = socket(AF_INET, SOCK_STREAM, 0); // IPV4 字节流套字节 TCP协议
        if (server_socket < 0)
        {
            // socket创建失败
            ShowSocketErr("create server fail", server_socket);
            close(server_socket); //关闭SOCKET
            return ESP_FAIL;
        }

        server_addr.sin_family = AF_INET;                // IPV4
        server_addr.sin_port = htons(TCP_SERVER_PORT);   //设置端口
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //允许任意IP连入服务器

        //地址绑定
        if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            // socket创建失败
            ShowSocketErr("bind server fail", server_socket);
            close(server_socket); //关闭SOCKET
            return ESP_FAIL;
        }
    }

    //已创建server 直接listen,将主动连接套字节转变为被连接套字节,支持10个客户端连接
    if (listen(server_socket, 5) < 0)
    {
        ShowSocketErr("server socket listen fail", server_socket);
        close(server_socket); //关闭SOCKET
        return ESP_FAIL;
    }

    // accept返回socketfd用于与客户端连接,对于server而言 读写的操作为server_connect_socket
    server_connect_socket = accept(server_socket, (struct sockaddr *)&client_addr, &socklen);
    if (server_connect_socket < 0)
    {
        ShowSocketErr("accept server fail", server_socket);
        close(server_socket); //关闭SOCKET
        return ESP_FAIL;
    }

    ESP_LOGI(TCP_SERVER_TAG, "tcp server connection established!");

    return ESP_OK;
}

/**
 * @brief 服务器接收数据线程
 *
 * @param pvParameters
 */
// void ServerRecvDataTask_Entry(void *pvParameters)
// {
//     while (1)
//     {
//         memset(serverRecvData, 0x00, sizeof(serverRecvData));
//         serverRecvLength = recv(server_connect_socket, serverRecvData, sizeof(serverRecvData), 0);
//         tcpRestart = false;
//         if (serverRecvLength > 0)
//         {
//             ESP_LOGI(TCP_SERVER_TAG, "recvData: %s", serverRecvData);               //打印接收到的数组
//             send(server_connect_socket, serverRecvData, strlen(serverRecvData), 0); //接收数据回发
//         }
//         else
//         {
//             ShowSocketErr("recv_data", server_connect_socket); //打印错误信息
//             tcpRestart = true;                                 //服务器故障，标记重连
//             vTaskDelete(NULL);
//         }
//     }
//     close(server_socket);
//     close(server_connect_socket);
//     tcpRestart = true; //标记重连
//     vTaskDelete(NULL);
// }

/**
 * @brief TCP SERVER任务创建
 *
 * @param pvParameters
 */
void TcpServerCreateTask_Entry(void *pvParameters)
{
    // int socket_result = 0;
    // while (1)
    // {
    //     tcpRestart = false;                                                                      //该标志在网络断开连接是重置
    //     xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY); //一直等待WIFI连接
    //     //创建server
    //     socket_result = CreateTcpServer(true);
    //     if (socket_result == ESP_FAIL)
    //     {
    //         ESP_LOGI(TCP_SERVER_TAG, "server socket create Failed");
    //         continue; //创建失败后继续创建
    //     }
    //     else
    //     {
    //         // server创建成功
    //         ESP_LOGI(TCP_SERVER_TAG, "server socket create Success");
    //         //创建接收任务
    //         if (pdPASS != xTaskCreate(&TcpServerAcceptTask_Entry, "accpet socket", 4096, NULL, 4, NULL))
    //         {
    //             ESP_LOGI(TCP_SERVER_TAG, "accept socket task create fail!");
    //         }
    //         else
    //         {
    //             ESP_LOGI(TCP_SERVER_TAG, "Server Recv task create succeed!");
    //         }
    //     }

    //     //检查TCP是否需要重新创建
    //     while (1)
    //     {
    //         vTaskDelay(3000 / portTICK_PERIOD_MS);
    //         if (tcpRestart)
    //         {
    //             vTaskDelay(3000 / portTICK_PERIOD_MS);
    //             ESP_LOGI(TCP_SERVER_TAG, "restart create tcp server");
    //             socket_result = CreateTcpServer(false); //重新创建TCP
    //             if (socket_result == ESP_FAIL)
    //             {
    //                 ESP_LOGI(TCP_SERVER_TAG, "restart socket server create Failed");
    //                 continue; //继续创建
    //             }
    //             else
    //             {
    //                 ESP_LOGI(TCP_SERVER_TAG, "restart socket server create Success");
    //                 tcpRestart = false; //取消重启标志
    //                 //创建Recv任务
    //                 if (pdPASS != xTaskCreate(&ServerRecvDataTask_Entry, "recv_data", 4096, NULL, 4, NULL))
    //                 {
    //                     ESP_LOGI(TCP_SERVER_TAG, "Server Recv task create fail!");
    //                 }
    //                 else
    //                 {
    //                     ESP_LOGI(TCP_SERVER_TAG, "Server Recv task create succeed!");
    //                 }
    //             }
    //         }
    //     }
    // }
}

esp_err_t TcpServerCreateTest()
{
    server_socket = socket(AF_INET, SOCK_STREAM, 0); // IPV4 字节流套字节 TCP协议
    if (server_socket < 0)
    {
        // socket创建失败
        ShowSocketErr("create server fail", server_socket);
        close(server_socket); //关闭SOCKET
        return ESP_FAIL;
    }

    server_addr.sin_family = AF_INET;                // IPV4
    server_addr.sin_port = htons(TCP_SERVER_PORT);   //设置端口
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //允许任意IP连入服务器

    //地址绑定
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        // socket创建失败
        ShowSocketErr("bind server fail", server_socket);
        close(server_socket); //关闭SOCKET
        return ESP_FAIL;
    }

    //监听端口，最多10个client在就绪队伍中
    if (listen(server_socket, 10) < 0)
    {
        ShowSocketErr("server socket listen fail", server_socket);
        close(server_socket); //关闭SOCKET
        return ESP_FAIL;
    }
    return ESP_OK;
}

/**
 * @brief 子线程 搜集连接的客户端的信息
 *
 * @param pvParameters
 *
 * 每一个客户端连接都会创建sockfd,接下来就是创建接收线程
 */
void TcpServerAcceptTask_Entry(void *pvParameters)
{
    int count = 0;
    while (1)
    {
        int sockCon = accept(server_socket, (struct sockaddr *)&client_addr, &socklen);
        if (sockCon < 0)
        {
            //对应客户端的sockfd创建失败
            ESP_LOGI(TCP_SERVER_TAG, "socket for client create failed,count:%d", count);
        }
        else
        {
            //对应的sockfd创建成功
            //保存客户端信息
            _MySocketInfo clientInfo;
            clientInfo.socketCon = sockCon;                      //获取当前client的sockfd
            clientInfo.ipaddr = inet_ntoa(client_addr.sin_addr); //获取当前client的ip
            clientInfo.port = client_addr.sin_port;              //获取当前client的端口号
            clientSockArryInfo[count] = clientInfo;              //保存当前client的信息

            xTaskCreate(&ReceiveClientInfoTask_Entry, "accpet socket", 1024 * 2, &clientInfo, 5, &clientInfo.xHandle);
            ESP_LOGI(TCP_SERVER_TAG, "socket for client create success,count:%d", count);
            count++;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief 每一个client来了之后，都会创建对应的接收线程
 *
 * @param clientinfo
 * @return void*
 */
void ReceiveClientInfoTask_Entry(void *clientinfo)
{
    _MySocketInfo _socketInfo = *((_MySocketInfo *)clientinfo);
    while (1)
    {
        memset(serverRecvData, 0x00, sizeof(serverRecvData)); //清空缓存区域
        serverRecvLength = recv(_socketInfo.socketCon, serverRecvData, sizeof(serverRecvData), 0);
        tcpRestart = false;
        if (serverRecvLength > 0)
        {
            ESP_LOGI(TCP_SERVER_TAG, "sockfd:%d,recvData: %s", _socketInfo.socketCon, serverRecvData); //打印接收到的数组
            send(_socketInfo.socketCon, serverRecvData, strlen(serverRecvData), 0);                    //接收数据回发
        }
        else
        {
            // TCP未被接收,应该删除TaskBuf
            ShowSocketErr("recv_data", _socketInfo.socketCon);                                         //打印错误信息
            ESP_LOGI(TCP_SERVER_TAG, "Reomve Task:%d", _socketInfo.socketCon); //打印接收到的数组
            close(_socketInfo.socketCon);
            vTaskDelete(_socketInfo.xHandle);
        }
    }
}
