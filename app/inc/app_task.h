#ifndef _APP_TASK_H_
#define _APP_TASK_H_

#include <stdint.h>
#include "multi_timer.h" 

typedef struct {
    const char* task_name;         /* 任务名称，方便调试 */
    void (*init_func)(void);       /* 任务的初始化逻辑 */
    struct Timer timer_handle;     /* 直接利用底层 handle 存储运行态、周期等信息 */
} app_task_info_t;

void app_tasks_init(void);
void app_tasks_state_manager(uint8_t power_state);

#endif /* _APP_TASK_H_ */
