#ifndef _APP_DEBUG_H_
#define _APP_DEBUG_H_

#include "app_config.h" // 引用你的全局配置宏

// 定义日志级别
#define APP_LOG_LEVEL_NONE  0
#define APP_LOG_LEVEL_ERROR 1
#define APP_LOG_LEVEL_INFO  2
#define APP_LOG_LEVEL_DEBUG 3

// 串口打印解耦的关键：通过宏定义将应用层接口映射到当前的驱动/BSP实现上
#if ZXH_EBike
#if (LOG_ENABLE)
    #include "user_uart.h" // 引用底层驱动头文件
    
    // 应用层统一使用的接口
    #define APP_PRINT(...)      user_print(__VA_ARGS__) 
    #define APP_BLOCK_PRINT(...) user_block_print(__VA_ARGS__)
#else
    #define APP_PRINT(...)
    #define APP_BLOCK_PRINT(...)
#endif
#endif

// 带有级别的日志封装示例
#define APP_LOG_I(tag, fmt, ...) \
    APP_PRINT("[%s][INFO]: " fmt "\r\n", tag, ##__VA_ARGS__)

#define APP_LOG_E(tag, fmt, ...) \
    APP_PRINT("[%s][ERROR]: " fmt "\r\n", tag, ##__VA_ARGS__)

#endif
