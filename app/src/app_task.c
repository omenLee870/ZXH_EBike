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

/* --- 静态分配任务句柄，避免占用堆栈 --- */
static struct Timer timer_test;

/**
 * @brief 专用于业务层的硬件外设初始化
 * @details 这里只初始化外挂的设备引脚，不碰 MCU 内核时钟
 */
void app_hardware_init(void) 
{
    ws8300_gpio_remap(test_pin, GPIO_Remap_GPIO);
    ws8300_gpio_init(test_pin, GPIO_Mode_Output);
    ws8300_gpio_set(test_pin);
}

/**
 * @brief LED 翻转回调函数
 */
static void timer_test_cb(void* arg) {
    static uint8_t io_state = 0;
    if(io_state){
        ws8300_gpio_reset(test_pin);
        io_state = 0;
    }else{
        ws8300_gpio_set(test_pin);
        io_state = 1;
    }
}

void app_tasks_init(void) {
    /* 1. 初始化调度适配器 */
    sys_scheduler_init();
    /* 2. 业务层硬件初始化 */
    app_hardware_init();
    /* 3. 测试1ms测试任务 */
    mt_timer_init(&timer_test, timer_test_cb, 50, 500); 
    mt_timer_start(&timer_test);

}

void app_tasks_state_manager(uint8_t power_state) {
    if (power_state) {
    } else {
    }
}
