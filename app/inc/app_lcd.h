#ifndef _APP_LCD_H_
#define _APP_LCD_H_

#include <stdint.h>

/**
 * @brief LCD 业务任务的初始化函数
 * @note  由 app_task 框架在系统启动时统一调用
 */
void app_lcd_init(void);

/**
 * @brief LCD 业务任务的周期执行函数
 * @param arg 框架传入的参数（这里设为NULL）
 * @note  由 multi_timer 调度器按周期回调
 */
void app_lcd_task_run(void* arg);

#endif /* _APP_LCD_H_ */
