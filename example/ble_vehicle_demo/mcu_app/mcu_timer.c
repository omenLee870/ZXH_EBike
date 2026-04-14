#include "bsp.h"

/* --- 全局系统心跳 --- */
volatile uint32_t g_sys_tick_ms = 0; 
uint16_t time_1s_count = 0;

void time_init(void)
{
    timer_init_type_def  timer_init_structure;

    timer_init_structure.TIMER_prescaler = __SYSTEM_CLOCK/10000 -1 ; 
    timer_init_structure.TIMER_reload_cnt_val = 10-1;                 

    timer_it_config(timer0,TIMER0_int_cfg_0,ENABLE);
    timer_update_en(timer0,ENABLE);
    timer_init(timer0 ,&timer_init_structure);
    timer_cmd(timer0,ENABLE);
}

/* 仅保留 1ms 中断回调 */
static void time0_1ms_cbk(void)
{
    g_sys_tick_ms++; /* 给我们自己的调度器提供心跳 */
}

/* 提供外部获取 Tick 的接口 */
uint32_t get_sys_tick_ms(void)
{
    return g_sys_tick_ms;
}

/* 定时器0 - 1ms秒中断 */
void TIMER_IRQHandler(void)
{   
	if(timer_get_it_state(timer0) == SET)
	{  
        /* 用于阻塞延时-1ms累加 */
		time0_1ms_cbk();
        
		timer_it_clear(timer0);
	}
}
