/**
******************************************************************************************************************************************************************************************
*@file                       cpu_auadc.h
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
#ifndef _CPU_AUADC_
#define _CPU_AUADC_

#include "cpu.h"


//typedef enum 
//{
//	GPADC_OFFSET 	= (uint32_t)0x0000 << 8,
//	GPADC_TEMP 	= (uint32_t)0x0001 << 8,
//	GPADC_HVIN 	= (uint32_t)0x0002 << 8,
//	GPADC_GPIO 	= (uint32_t)0x0003 << 8,
//}gpadc_mode_t;

#define MAX_READ_TIMES			5

#define	HVIN_SLOPE1			1.539713
#define	HVIN_INTERCEPT1		2906 //2884.589

#define	HVIN_SLOPE2			1.55555
#define	HVIN_INTERCEPT2		1200



#define	GPIO_SLOPE				1100/3477 //0.3163646821
//#define	GPIO_INTERCEPT			589.50161
#define	GPIO_INTERCEPT			50

#define	TEMP_SLOPE				-0.186603
#define	TEMP_INTERCEPT			107.49333

typedef struct
{ 	
	uint32_t AUADC_AUADC_DIFF_EN;
	
	uint32_t AUADC_DC_BW;
	
	uint32_t AUADC_DC_BP;

	uint32_t AUADC_Sample_Rate;                                            

	uint32_t AUADC_AGC_EN;  

	uint32_t AUADC_GAIN_VAL;
	
	uint32_t AUADC_GPIO_CFG_AGC;
	
	uint32_t AUADC_AGC_SET_IDX;
	
	uint32_t AUADC_GAINADJ_L;
	
	uint32_t AUADC_GAINADJ_H;

	uint32_t AUADC_EN;          
	
	uint32_t AUADC_FIFO_Threshold;
	
	uint32_t AUADC_FIFO_Flush;
	
	uint32_t AUADC_FIFO_LEVEL;
	
	uint32_t AUADC_BIAS_EN;
	
	uint32_t AUADC_DATA_SRC;
	
	uint32_t GPADC_SEL2;
	
	uint32_t GPADC_SEL1;
	
	uint32_t GPADC_VCM_SEL;
	
	uint32_t GPADC_CH_SEL;
	
}auadc_init_type_def;

// delay sample auadc

typedef enum _DELAY_SAMPLE{
	AUADC_SAMPLE_DELAY_0US,
	AUADC_SAMPLE_DELAY_10US,
	AUADC_SAMPLE_DELAY_20US,
	AUADC_SAMPLE_DELAY_40US,
}DELAY_SAMPLE;

#define AUADC_SAMPLE_DELAY(x)		((uint32_t)x << 30)

// auadc pre amp
#define AGC_MIN_LIMIT(x)			((uint32_t)(x&0x0F) << 20)
#define AGC_MAX_LIMIT(x)			((uint32_t)(x&0x0F) << 16)
// when write agc idx ,should write agc idx frist to clear agc idx
#define AGC_IDX_ENABLE(enable)	((uint32_t)enable << 15)
#define AGC_IDX(x)					((uint32_t)(x&0x0F) << 11)

//auadc   debug mode gpio set agc
#define DEBUG_GPIO_AGC(enable)	((uint32_t)(enable&0x01) << 10)

// program used agc auto then we used auadc gain to set auadc gain
#define AUADC_GAIN(x)				((uint32_t)(x&0x3F) << 4)	

// auadc agc enable
#define AUAUC_AGC_ENABLE(enable)	((uint32_t)enable << 3)

// auadc agc sample rate
typedef enum _SAMPLE_RATE{
	AUADC_SAMPLE_RATE_8K,
	AUADC_SAMPLE_RATE_16K,
	AUADC_SAMPLE_RATE_32K,
// 11 not exist	
}SAMPLE_RATE;

#define AUADC_SAMPLE_RATE(rate)		((uint32_t)(rate & 0x03) << 1)

// auadc agc sample enable
#define AUADC_SAMPLE_ENABLE(enable) ((uint32_t)(enable & 0x01) << 0)



// auadc agc sample rate
typedef enum _SAMPLE_MODE{
	AUADC_SAMPLE_AMIC_DATA_16BIT,
	AUADC_SAMPLE_ORG_DATA_10BIT,
	AUADC_SAMPLE_RATE_1024_AVERAGE_10BIT,
	AUADC_SAMPLE_RATE_8192_AVERAGE_10BIT,
}SAMPLE_MODE;


/*AUADC_DATA_SRC*/
#define DATA_SRC_MODE(mode)			((uint32_t)(mode & 0x03) << 25)	// auadc 16Bit


