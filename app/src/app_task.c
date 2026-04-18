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
    APP_TASK_LCD_REFRESH,   // 【新增】添加 LCD 刷新任务的枚举 ID
    APP_TASK_KEY_SCAN,       // 【新增】添加按键扫描任务的枚举 ID
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
    },
    // 【新增】LCD 刷新任务注册
    [APP_TASK_LCD_REFRESH] = {
        .task_name = "LCD_Refresh_Task",
        .init_func = app_lcd_init,       // 挂载我们写的 Init 接口
        .timer_handle = {
            .timeout = 50,               // 启动后延迟 50ms 触发第一次
            .repeat  = 50,               // 之后每 50ms (即20Hz刷新率) 执行一次 run 函数
            .timeout_cb = app_lcd_task_run, // 挂载我们写的 Run 接口
            .arg = NULL
        }
    },
    [APP_TASK_KEY_SCAN] = {
        .task_name = "Key_Scan_Task",
        .init_func = app_key_init,       // 挂载按键初始化接口
        .timer_handle = {
            .timeout = 10,               // 启动后延迟 10ms 触发
            .repeat  = 10,               // 之后每 10ms 执行一次 (配合 APPKEY_DEBOUNCE_MS)
            .timeout_cb = app_key_task_run, // 挂载我们暴露的执行逻辑
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
