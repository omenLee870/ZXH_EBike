#ifndef __WS1850S_PORT_H_
#define __WS1850S_PORT_H_

#include "cpu_gpio.h"
#include <stdint.h>
#include <stdbool.h>

/* 根据你的原理图配置引脚 */
#define NFC_SDA_PIN    GPIO_GPIOA_Pin0
#define NFC_SCL_PIN    GPIO_GPIOA_Pin1
#define NFC_RST_PIN    GPIO_GPIOA_Pin2

/* WS1850S I2C 设备地址 */
#define WS1850S_I2C_ADDR_WRITE 0x50
#define WS1850S_I2C_ADDR_READ  0x51

/* 硬件初始化接口 */
void ws1850s_hw_init(void);
void ws1850s_hw_reset(void);

/* 核心寄存器读写接口 (提供给第二层驱动层使用) */
void pcd_write_reg(uint8_t reg_addr, uint8_t val);
uint8_t pcd_read_reg(uint8_t reg_addr);
void pcd_clear_bit_mask(uint8_t reg_addr, uint8_t mask);
void pcd_set_bit_mask(uint8_t reg_addr, uint8_t mask);

#endif /* __WS1850S_PORT_H_ */
