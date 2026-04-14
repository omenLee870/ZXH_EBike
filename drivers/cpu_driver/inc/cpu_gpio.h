/**
******************************************************************************************************************************************************************************************
*@file                       cpu_gpio.h
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
#ifndef CPU_GPIO_H
#define CPU_GPIO_H

#include "cpu.h"

typedef enum
{ 
	GPIO_Remap_GPIO         = 0x00,
	GPIO_Remap_UART0_RX     = 0x01,
	GPIO_Remap_UART0_TX     = 0x02,
	GPIO_Remap_UART0_CTSN   = 0x03,
	GPIO_Remap_UART0_RTSN   = 0x04,
	GPIO_Remap_5RESERVED    = 0x05,
	GPIO_Remap_6RESERVED    = 0x06,
	GPIO_Remap_7RESERVED    = 0x07,
	GPIO_Remap_8RESERVED    = 0x08,
	GPIO_Remap_SPI0_CSN     = 0x09,
	GPIO_Remap_SPI0_CLK     = 0x0a,
	GPIO_Remap_SPI0_MOSI    = 0x0b,	
	GPIO_Remap_SPI0_MISO    = 0x0c,
	GPIO_Remap_PWM_3N       = 0x0d,
	GPIO_Remap_PWM_2N       = 0x0e,
	GPIO_Remap_PWM_2        = 0x0f,	
	GPIO_Remap_PWM_3        = 0x10,
	GPIO_Remap_PWM_4        = 0x11,
	GPIO_Remap_PWM_5        = 0x12,
	GPIO_Remap_PWM_1        = 0x13,
	GPIO_Remap_PWM_1N       = 0x14,
	GPIO_Remap_QDEC_CHA     = 0x15,
	GPIO_Remap_QDEC_CHB     = 0x16,
	GPIO_Remap_QDEC_IDX     = 0x17,
	GPIO_Remap_24RESERVED   = 0x18,
	GPIO_Remap_25RESERVED   = 0x19,
	GPIO_Remap_26RESERVED   = 0x1a,
	GPIO_Remap_JTAC_TCLK    = 0x1b,	
	GPIO_Remap_JTAC_TDO     = 0x1c,
	GPIO_Remap_JTAC_TDI     = 0x1d,
	GPIO_Remap_JATC_TMS     = 0x1e,
	GPIO_Remap_JTAC_TRST    = 0x1f,	
	GPIO_Remap_PWM_6        = 0x20,
	GPIO_Remap_33RESERVED   = 0x21,
	GPIO_Remap_34RESERVED   = 0x22,
	GPIO_Remap_35RESERVED   = 0x23,
	GPIO_Remap_36RESERVED   = 0x24,
	GPIO_Remap_37RESERVED   = 0x25,
	GPIO_Remap_28RESERVED   = 0x26,
	GPIO_Remap_39RESERVED   = 0x27,
	GPIO_Remap_40RESERVED   = 0x28,
	GPIO_Remap_41RESERVED   = 0x29,
	GPIO_Remap_42RESERVED   = 0x2a,
	GPIO_Remap_43RESERVED   = 0x2b,	
	GPIO_Remap_44RESERVED   = 0x2c,
	GPIO_Remap_456RESERVED  = 0x2d,
	GPIO_Remap_46RESERVED   = 0x2e,
	GPIO_Remap_47RESERVED   = 0x2f,	
	GPIO_Remap_ANALOG       = 0x30,
	GPIO_Remap_IRDA_RX      = 0x31,
	GPIO_Remap_IRDA_TX      = 0x32,
	GPIO_Remap_12M          = 0x33,
	GPIO_Remap_SYS_DBG3     = 0x34,
	GPIO_Remap_SYS_DBG2     = 0x35,
	GPIO_Remap_OSC          = 0x36,
	GPIO_Remap_AUADC_D0_D1  = 0x37,
	GPIO_Remap_AUADC_D2_D9  = 0x38,
	GPIO_Remap_SYS_DBG	    = 0x39,
	GPIO_Remap_CLK32K	    = 0x3a,
	GPIO_Remap_MDM_CLK	    = 0x3b,	
	GPIO_Remap_BLE_DEG0     = 0x3c,
	GPIO_Remap_BLE_DEG1     = 0x3d,
	GPIO_Remap_BLE_DEG2     = 0x3e,
	GPIO_Remap_BLE_DEG3     = 0x3f,	
}gpio_remap_type_def;

typedef enum
{
	GPIO_PinNum_0 = 0,
	GPIO_PinNum_1 = 1,
	GPIO_PinNum_2 = 2,
	GPIO_PinNum_3 = 3,
	GPIO_PinNum_4 = 4,
	GPIO_PinNum_5 = 5,
	GPIO_PinNum_6 = 6,
	GPIO_PinNum_7 = 7,
	GPIO_PinNum_Max = GPIO_PinNum_7,
}gpio_pin_num_type_def;

typedef enum
{
	GPIO_Mode_Input,
	GPIO_Mode_Input_Pull_Up_Resistor,
	GPIO_Mode_Input_Pull_Down_Resistor,
	GPIO_Mode_Output,
}gpio_mode_type_def;

typedef struct
{
	uint32_t gpio_pin;             /*!< Specifies the GPIO pins to be configured.
                                      This parameter can be any value of @ref GPIO_pins_define */	
	gpio_mode_type_def GPIO_Mode;
	
	gpio_remap_type_def GPIO_Remap;
    
}gpio_init_type_def;


