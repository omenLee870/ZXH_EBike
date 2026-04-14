/**
******************************************************************************************************************************************************************************************
*@file                       cpu_sysctrl.c
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
#include "cpu.h"

void crm_cmd_periph_clock(uint32_t crm_periphclock, functional_state new_state)
{
	if (new_state != DISABLE)
  {
    CRM->CG_CTRL |= crm_periphclock;
  }
  else
  {
    CRM->CG_CTRL &= ~crm_periphclock;
  }
}

void crm_reset_periph(uint32_t crm_periphreset)
{
	if(crm_periphreset)
	{
		CRM->SOFT_RST = crm_periphreset;
	}
	
}

void syscfg_cmd_wakeup_source(uint32_t syscfg_wakeup_source, functional_state new_state)
{	
	if (new_state != DISABLE)
  {
    SYSCFG->WAKE_MCU_CTRL |= syscfg_wakeup_source;
  }
  else
  {
    SYSCFG->WAKE_MCU_CTRL &= ~syscfg_wakeup_source;
  }
}

void syscfg_cmd_deep_sleep_wakeup_source(uint32_t syscfg_deep_sleep_wakeup_source,functional_state new_state)
{
	if (new_state != DISABLE)
	{
		SYSCFG->SLEEP_CTRL |= syscfg_deep_sleep_wakeup_source;
	}
	else
	{
		SYSCFG->SLEEP_CTRL &= ~syscfg_deep_sleep_wakeup_source;
	}
}

void syscfg_cpu_sleep_cmd_wakeup_source(uint32_t syscfg_wakeup_source, functional_state new_state)
{	
	if (new_state != DISABLE)
  {
    SYSCFG->WAKE_MCU_CTRL |= syscfg_wakeup_source;
  }
  else
  {
    SYSCFG->WAKE_MCU_CTRL &= ~syscfg_wakeup_source;
  }
	
}

void syscfg_deep_sleep_set_mode(uint32_t syscfg_deep_sleep_mode)
{
	SYSCFG->SLEEP_CTRL &= ~SYSCFG_SLEEP_CTRL_deep_slp_sel;
	SYSCFG->SLEEP_CTRL |= syscfg_deep_sleep_mode;
}

void syscfg_deep_sleep_cmd_wakeup_source(uint32_t syscfg_deep_sleep_wakeup_source,functional_state new_state)
{
	if (new_state != DISABLE)
	{
		SYSCFG->SLEEP_CTRL |= syscfg_deep_sleep_wakeup_source;
	}
	else
	{
		SYSCFG->SLEEP_CTRL &= ~syscfg_deep_sleep_wakeup_source;
	}
}

void syscfg_deep_sleep_set_sram(uint32_t Retention_Sram_x)
{
	SYSCFG->SLEEP_CTRL &= ~SYSCFG_SLEEP_CTRL_sram_ret_sel;
	SYSCFG->SLEEP_CTRL |= Retention_Sram_x;
}

void syscfg_cmd_extern_16M_xtal(void)
{
	SYSCFG->HFCLK_SEL |= (BIT9 + BIT0);
	SYSCFG->HFCLK_SEL &= ~BIT8;
}

void syscfg_cmd_extern_32M_xtal(void)
{
	SYSCFG->HFCLK_SEL |= (BIT9 + BIT0);
	SYSCFG->HFCLK_SEL &= ~BIT8;
	SYSCFG->HFCLK_SEL |= BIT1;
	SYSCFG->FREQ_SEL = (BIT4 + BIT2);//set rwip pclk use clk/2
}

void syscfg_cmd_extern_32K_xtal(void)
{
	SYSCFG->LFCLK_SEL |= (BIT9 + BIT0);
	SYSCFG->LFCLK_SEL &= ~BIT8;		
}

void syscfg_cmd_rc_32K(void)
{
	SYSCFG->LFCLK_SEL |= BIT8;	
	SYSCFG->LFCLK_SEL &= ~(BIT9 + BIT0);
}

void syscfg_cmd_rc_16M(void)
{	
	SYSCFG->HFCLK_SEL |= BIT8;
	SYSCFG->HFCLK_SEL &= ~BIT0;
	SYSCFG->HFCLK_SEL &= ~BIT9;	
}

void syscfg_common_config(void)
{
	*(uint32_t volatile *)0x500080C4 &= ~(BIT16+BIT17+BIT18); 
	*(uint32_t volatile *)0x500080C4 |= (BIT16+BIT17+BIT18);
	*(uint32_t volatile *)0x500080C4 &= ~(BIT21+BIT22+BIT23); 
	*(uint32_t volatile *)0x500080C4 |= (BIT21+BIT23);  
	*(uint32_t volatile *)0x500080C4 |= BIT8;
}


