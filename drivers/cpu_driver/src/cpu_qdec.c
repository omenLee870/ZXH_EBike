/**
******************************************************************************************************************************************************************************************
*@file                       cpu_qdec.c
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
#include "string.h"
#include "cpu_qdec.h"

static volatile int32_t qdec_sensitivity = QDEC_SENSITIVITY;
static volatile int32_t qdec_internalCounter_reloadVal = QDEC_InternalCounter_RELOAD_VAL;
static volatile int32_t qdec_qrw_reloadVal = QDEC_QRW_RELOAD_VAL;

static volatile int32_t qdec_counter = 0;

void (*qdec_callback)(qdec_evt_t evt);

void register_qdec_irq(void (*irq_cb)(qdec_evt_t evt))
{
	qdec_callback = irq_cb;
}

void qdec_init(functional_state new_state)
{
	QDEC->QCR &= ~QDEC_QCR_ECNT; // disable qdec
	if(new_state != DISABLE)
	{
		QDEC->QCR &= ~QDEC_QCR_ECNT; // disable qdec
		QDEC->QCK = 1;				//时钟分频设置，不能为0
		qdec_counter = 0;
		qdec_set_internal_counter(qdec_internalCounter_reloadVal);
		QDEC->QRW = qdec_qrw_reloadVal;
		QDEC->QCR |= QDEC_QCR_ECNT; // enable qdec
	}		
}

void qdec_set_internal_counter(int32_t val)
{
	QDEC->QRW = val;
	QDEC->QCR |= QDEC_QCR_PLCT;
	while(QDEC->QCR & QDEC_QCR_PLCT);
}

int32_t qdec_get_internal_counter(void)
{
	QDEC->QCR |= QDEC_QCR_QLAT;
	while(QDEC->QCR & QDEC_QCR_QLAT);
	return QDEC->QRW;
}

void qdec_it_config(uint32_t qdec_it, functional_state new_state)
{
	if(new_state != DISABLE)
		QDEC->QIR |= qdec_it;
	else
		QDEC->QIR &= ~qdec_it;
	
}

itstatus qdec_get_it_status(uint32_t qdec_it)
{
	itstatus bitstatus = RESET; 
  

  if ((QDEC->QSR & qdec_it) != RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}


void qdec_clear_it_pending_bit(uint32_t qdec_it)
{
	QDEC->QSR &= ~qdec_it;
	
}

int32_t qdec_set_sensitivity(int32_t sensitivity)
{
	qdec_sensitivity = sensitivity;
	qdec_internalCounter_reloadVal = qdec_sensitivity - 1;
	qdec_qrw_reloadVal = qdec_internalCounter_reloadVal + qdec_sensitivity;
	return qdec_sensitivity;
}

int32_t qdec_get_external_counter(void)
{
	return qdec_counter;
}

void QDEC_IRQHandler(void)
{
	uint32_t state = QDEC->QSR;
	if(state & QDEC_IT_CM)
	{
		qdec_counter++;

		qdec_it_config(QDEC_IT_CM ,DISABLE);
		
		qdec_set_internal_counter(qdec_internalCounter_reloadVal);
		QDEC->QRW = qdec_qrw_reloadVal;
		QDEC->QSR = QDEC_IT_CM;
		
		if(qdec_callback) qdec_callback(QDEC_UP);
		
		qdec_it_config(QDEC_IT_CM ,ENABLE);
		
	}
	 if(state & QDEC_IT_UN)
	{
		qdec_counter--;
		qdec_it_config(QDEC_IT_CM | QDEC_IT_UN ,DISABLE);
		
		qdec_set_internal_counter(qdec_internalCounter_reloadVal);
		QDEC->QRW = qdec_qrw_reloadVal;
		QDEC->QSR = QDEC_IT_UN | QDEC_IT_CM;
		
		if(qdec_callback) qdec_callback(QDEC_DOWN);

		qdec_it_config(QDEC_IT_CM | QDEC_IT_UN ,ENABLE);
	}
	if(state & QDEC_IT_QE)
	{
		while(1)
		{
			break;
		}
	}

	qdec_clear_it_pending_bit(QDEC_IT_CM | QDEC_IT_UN | QDEC_IT_QE);
	NVIC_ClearPendingIRQ(QDEC_IRQn);
	
}
