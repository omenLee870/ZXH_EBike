/**
******************************************************************************************************************************************************************************************
*@file                       cpu_gpadc.h
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
#ifndef _CPU_GPADC_
#define _CPU_GPADC_

#include "cpu.h"

typedef struct
{
	uint32_t GPADC_Gain;
	
	uint32_t GPADC_Vcm_Sel;
	
	uint32_t GPADC_InMode;                                           
																	
	uint32_t GPADC_Ch_Sel;                                        

	uint32_t GPADC_Sample_Rate;                                            

	uint32_t GPADC_WriteIn_Mode;                                           

	uint32_t GPADC_EN;              
	
	uint32_t GPADC_FIFO_Threshold;
	
	uint32_t GPADC_FIFO_Flush;
	
	uint32_t GPADC_DMA_EN;
	
	uint32_t GPADC_DMA_Rdlr;
	
}GPADC_InitTypeDef;

/*GPADC_Gain*/
#define GPADC_Gain_05		((uint32_t)0x0000 << 14)
#define GPADC_Gain_1		((uint32_t)0x0001 << 14)
#define GPADC_Gain_2		((uint32_t)0x0002 << 14)
#define GPADC_Gain_4		((uint32_t)0x0003 << 14)

/*GPADC_Vcm_Sel*/
#define GPADC_VCM_045		((uint32_t)0x0000 << 4)
#define GPADC_VCM_050		((uint32_t)0x0001 << 4)
#define GPADC_VCM_055		((uint32_t)0x0002 << 4)
#define GPADC_VCM_060		((uint32_t)0x0003 << 4)
#define GPADC_VCM_065		((uint32_t)0x0004 << 4)
#define GPADC_VCM_070		((uint32_t)0x0005 << 4)
#define GPADC_VCM_075		((uint32_t)0x0006 << 4)
#define GPADC_VCM_080		((uint32_t)0x0007 << 4)

/*GPADC_InMode*/
#define GPADC_SINGLE_IN		((uint32_t)0x0000 << 3)
#define GPADC_DIFF_IN		((uint32_t)0x0001 << 3)

/*GPADC_Ch_Sel*/
#define GPADC_CH_OFFSET		((uint32_t)0x0000 << 1)
#define GPADC_CH_TEMP		((uint32_t)0x0001 << 1)
#define GPADC_CH_CELL		((uint32_t)0x0002 << 1)
#define GPADC_CH_GPIO		((uint32_t)0x0003 << 1)

/*GPADC_EN*/
#define GPADC_OPEN			((uint32_t)0x0001)
#define GPADC_DOWN			((uint32_t)0x0000)

/*GPADC_DMA_EN*/
#define GPADC_DMA_OPEN		((uint32_t)0x0001)
#define GPADC_DMA_DOWN		((uint32_t)0x0000)

/*GPADC_WriteIn_Mode*/
#define GPADC_MODE_SINGLE	((uint32_t)0x0000  << 1)
#define GPADC_MODE_MANY		((uint32_t)0x0001  << 1)
#define GPADC_MODE_CONTINU	((uint32_t)0x0002  << 1)

/*GPADC_Sample_Rate*/
#define GPADC_SAMPLERATE_2k         2000
#define GPADC_SAMPLERATE_4k         4000
#define GPADC_SAMPLERATE_8k         8000
#define GPADC_SAMPLERATE_16k        16000
#define GPADC_SAMPLERATE_32k        32000

#define GPADC_IT_DONE_CLEAR					((uint32_t)0x0001)				
#define GPADC_IT_ALERT_CLEAR				((uint32_t)0x0002)				
#define GPADC_IT_OVFL_CLEAR					((uint32_t)0x0004)				
#define GPADC_IT_DONE_EN					((uint32_t)0x0001)				
#define GPADC_IT_ALERT_EN					((uint32_t)0x0002)				
#define GPADC_IT_OVFL_EN					((uint32_t)0x0004)				
#define GPADC_IT_DONE_MASK					((uint32_t)0x0001)				
#define GPADC_IT_ALERT_MASK					((uint32_t)0x0002)				
#define GPADC_IT_OVFL_MASK					((uint32_t)0x0004)	

void GPADC_Init(GPADC_InitTypeDef* GPADC_InitStruct);
void GPADC_Cmd(functional_state NewState);
void GPADC_ITConfig(uint32_t GPADC_IT_EN,functional_state NewState);
uint16_t GPADC_ReceiveFIFOData(void);
uint16_t GPADC_ReceiveData(void);
uint32_t GPADC_ReceiveFIFOLevel(void);
void GPADC_ClearITPendingBit(uint32_t GPADC_IT);

#endif
