/**
 * @file multi_timer.c
 * @brief 调度器核心实现（溢出安全版）
 */

#include "multi_timer.h"

static struct Timer*  head_handle = NULL;         /**< 任务链表头指针 */
static uint32_t (*_get_timer_tick)(void) = NULL;  /**< 外部 Tick 接口指针 */

void mt_timer_install(uint32_t (*get_tick)(void)) {
    _get_timer_tick = get_tick;
}

void mt_timer_init(struct Timer* handle, void(*timeout_cb)(void* arg), uint32_t timeout, uint32_t repeat) {
    if (handle && _get_timer_tick) {
        handle->timeout_cb = timeout_cb;
        handle->timeout = _get_timer_tick() + timeout;
        handle->repeat = repeat;
    }
}

int mt_timer_start(struct Timer* handle) {
    struct Timer** target = &head_handle;
    while (*target) {
        if (*target == handle) return -1; 
        target = &(*target)->next;
    }
    handle->next = head_handle;
    head_handle = handle;
    return 0;
}

void mt_timer_stop(struct Timer* handle) {
    struct Timer** target = &head_handle;
    while (*target) {
        if (*target == handle) {
            *target = handle->next;
            handle->next = NULL;
            return;
        }
        target = &(*target)->next;
    }
}

void mt_timer_loop(void) {
    struct Timer* target = head_handle;
    uint32_t current_tick = _get_timer_tick();
    
    while (target) {
        /**
         * @brief 核心：处理 uint32_t 计数回绕溢出问题
         * 将差值强转为 int32_t 判断，确保系统在运行 49.7 天后逻辑依然稳健
         */
        if ((int32_t)(current_tick - target->timeout) >= 0) {
            struct Timer* next = target->next; 

            if (target->repeat == 0) {
                mt_timer_stop(target);
            } else {
                target->timeout = current_tick + target->repeat;
            }
            
            if (target->timeout_cb) {
                target->timeout_cb(target->arg);
            }
            target = next;
        } else {
            target = target->next;
        }
    }
}
