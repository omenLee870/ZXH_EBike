/**
 * @file app_display_service.c
 * @brief 显示服务中间层
 */
#include "app_display_service.h"
#include "bsp_vk1621.h"

/* * 显存 Buffer，与硬件 RAM 一一对应
 * 数组共 32 个元素，对应 SEG0 ~ SEG31
 * 每个元素的低 4 位对应 COM0 ~ COM3
 */
static uint8_t s_display_buffer[DISPLAY_SEG_NUM];

/* * 数字字模表 0-9 
 * 参考原厂映射逻辑: A~G共7段。
 * 原厂数据为：0xF5, 0x05, 0xD3, 0x97, 0x27, 0xB6, 0xF6, 0x15, 0xF7, 0xB7
 */
static const uint8_t s_num_font[10] = {
    0xF5, 0x05, 0xD3, 0x97, 0x27, 0xB6, 0xF6, 0x15, 0xF7, 0xB7
};

/* --- 内部私有函数：操作特定 SEG 和 COM 的状态 --- */
static void set_dot(uint8_t seg, uint8_t com, bool is_on) {
    if (seg >= DISPLAY_SEG_NUM || com > 3) return;
    
    if (is_on) {
        s_display_buffer[seg] |= (1 << com);
    } else {
        s_display_buffer[seg] &= ~(1 << com);
    }
}

/* --- 内部私有函数：向指定位置写入一个数字 --- */
/*
 * 说明：一个 8 字通常由 2 个相邻的 SEG 管脚控制（每个管脚控制 4 个 COM），共计 8 个点位（7段+1个小数点）。
 * 这里的逻辑需要严格对照你这块液晶屏的规格书（真值表）。
 * 以下逻辑基于：数字占两个SEG（奇数和偶数），字模按低位到高位分布。
 */
static void draw_digit(uint8_t start_seg, uint8_t num) {
    if (start_seg >= DISPLAY_SEG_NUM - 1 || num > 9) return;
    
    uint8_t font_data = s_num_font[num];
    
    // 假设：偶数 SEG 控制字体的高 4 划，奇数 SEG 控制字体的低 4 划
    // 具体拆解方式必须查阅 LCD 屏幕厂家的段码图！这里仅作通用展示：
    s_display_buffer[start_seg]     = (font_data >> 4) & 0x0F; // 取高4位填入 COM0-COM3
    s_display_buffer[start_seg + 1] = font_data & 0x0F;        // 取低4位填入 COM0-COM3
}


void display_service_init(void) {
    bsp_vk1621_init_io();
    
    // VK1621 芯片初始化序列
    bsp_vk1621_write_cmd(VK1621_CMD_OSC_ON);
    bsp_vk1621_write_cmd(VK1621_CMD_DISP_ON);
    bsp_vk1621_write_cmd(VK1621_CMD_COM_1_3__4); // 最常用的 1/3 bias, 4 COM 配置
    bsp_vk1621_write_cmd(VK1621_CMD_BUZZ_OFF);
    bsp_vk1621_write_cmd(VK1621_CMD_TIMER_DIS);
    bsp_vk1621_write_cmd(VK1621_CMD_WDT_DIS);
    
    display_service_set_all(0x00);
    display_service_refresh();
}

void display_service_set_all(uint8_t val) {
    // 因为每次只发送 4bit 数据，所以需要保证 val 只有低四位有效
    val &= 0x0F; 
    for (int i = 0; i < DISPLAY_SEG_NUM; i++) {
        s_display_buffer[i] = val;
    }
}

void display_service_set_speed(uint8_t speed) {
    if (speed > 99) speed = 99; // 限制极值
    
    uint8_t tens = speed / 10;
    uint8_t units = speed % 10;
    
    // 【需修改点】假设：速度十位对应硬件的 SEG2 和 SEG3；个位对应 SEG4 和 SEG5
    if (tens == 0) {
        // 首位为 0 时可以消隐（不显示）
        s_display_buffer[2] = 0x00;
        s_display_buffer[3] = 0x00;
    } else {
        draw_digit(2, tens);
    }
    
    draw_digit(4, units);
}

void display_service_set_battery_grid(uint8_t grids) {
    // 【需修改点】假设：电池框对应 SEG10-COM0, 格子对应 SEG10-COM1 到 COM3
    set_dot(10, 0, true); // 永远点亮电池外框
    
    set_dot(10, 1, (grids >= 1)); // 第1格
    set_dot(10, 2, (grids >= 2)); // 第2格
    set_dot(10, 3, (grids >= 3)); // 第3格
    set_dot(11, 0, (grids >= 4)); // 第4格 (可能跨越到了下一个 SEG)
}

void display_service_set_icon(DisplayIcon_t icon, bool is_on) {
    // 【需修改点】根据你的 LCD 真值表修改以下映射
    switch (icon) {
        case ICON_HEADLIGHT:
            set_dot(15, 2, is_on); // 假设大灯图标在 SEG15, COM2
            break;
        case ICON_BLUETOOTH:
            set_dot(16, 0, is_on);
            break;
        case ICON_FAULT_WARNING:
            set_dot(18, 3, is_on);
            break;
        default:
            break;
    }
}

void display_service_refresh(void) {
    // 一次性将 32 字节的显存全部刷入 VK1621，地址从 0 开始
    bsp_vk1621_write_data(0x00, s_display_buffer, DISPLAY_SEG_NUM);
}
