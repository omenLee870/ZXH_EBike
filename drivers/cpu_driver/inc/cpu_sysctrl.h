/**
******************************************************************************************************************************************************************************************
*@file                       cpu_sysctrl.h
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
#ifndef CPU_SYSCTRL_H
#define CPU_SYSCTRL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cpu.h"
     
#define SYSCFG_Retention_Sram_20k		(0x00000)
#define SYSCFG_Retention_Sram_16k		(0x20000)


#define SYSCFG_WakeUpSource_QDEC			SYSCFG_WAKE_MCU_CTRL_qdec			
#define SYSCFG_WakeUpSource_BT              SYSCFG_WAKE_MCU_CTRL_BT     
#define SYSCFG_WakeUpSource_GPIO_INT3       SYSCFG_WAKE_MCU_CTRL_gpio_int3
#define SYSCFG_WakeUpSource_GPIO_INT2       SYSCFG_WAKE_MCU_CTRL_gpio_int2
#define SYSCFG_WakeUpSource_GPIO_INT1       SYSCFG_WAKE_MCU_CTRL_gpio_int1
#define SYSCFG_WakeUpSource_GPIO_INT0       SYSCFG_WAKE_MCU_CTRL_gpio_int0
#define SYSCFG_WakeUpSource_RTC         	SYSCFG_WAKE_MCU_CTRL_rtc			
#define SYSCFG_WakeUpSource_TIMER3          SYSCFG_WAKE_MCU_CTRL_timer3   
#define SYSCFG_WakeUpSource_TIMER2          SYSCFG_WAKE_MCU_CTRL_timer2   
#define SYSCFG_WakeUpSource_TIMER1          SYSCFG_WAKE_MCU_CTRL_timer1   
#define SYSCFG_WakeUpSource_TIMER0          SYSCFG_WAKE_MCU_CTRL_timer0   
#define SYSCFG_WakeUpSource_KPC           	SYSCFG_WAKE_MCU_CTRL_kpc      
#define SYSCFG_WakeUpSource_SPI1	        SYSCFG_WAKE_MCU_CTRL_spi1     
#define SYSCFG_WakeUpSource_SPI0	        SYSCFG_WAKE_MCU_CTRL_spi0     
#define SYSCFG_WakeUpSource_UART1	        SYSCFG_WAKE_MCU_CTRL_uart1    
#define SYSCFG_WakeUpSource_UART0	        SYSCFG_WAKE_MCU_CTRL_uart0    
#define SYSCFG_WakeUpSource_I2C		        SYSCFG_WAKE_MCU_CTRL_i2c   

#define SYSCFG_DeepSleepMode_ShutClock							(0)
#define SYSCFG_DeepSleepMode_ShutClock_Periph					(1)
#define SYSCFG_DeepSleepMode_ShutClock_Periph_CPU				(2)

#define SYSCFG_DeepSleepWakeUpSource_BLE_EXT	SYSCFG_SLEEP_CTRL_ble_extwkupdsb  
#define SYSCFG_DeepSleepWakeUpSource_GPIO       SYSCFG_SLEEP_CTRL_gpio_wk_en			
#define SYSCFG_DeepSleepWakeUpSource_Keypad     SYSCFG_SLEEP_CTRL_kpc_wk_en       
#define SYSCFG_DeepSleepWakeUpSource_RTC        SYSCFG_SLEEP_CTRL_rtc_wk_en       

#define SYSCFG_GPIOWakeUpSource_GPIO_INT0						(0)
#define SYSCFG_GPIOWakeUpSource_GPIO_INT1						(1)
#define SYSCFG_GPIOWakeUpSource_GPIO_INT2						(2)
#define SYSCFG_GPIOWakeUpSource_GPIO_INT3						(3)

#define SYSCFG_GPIOWakeUpType_Disable							(0x00)
#define SYSCFG_GPIOWakeUpType_Hight							    (0x01)
#define SYSCFG_GPIOWakeUpType_Hight_One32KCycle_AntiShake		(0x02)
#define SYSCFG_GPIOWakeUpType_LOW							    (0x03)
#define SYSCFG_GPIOWakeUpType_LOW_One32KCycle_AntiShake		    (0x04)
#define SYSCFG_GPIOWakeUpType_RisingEdge				        (0x05)
#define SYSCFG_GPIOWakeUpType_FallingEdge				        (0x06)


#define SYSCFG_GPIOA_Pin0				(0x00)
#define SYSCFG_GPIOA_Pin1				(0x01)
#define SYSCFG_GPIOA_Pin2				(0x02)
#define SYSCFG_GPIOA_Pin3				(0x03)
#define SYSCFG_GPIOA_Pin4				(0x04)
#define SYSCFG_GPIOA_Pin5				(0x05)
#define SYSCFG_GPIOA_Pin6				(0x06)
#define SYSCFG_GPIOA_Pin7				(0x07)
#define SYSCFG_GPIOB_Pin0				(0x08)
#define SYSCFG_GPIOB_Pin1				(0x09)
#define SYSCFG_GPIOB_Pin2				(0x0A)
#define SYSCFG_GPIOB_Pin3				(0x0B)
#define SYSCFG_GPIOB_Pin4				(0x0C)
#define SYSCFG_GPIOB_Pin5				(0x0D)
#define SYSCFG_GPIOB_Pin6				(0x0E)
#define SYSCFG_GPIOB_Pin7				(0x0F)
#define SYSCFG_GPIOC_Pin0				(0x10)
#define SYSCFG_GPIOC_Pin1				(0x11)
#define SYSCFG_GPIOC_Pin2				(0x12)
#define SYSCFG_GPIOC_Pin3				(0x13)
#define SYSCFG_GPIOC_Pin4				(0x14)
#define SYSCFG_GPIOC_Pin5				(0x15)
#define SYSCFG_GPIOC_Pin6				(0x16)
#define SYSCFG_GPIOC_Pin7				(0x17)
#define SYSCFG_GPIOD_Pin0				(0x18)
#define SYSCFG_GPIOD_Pin1				(0x19)
#define SYSCFG_GPIOD_Pin2				(0x1A)
#define SYSCFG_GPIOD_Pin3				(0x1B)
#define SYSCFG_GPIOD_Pin4				(0x1C)
#define SYSCFG_GPIOD_Pin5				(0x1D)
#define SYSCFG_GPIOD_Pin6				(0x1E)
#define SYSCFG_GPIOD_Pin7				(0x1F)

void crm_cmd_periph_clock(uint32_t crm_periphclock, functional_state new_state);
void crm_reset_periph(uint32_t crm_periphreset);
void syscfg_cpu_sleep_cmd_wakeup_source(uint32_t syscfg_wakeup_source, functional_state new_state);
void syscfg_deep_sleep_set_mode(uint32_t syscfg_deep_sleep_mode);
void syscfg_deep_sleep_cmd_wakeup_source(uint32_t syscfg_deep_sleep_wakeup_source,functional_state new_state);
void syscfg_deep_sleep_set_sram(uint32_t Retention_Sram_x);
void syscfg_cmd_extern_16M_xtal(void);
void syscfg_cmd_extern_32M_xtal(void);
void syscfg_cmd_extern_32K_xtal(void);
void syscfg_cmd_rc_32K(void);
void syscfg_cmd_rc_16M(void);
void syscfg_common_config(void);

#ifdef __cplusplus
}
#endif

#endif 
