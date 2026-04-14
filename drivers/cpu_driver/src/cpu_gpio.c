/**
******************************************************************************************************************************************************************************************
*@file                       cpu_gpio.c
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
#include "cpu_gpio.h"

GPIO_TypeDef gpio_a = {GPIOA_INOUT_CTRL,GPIOA_MAP_CTRL};
GPIO_TypeDef gpio_b = {GPIOB_INOUT_CTRL,GPIOB_MAP_CTRL};
GPIO_TypeDef gpio_c = {GPIOC_INOUT_CTRL,GPIOC_MAP_CTRL};
GPIO_TypeDef gpio_d = {GPIOD_INOUT_CTRL,GPIOD_MAP_CTRL};

GPIO_TypeDef *GPIOA = &gpio_a;
GPIO_TypeDef *GPIOB = &gpio_b;
GPIO_TypeDef *GPIOC = &gpio_c;
GPIO_TypeDef *GPIOD = &gpio_d;

GPIO_TypeDef *gpio_group[4] = {&gpio_a,&gpio_b,&gpio_c,&gpio_d};
void gpio_pin_remap_config(GPIO_TypeDef* GPIOx, gpio_pin_num_type_def GPIO_PinNum, gpio_remap_type_def GPIO_Remap)
{
	uint32_t index = GPIO_PinNum/4;
	uint32_t mask = 0x3f<<((GPIO_PinNum%4)*8);
	uint32_t val = GPIO_Remap<<((GPIO_PinNum%4)*8);
	GPIOx->map_ctrl->MAP_MODE[index] &= ~mask;
	GPIOx->map_ctrl->MAP_MODE[index] |= val;
}

void gpio_init(GPIO_TypeDef* GPIOx,uint32_t gpio_pin,gpio_mode_type_def GPIO_Mode)
{
	uint32_t pin_num = 0;
	uint32_t gpio_mode = 0;
	
	for(pin_num = 0; pin_num <= GPIO_PinNum_Max; pin_num++)
	{
		if(gpio_pin &0x01)
		{
			GPIOx->inout_ctrl->CON &= ~(3<<(pin_num*2));
			gpio_mode = (GPIO_Mode << (pin_num*2));
			GPIOx->inout_ctrl->CON |= gpio_mode;
			
		}
		gpio_pin >>= 1;
	}
}

void gpio_set(GPIO_TypeDef* GPIOx,uint32_t gpio_pin)
{
	GPIOx->inout_ctrl->BSR = gpio_pin;
}


void gpio_reset(GPIO_TypeDef* GPIOx,uint32_t gpio_pin)
{
	GPIOx->inout_ctrl->BSR = (gpio_pin<<8);
}

void gpio_set_output_level(GPIO_TypeDef* GPIOx,uint32_t gpio_pin,uint32_t level)
{
	if(level)
	{
		GPIOx->inout_ctrl->BSR = gpio_pin;
	}
	else
	{
		GPIOx->inout_ctrl->BSR = (gpio_pin<<8);
	}
}

uint32_t gpio_get_intput_level(GPIO_TypeDef* GPIOx)
{
	uint32_t tmp = 0;
	tmp = GPIOx->inout_ctrl->IOD;
	
	return ((tmp>>8)&0xff);
}

uint32_t gpio_get_single_pin_input_level(GPIO_TypeDef* GPIOx,uint32_t gpio_pin)
{
	uint32_t pin_Level = 0;
	
	pin_Level = gpio_get_intput_level(GPIOx);
	
	if(pin_Level & gpio_pin)
		return HIGH;
	else
		return LOW;
}

void gpio_set_interrupt_source_and_type(uint32_t GPIO_Intx,uint32_t GPIO_Pinx,uint32_t gpio_int_type)
{
	uint32_t idx = GPIO_Intx - GPIO_Int0;
	uint32_t mask = (0x03U)<<(idx*8);
	GPIO_MODULE->INTP_TYPE &= ~mask;
	GPIO_MODULE->INTP_TYPE |= (gpio_int_type<<(idx*8));
	
	GPIO_MODULE->INTP_SRC  |= (GPIO_Pinx<<(idx*8));
}

void gpio_deepsleepwksrc_config(uint32_t gpio_int_num,uint32_t gpio_int_type,uint32_t gpio_int_src)
{
	uint32_t idx = gpio_int_num - SYSCFG_GPIOWakeUpSource_GPIO_INT0;
	uint32_t mask = (0x03U)<<(idx*8);
	SYSCFG->WKUP_TYPE &= ~mask;
	SYSCFG->WKUP_TYPE |= (gpio_int_type<<(idx*8));
	
	mask = (0x1fU)<<(idx*8);
	SYSCFG->WKUP_SRC  &= ~mask;
	SYSCFG->WKUP_SRC  |= (gpio_int_src<<(idx*8));
}

uint8_t gpio_it_pending_get(uint32_t GPIO_Intx)
{
	uint8_t state = (GPIO_MODULE->INTP_STAT >> (GPIO_Intx * 8)) & 0xff;
	GPIO_MODULE->INTP_STAT |= (state << (GPIO_Intx * 8)); // clear all gpio int state
	return state;
}

void gpio_clear_it_pending(uint32_t GPIO_Intx,uint32_t GPIO_Pin_x)
{

	uint32_t idx = GPIO_Intx - GPIO_Int0;
	uint32_t mask = GPIO_Pin_x<<(idx*8); 
    
    mask = (0xffU)<<(idx*8);
	GPIO_MODULE->INTP_STAT  &= ~mask;
	GPIO_MODULE->INTP_STAT  |= (GPIO_Pin_x<<(idx*8));
}

boolean gpio_get_it_status(uint32_t GPIO_Intx,uint32_t GPIO_Pin_x)
{
    uint32_t idx = GPIO_Intx - GPIO_Int0;
    if(GPIO_MODULE->INTP_STAT & (GPIO_Pin_x<<(idx*8)))
		return TRUE;
	else
		return FALSE;
}
void ws8300_gpio_remap(ws8300_gpio gpio, gpio_remap_type_def remap)
{
//    GPIO_INOUT_TypeDef * inout_ctrl = GPIOA_INOUT_CTRL + (gpio >> 3)*0x10;
	if(gpio > GPIO_GPIOD_Pin7)
		return;

	uint32_t index = ((gpio%8) >> 2);
	uint32_t pos = (((gpio%8)& 0x03) << 3);
	uint32_t mask = 0x3f 		<< pos;
	uint32_t val = remap 	<< pos;
	gpio_group[gpio/8]->map_ctrl->MAP_MODE[index] &= ~mask;
	gpio_group[gpio/8]->map_ctrl->MAP_MODE[index] |= val;

}

void ws8300_gpio_init(ws8300_gpio gpio,gpio_mode_type_def GPIO_Mode)
{

	 if(gpio > GPIO_GPIOD_Pin7)
		 return;

	uint32_t pin_num = 0;
	uint32_t gpio_mode = 0;
	uint32_t gpio_pin = 1<<(gpio%8);

	for(pin_num = 0; pin_num <= GPIO_PinNum_Max; pin_num++)
	{
		if(gpio_pin &0x01)
		{
			gpio_group[gpio/8]->inout_ctrl->CON &= ~(3<<(pin_num*2));
			gpio_mode = (GPIO_Mode << (pin_num*2));
			gpio_group[gpio/8]->inout_ctrl->CON |= gpio_mode;
		}
		gpio_pin >>= 1;
	}
}


void ws8300_gpio_set(ws8300_gpio gpio)
{    
	if(gpio > GPIO_GPIOD_Pin7)
		return;

	
	gpio_group[gpio/8]->inout_ctrl->BSR |= 1<<(gpio%8);
}


void ws8300_gpio_reset(ws8300_gpio gpio)
{	
	if(gpio > GPIO_GPIOD_Pin7)
		return;
	gpio_group[gpio/8]->inout_ctrl->BSR |= (1<<(gpio%8) << 8);
}

void ws8300_gpio_set_output_level(ws8300_gpio gpio,uint32_t level)
{
	if(gpio > GPIO_GPIOD_Pin7)
		return;
	
	if(level)
	{
		gpio_group[gpio/8]->inout_ctrl->BSR = 1<<(gpio%8);
	}
	else
	{
		gpio_group[gpio/8]->inout_ctrl->BSR = (1<<(gpio%8)<<8);
	}
}

uint32_t ws8300_gpio_get_intput_level(ws8300_gpio gpio)
{
	if(gpio > GPIO_GPIOD_Pin7)
		return 0;
	
	uint32_t tmp = 0;

	tmp = gpio_group[gpio/8]->inout_ctrl->IOD;

    if (((tmp>>8)&0xff) & (1 << (gpio%8))) {
        return HIGH;
    } else 
        return LOW;	
}