#define GPIO_Pin_0                 ((uint32_t)0x01)
#define GPIO_Pin_1                 ((uint32_t)0x02)
#define GPIO_Pin_2                 ((uint32_t)0x04)
#define GPIO_Pin_3                 ((uint32_t)0x08)
#define GPIO_Pin_4                 ((uint32_t)0x10)
#define GPIO_Pin_5                 ((uint32_t)0x20)
#define GPIO_Pin_6                 ((uint32_t)0x40)
#define GPIO_Pin_7                 ((uint32_t)0x80)
#define GPIO_Pin_All               ((uint32_t)0xff)

#define GPIO_Int0						(0)
#define GPIO_Int1						(1)
#define GPIO_Int2						(2)
#define GPIO_Int3						(3)

#define GPIO_GPIOA_Pin0				(0x00)
#define GPIO_GPIOA_Pin1				(0x01)
#define GPIO_GPIOA_Pin2				(0x02)
#define GPIO_GPIOA_Pin3				(0x03)
#define GPIO_GPIOA_Pin4				(0x04)
#define GPIO_GPIOA_Pin5				(0x05)
#define GPIO_GPIOA_Pin6				(0x06)
#define GPIO_GPIOA_Pin7				(0x07)
#define GPIO_GPIOB_Pin0				(0x08)
#define GPIO_GPIOB_Pin1				(0x09)
#define GPIO_GPIOB_Pin2				(0x0A)
#define GPIO_GPIOB_Pin3				(0x0B)
#define GPIO_GPIOB_Pin4				(0x0C)
#define GPIO_GPIOB_Pin5				(0x0D)
#define GPIO_GPIOB_Pin6				(0x0E)
#define GPIO_GPIOB_Pin7				(0x0F)
#define GPIO_GPIOC_Pin0				(0x10)
#define GPIO_GPIOC_Pin1				(0x11)
#define GPIO_GPIOC_Pin2				(0x12)
#define GPIO_GPIOC_Pin3				(0x13)
#define GPIO_GPIOC_Pin4				(0x14)
#define GPIO_GPIOC_Pin5				(0x15)
#define GPIO_GPIOC_Pin6				(0x16)
#define GPIO_GPIOC_Pin7				(0x17)
#define GPIO_GPIOD_Pin0				(0x18)
#define GPIO_GPIOD_Pin1				(0x19)
#define GPIO_GPIOD_Pin2				(0x1A)
#define GPIO_GPIOD_Pin3				(0x1B)
#define GPIO_GPIOD_Pin4				(0x1C)
#define GPIO_GPIOD_Pin5				(0x1D)
#define GPIO_GPIOD_Pin6				(0x1E)
#define GPIO_GPIOD_Pin7				(0x1F)

#define GPIO_Int_Type_Disable			(0)
#define GPIO_Int_Type_RisingEdge		(1)
#define GPIO_Int_Type_FallingEdge		(2)
#define GPIO_Int_Type_BothEdge			(3)

typedef uint8_t ws8300_gpio;

void gpio_pin_remap_config(GPIO_TypeDef* GPIOx, gpio_pin_num_type_def GPIO_PinNum, gpio_remap_type_def GPIO_Remap);
void gpio_init(GPIO_TypeDef* GPIOx,uint32_t gpio_pin,gpio_mode_type_def GPIO_Mode);
void gpio_set(GPIO_TypeDef* GPIOx,uint32_t gpio_pin);
void gpio_reset(GPIO_TypeDef* GPIOx,uint32_t gpio_pin);
void gpio_set_output_level(GPIO_TypeDef* GPIOx,uint32_t gpio_pin,uint32_t level);
uint32_t gpio_get_intput_level(GPIO_TypeDef* GPIOx);
uint32_t gpio_get_single_pin_input_level(GPIO_TypeDef* GPIOx,uint32_t gpio_pin);
void gpio_set_interrupt_source_and_type(uint32_t GPIO_Intx,uint32_t GPIO_Pinx,uint32_t gpio_int_type);
uint8_t gpio_it_pending_get(uint32_t GPIO_Intx);
void gpio_clear_it_pending(uint32_t GPIO_Intx,uint32_t GPIO_Pin_x);
boolean gpio_get_it_status(uint32_t GPIO_Intx,uint32_t GPIO_Pin_x);
void gpio_deepsleepwksrc_config(uint32_t gpio_int_num,uint32_t gpio_int_type,uint32_t gpio_int_src);
extern uint32_t ws8300_gpio_get_intput_level(ws8300_gpio gpio);
extern void ws8300_gpio_init(ws8300_gpio gpio,gpio_mode_type_def GPIO_Mode);
extern void ws8300_gpio_remap(ws8300_gpio gpio, gpio_remap_type_def remap);
extern void ws8300_gpio_reset(ws8300_gpio gpio);
extern void ws8300_gpio_set(ws8300_gpio gpio);
extern void ws8300_gpio_set_output_level(ws8300_gpio gpio,uint32_t level);
#endif
