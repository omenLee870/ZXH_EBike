#include "bsp.h"

void bsp_nvic_init(void);

/* 测试函数 */
void test_fun(uint8_t num){}

/* timer处理，里面如果用到函数，需要把这个函数通过flash_cache_load的方式初始化 */	
void timer_cb(void)
{
	if((TIMER->TIMER_INT_STATE & BIT0) == BIT0 )
	{
		TIMER->TIMER_INT_STATE &= ~BIT0 ;
		test_fun(10);
	}
}

void flash_cache_init(void)
{
	flash_cache_load((uint32_t )timer_cb,512);
	flash_cache_load((uint32_t )test_fun,128);
}
void bsp_init(void)
{
	hardware_chache_init(flash_cache_init);
	hardware_flash_idle_sched_set(timer_cb);
    
    /* 定时器初始化 */
	time_init();  
    
    /* nvs flash初始化 */
	storage_init();	
    
    /* 读卡flash */
	read_card_flash();            	
    
    /* 读配对信息 */
	read_pair_info_from_flash();	
        
    /* nvic初始化 */
	bsp_nvic_init();									
    
    /* 校准rssi */
	lib_rssi_adjust(10, 3, RSSI_AVERAGE_ALGORITHM);
    
	LogInfo("bsp_init success\r\n");
}



void bsp_nvic_init(void)
{
	/* 蓝牙优先级-0x01、不是特别需要处理的事情设置优先级比蓝牙低 */
	NVIC_SetPriorityGrouping(0x05);//4bit for preempt and 4bit for sub
    
	NVIC_SetPriority(UART0_IRQn, NVIC_EncodePriority(0x05, 0x02,0x01));
	NVIC_SetPriority(TIMER_IRQn, NVIC_EncodePriority(0x05, 0x02,0x01));
    
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_EnableIRQ(TIMER_IRQn);
}


