#define AUADC_NEGATIVE(auadc_pin)		((uint32_t)(auadc_pin & 0x07) << 19)	
#define AUADC_POSITIVE(auadc_pin)		((uint32_t)(auadc_pin & 0x07) << 16)	

/*GPADC_Vcm_Sel*/
typedef enum _SAMPLE_VCM{
	AUADC_VCM_045,
	AUADC_VCM_050,			
	AUADC_VCM_055,			
	AUADC_VCM_060,			
	AUADC_VCM_065,			
	AUADC_VCM_070,			
	AUADC_VCM_075,			
	AUADC_VCM_080,			
}SAMPLE_VCM;
#define AUADC_VCM_SEL(vcm)				((uint32_t)(vcm & 0x07) << 10)

/*GPADC_Ch_Sel*/
typedef enum _AUADC_CH_SEL{
	AUADC_CH_OFFSET,
	AUADC_CH_TEMP,
	AUADC_CH_HVIN,
	AUADC_CH_GPIO,
}AUADC_CH_SEL;


#define AUADC_MODE_SELECT(ch)			((uint32_t)(ch & 0x03) << 8)
	
/*AUADC_AUADC_DIFF_EN*/

typedef enum _AUADC_GPIO_DIFF{
	AUADC_AUADC_DIFF_DOWN,
	AUADC_AUADC_DIFF_OPEN
}AUADC_GPIO_DIFF;

#define AUADC_GPIO_DIFF_MODE(enable)	((uint32_t)(enable & 0x01) << 7)


/*AUADC_DC_BW*/
typedef enum _AUADC_DC_BWS{
	AUADC_DC_BW_09,
	AUADC_DC_BW_10,
	AUADC_DC_BW_11,
	AUADC_DC_BW_12,
}AUADC_DC_BWS;

#define AUADC_DC_BW(dc_bw)				((uint32_t)(dc_bw & 0x03) << 1)

/*AUADC_DC_BP*/
typedef enum _AUADC_DC_BP_SWT{
	AUADC_DC_BP_DOWN,
	AUADC_DC_BP_OPEN
}AUADC_DC_BP_SWT;

#define AUADC_DC_BP_OPEN				((uint32_t)0x0001)
#define AUADC_DC_BP_DOWN				((uint32_t)0x0000)

/*AUADC_AGC_EN*/
typedef enum _AUADC_AGC_SWT{
	AUADC_AGC_DOWN,
	AUADC_AGC_OPEN
}AUADC_AGC_SWT;
#define AUADC_AGC(open)				((uint32_t)(open & 0x01) << 3)

/*AUADC_GPIO_CFG_AGC*/

typedef enum _AUADC_GPIO_AGC{
	AUADC_GPIO_CFG_AGC_DOWN,
 	AUADC_GPIO_CFG_AGC_OPEN,
}__AUADC_GPIO_AGC;

#define AUADC_GPIO_AGC(open)			((uint32_t)(open & 0x01) << 10)

/*AUADC_BAIS_OPEN*/
typedef enum AUADC_GPIO_AGC_SWT{
	AUADC_BIAS_DOWN,
	AUADC_BIAS_OPEN,
}AUADC_GPIO_AGC_SWT;
/*AUADC_BIAS_EN*/
#define AUADC_BIAS_ON(on)				((uint32_t)(on & 0x01) << 24)



/*AUADC_EN*/
#define AUADC_OPEN						((uint32_t)0x0001)
#define AUADC_DOWN						~((uint32_t)0x0001)

#define AUADC_IT_ALERT_CLEAR			((uint32_t)0x0002)				
#define AUADC_IT_OVFL_CLEAR			((uint32_t)0x0004)							
#define AUADC_IT_ALERT_EN				((uint32_t)0x0002)				
#define AUADC_IT_OVFL_EN				((uint32_t)0x0004)							
#define AUADC_IT_ALERT_MASK			((uint32_t)0x0200)				
#define AUADC_IT_OVFL_MASK				((uint32_t)0x0400)	


uint16_t auadc_receive_fifo_data(void);
uint16_t auadc_receive_data(void);
uint32_t auadc_receive_fifo_level(void);
void auadc_init(auadc_init_type_def* auadc_init_struct);
uint16_t auadc_receive_org_data(void);
void gpadc_cmd(functional_state new_state);
void auadc_cmd(functional_state new_state);
void auadc_agc_cmd(functional_state new_state);
void auadc_agc_set_idx_cl(uint32_t agc_set_idx);
void auadc_it_config(uint32_t auadc_it_en,functional_state new_state);
void auadc_clear_it_pending_bit(uint32_t gpadc_it);
void read_hvin_adc_param(void);

typedef void (*user_adc_cbk)(void);

#endif
