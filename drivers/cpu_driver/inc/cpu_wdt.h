/**
******************************************************************************************************************************************************************************************
*@file                       cpu_IWDT.h
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
#ifndef CPU_IWDT_H
#define CPU_IWDT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cpu.h"

typedef enum
{
	IWDT_Mode_CPUReset = 0,
	IWDT_Mode_Interrupt = 1
}iwdt_mode_type_def;
	 
void iwdt_set_reload(uint32_t Reload);
void iwdt_reload_counter(void);
void iwdt_enable(void);
void iwdt_mode_config(iwdt_mode_type_def iwdt_mode);
	 
itstatus iwdt_get_it_status(void);
void iwdt_clear_it_pending_bit(void);

#ifdef __cplusplus
}
#endif

#endif
