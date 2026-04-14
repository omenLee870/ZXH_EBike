/**
******************************************************************************************************************************************************************************************
*@file                       user_uart.h
*@author         
*@data                       
*@version                    v1.0
*@par Copyright(c):
*
*@par History:
*     version:author,date,desc
*     v1.0   :create this file
*
*******************************************************************************************************************************************************************************************
*/
#ifndef _USER_TIMER_H_
#define _USER_TIMER_H_
#include "cpu.h"

typedef uint32_t tick;

typedef enum
{
  IDX_TIMER0,
	IDX_TIMER1,
	IDX_TIMER2,
	IDX_TIMER3,
	IDX_TIMER4,
	IDX_TIMER5,
	IDX_TIMER_NUM,
}timer_idx_t;

typedef struct
{
    uint32_t timer_period;
}timer_param_t;

typedef  void (*timer_event_cbk_t)(void);

typedef struct
{
    timer_idx_t    timer_idx;
    timer_param_t  timer_param;
	timer_event_cbk_t cbk;
}timer_config_t;

void user_timer_init(timer_config_t *p_config);
void user_timer_resume(timer_config_t *p_config);
void user_timer_start(timer_idx_t id);
void user_timer_stop(timer_idx_t id);
//uint32_t get_tick(void);
//int is_timeout(uint32_t start_time, uint32_t interval);
//void mdelay(uint32_t msec);

#endif

