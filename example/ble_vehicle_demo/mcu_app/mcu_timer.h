#ifndef __MCU_TIME_H
#define __MCU_TIME_H

#include "cpu.h"

#define SCAN_CARD_INTERVAL_TIME   50

extern void beep_time_init(uint32_t freq,uint8_t duty);

void time_init(void);

#endif



