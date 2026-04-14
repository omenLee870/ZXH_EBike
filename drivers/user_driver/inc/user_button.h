#ifndef _USER_BUTTON_H
#define _USER_BUTTON_H

#include <stdint.h>

void user_gpio_int_irq(void (*irq)(uint8_t GPIO_Intx,uint8_t pin));
void user_gpio_remap(uint32_t gpio_pins_map);
void user_gpio_dir_in(uint32_t gpio_pins_map,uint32_t interr_type,gpio_mode_type_def mode);
void user_gpio_dir_out(uint32_t gpio_pins_map, uint32_t pins_value,gpio_mode_type_def mode,uint8_t state);
uint32_t user_gpio_get_pins_value(uint32_t pins_map);
void user_gpio_set_wake(uint32_t pins_map);
void user_gpio_nvic_init(void);
uint32_t user_gpio_set_mode(uint32_t pins_map,gpio_mode_type_def mode);

#endif

