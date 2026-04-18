#include "app.h"
#include "app_key.h"
#include "bsp_adc_key.h"

// ====================== 电压阈值配置 ======================
static const int32_t KEY_VOLTAGE_MAP[KEY_ID_MAX] = {
    [KEY_ID_ADD]   = 400,   // 比如 +键 分压后是 0.4V
    [KEY_ID_MINUS] = 1000,  // 比如 -键 分压后是 1.0V
    [KEY_ID_LIGHT] = 1600,  // 比如 灯键 分压后是 1.6V
    [KEY_ID_POWER] = 2200   // 比如 电源键 分压后是 2.2V
};

// ====================== 内部状态与变量 ======================
static AppKey_Event_t s_event_out;      // 供外部获取的事件缓存

// 按键状态机枚举
typedef enum {
    KEY_STATE_IDLE,
    KEY_STATE_DEBOUNCE,
    KEY_STATE_PRESSED,
    KEY_STATE_WAIT_RELEASE
} KeyState_t;

static KeyState_t s_fsm_state = KEY_STATE_IDLE;
static AppKey_Id_t s_current_key_id = KEY_ID_NONE;
static uint32_t s_press_time_ms = 0;    // 按下时长统计

// ====================== 私有函数 ======================

// 根据电压查找对应按键 ID
static AppKey_Id_t map_voltage_to_key(int32_t voltage_mv) {
    if (voltage_mv > 3000) return KEY_ID_NONE; // 假设高于3V视为未按下(上拉)

    for (int i = 0; i < KEY_ID_MAX; i++) {
        int32_t target = KEY_VOLTAGE_MAP[i];
        if ((voltage_mv >= target - APPKEY_VOLT_TOLERANCE) && 
            (voltage_mv <= target + APPKEY_VOLT_TOLERANCE)) {
            return (AppKey_Id_t)i;
        }
    }
    return KEY_ID_NONE;
}

// 抛出事件到缓存
static void dispatch_event(AppKey_EventType_t type, AppKey_Id_t id) {
    if (id == KEY_ID_NONE) return;
    
    s_event_out.type = type;
    s_event_out.key_mask = KEY_MASK(id);
    s_event_out.valid = true;
}


// ====================== 开放接口 ======================

void app_key_init(void) {
    // 1. 初始化底层硬件
    bsp_adc_key_init();
    
    // 2. 清理事件状态
    s_event_out.valid = false;
    s_fsm_state = KEY_STATE_IDLE;
}

void app_key_task_run(void* arg) {
    int32_t vol = bsp_adc_key_get_voltage();
    AppKey_Id_t raw_id = map_voltage_to_key(vol);

    switch (s_fsm_state) {
        case KEY_STATE_IDLE:
            if (raw_id != KEY_ID_NONE) {
                s_current_key_id = raw_id;
                s_press_time_ms = 0;
                s_fsm_state = KEY_STATE_DEBOUNCE;
            }
            break;

        case KEY_STATE_DEBOUNCE:
            s_press_time_ms += 10; // 定时器周期 10ms
            if (raw_id == s_current_key_id) {
                if (s_press_time_ms >= APPKEY_DEBOUNCE_MS) {
                    // 去抖成功，确认为按下
                    s_fsm_state = KEY_STATE_PRESSED;
                    dispatch_event(APPKEY_EVENT_DOWN, s_current_key_id);
                }
            } else {
                // 属于抖动或误触，返回空闲
                s_fsm_state = KEY_STATE_IDLE; 
            }
            break;

        case KEY_STATE_PRESSED:
            if (raw_id == s_current_key_id) {
                s_press_time_ms += 10;
                // 检测长按
                if (s_press_time_ms >= APPKEY_LONG_PRESS_MS) {
                    dispatch_event(APPKEY_EVENT_LONGPRESS, s_current_key_id);
                    s_fsm_state = KEY_STATE_WAIT_RELEASE; // 长按触发后，等待释放
                }
            } else if (raw_id == KEY_ID_NONE) {
                // 没到长按时间就抬起了，判定为短按 Click
                dispatch_event(APPKEY_EVENT_CLICK, s_current_key_id);
                dispatch_event(APPKEY_EVENT_UP, s_current_key_id);
                s_fsm_state = KEY_STATE_IDLE;
            }
            break;

        case KEY_STATE_WAIT_RELEASE:
            // 等待手松开，防止长按后产生多余的 Click 事件
            if (raw_id == KEY_ID_NONE) {
                dispatch_event(APPKEY_EVENT_UP, s_current_key_id);
                s_fsm_state = KEY_STATE_IDLE;
            }
            break;
    }
}

bool app_key_get_event(AppKey_Event_t *out_event) {
    if (s_event_out.valid && out_event != NULL) {
        // 拷贝事件给用户
        *out_event = s_event_out;
        // 清除标志，表示已被消费
        s_event_out.valid = false;
        return true;
    }
    return false;
}
