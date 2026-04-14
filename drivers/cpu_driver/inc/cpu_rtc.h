/**
******************************************************************************************************************************************************************************************
*@file                       cpu_rtc.h
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
#ifndef CPU_RTC_H
#define CPU_RTC_H

#include "cpu.h"

void rtc_cmd(functional_state new_state);

void rtc_set_match_value(uint32_t rtc_counter_match_value);
void rtc_it_config(functional_state new_state);
void rtc_set_reload_value(uint32_t rtc_reload_counter_value);
void rtc_set_div(uint16_t DIV);

void rtc_clear_it_pending(void);
void rtc_clear_div(void);

#endif
