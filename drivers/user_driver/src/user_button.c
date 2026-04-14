
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "cpu.h"
#include "user_button.h"
#include "app_config.h"

void (*gpio_irq_callback)(uint8_t GPIO_Intx,uint8_t pin);

/**
*  brief: gpio int0 interrupt
**/
void GPIO_INT0_IRQHandler()
{
	uint8_t state = gpio_it_pending_get(GPIO_Int0);
	if(gpio_irq_callback) gpio_irq_callback(GPIO_Int0,state);
}

/**
*  brief: gpio int1 interrupt
**/
void GPIO_INT1_IRQHandler()
{
    uint8_t state = gpio_it_pending_get(GPIO_Int1);
	if(gpio_irq_callback) gpio_irq_callback(GPIO_Int1,state);
}

/**
*  brief: gpio int2 interrupt
**/
void GPIO_INT2_IRQHandler()
{
    uint8_t state = gpio_it_pending_get(GPIO_Int2);
	if(gpio_irq_callback) gpio_irq_callback(GPIO_Int2,state);
}

/**
*  brief: gpio int3 interrupt
**/
void GPIO_INT3_IRQHandler()
{
   uint8_t state = gpio_it_pending_get(GPIO_Int3);
	if(gpio_irq_callback) gpio_irq_callback(GPIO_Int3,state);
}

/**
*  brief: register gpio irq interrupt
*  parameter[0]: irq
**/
void user_gpio_int_irq(void (*irq)(uint8_t GPIO_Intx,uint8_t pin))
{
	gpio_irq_callback = irq;
}

/**
*  brief: gpio remap
*
*  parameter[0]: gpio_pins_map
**/
void user_gpio_remap(uint32_t gpio_pins_map)
{
	for(int i = 0; i < 8; i++)
	{
		if((gpio_pins_map & 0xFF) & (1 << i))
		{
			gpio_pin_remap_config(GPIOA,(gpio_pin_num_type_def)i,GPIO_Remap_GPIO);
		}
		if((gpio_pins_map >> 8 & 0xFF) & (1 << i))
		{
			gpio_pin_remap_config(GPIOB,(gpio_pin_num_type_def)i,GPIO_Remap_GPIO);
		}
		if((gpio_pins_map >> 16 & 0xFF) & (1 << i))
		{
			gpio_pin_remap_config(GPIOC,(gpio_pin_num_type_def)i,GPIO_Remap_GPIO);
		}
		if((gpio_pins_map >> 24 & 0xFF) & (1 << i))
		{
			gpio_pin_remap_config(GPIOD,(gpio_pin_num_type_def)i,GPIO_Remap_GPIO);
		}
	}
}

/**
*  brief: gpio in
*
*  parameter[0]: gpio_pins_map
*  parameter[1]: interr_type,interrupt type
**/
void user_gpio_dir_in(uint32_t gpio_pins_map,uint32_t interr_type,gpio_mode_type_def mode)
{
    uint8_t i=0;
    gpio_init(GPIOA,gpio_pins_map    ,mode);
    gpio_init(GPIOB,gpio_pins_map>>8 ,mode);
    gpio_init(GPIOC,gpio_pins_map>>16,mode);
    gpio_init(GPIOD,gpio_pins_map>>24,mode);

    for(i=0; i<32; i++)
    {
        if((gpio_pins_map>>i) & 0x00000001)
        {
            gpio_set_interrupt_source_and_type(i / 8,1 << (i % 8),interr_type);
        }
    }
}

/**
*  brief: gpio out
*
*  parameter[0]: gpio_pins_map
*  parameter[1]: pins_value,out value
**/
void user_gpio_dir_out(uint32_t gpio_pins_map, uint32_t pins_value,gpio_mode_type_def mode,uint8_t state)
{

	  gpio_init(GPIOA,gpio_pins_map    ,mode);
    gpio_init(GPIOB,gpio_pins_map>>8 ,mode);
    gpio_init(GPIOC,gpio_pins_map>>16,mode);
    gpio_init(GPIOD,gpio_pins_map>>24,mode);
	
    gpio_init(GPIOA,pins_value    ,GPIO_Mode_Output);
    gpio_init(GPIOB,pins_value>>8 ,GPIO_Mode_Output);
    gpio_init(GPIOC,pins_value>>16,GPIO_Mode_Output);
    gpio_init(GPIOD,pins_value>>24,GPIO_Mode_Output);
	
    if(state){
	    gpio_set(GPIOA,pins_value&0xff);
	    gpio_set(GPIOB,(pins_value>>8)&0xff);
	    gpio_set(GPIOC,(pins_value>>16)&0xff);
	    gpio_set(GPIOD,(pins_value>>24)&0xff);
    }else{
    	gpio_reset(GPIOA,pins_value&0xff);
	    gpio_reset(GPIOB,(pins_value>>8)&0xff);
	    gpio_reset(GPIOC,(pins_value>>16)&0xff);
	    gpio_reset(GPIOD,(pins_value>>24)&0xff);
    }
}

/**
*  brief: get pin value
*
*  parameter: pins_map
**/
uint32_t user_gpio_set_mode(uint32_t pins_map,gpio_mode_type_def mode)
{
    gpio_init(GPIOA,pins_map    ,mode);
    gpio_init(GPIOB,pins_map>>8 ,mode);
    gpio_init(GPIOC,pins_map>>16,mode);
    gpio_init(GPIOD,pins_map>>24,mode);
	return 0;
}

/**
*  brief: get pin value
*
*  parameter: pins_map
**/
uint32_t user_gpio_get_pins_value(uint32_t pins_map)
{
	
    uint32_t pins_value = 0;
    pins_value = (GPIOD->inout_ctrl->IOD>>8<<24) | (GPIOC->inout_ctrl->IOD>>8<<16) |\
                 (GPIOB->inout_ctrl->IOD>>8<<8)| (GPIOA->inout_ctrl->IOD>>8);

#if (LOW_LEVEL_WAKE_UP)
    return (pins_map & ~pins_value);
#else
	return (pins_map & pins_value);
#endif
}

/**
*  brief: user_gpio_set_wake
*
*  parameter: pins_map
**/
void user_gpio_set_wake(uint32_t pins_map)
{
    int i=0,k=0;
    for(i=0; i<32; i++)
    {
        if((pins_map & ((0x00000001)<< i)) && (k<4))
        {  
        #if (LOW_LEVEL_WAKE_UP)
			gpio_deepsleepwksrc_config(k,4,i);
		#else
			gpio_deepsleepwksrc_config(k,2,i);
		#endif
            k++;
        }
    }
}

/**
*  brief: user_gpio_nvic_init
*
*  void
**/
void user_gpio_nvic_init(void)
{
	NVIC_SetPriority(GPIO_INT0_IRQn ,NVIC_EncodePriority(0x05, 0x02,0x00)); 
	NVIC_SetPriority(GPIO_INT1_IRQn ,NVIC_EncodePriority(0x05, 0x02,0x00)); 
	NVIC_SetPriority(GPIO_INT2_IRQn ,NVIC_EncodePriority(0x05, 0x02,0x00)); 
	NVIC_SetPriority(GPIO_INT3_IRQn ,NVIC_EncodePriority(0x05, 0x02,0x00)); 
	NVIC_EnableIRQ(GPIO_INT0_IRQn);
	NVIC_EnableIRQ(GPIO_INT1_IRQn);
	NVIC_EnableIRQ(GPIO_INT2_IRQn);
	NVIC_EnableIRQ(GPIO_INT3_IRQn);
}
