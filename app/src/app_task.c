/**
 * @file app_task.c
 * @brief 业务应用层任务实现
 */

#include "app.h"
#include "app_task.h"
#include "cpu_gpio.h"
#include "sys_scheduler.h"
#include "multi_timer.h"

#define test_pin GPIO_GPIOA_Pin2

/* 枚举自动管理任务数量 */
typedef enum {
    APP_TASK_MS_TEST = 0,
    APP_TASKS_MAX_NUM
} app_task_id_t;

/* --- 提前声明运行态函数 --- */
static void test_task_init(void);
static void test_task_run(void* arg);

/* --- 具体任务逻辑 --- */
static void test_task_init(void) {
    ws8300_gpio_remap(test_pin, GPIO_Remap_GPIO);
    ws8300_gpio_init(test_pin, GPIO_Mode_Output);
    ws8300_gpio_set(test_pin);
}

static void test_task_run(void* arg) {
    static uint8_t io_state = 0;
    if(io_state){
        ws8300_gpio_reset(test_pin);
        io_state = 0;
    }else{
        ws8300_gpio_set(test_pin);
        io_state = 1;
    }
}

/* --- 静态任务表定义 --- */
static app_task_info_t app_tasks_table[APP_TASKS_MAX_NUM] = {
    [APP_TASK_MS_TEST] = {
        .task_name = "Test_Task",
        .init_func = test_task_init,
        .timer_handle = {
            .timeout = 1000,
            .repeat  = 500,
            .timeout_cb = test_task_run,
            .arg = NULL
        }
    }
};

/* --- 任务管理器初始化 --- */
void app_tasks_init(void) {
    uint8_t i;

    /* 初始化调度适配器 */
    sys_scheduler_init();

    for (i = 0; i < APP_TASKS_MAX_NUM; i++) {
        
        /* 1. 执行硬件或状态的初始化 */
        if (app_tasks_table[i].init_func != NULL) {
            app_tasks_table[i].init_func();
        }

        /* 2. 读取配置，注册并激活定时器任务 */
        if (app_tasks_table[i].timer_handle.timeout_cb != NULL) {
            
            uint32_t delay = app_tasks_table[i].timer_handle.timeout;
            uint32_t period = app_tasks_table[i].timer_handle.repeat;
            
            /* 调用底层 API 格式化句柄 */
            mt_timer_init(&app_tasks_table[i].timer_handle, 
                          app_tasks_table[i].timer_handle.timeout_cb, 
                          delay, 
                          period);
            
            /* 移交底层管理（底层会自动处理 next 链表） */
            mt_timer_start(&app_tasks_table[i].timer_handle);
        }
    }
}

void app_tasks_state_manager(uint8_t power_state) {
    if (power_state) {
        // 开机恢复逻辑
    } else {
        // 关机挂起逻辑
    }
}
