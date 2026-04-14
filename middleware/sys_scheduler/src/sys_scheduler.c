/**
 * @file sys_scheduler.c
 * @brief 系统调度适配层实现
 * @details 连接 MCU 硬件定时器与 MultiTimer 调度内核
 */

#include "sys_scheduler.h"
#include "multi_timer.h"

/**
 * @brief 引用底层驱动提供的毫秒滴答接口
 * @return 系统运行以来的总毫秒数
 */
extern uint32_t get_sys_tick_ms(void); 

void sys_scheduler_init(void) {
    /* 向内核注册硬件时间戳函数 */
    mt_timer_install(get_sys_tick_ms);
}

void sys_scheduler_run(void) {
    /* 运行调度核心引擎 */
    mt_timer_loop();
}
