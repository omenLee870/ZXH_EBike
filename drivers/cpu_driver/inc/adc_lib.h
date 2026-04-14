#ifndef __ADC_LIB_H__
#define __ADC_LIB_H__
#include "stdint.h"
typedef enum _ADC_TYPE
{
	GPIO_ADC,		// gpio ADC
	R_HVIN_ADC,	// ??????HVIN ADC
	HVIN_ADC,		// ????HVIN adc
}ADC_TYPE;
typedef enum _HVIN_PARAM_SZ
{
	SZ_NO_CALIBRATE	= 0x00,
	SZ_HVIN_CAL_OK		= 0x01,
	SZ_R_HVIN_CAL_OK	= 0x02,
	SZ_GPIO_CAL_OK		= 0x04,
}HVIN_PARAM_SZ;

int32_t get_adc_volum(int16_t adc_val,ADC_TYPE is_hvin);

uint32_t cal_gpadc_hvin_in_series(uint8_t vcc);
HVIN_PARAM_SZ interliaze_hvin_param(void);

#endif



