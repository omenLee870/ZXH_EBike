/**
******************************************************************************************************************************************************************************************
*@file                       cpu_IWDT.c
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
#include <stdint.h>
#include "cpu_wdt.h"


#define COUNTER_RELOAD_KEY				((uint32_t)0x76)

void iwdt_set_reload(uint32_t Reload)
{
	IWDT->IWDT_RLD = Reload;
}

void iwdt_reload_counter(void)
{
	IWDT->IWDT_CRR = COUNTER_RELOAD_KEY;
}

itstatus iwdt_get_it_status(void)
{
	itstatus bitstatus = RESET;

	if(!(IWDT->IWDT_STAT & IWDT_IWDT_STATE))
	{
		bitstatus = RESET;
	}
	else
	{
		bitstatus = SET;
	}

	return bitstatus;

}

void iwdt_clear_it_pending_bit(void)
{
	//read EOI register to clear interrupt panding
	uint32_t clr = IWDT->IWDT_EOI;
}

void iwdt_enable(void)
{
	IWDT->IWDT_CR |= IWDT_IWDT_CR_IWDT_EN;
}

void iwdt_mode_config(iwdt_mode_type_def iwdt_mode)
{
	if(iwdt_mode == IWDT_Mode_Interrupt)
		IWDT->IWDT_CR |= IWDT_IWDT_CR_RMOD;
	else if(iwdt_mode == IWDT_Mode_CPUReset)
		IWDT->IWDT_CR &= ~IWDT_IWDT_CR_RMOD;
}




