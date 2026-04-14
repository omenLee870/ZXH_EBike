#include "stdint.h"
#include "cpu.h"
#include "host_stack.h"
#include "app_config.h"

uint8_t ble_p_priority = 0x01;
void nvic_configuration(void)
{
	NVIC_SetPriorityGrouping(0x05);//4bit for preempt and 4bit for sub
	/*�������ȼ�-0x02*/
	NVIC_SetPriority(LIB_13_IRQn,NVIC_EncodePriority(0x05, ble_p_priority,0x00));
	NVIC_SetPriority(LIB_5_IRQn ,NVIC_EncodePriority(0x05, ble_p_priority,0x00)); 
	NVIC_SetPriority(LIB_8_IRQn ,NVIC_EncodePriority(0x05, ble_p_priority,0x00));    
	NVIC_SetPriority(LIB_6_IRQn, NVIC_EncodePriority(0x05, ble_p_priority,0x00)); 
	NVIC_SetPriority(LIB_7_IRQn, NVIC_EncodePriority(0x05, ble_p_priority,0x00));  
	NVIC_SetPriority(LIB_9_IRQn ,NVIC_EncodePriority(0x05, ble_p_priority,0x00)); 
	NVIC_SetPriority(LIB_4_IRQn ,NVIC_EncodePriority(0x05, ble_p_priority,0x00)); 
	NVIC_SetPriority(LIB_10_IRQn,NVIC_EncodePriority(0x05, ble_p_priority,0x00)); 
	NVIC_SetPriority(LIB_11_IRQn,NVIC_EncodePriority(0x05, ble_p_priority,0x00));
	NVIC_SetPriority(LIB_16_IRQn,NVIC_EncodePriority(0x05, ble_p_priority,0x00));
	NVIC_SetPriority(LIB_18_IRQn,NVIC_EncodePriority(0x05, ble_p_priority,0x00));
	NVIC_SetPriority(LIB_20_IRQn,NVIC_EncodePriority(0x05, ble_p_priority,0x00));
	NVIC_SetPriority(LIB_19_IRQn,NVIC_EncodePriority(0x05, ble_p_priority,0x00));


	NVIC_EnableIRQ(LIB_13_IRQn);
	NVIC_EnableIRQ(LIB_5_IRQn);
	NVIC_EnableIRQ(LIB_8_IRQn);
	NVIC_EnableIRQ(LIB_6_IRQn);
	NVIC_EnableIRQ(LIB_7_IRQn);
	NVIC_EnableIRQ(LIB_9_IRQn);
	NVIC_EnableIRQ(LIB_4_IRQn);
	NVIC_EnableIRQ(LIB_10_IRQn);
	NVIC_EnableIRQ(LIB_11_IRQn);

}

void sleep_configuration(void)
{
	syscfg_deep_sleep_set_mode(SYSCFG_DeepSleepMode_ShutClock_Periph);
	syscfg_deep_sleep_set_sram(SYSCFG_Retention_Sram_20k);
}

void iwdt_configuration(void)
{
	iwdt_set_reload(32768 * WDT_TIMEOUT);
	iwdt_enable();
}

void hardware_init(void)
{
	crm_cmd_periph_clock(CRM_CG_CTRL_IWDT | CRM_CG_CTRL_UART0 | CRM_CG_CTRL_QDEC | CRM_CG_CTRL_TIMER | CRM_CG_CTRL_GPIO | CRM_CG_CTRL_SYSCFG | CRM_CG_CTRL_BT | CRM_CG_CTRL_HW_RF_SPI | CRM_CG_CTRL_LIB_22,ENABLE);
	crm_reset_periph(CRM_SOFT_RST_UART0 | CRM_SOFT_RST_BT | CRM_SOFT_RST_QDEC);
	
	syscfg_common_config();

	#if (USE_EXT_XTAL_32M)
	syscfg_cmd_extern_32M_xtal();
	#endif
	
	#if (USE_EXT_XTAL_16M)
	syscfg_cmd_extern_16M_xtal();
	#endif
	
	#if (USE_EXT_XTAL_32K)
	syscfg_cmd_extern_32K_xtal();
	lib_extern_32k_calib();
	#endif
	
	#if (USE_INTER_RC_32K)
	//Initialize rc param
	syscfg_cmd_rc_32K();
	lib_rc_32k_ppm(RC32K_DEFAULT_PPM,RC32K_DEFAULT_JITTER,RC32K_DEFAULT_DRIFFT);
	#endif
	
	#if (WDT_ENABLE)
	iwdt_configuration();
	#endif
		
	//Initialize sleep mode
	sleep_configuration();
	
	//Initialize radio
	lib_radio_configuration(0);
	
	//Initialize xtal freq dev
	lib_set_xtal_freq_dev(XTAL_FREQ_DEV);

	lib_set_lock_irq(ble_p_priority);	
	//Initialize nvic
	nvic_configuration();
	/*���÷��书��*/
	lib_radio_set_tx_power(TX_POWER_0);
}




