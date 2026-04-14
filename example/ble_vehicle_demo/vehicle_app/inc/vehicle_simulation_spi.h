#ifndef _VEHICLE_SIMULATION_SPI_H
#define _VEHICLE_SIMULATION_SPI_H


#include "cpu_gpio.h"

#define SPI_MODE

extern void ws8300_gpio_set(ws8300_gpio gpio);
extern void ws8300_gpio_init(ws8300_gpio gpio,gpio_mode_type_def GPIO_Mode);
extern void ws8300_gpio_reset(ws8300_gpio gpio);
extern uint32_t ws8300_gpio_get_intput_level(ws8300_gpio gpio);

#define  NFC_CS_PIN 			GPIO_GPIOC_Pin7	
#define  NFC_CLK_PIN 			GPIO_GPIOC_Pin6
#define  NFC_MISO_PIN 		GPIO_GPIOC_Pin4
#define  NFC_MOSI_PIN 		GPIO_GPIOC_Pin5
#define  NFC_NREST_PIN 		GPIO_GPIOC_Pin3

#define  SET_SPI_CS       ws8300_gpio_set(NFC_CS_PIN)	//NSS
#define  CLR_SPI_CS       ws8300_gpio_reset(NFC_CS_PIN)

#define  SET_SPI_CK  	    ws8300_gpio_set(NFC_CLK_PIN)//CLK，SPI时钟
#define  CLR_SPI_CK  	    ws8300_gpio_reset(NFC_CLK_PIN)

#define  SET_SPI_MOSI  	  ws8300_gpio_set(NFC_MOSI_PIN)//MOSI，从设备数据输入
#define  CLR_SPI_MOSI  	  ws8300_gpio_reset(NFC_MOSI_PIN)

#define  STU_SPI_MISO  	  ws8300_gpio_get_intput_level(NFC_MISO_PIN)//MISO，从设备数据输出

#define  SET_NFC_RST()  	ws8300_gpio_set(NFC_NREST_PIN)//复位,低电平有效
#define  CLR_NFC_RST()  	ws8300_gpio_reset(NFC_NREST_PIN)



#define  IRQ_PIN 					GPIO_GPIOA_Pin0
#define  IRQ() 				 		ws8300_gpio_get_intput_level(IRQ_PIN)
#define  INIT_NFC_IRQ()   gpio_set_interrupt_source_and_type(GPIO_Int0,GPIO_Pin_0,GPIO_Int_Type_RisingEdge)



/*声明函数*/
extern unsigned char ReadRawRC(unsigned char Address);
extern void WriteRawRC(unsigned char Address, unsigned char value);
extern void SetBitMask(unsigned char reg,unsigned char mask);
extern void ClearBitMask(unsigned char reg,unsigned char mask);













#endif
