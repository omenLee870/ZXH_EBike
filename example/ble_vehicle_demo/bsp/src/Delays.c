#include "bsp.h"

uint32_t timer_tick;
uint32_t get_tick(void)
{
	return timer_tick;
}

static uint32_t get_diff_tick(uint32_t cur_tick, uint32_t prior_tick)
{
	if(cur_tick < prior_tick)
	{
		return (cur_tick + (~prior_tick));
	}else
	{
		return (cur_tick - prior_tick);
	}
}
uint8_t is_timeout(uint32_t start_tick, uint32_t interval)
{
	return (get_diff_tick(get_tick(), start_tick) >= interval);
}


//毫秒延时
void mdelay(uint32_t mSec)
{
	uint32_t old_tick;

	old_tick = get_tick();
	while(get_diff_tick(get_tick(), old_tick) < mSec)
	{
        /* 阻塞期间加蓝牙调度 */
		lib_bluetooth_schedule();
	}
}
//秒延时
void secDelay(uint16_t sec)
{
	mdelay((uint32_t)sec*1000);
}


















