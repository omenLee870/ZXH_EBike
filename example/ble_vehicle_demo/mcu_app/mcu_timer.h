#ifndef __MCU_TIME_H
#define __MCU_TIME_H

#include "cpu.h"
#include <stdint.h>

#define SCAN_CARD_INTERVAL_TIME   50

extern void beep_time_init(uint32_t freq,uint8_t duty);

void time_init(void);

/**
 * @brief 提供给外部调度框架的 Tick 获取接口
 * @return 系统毫秒级时间戳
 */
uint32_t get_sys_tick_ms(void);

#endif



