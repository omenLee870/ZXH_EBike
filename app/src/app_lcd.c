#include "app.h"
#include "app_lcd.h"
#include "display_service.h" 

// 内部状态机，用于控制LCD开机自检动画等
typedef enum {
    LCD_STATE_INIT = 0,
    LCD_STATE_STARTUP_ANIM, // 开机全显测试
    LCD_STATE_NORMAL,       // 正常显示
    LCD_STATE_POWER_OFF     // 关机息屏
} lcd_work_state_t;

static lcd_work_state_t s_lcd_state = LCD_STATE_INIT;
static uint16_t s_anim_tick = 0; // 动画计时器

void app_lcd_init(void) {
    // 1. 调用 Service 层进行底层引脚和芯片初始化
    display_service_init();
    
    // 2. 状态初始化
    s_lcd_state = LCD_STATE_STARTUP_ANIM;
    s_anim_tick = 0;
    
    // APP_DEBUG("LCD Task Initialized.\r\n");
}

void app_lcd_task_run(void* arg) {
    // 1. 获取电动车全局电源状态 (调用自 vehicle_app.c)
    // 根据电源状态切换 LCD 状态机
    // if (is_cur_vehicle_state(POWER_OFF)) {
    //     s_lcd_state = LCD_STATE_POWER_OFF;
    // } else if (s_lcd_state == LCD_STATE_POWER_OFF) {
    //     // 如果车辆刚开机，重新进入开机动画
    //     s_lcd_state = LCD_STATE_STARTUP_ANIM;
    //     s_anim_tick = 0;
    // }

    // 2. LCD 业务状态机处理
    switch (s_lcd_state) {
        
        case LCD_STATE_STARTUP_ANIM:
            // 开机全显 1.5 秒 (假设该任务 50ms 进一次，30次即 1.5s)
            display_service_set_all(0x0F); // 将显存置位，全部点亮
            display_service_refresh();
            
            s_anim_tick++;
            if (s_anim_tick >= 30) {
                display_service_set_all(0x00); // 闪烁一下（全灭）
                display_service_refresh();
                s_lcd_state = LCD_STATE_NORMAL;
            }
            break;

        case LCD_STATE_NORMAL:
        {
            // --- 读取电动车的具体外设/业务数据 (这里可以接你的其他ADC、传感器任务) ---
            uint8_t current_speed = 35;          // 模拟车速 35km/h
            uint8_t battery_level = 80;          // 模拟电量 80%
            bool is_headlight_on = true;         // 模拟大灯开启
            bool has_motor_fault = false;        // 模拟电机故障

            // --- 更新 Display Service 的逻辑显存 ---
            
            // 1. 更新车速
            display_service_set_speed(current_speed);
            
            // 2. 更新电量 (简单分段映射到你的电池格数)
            if (battery_level > 75) {
                display_service_set_battery_grid(4);
            } else if (battery_level > 50) {
                display_service_set_battery_grid(3);
            } else if (battery_level > 25) {
                display_service_set_battery_grid(2);
            } else {
                display_service_set_battery_grid(1);
            }
            
            // 3. 更新图标
            display_service_set_icon(ICON_HEADLIGHT, is_headlight_on);
            display_service_set_icon(ICON_FAULT_WARNING, has_motor_fault);

            // --- 统一将显存推给底层硬件 (防闪烁) ---
            display_service_refresh();
            break;
        }
            
        case LCD_STATE_POWER_OFF:
            // 关机状态下，清空屏幕并停止刷新
            display_service_set_all(0x00);
            display_service_refresh();
            // 也可以在这里调用 vk1621 的 Enter_PowerOff() 降低功耗
            break;
            
        default:
            break;
    }
}
