/**
******************************************************************************************************************************************************************************************
*@file                       cpu_GPADC.c
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
#include "cpu_gpadc.h"
typedef struct _adc_env{
	int adc_offset;
	int adc_inited;
}adc_env;

static adc_env adc_envir={
	.adc_offset = 0,
	.adc_inited = 0,	
};

/*****************************************************************************
 函 数 名  : read_otp_adc_param
 功能描述  : 加载adc opt参数
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
static void read_otp_adc_param(void)
{
	uint16_t adc_value = 0;
	uint8_t adc_buf[4];

	if( TRUE == flash_page_otp_read(1,252, 4, adc_buf) )
	{
//		dbg_printk_hex("otp:: ",adc_buf,4);
		if(adc_buf[3] == 0x55){
			adc_value 	= adc_buf[1];
			adc_value 	<<= 8;
			adc_value 	+= adc_buf[0];
			adc_envir.adc_offset = 246 - adc_value;
			return;
		}
	}
	adc_envir.adc_offset = 0;

}


void GPADC_Init(GPADC_InitTypeDef* GPADC_InitStruct)
{
	
	GPADC->GPADC_CONF = GPADC_InitStruct->GPADC_Gain | GPADC_InitStruct->GPADC_Vcm_Sel | GPADC_InitStruct->GPADC_InMode | GPADC_InitStruct->GPADC_Ch_Sel;

	GPADC->GPADC_CTRL = (((1333333 + GPADC_InitStruct->GPADC_Sample_Rate / 2) / GPADC_InitStruct->GPADC_Sample_Rate) - 1 ) << 8 | GPADC_InitStruct->GPADC_WriteIn_Mode + GPADC_InitStruct->GPADC_EN;

	GPADC->GPADC_THRESHOLD = GPADC_InitStruct->GPADC_FIFO_Threshold;

	GPADC->DMA_CR = GPADC_InitStruct->GPADC_DMA_EN;

	GPADC->DMA_RDLR = GPADC_InitStruct->GPADC_DMA_Rdlr;
	
	read_otp_adc_param();
	
}

void GPADC_Cmd(functional_state NewState)
{
	if (NewState != DISABLE)
	{
		
		GPADC->GPADC_CTRL |= GPADC_OPEN;
	}
	else
	{
		
		GPADC->GPADC_CTRL |= GPADC_DOWN;		
	}
}

void GPADC_ITConfig(uint32_t GPADC_IT_EN,functional_state NewState)
{
	if(NewState != DISABLE)
		GPADC->GPADC_INT_CTRL = GPADC_IT_EN;
	else
		GPADC->GPADC_INT_CTRL &= ~GPADC_IT_EN;
}

uint16_t GPADC_ReceiveFIFOData()
{
	/* Return the data in the FIFODATA register */
	return (uint16_t)GPADC->GPADC_FIFODATA + adc_envir.adc_offset;
}

uint16_t GPADC_ReceiveData()
{
	/* Return the data in the DATA register */
	return (uint16_t)GPADC->GPADC_DATA/* + adc_envir.adc_offset*/;
}

uint32_t GPADC_ReceiveFIFOLevel()
{
	/* Return the data in the DATA register */
	return (uint32_t)GPADC->GPADC_FIFOLEVEL;
}

void GPADC_ClearITPendingBit(uint32_t GPADC_IT)
{
//	uint32_t tmpReg = 0;

	if (GPADC_IT_DONE_CLEAR	 | GPADC_IT)
	{
		GPADC->GPADC_STAT = GPADC_IT;
	}
	if (GPADC_IT_ALERT_CLEAR | GPADC_IT)
	{
		GPADC->GPADC_STAT = GPADC_IT;
	}
	if (GPADC_IT_OVFL_CLEAR	 | GPADC_IT)
	{
		GPADC->GPADC_STAT = GPADC_IT;
	}
	
}						

