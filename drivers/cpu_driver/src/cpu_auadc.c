/**
******************************************************************************************************************************************************************************************
*@file                       cpu_AUADC.c
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
#include "cpu_auadc.h"
#include "cpu_flash.h"
#include "user_gpadc.h"
//#include "app_config.h"

typedef struct _adc_env
{
	int adc_offset;
	int link_offset;
	int coffe_a;
	int coffe_b;
	int adc_inited;
	int adc_vaild;
	user_adc_cbk adc_cbk;
}adc_env;

static adc_env adc_envir=
{
	.adc_offset = 0,
	.adc_inited = 0,	
	.adc_cbk = NULL
};


/*****************************************************************************
 函 数 名  : read_otp_adc_param
 功能描述  : 加载adc opt参数
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
*****************************************************************************/
void read_auadc_otp_adc_param(void)
{
	uint32_t adc_value = 0;
	uint8_t adc_buf[4];
	if(adc_envir.adc_inited == 1)
	{
		return;
	}
	
	adc_envir.adc_inited = 1;
	adc_envir.adc_offset = 0;
	if(TRUE == flash_page_otp_read(1,252, 4, adc_buf) )
	{
//		dbg_block_printk_hex("otp:: ",adc_buf,4);
		if(adc_buf[3] == 0x55){
			adc_value 	= adc_buf[1];
			adc_value 	<<= 8;
			adc_value 	+= adc_buf[0];
//			adc_envir.adc_offset = 267 - adc_value;
//			adc_envir.adc_offset = 64 - adc_value;
			adc_envir.adc_offset = 237 - adc_value;
//			return;
		}
	}
}

uint16_t convert_adc_value(uint16_t gpadc_value)
{
	gpadc_value = gpadc_value * adc_envir.coffe_b;
	gpadc_value = gpadc_value / adc_envir.coffe_a;
	return gpadc_value;
}

void auadc_init(auadc_init_type_def* auadc_init_struct)
{
	AUADC->AUADC_CTRL0 = AGC_MIN_LIMIT(auadc_init_struct->AUADC_GAINADJ_L) 
						| AGC_MAX_LIMIT(auadc_init_struct->AUADC_GAINADJ_H)
						| AGC_IDX_ENABLE(ENABLE)
						| AGC_IDX(auadc_init_struct->AUADC_AGC_SET_IDX) 
						| DEBUG_GPIO_AGC(auadc_init_struct->AUADC_GPIO_CFG_AGC) 
						| AUADC_GAIN(auadc_init_struct->AUADC_GAIN_VAL) 
						| AUADC_SAMPLE_RATE(auadc_init_struct->AUADC_Sample_Rate);

	AUADC->AUADC_CTRL1 = DATA_SRC_MODE(auadc_init_struct->AUADC_DATA_SRC) 
						| AUADC_BIAS_ON(auadc_init_struct->AUADC_BIAS_EN) 
						| AUADC_NEGATIVE(auadc_init_struct->GPADC_SEL2)
						| AUADC_POSITIVE(auadc_init_struct->GPADC_SEL1)
						| AUADC_VCM_SEL(auadc_init_struct->GPADC_VCM_SEL)
						| AUADC_MODE_SELECT(auadc_init_struct->GPADC_CH_SEL)
						| AUADC_GPIO_DIFF_MODE(auadc_init_struct->AUADC_AUADC_DIFF_EN)
						| AUADC_DC_BW(AUADC_DC_BW_11);

	AUADC->AUADC_Threshold = auadc_init_struct->AUADC_FIFO_Threshold;

	read_auadc_otp_adc_param();
}

void gpadc_cmd(functional_state new_state)
{
	if (new_state != DISABLE)
	{
		auadc_agc_cmd(DISABLE);				// gpadc mode close agc
		AUADC->AUADC_CTRL1 |= 0x0000007D;
	}
	else
	{
		AUADC->AUADC_CTRL1 &= 0xFFFFFF84;		
	}
}

void auadc_cmd(functional_state new_state)
{
	if (new_state != DISABLE)
	{
		AUADC->AUADC_CTRL0 |= AUADC_OPEN;
	}
	else
	{
		AUADC->AUADC_CTRL0 &= AUADC_DOWN;		
	}
}

void auadc_agc_cmd(functional_state new_state)
{
	if (new_state != DISABLE)
	{
		AUADC->AUADC_CTRL0 |= AUADC_AGC(AUADC_AGC_OPEN);
	}
	else
	{
		AUADC->AUADC_CTRL0 &= ~(AUADC_AGC(AUADC_AGC_OPEN));		
	}
}

void auadc_agc_set_idx_cl(uint32_t agc_set_idx)
{
	if(agc_set_idx){
		AUADC->AUADC_CTRL0 |= AGC_IDX_ENABLE(agc_set_idx);
	}else{
		AUADC->AUADC_CTRL0 &= ~(AGC_IDX_ENABLE(agc_set_idx));
	}
}


void auadc_it_config(uint32_t auadc_it_en,functional_state new_state)
{
	if(new_state != DISABLE)
		AUADC->AUADC_INT_CTRL = auadc_it_en;
	else
		AUADC->AUADC_INT_CTRL &= ~auadc_it_en;
}

uint16_t auadc_receive_fifo_data(void)
{
	/* Return the data in the FIFODATA register */
	return (uint16_t)AUADC->AUADC_FIFODATA;
}


uint16_t auadc_receive_data(void)
{
	/* Return the data in the DATA register */
	return (uint16_t)AUADC->AUADC_ADC_DATA;
}


uint32_t auadc_receive_fifo_level(void)
{
	/* Return the data in the DATA register */
	return (uint32_t)AUADC->AUADC_FIFOLEVEL;
}

void auadc_clear_it_pending_bit(uint32_t gpadc_it)
{
	if (AUADC_IT_ALERT_CLEAR & gpadc_it)
	{
		AUADC->AUADC_STAT = gpadc_it;
	}
	if (AUADC_IT_OVFL_CLEAR & gpadc_it)
	{
		AUADC->AUADC_STAT = gpadc_it;
	}	
}						

