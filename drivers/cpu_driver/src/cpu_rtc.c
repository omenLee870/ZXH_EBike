/**
******************************************************************************************************************************************************************************************
*@file                       cpu_rtc.c
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
#include "cpu_rtc.h"

void rtc_cmd(functional_state new_state)
{
	if (DISABLE != new_state)
	{
		RTC->RTC_CCR |= RTC_RTC_CCR_RTC_EN | RTC_RTC_CCR_RTC_WEN ;
	}
	else
	{
		RTC->RTC_CCR &= ~RTC_RTC_CCR_RTC_EN;
	}
}

void rtc_set_div(uint16_t DIV)
{
	SYSCFG->RTC_DIV_VAL = DIV;
}

void rtc_set_match_value(uint32_t rtc_counter_match_value)
{
	RTC->RTC_CMR = rtc_counter_match_value;
}

void rtc_set_reload_value(uint32_t rtc_reload_counter_value)
{
	RTC->RTC_CLR = rtc_reload_counter_value;
}

void rtc_it_config(functional_state new_state)
{
	if (DISABLE != new_state)
	{
		RTC->RTC_CCR |= RTC_RTC_CCR_RTC_ien;
		RTC->RTC_CCR &= ~RTC_RTC_CCR_RTC_Mask;
	}
	else
	{
		RTC->RTC_CCR &= ~RTC_RTC_CCR_RTC_ien;
		RTC->RTC_CCR |= RTC_RTC_CCR_RTC_Mask;
	}
}

void rtc_clear_it_pending(void)
{
	uint32_t tmp = RTC->RTC_EOI;
	tmp = RTC->RTC_RSTAT;
}

void rtc_set_counter_val(uint32_t val)
{
	RTC->RTC_CLR = val;
}

void rtc_clear_div(void)//系统全局软复位重启前必须要调用该函数
{
	uint32_t tick = RTC->RTC_CCVR;
	while(RTC->RTC_CCVR - tick < 1);
	SYSCFG->RTC_DIV_VAL = 0;
}

