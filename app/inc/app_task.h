/**
 * @file app_task.h
 * @brief 业务任务管理头文件
 */

#ifndef _APP_TASK_H_
#define _APP_TASK_H_

#include <stdint.h>

/**
 * @brief 应用层任务统一初始化
 * @details 包含 LCD、NFC 等业务任务的注册与启动
 */
void app_tasks_init(void);

/**
 * @brief 业务状态机更新接口
 * @param power_state 1: 开机状态(POWER_ON); 0: 关机状态(POWER_OFF)
 */
void app_tasks_state_manager(uint8_t power_state);

#endif /* _APP_TASK_H_ */
