# ESP32-S2模板项目



ESP32-S2模板项目工程，项目内所有驱动均已测试，RTOS需自行实现

相关CmakeLists下的模块路径务必<font color="red">**自行修改**</font>，Vscode工程路径在 .vscode内自行修改

本项目仅在ESP-IDF V4.3下测试，其余环境未测试



有问题提交ISSUES



## 以支持硬件驱动

1. SHT20温湿度传感器

2. ADXL345加速度传感器

3. BH1750光照传感器

4. 通用按键驱动

5. 通用GPIO操作函数

   

## 以支持通讯协议

1. WIFI-STA连接
2. MQTT连接
2. UART串口
2. I2C通讯协议
2. WIFI TCP 客户端
2. 模拟IIC驱动



## 文件夹说明

Doc:存放各类数据手册

Other:存放参考驱动，本项目暂未验证

template-app：存放ESP32项目工程



## 未验证功能

1. WIFI-----STA-------TCP Client
2. WIFI-----STA-------TCP Server



## 项目改动

1. 修正WIFI驱动缺陷

## 开源项目支持

https://github.com/0x1abin/MultiButton

https://blog.csdn.net/cnicfhnui/category_10350929.html

逐飞科技开源支持

