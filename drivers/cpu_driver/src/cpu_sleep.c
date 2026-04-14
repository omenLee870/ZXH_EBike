#include "stdint.h"
#include "stdbool.h"
#include "cpu_sleep.h"
#include "cpu_sysctrl.h"


uint32_t sleep_event_field = 0;

void cpu_sleep_event_set(uint8_t event_type)
{
	 __set_PRIMASK(1);
	if(event_type < SLEEP_EVENT_MAX)
    {
        // Set the event in the bit field
        sleep_event_field |= (1 << event_type);
    }
	 __set_PRIMASK(0);
}

void cpu_sleep_event_clear(uint8_t event_type)
{
	 __set_PRIMASK(1);
	if(event_type < SLEEP_EVENT_MAX)
    {
        // clear the event in the bit field
        sleep_event_field &= ~(1 << event_type);
    }
	 __set_PRIMASK(0);
}

uint32_t cpu_sleep_event_get(void)
{
	return sleep_event_field;
}

bool sleep_check(void)
{
	return (cpu_sleep_event_get() == 0);
}





