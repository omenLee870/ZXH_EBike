/**
 ****************************************************************************************
 *
 * @file arch_main.c
 *
 * @brief Main loop of the application.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */
 
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "bsp.h"
#include "app.h"
#include "sys_scheduler.h"

void bt_task_schdule(void)
{
	lib_bluetooth_schedule();
    
	transport_data_schedule();
}

int main(void)
{
    /* 芯片硬件配置 */
    hardware_init(); 
    /* 蓝牙 + bsp_init */
    transport_app_init();
	app_tasks_init();
    while(1) {
		/* 蓝牙事件调度 */
		bt_task_schdule();
		sys_scheduler_run();
        
#if (WDT_ENABLE)
		iwdt_reload_counter();
#endif
		
#if (SDK_ENABLE_SLEEP)
		/* 检查休眠标志 */
		 if(!sleep_check()) 
		 	continue;
		
		/* 关闭全局中断 */
		 lib_global_irq_lock();
	
		/* 检查协蓝牙是否能休眠 */
		 if(lib_bluetooth_sleep())
		 {
			 /* 休眠之前备份 */
			 lib_sleep_prepare();
			 
			 /* 使能休眠 */
			 lib_sleep_enter();
			 
			 /* 退出休眠并恢复备份内容 */
			 lib_sleep_exit();
		 }
		 
		 /* 打开全局中断 */
		 lib_global_irq_unlock();	 
#endif
	}
}














