/**
 * @file app_key.h
 * @brief 按键应用逻辑层 (支持长短按、掩码机制、基于事件驱动)
 */
#ifndef _APP_KEY_H_
#define _APP_KEY_H_

#include <stdint.h>
#include <stdbool.h>

// ====================== 1. 配置参数 ======================
#define APPKEY_DEBOUNCE_MS       20     // 软件去抖时间 (ms)
#define APPKEY_LONG_PRESS_MS     1500   // 长按触发阈值 (ms)
#define APPKEY_VOLT_TOLERANCE    150    // ADC电压识别容差 (±mV)

// ====================== 2. 键值与掩码定义 ======================
typedef enum {
    KEY_ID_NONE  = -1,
    KEY_ID_ADD   = 0,  // +键 对应 bit0
    KEY_ID_MINUS = 1,  // -键 对应 bit1
    KEY_ID_LIGHT = 2,  // 大灯键 对应 bit2
    KEY_ID_POWER = 3,  // 电源键 对应 bit3
    KEY_ID_MAX
} AppKey_Id_t;

// 将 ID 转换为掩码 (兼容你参考代码的 mask 思想)
#define KEY_MASK(id)  (1UL << (id))

// ====================== 3. 按键事件定义 ======================
typedef enum {
    APPKEY_EVENT_NONE = 0,
    APPKEY_EVENT_DOWN,       // 按下瞬间
    APPKEY_EVENT_UP,         // 抬起瞬间
    APPKEY_EVENT_CLICK,      // 短按(单击)释放
    APPKEY_EVENT_LONGPRESS   // 长按达到阈值
} AppKey_EventType_t;

// 按键事件结构体 (类似于系统的消息队列元素)
typedef struct {
    bool                valid;      // 1: 有未处理事件
    AppKey_EventType_t  type;       // 事件类型
    uint32_t            key_mask;   // 产生事件的按键掩码
} AppKey_Event_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化按键服务模块 (仅初始化硬件和状态)
 */
void app_key_init(void);

/**
 * @brief 按键扫描任务执行函数 (供 app_task 周期性调度)
 * @param arg 回调参数 (预留)
 */
void app_key_task_run(void* arg);

/**
 * @brief 获取按键事件 (主循环业务调用)
 * @param out_event 用于接收事件的指针
 * @return bool true=有事件发生，false=无事件
 */
bool app_key_get_event(AppKey_Event_t *out_event);

#ifdef __cplusplus
}
#endif

#endif /* _APP_KEY_H_ */
