/**
 * @file display_service.h
 * @brief 显示服务层，负责管理显存和逻辑映射
 */
#ifndef _DISPLAY_SERVICE_H_
#define _DISPLAY_SERVICE_H_

#include <stdint.h>
#include <stdbool.h>

#define DISPLAY_SEG_NUM 32

/* 电动车仪表常用图标枚举定义 */
typedef enum {
    ICON_HEADLIGHT,
    ICON_BLUETOOTH,
    ICON_FAULT_WARNING,
    ICON_BATTERY_FRAME
} DisplayIcon_t;

void display_service_init(void);
void display_service_set_all(uint8_t val);

/* 业务接口 */
void display_service_set_speed(uint8_t speed);
void display_service_set_battery_grid(uint8_t grids);
void display_service_set_icon(DisplayIcon_t icon, bool is_on);

/* 将显存一次性刷入硬件 */
void display_service_refresh(void);

#endif /* _DISPLAY_SERVICE_H_ */
