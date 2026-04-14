
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "cpu.h"
#include "user_timer.h"
#include "app_config.h"
#include "cpu_sleep.h"
#include "lib_api.h"


typedef struct
{
		uint32_t g_time_tick;
    timer_event_cbk_t timer_cbk[IDX_TIMER_NUM];
}timer_env_t;

const uint32_t timer_it_type_table[] = {
	[IDX_TIMER0] = TIMER0_int_cfg_0,
	[IDX_TIMER1] = TIMER1_int_cfg_0,
	[IDX_TIMER2] = TIMER2_int_cfg_0,
	[IDX_TIMER3] = TIMER3_int_cfg_0,
	[IDX_TIMER4] = TIMER4_int_cfg_0,
	[IDX_TIMER5] = TIMER5_int_cfg_0,
};

const uint32_t timer_sleep_table[] = {
	[IDX_TIMER0] = TIMER0_ONGOING,
	[IDX_TIMER1] = TIMER1_ONGOING,
	[IDX_TIMER2] = TIMER2_ONGOING,
	[IDX_TIMER3] = TIMER3_ONGOING,
	[IDX_TIMER4] = TIMER4_ONGOING,
	[IDX_TIMER5] = TIMER5_ONGOING,
};

/// timer environment
timer_env_t timer_env;

void timer_x_isr(timer_idx_t timer_idx)
{
    timer_it_clear(timer_idx);
		timer_env.g_time_tick ++;
    if(timer_env.timer_cbk[timer_idx] != NULL) timer_env.timer_cbk[timer_idx]();
}

//void TIMER_IRQHandler(void)
//{
//    if (timer_get_it_state(IDX_TIMER0) == SET) //检查指定的TIM中断发生与否:TIM 中断源 
//    {
//        timer_x_isr(IDX_TIMER0);
//    }
//	if (timer_get_it_state(IDX_TIMER1) == SET) //检查指定的TIM中断发生与否:TIM 中断源 
//    {
//        timer_x_isr(IDX_TIMER1);
//    }
//	if (timer_get_it_state(IDX_TIMER2) == SET) //检查指定的TIM中断发生与否:TIM 中断源 
//    {
//        timer_x_isr(IDX_TIMER2);
//    }
//	if (timer_get_it_state(IDX_TIMER3) == SET) //检查指定的TIM中断发生与否:TIM 中断源 
//    {
//        timer_x_isr(IDX_TIMER3);
//    }
//	if (timer_get_it_state(IDX_TIMER4) == SET) //检查指定的TIM中断发生与否:TIM 中断源 
//    {
//        timer_x_isr(IDX_TIMER4);
//    }
//	if (timer_get_it_state(IDX_TIMER5) == SET) //检查指定的TIM中断发生与否:TIM 中断源 
//    {
//        timer_x_isr(IDX_TIMER5);
//    }
//}

/*
* brief: init timer nvic
*/
void timer_nvic_init(void)
{
	NVIC_SetPriority(TIMER_IRQn ,NVIC_EncodePriority(0x05, 0x02,0x00)); 
	NVIC_EnableIRQ(TIMER_IRQn);
}

/*
* brief: timer param config
*/
void user_timer_config(timer_config_t *p_config)
{
	timer_init_type_def timerx; 

	if(p_config == NULL) return;

	timerx.TIMER_prescaler = 16000000/10000 - 1 ;  //分频系数1600 ，频率：16000000/1600 = 10KHz（0.1ms）   
	
    timerx.TIMER_reload_cnt_val = p_config->timer_param.timer_period * 10;
	
    timer_env.timer_cbk[p_config->timer_idx] = p_config->cbk;
	
    timer_it_config(p_config->timer_idx,timer_it_type_table[p_config->timer_idx],ENABLE);    
	timer_update_en(p_config->timer_idx,ENABLE);
    timer_init(p_config->timer_idx ,&timerx);
}

/*
* brief: timer resume
*/
void user_timer_resume(timer_config_t *p_config)
{
	user_timer_config(p_config);
}

/*
* brief: timer init
*/
void user_timer_init(timer_config_t *p_config)
{
  user_timer_config(p_config);
	
	timer_nvic_init();
}

/*
* brief: timer start
*/
void user_timer_start(timer_idx_t id)
{
	timer_cmd(id,ENABLE);
	cpu_sleep_event_set(timer_sleep_table[id]);
}

/*
* brief: timer stop
*/
void user_timer_stop(timer_idx_t id)
{
	timer_cmd(id,DISABLE);
	cpu_sleep_event_clear(timer_sleep_table[id]);
}

//uint32_t get_tick(void)
//{
//	uint32_t tick;
//	if(timer_env.g_time_tick){
//		tick = timer_env.g_time_tick;
//	}else{
//		tick = lib_get_ms();
//	}
//	return tick;
//}

//uint32_t get_diff_tick(uint32_t cur_tick, uint32_t prior_tick)
//{
//	if (cur_tick < prior_tick){
//		return (cur_tick + (~prior_tick));
//	}
//	else{
//		return (cur_tick - prior_tick);
//	}
//}

//int is_timeout(uint32_t start_time, uint32_t interval)
//{
//	return (get_diff_tick(get_tick(), start_time) >= interval);
//}

//void mdelay(uint32_t msec)
//{
//	uint32_t old_tick;

//	old_tick = get_tick();
//	while (get_diff_tick(get_tick(), old_tick) < msec)
//	{
//		
//	}
//}
