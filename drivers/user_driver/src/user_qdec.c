
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "cpu.h"
#include "user_qdec.h"
#include "app_config.h"

static const GPIO_TypeDef io_map[]  = {
	[0] = {GPIOA_INOUT_CTRL,GPIOA_MAP_CTRL},
	[1] = {GPIOB_INOUT_CTRL,GPIOB_MAP_CTRL},
	[2] = {GPIOC_INOUT_CTRL,GPIOC_MAP_CTRL},
	[3] = {GPIOD_INOUT_CTRL,GPIOD_MAP_CTRL},
};

void qdec_nvic_init(void)
{
	NVIC_SetPriority(QDEC_IRQn ,NVIC_EncodePriority(0x05, 0x02,0x00)); 
	NVIC_EnableIRQ(QDEC_IRQn);
}

void user_qdec_configuration(void)
{
	gpio_pin_remap_config((GPIO_TypeDef *)&io_map[QDEC_GPIO_CHA / 8],(gpio_pin_num_type_def)(QDEC_GPIO_CHA % 8),GPIO_Remap_QDEC_CHA);
	gpio_pin_remap_config((GPIO_TypeDef *)&io_map[QDEC_GPIO_CHB / 8],(gpio_pin_num_type_def)(QDEC_GPIO_CHB % 8),GPIO_Remap_QDEC_CHB);
	gpio_init((GPIO_TypeDef *)&io_map[QDEC_GPIO_CHA / 8], 1 << (QDEC_GPIO_CHA % 8),GPIO_Mode_Input_Pull_Up_Resistor); 
	gpio_init((GPIO_TypeDef *)&io_map[QDEC_GPIO_CHB / 8], 1 << (QDEC_GPIO_CHB % 8),GPIO_Mode_Input_Pull_Up_Resistor); 
	qdec_clear_it_pending_bit(QDEC_IT_CM | QDEC_IT_UN | QDEC_IT_QE);
	qdec_it_config(QDEC_IT_CM | QDEC_IT_UN | QDEC_IT_QE,ENABLE);
	qdec_init(ENABLE);
}

void user_qdec_init(void (*qdec_irq_callback)(qdec_evt_t evt))
{
	register_qdec_irq(qdec_irq_callback);
	qdec_nvic_init();
	user_qdec_configuration();
}

void user_qdec_resume(void)
{
	user_qdec_configuration();
}

void user_qdec_sleep(void)
{
	gpio_pin_remap_config((GPIO_TypeDef *)&io_map[QDEC_GPIO_CHA / 8],(gpio_pin_num_type_def)(QDEC_GPIO_CHA % 8),GPIO_Remap_GPIO);
	gpio_pin_remap_config((GPIO_TypeDef *)&io_map[QDEC_GPIO_CHB / 8],(gpio_pin_num_type_def)(QDEC_GPIO_CHB % 8),GPIO_Remap_GPIO);
}
