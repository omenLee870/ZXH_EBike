/**
 * @file sys_scheduler.h
 * @brief 系统调度适配层头文件
 */

#ifndef _SYS_SCHEDULER_H_
#define _SYS_SCHEDULER_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化调度适配层
 * @details 负责将底层 MCU 的硬件 Tick 接入调度器内核
 */
void sys_scheduler_init(void);

/**
 * @brief 执行调度引擎轮询
 * @note 放入 main.c 的主循环中执行
 */
void sys_scheduler_run(void);

#ifdef __cplusplus
}
#endif

#endif /* _SYS_SCHEDULER_H_ */
