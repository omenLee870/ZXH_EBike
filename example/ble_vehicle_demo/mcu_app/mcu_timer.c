#include "bsp.h"
                           
void beep_time_init(uint32_t freq,uint8_t duty)
{
	timer_init_type_def  timer_init_structure;
	
	timer_init_structure.TIMER_prescaler = __SYSTEM_CLOCK/(freq*100) -1 ;  
	timer_init_structure.TIMER_reload_cnt_val = 100; 
	timer_init_structure.TIMER_pwm_pol = TIMER_PWM_POL_LOW;    

	timer_toggle_set(timer3,duty);
	timer_update_en(timer3,ENABLE);
	timer_init(timer3 ,&timer_init_structure);	
	timer_cmd(timer3,ENABLE);
}

void time_init(void)
{
	timer_init_type_def  timer_init_structure;

	//定时器0配置 1ms
	timer_init_structure.TIMER_prescaler = __SYSTEM_CLOCK/10000 -1 ; 
	timer_init_structure.TIMER_reload_cnt_val = 10-1;                 

	timer_it_config(timer0,TIMER0_int_cfg_0,ENABLE); // 启动定时器
	timer_update_en(timer0,ENABLE);
	timer_init(timer0 ,&timer_init_structure);
	timer_cmd(timer0,ENABLE);
}

uint16_t time_1s_count          =       0;
uint8_t  time_10ms_count        =       0;
uint8_t  time_scan_card_count   =       0;
uint8_t  time_rssi_count        =       0;

static void time0_1ms_cbk(void)
{
	if (time_1s_count++ >= 1000) {
		time_1s_count = 0;
        
        get_vehicle_adc_val();
        
//		LogInfo("time_1s_count\r\n");
	}
}

static void time0_10ms_cbk(void)
{
	if (time_10ms_count++ >= 10) {
		time_10ms_count = 0;
        
	}
}

static void time0_card_scan(void)
{
	if (time_scan_card_count++ >= SCAN_CARD_INTERVAL_TIME) {
		time_scan_card_count = 0;
		
		set_card_process_flag(1);
	}	
}

static void time0_rssi_scan(void)
{
	if (time_rssi_count++ >= 100) {
		time_rssi_count = 0;
        
		rssi_process();
	}	
}

/* 定时器0 - 1ms秒中断 */
void TIMER_IRQHandler(void)
{   
	if(timer_get_it_state(timer0) == SET)
	{  
        /* 用于阻塞延时-1ms累加 */
		timer_tick++;
        
		time0_1ms_cbk();
        
		time0_10ms_cbk();
        
		time0_card_scan();
        
		time0_rssi_scan();
        
		timer_it_clear(timer0);
	}
}







