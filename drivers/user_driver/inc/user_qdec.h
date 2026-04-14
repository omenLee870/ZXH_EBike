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
#ifndef _USER_QDEC_H_
#define _USER_QDEC_H_
#include "cpu.h"

void user_qdec_init(void (*qdec_irq_callback)(qdec_evt_t evt));
void user_qdec_resume(void);
void user_qdec_sleep(void);


#endif

