#ifndef __CPU_SLEEP_H
#define __CPU_SLEEP_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <stdint.h>
#include "stdbool.h"
	 
enum SLEEP_EVENT_TYPE{
	UART0_ONGOING,
    TIMER0_ONGOING,
	TIMER1_ONGOING,
	TIMER2_ONGOING,
	TIMER3_ONGOING,
	TIMER4_ONGOING,
	TIMER5_ONGOING,
	BT3_0_ONGOING,
	BT4_0_ONGOING,
	KEY_TSAK_ONGOING,
	SLEEP_EVENT_MAX
};


void cpu_sleep_event_clear(uint8_t event_type);
void cpu_sleep_event_set(uint8_t event_type);
uint32_t cpu_sleep_event_get(void);
bool sleep_check(void);


#ifdef __cplusplus
}
#endif

#endif

