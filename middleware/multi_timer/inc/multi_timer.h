/**
 * @file multi_timer.h
 * @brief 轻量级软件定时器内核
 * @details 采用静态链表架构，无需动态内存分配，完美适配 28KB SRAM 环境
 * @note 所有接口已添加 mt_ 前缀，解决与原厂驱动的命名冲突
 */

#ifndef _MULTI_TIMER_H_
#define _MULTI_TIMER_H_

#include <stdint.h>
#include <stddef.h>

/**
 * @brief 定时器任务结构体定义
 */
typedef struct Timer {
    uint32_t timeout;           /**< 下一次触发的目标时间戳 */
    uint32_t repeat;            /**< 重复周期 (ms)，若为 0 则为单次任务 */
    void (*timeout_cb)(void*);  /**< 任务到期后的回调函数指针 */
    void *arg;                  /**< 传递给回调函数的参数指针 */
    struct Timer* next;         /**< 任务链表指针 */
} Timer;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  安装硬件时间戳获取接口
 * @param  get_tick 底层提供毫秒计数的函数指针
 */
void mt_timer_install(uint32_t (*get_tick)(void));

/**
 * @brief  初始化定时器对象
 * @param  handle  定时器结构体指针
 * @param  timeout_cb 到期回调函数
 * @param  timeout 初次触发的延迟时间 (ms)
 * @param  repeat  后续重复触发的周期 (ms)
 */
void mt_timer_init(struct Timer* handle, void(*timeout_cb)(void* arg), uint32_t timeout, uint32_t repeat);

/**
 * @brief  将定时器加入调度队列并启动
 * @param  handle 定时器结构体指针
 * @return 0: 成功; -1: 任务已在队列中
 */
int  mt_timer_start(struct Timer* handle);

/**
 * @brief  从调度队列中移除定时器
 * @param  handle 定时器结构体指针
 */
void mt_timer_stop(struct Timer* handle);

/**
 * @brief  调度器核心轮询引擎
 * @note   需在 main 函数的 while(1) 中非阻塞调用
 */
void mt_timer_loop(void);

#ifdef __cplusplus
}
#endif

#endif /* _MULTI_TIMER_H_ */
