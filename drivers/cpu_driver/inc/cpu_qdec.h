/**
******************************************************************************************************************************************************************************************
*@file                       cpu_qdec.h
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
#ifndef QDEC_H
#define QDEC_H

#include "cpu.h"

#define QDEC_SENSITIVITY			2
#define QDEC_InternalCounter_RELOAD_VAL		(QDEC_SENSITIVITY-1)
#define QDEC_QRW_RELOAD_VAL								(QDEC_InternalCounter_RELOAD_VAL+QDEC_SENSITIVITY)

#define QDEC_IT_CM		QDEC_QIR_CMIE
#define QDEC_IT_IN      QDEC_QIR_INIE
#define QDEC_IT_UN      QDEC_QIR_UNIE
#define QDEC_IT_OV      QDEC_QIR_OVIE
#define QDEC_IT_QE      QDEC_QIR_QEIE

typedef enum
{
    QDEC_UP   = 0x01,
	QDEC_OVER = 0x02,
	QDEC_DOWN = 0xff,
}qdec_evt_t;



void qdec_init(functional_state new_state);

void qdec_set_internal_counter(int32_t val);

int32_t qdec_get_internal_counter(void);

void qdec_it_config(uint32_t qdec_it, functional_state new_state);

itstatus qdec_get_it_status(uint32_t qdec_it);

void qdec_clear_it_pending_bit(uint32_t qdec_it);

int32_t qdec_set_sensitivity(int32_t sensitivity);

int32_t qdec_get_external_counter(void);

void register_qdec_irq(void (*irq_cb)(qdec_evt_t evt));

#endif

