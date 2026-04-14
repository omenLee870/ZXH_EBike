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
#ifndef _USER_GPADC_H_
#define _USER_GPADC_H_

#include "cpu.h"
#include "cpu_auadc.h"

typedef enum
{
	GPADC_GPIOA_PIN1 = 0,
	GPADC_GPIOA_PIN3,
	GPADC_GPIOA_PIN2,
	GPADC_GPIOA_PIN4,
	GPADC_GPIOB_PIN6,
	GPADC_GPIOC_PIN0,
	GPADC_GPIOC_PIN1,
	GPADC_GPIOC_PIN2,
	GPADC_OFFSET_MODE,		//链路offset
	GPADC_TEMP_MODE,		//温度
	GPADC_HVIN_MODE,		// HVIN
	GPADC_INVALID,			// 无效
}gpadc_gpio_t;

typedef enum _REGIST_INIT
{
    RIGISTER_INIT,
    RIGISTER_REINIT,
}REGIST_INIT;

typedef enum 
{
	GPADC_TEMP = (uint32_t)0x0001 << 8,
	GPADC_HVIN = (uint32_t)0x0002 << 8,
	GPADC_GPIO = (uint32_t)0x0003 << 8,
}gpadc_mode_t;

/* 函数声明 */
float digital_conversion_analog(gpadc_mode_t mode,int32_t value);

void gpadc_gpio_config(gpadc_gpio_t gpio);

void gapdc_configuration(gpadc_mode_t mode,gpadc_gpio_t gpio);

int get_gpadc_value(gpadc_mode_t mode,gpadc_gpio_t gpio,uint32_t *adc_value);

/* 用于校准芯片获取adc值 */
int16_t get_adc_value(gpadc_gpio_t gpio,uint16_t cycle,int16_t *adc_value,uint16_t adc_cnt);
#endif

