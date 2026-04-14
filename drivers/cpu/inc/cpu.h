/**
******************************************************************************************************************************************************************************************
*@file                       cpu.h
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
#ifndef CPU_H      /* ToDo: replace '<Device>' with your device name */
#define CPU_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum IRQn
{
/* -------------------  Cortex-M0 Processor Exceptions Numbers  ------------------- */
  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt                        */
  MemoryManagement_IRQn         = -12,      /*!<  4 Memory Management Interrupt                   */
  BusFault_IRQn                 = -11,      /*!<  5 Bus Fault Interrupt                           */
  UsageFault_IRQn               = -10,      /*!<  6 Usage Fault Interrupt                         */
  SVCall_IRQn                   = -5,       /*!< 11 SV Call Interrupt                             */
  DebugMonitor_IRQn             = -4,       /*!< 12 Debug Monitor Interrupt                       */
  PendSV_IRQn                   = -2,       /*!< 14 Pend SV Interrupt                             */
  SysTick_IRQn                  = -1,       /*!< 15 System Tick Interrupt                         */


/* ----------------------  ARMCM0 Specific Interrupt Numbers  --------------------- */
    RESERVE1                                        = 0,
    RESERVE2                                        = 1,
	UART0_IRQn	= 2,
    RESERVE3                                        = 3,
	SPI0_IRQn	= 4,
    RESERVE4                                        = 5,
	QDEC_IRQn	= 6,
    RESERVE5                                        = 7,
    TIMER_IRQn                                      = 8,
    RESERVE6                                        = 9,
    RESERVE7                                        = 10,
    RESERVE8                                        = 11,
    RESERVE9                                        = 12,
	RTC_IRQn	= 13,
    R24G_IRQn                                       = 14,
	RNG_IRQn	= 15,
	GPIO_INT0_IRQn	= 16,
	GPIO_INT1_IRQn	= 17,
	GPIO_INT2_IRQn	= 18,
	GPIO_INT3_IRQn	= 19,
	LIB_0_IRQn	= 20,
	LIB_1_IRQn	= 21,
	LIB_2_IRQn	= 22,
	LIB_3_IRQn	= 23,
	LIB_4_IRQn	= 24,
	LIB_5_IRQn	= 25,
	LIB_6_IRQn		= 26,
	LIB_7_IRQn	= 27,
	LIB_8_IRQn	= 28,
	LIB_9_IRQn	= 29,
	LIB_10_IRQn	= 30,
	LIB_11_IRQn	= 31,
	LIB_12_IRQn	= 32,
	LIB_13_IRQn			= 33,
	LIB_22_IRQn				= 34,
    AUDIO_ADC_IRQn                                  = 35,
	IRDA_IRQn				= 36,
	LIB_14_IRQn		= 37,
	LIB_15_IRQn		= 38,
  LIB_16_IRQn             =39,
  LIB_17_IRQn          =40,
  LIB_18_IRQn               = 41,
  LIB_19_IRQn         =42,
  LIB_20_IRQn            =43,
} IRQn_Type;

#define __CPU_REV                 0x0200    /// Core Revision r2p0                               */
#define __NVIC_PRIO_BITS          3         /// Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /// Set to 1 if different SysTick Config is used     */
#define __MPU_PRESENT             0         /// MPU present or not                               */
/// ToDo: define __FPU_PRESENT if your devise is a CORTEX-M4                                       */
#define __FPU_PRESENT             0         /// FPU present or not 


#include "cpu_core.h"                       /// Cortex-M# processor and core peripherals           */
#include "system_cpu.h"                     /// <Device> System  include file                      */
#include <stdint.h>

#undef TRUE
#undef FALSE
typedef enum {FALSE   = 0, TRUE    = !FALSE  } boolean;
typedef enum {LOW     = 0, HIGH    = 1       } level;
typedef enum {RESET   = 0, SET     = !RESET  } flagstatus, itstatus;
typedef enum {DISABLE = 0, ENABLE  = !DISABLE} functional_state;
typedef enum {ERROR   = 0, SUCCESS = !ERROR  } errorstatus;

#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

/* ToDo: add here your device specific peripheral access structure typedefs
         following is an example for a timer                                  */
#define BIT0        (0x00000001U)
#define BIT1        (0x00000002U)
#define BIT2        (0x00000004U)
#define BIT3        (0x00000008U)
#define BIT4        (0x00000010U)
#define BIT5        (0x00000020U)
#define BIT6        (0x00000040U)
#define BIT7        (0x00000080U)
#define BIT8        (0x00000100U)
#define BIT9        (0x00000200U)
#define BIT10       (0x00000400U)
#define BIT11       (0x00000800U)
#define BIT12       (0x00001000U)
#define BIT13       (0x00002000U)
#define BIT14       (0x00004000U)
#define BIT15       (0x00008000U)
#define BIT16       (0x00010000U)
#define BIT17       (0x00020000U)
#define BIT18       (0x00040000U)
#define BIT19       (0x00080000U)
#define BIT20       (0x00100000U)
#define BIT21       (0x00200000U)
#define BIT22       (0x00400000U)
#define BIT23       (0x00800000U)
#define BIT24       (0x01000000U)
#define BIT25       (0x02000000U)
#define BIT26       (0x04000000U)
#define BIT27       (0x08000000U)
#define BIT28       (0x10000000U)
#define BIT29       (0x20000000U)
#define BIT30       (0x40000000U)
#define BIT31       (0x80000000U)


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/

/* Peripheral and SRAM base address */
#define CPU_FLASH_BASE           (0x00000000UL)                              /*!< (FLASH     ) Base Address */
#define CPU_SRAM_BASE            (0x20000000UL)                              /*!< (SRAM      ) Base Address */
#define CPU_PERIPH_BASE          (0x40000000UL)                              /*!< (Peripheral) Base Address */

/* Peripheral memory map */
#define CPU_AHB_BASE                (CPU_PERIPH_BASE)
#define CPU_APB0_BASE               (0x50000000UL)

#define FLASH_BASE                  (0x01000000UL)

#define BLE_BASE                    (0x30007000UL)
#define CRM_BASE                    (CPU_AHB_BASE + 0x6000)
#define LIB_22_BASE                   (CPU_AHB_BASE + 0x9000)
#define CACHE_BASE                  (CPU_AHB_BASE + 0x5020)

#define UART0_BASE                  (CPU_APB0_BASE + 0x1000)
#define SPI0_BASE                   (CPU_APB0_BASE + 0x3000)
#define QDEC_BASE                   (CPU_APB0_BASE + 0x6000)
#define SYSCFG_BASE                 (CPU_APB0_BASE + 0x8000)
#define TIMER_BASE                  (CPU_APB0_BASE + 0xA000)
#define IWATCHDOG_BASE              (CPU_APB0_BASE + 0x5000)
#define RTC_BASE                    (CPU_APB0_BASE + 0xD000)
#define GPIO_BASE                   (CPU_APB0_BASE + 0xE000)
#define IRDA_BASE               	(CPU_APB0_BASE + 0x51000)
#define AUADC_BASE                  (CPU_APB0_BASE + 0x50000)
#define HW_RF_SPI0_BASE				(0x50052000)

#define GPIOA_INOUT_BASE            (GPIO_BASE)
#define GPIOB_INOUT_BASE            (GPIO_BASE + 0x10)
#define GPIOC_INOUT_BASE            (GPIO_BASE + 0x20)
#define GPIOD_INOUT_BASE            (GPIO_BASE + 0x30)
#define GPIOA_MAP_BASE              (GPIO_BASE + 0x40)
#define GPIOB_MAP_BASE              (GPIO_BASE + 0x48)
#define GPIOC_MAP_BASE              (GPIO_BASE + 0x50)
#define GPIOD_MAP_BASE              (GPIO_BASE + 0x58)

/*
* @brief: peripheral
* 1. CRM
* 2. SYSCTRL
* 3. GPIO
* 4. RTC
* 5. IWDT 
* 6. TIMER
* 7. UART
* 8. SPI
* 10.GPADC
* 11.QUADDEC
*/

/********************************************** CRM **************************************************/
#define CRM_CG_CTRL_HW_RF_SPI               (BIT22)
#define CRM_CG_CTRL_IRDA                    (BIT21)
#define CRM_CG_CTRL_AUADC                   (BIT20)
#define CRM_CG_CTRL_IWDT                    (BIT19)
#define CRM_CG_CTRL_LIB_22                    (BIT18)
#define CRM_CG_CTRL_CRC                     (BIT17)
#define CRM_CG_CTRL_BT                      (BIT16)
#define CRM_CG_CTRL_DMA					    (BIT15)
#define CRM_CG_CTRL_GPADC                   (BIT14)
#define CRM_CG_CTRL_QDEC                    (BIT13)
#define CRM_CG_CTRL_GPIO                    (BIT12)
#define CRM_CG_CTRL_RTC                     (BIT11)
#define CRM_CG_CTRL_WDT                     (BIT10)
#define CRM_CG_CTRL_TIMER                   (BIT8)
#define CRM_CG_CTRL_RNG                     (BIT7)  
#define CRM_CG_CTRL_SYSCFG                  (BIT6)
#define CRM_CG_CTRL_KPC                     (BIT5)
#define CRM_CG_CTRL_SPI1                    (BIT4)
#define CRM_CG_CTRL_SPI0                    (BIT3)
#define CRM_CG_CTRL_UART1                   (BIT2)
#define CRM_CG_CTRL_UART0                   (BIT1)
#define CRM_CG_CTRL_I2C                     (BIT0)


#define CRM_SOFT_RST_HW_RF_SPI              (BIT28)
#define CRM_SOFT_RST_MDM                    (BIT27)
#define CRM_SOFT_RST_IRDA                   (BIT26)
#define CRM_SOFT_RST_AUADC                  (BIT25)
#define CRM_SOFT_RST_GLB                    (BIT24)
#define CRM_SOFT_RST_IWDT                   (BIT23)
#define CRM_SOFT_RST_LIB_22                   (BIT22)
#define CRM_SOFT_RST_CRC                    (BIT19)
#define CRM_SOFT_RST_CM3                    (BIT18)
#define CRM_SOFT_RST_CRM                    (BIT17)
#define CRM_SOFT_RST_BT                     (BIT16)
#define CRM_SOFT_RST_DMA 				    (BIT15)
#define CRM_SOFT_RST_GPADC                  (BIT14)
#define CRM_SOFT_RST_QDEC                   (BIT13)
#define CRM_SOFT_RST_GPIO                   (BIT12)
#define CRM_SOFT_RST_RTC                    (BIT11)
#define CRM_SOFT_RST_WDT                    (BIT10)
#define CRM_SOFT_RST_TIMER                  (BIT8)
#define CRM_SOFT_RST_RNG                    (BIT7)
#define CRM_SOFT_RST_SYSCFG                 (BIT6)
#define CRM_SOFT_RST_KPC                    (BIT5)
#define CRM_SOFT_RST_SPI1                   (BIT4)
#define CRM_SOFT_RST_SPI0                   (BIT3)
#define CRM_SOFT_RST_UART1                  (BIT2)
#define CRM_SOFT_RST_UART0                  (BIT1)
#define CRM_SOFT_RST_I2C                    (BIT0)

typedef struct
{
    __IO uint32_t CG_CTRL;
    __IO uint32_t SOFT_RST;
} CRM_TypeDef;

#define CRM                                     ((CRM_TypeDef    *)CRM_BASE)


/********************************************** SYSCTRL**************************************************/

#define SYSCFG_SPI_SLAVE_SEL_SPI1_SLV_SEL               (BIT1)
#define SYSCFG_SPI_SLAVE_SEL_SPI0_SLV_SEL               (BIT0)

#define SYSCFG_WAKE_MCU_CTRL_qdec                       (BIT16)
#define SYSCFG_WAKE_MCU_CTRL_bt                         (BIT15)
#define SYSCFG_WAKE_MCU_CTRL_gpio_int3                  (BIT14)
#define SYSCFG_WAKE_MCU_CTRL_gpio_int2                  (BIT13)
#define SYSCFG_WAKE_MCU_CTRL_gpio_int1                  (BIT12)
#define SYSCFG_WAKE_MCU_CTRL_gpio_int0                  (BIT11)
#define SYSCFG_WAKE_MCU_CTRL_rtc                        (BIT10)
#define SYSCFG_WAKE_MCU_CTRL_timer3                     (BIT9)
#define SYSCFG_WAKE_MCU_CTRL_timer2                     (BIT8)
#define SYSCFG_WAKE_MCU_CTRL_timer1                     (BIT7)
#define SYSCFG_WAKE_MCU_CTRL_timer0                     (BIT6)
#define SYSCFG_WAKE_MCU_CTRL_kpc                        (BIT5)
#define SYSCFG_WAKE_MCU_CTRL_spi1                       (BIT4)
#define SYSCFG_WAKE_MCU_CTRL_spi0                       (BIT3)
#define SYSCFG_WAKE_MCU_CTRL_uart1                      (BIT2)
#define SYSCFG_WAKE_MCU_CTRL_uart0                      (BIT1)
#define SYSCFG_WAKE_MCU_CTRL_i2c                        (BIT0)

#define SYSCFG_SLEEP_CTRL_sram_ret_sel                  (BIT16|BIT17)
#define SYSCFG_SLEEP_CTRL_ble_extwkupdsb                (BIT11)
#define SYSCFG_SLEEP_CTRL_gpio_wk_en                    (BIT10)
#define SYSCFG_SLEEP_CTRL_kpc_wk_en                     (BIT9)
#define SYSCFG_SLEEP_CTRL_rtc_wk_en                     (BIT8)
#define SYSCFG_SLEEP_CTRL_deep_slp_sel                  (BIT0|BIT1)

typedef struct
{
    __IO uint32_t   HFCLK_SEL;
    __IO uint32_t   LFCLK_SEL;
    __IO uint32_t   FREQ_SEL;
    __IO uint32_t   CALIB_EN;
    __IO uint32_t   CALIB_VAL;
    __IO uint32_t   CNT32K_VAL;
    __IO uint32_t   RESERVED0[2];
    __IO uint32_t   SLEEP_CTRL;
    __IO uint32_t   WKUP_TYPE;
    __IO uint32_t   WKUP_SRC;
    __IO uint32_t   RESERVED1[5];
    __IO uint32_t   WAKE_MCU_CTRL;
    __IO uint32_t  	RESERVED2[7];    
    __IO uint32_t   TIMER_PAUSE ;
    __IO uint32_t   RESERVED4[3];
    __IO uint32_t   RTC_DIV_VAL;
    __IO uint32_t   RTC_DIV_CNT;
    __IO uint32_t   RESERVED5[10];
    __IO uint32_t   DEV_PARAM;
    __IO uint32_t   DEVTEMP_N0;
    __IO uint32_t   DEV_TEMP_N1;
    __IO uint32_t   DEV_CUR_VAL;
    __IO uint32_t   RSSI_TEM_CAL;
    __IO uint32_t   RSSI_RFGAIN_ERR1;
    __IO uint32_t   RSSI_RFGAIN_ERR2;
    __IO uint32_t   RESERVED6[1];  
    __IO uint32_t   TEST1;
    __IO uint32_t   TEST2;
    __IO uint32_t   TEST3;
    __IO uint32_t   TEST4;
    __IO uint32_t   RESERVED7[5];     
    __IO uint32_t	RAM_DIS;
    __IO uint32_t   RESERVED8[1];	
    __IO uint32_t	RF_ONLY;
    __IO uint32_t  	JTAG_DIS;
    __IO uint32_t  	RESET_FLAG;
	__IO uint32_t  	FCU_PROT;
    __IO uint32_t   BTDM_DIAG;
}SYSCFG_TypeDef;

#define SYSCFG                                  ((SYSCFG_TypeDef *) SYSCFG_BASE)

/********************************************** GPIO **************************************************/
#define GPIO_INTP_TYPE_EN_INTP0_Pos             (0)
#define GPIO_INTP_TYPE_EN_INTP0_Mask            (0x03U<<GPIO_INTP_TYPE_EN_INTP0_Pos)

#define GPIO_INTP_TYPE_EN_INTP1_Pos             (2)
#define GPIO_INTP_TYPE_EN_INTP1_Mask            (0x03U<<GPIO_INTP_TYPE_EN_INTP1_Pos)

#define GPIO_INTP_TYPE_EN_INTP2_Pos             (4)
#define GPIO_INTP_TYPE_EN_INTP2_Mask            (0x03U<<GPIO_INTP_TYPE_EN_INTP2_Pos)

#define GPIO_INTP_TYPE_EN_INTP3_Pos             (6)
#define GPIO_INTP_TYPE_EN_INTP3_Mask            (0x03U<<GPIO_INTP_TYPE_EN_INTP3_Pos)

#define GPIO_INTP_TYPE_EN_INTP4_Pos             (8)
#define GPIO_INTP_TYPE_EN_INTP4_Mask            (0x03U<<GPIO_INTP_TYPE_EN_INTP4_Pos)

#define GPIO_INTP_TYPE_EN_INTP5_Pos             (10)
#define GPIO_INTP_TYPE_EN_INTP5_Mask            (0x03U<<GPIO_INTP_TYPE_EN_INTP5_Pos)

#define GPIO_INTP_TYPE_EN_INTP6_Pos             (12)
#define GPIO_INTP_TYPE_EN_INTP6_Mask            (0x03U<<GPIO_INTP_TYPE_EN_INTP6_Pos)

#define GPIO_INTP_TYPE_EN_INTP7_Pos             (14)
#define GPIO_INTP_TYPE_EN_INTP7_Mask            (0x03U<<GPIO_INTP_TYPE_EN_INTP7_Pos)

#define GPIO_INTP_SRC_H_INTP4_Pos               (0)
#define GPIO_INTP_SRC_H_INTP4_Mask              (0xFFU<<GPIO_INTP_SRC_H_INTP4_Pos)

#define GPIO_INTP_SRC_H_INTP5_Pos               (8)
#define GPIO_INTP_SRC_H_INTP5_Mask              (0xFFU<<GPIO_INTP_SRC_H_INTP5_Pos)

#define GPIO_INTP_SRC_H_INTP6_Pos               (16)
#define GPIO_INTP_SRC_H_INTP6_Mask              (0xFFU<<GPIO_INTP_SRC_H_INTP6_Pos)

#define GPIO_INTP_SRC_H_INTP7_Pos               (24)
#define GPIO_INTP_SRC_H_INTP7_Mask              (0xFFU<<GPIO_INTP_SRC_H_INTP7_Pos)

#define GPIO_INTP_SRC_L_INTP0_Pos               (0)
#define GPIO_INTP_SRC_L_INTP0_Mask              (0xFFU<<GPIO_INTP_SRC_L_INTP0_Pos)

#define GPIO_INTP_SRC_L_INTP1_Pos               (0)
#define GPIO_INTP_SRC_L_INTP1_Mask              (0xFFU<<GPIO_INTP_SRC_L_INTP1_Pos)

#define GPIO_INTP_SRC_L_INTP2_Pos               (0)
#define GPIO_INTP_SRC_L_INTP2_Mask              (0xFFU<<GPIO_INTP_SRC_L_INTP2_Pos)

#define GPIO_INTP_SRC_L_INTP3_Pos               (0)
#define GPIO_INTP_SRC_L_INTP3_Mask              (0xFFU<<GPIO_INTP_SRC_L_INTP3_Pos)

#define GPIO_INTP_CLR_INTP0_Pos                 (0)
#define GPIO_INTP_CLR_INTP0_Mask                (0x01U<<GPIO_INTP_CLR_INTP0_Pos)

#define GPIO_INTP_CLR_INTP1_Pos                 (1)
#define GPIO_INTP_CLR_INTP1_Mask                (0x01U<<GPIO_INTP_CLR_INTP1_Pos)

#define GPIO_INTP_CLR_INTP2_Pos                 (2)
#define GPIO_INTP_CLR_INTP2_Mask                (0x01U<<GPIO_INTP_CLR_INTP2_Pos)

#define GPIO_INTP_CLR_INTP3_Pos                 (3)
#define GPIO_INTP_CLR_INTP3_Mask                (0x01U<<GPIO_INTP_CLR_INTP3_Pos)

#define GPIO_INTP_CLR_INTP4_Pos                 (4)
#define GPIO_INTP_CLR_INTP4_Mask                (0x01U<<GPIO_INTP_CLR_INTP4_Pos)

#define GPIO_INTP_CLR_INTP5_Pos                 (5)
#define GPIO_INTP_CLR_INTP5_Mask                (0x01U<<GPIO_INTP_CLR_INTP5_Pos)

#define GPIO_INTP_CLR_INTP6_Pos                 (6)
#define GPIO_INTP_CLR_INTP6_Mask                (0x01U<<GPIO_INTP_CLR_INTP6_Pos)

#define GPIO_INTP_CLR_INTP7_Pos                 (7)
#define GPIO_INTP_CLR_INTP7_Mask                (0x01U<<GPIO_INTP_CLR_INTP7_Pos)

typedef struct
{
    __IO uint32_t IOD;
    __IO uint32_t BSR;
    __IO uint32_t CON;
} GPIO_INOUT_TypeDef;

typedef struct
{
    __IO uint32_t MAP_MODE[2];
} GPIO_MAP_TypeDef;

typedef struct
{
    GPIO_INOUT_TypeDef * inout_ctrl;
    GPIO_MAP_TypeDef * map_ctrl;
}GPIO_TypeDef;

typedef struct
{
    __IO uint32_t   PA_IOD  ;
    __IO uint32_t   PA_BSR  ;
    __IO uint32_t   PA_CON  ;
    __IO uint32_t   RESERVED0   ;   
    __IO uint32_t   PB_IOD  ;
    __IO uint32_t   PB_BSR  ;
    __IO uint32_t   PB_CON  ;
    __IO uint32_t   RESERVED1   ;   
    __IO uint32_t   PC_IOD  ;
    __IO uint32_t   PC_BSR  ;
    __IO uint32_t   PC_CON  ;
    __IO uint32_t   RESERVED2   ;   
    __IO uint32_t   PD_IOD  ;
    __IO uint32_t   PD_BSR  ;
    __IO uint32_t   PD_CON  ;
    __IO uint32_t   RESERVED3   ;   
    __IO uint32_t   PA_L_MODE   ;
    __IO uint32_t   PA_H_MODE   ;
    __IO uint32_t   PB_L_MODE   ;
    __IO uint32_t   PB_H_MODE   ;
    __IO uint32_t   PC_L_MODE   ;
    __IO uint32_t   PC_H_MODE   ;
    __IO uint32_t   PD_L_MODE   ;
    __IO uint32_t   PD_H_MODE   ;
    __IO uint32_t   RESERVED4[8]    ;   
    __IO uint32_t   INTP_TYPE   ;
    __IO uint32_t   INTP_SRC    ;
    __IO uint32_t   INTP_STAT   ;

}GPIO_MODULE_TypeDef;

#define GPIO_MODULE                     ((GPIO_MODULE_TypeDef*)GPIO_BASE)
#define GPIOA_INOUT_CTRL                ((GPIO_INOUT_TypeDef *)(GPIOA_INOUT_BASE))
#define GPIOB_INOUT_CTRL                ((GPIO_INOUT_TypeDef *)(GPIOB_INOUT_BASE))
#define GPIOC_INOUT_CTRL                ((GPIO_INOUT_TypeDef *)(GPIOC_INOUT_BASE))
#define GPIOD_INOUT_CTRL                ((GPIO_INOUT_TypeDef *)(GPIOD_INOUT_BASE))
#define GPIOA_MAP_CTRL                  ((GPIO_MAP_TypeDef   *)(GPIOA_MAP_BASE))
#define GPIOB_MAP_CTRL                  ((GPIO_MAP_TypeDef   *)(GPIOB_MAP_BASE))
#define GPIOC_MAP_CTRL                  ((GPIO_MAP_TypeDef   *)(GPIOC_MAP_BASE))
#define GPIOD_MAP_CTRL                  ((GPIO_MAP_TypeDef   *)(GPIOD_MAP_BASE))
extern GPIO_TypeDef *GPIOA;
extern GPIO_TypeDef *GPIOB;
extern GPIO_TypeDef *GPIOC;
extern GPIO_TypeDef *GPIOD;

/********************************************** RTC **************************************************/
/************ bit definition for RTC  RTC_CCR REGISTER ************/
#define RTC_RTC_CCR_RTC_WEN                             (BIT3)
#define RTC_RTC_CCR_RTC_EN                              (BIT2)
#define RTC_RTC_CCR_RTC_Mask                            (BIT1)
#define RTC_RTC_CCR_RTC_ien                             (BIT0)

typedef struct{
    __IO uint32_t       RTC_CCVR;
    __IO uint32_t       RTC_CMR;
    __IO uint32_t       RTC_CLR;
    __IO uint32_t       RTC_CCR;
    __IO uint32_t       RTC_STAT;
    __IO uint32_t       RTC_RSTAT;
    __IO uint32_t       RTC_EOI;
    __IO uint32_t       RTC_COMP_VERSION;
} RTC_TypeDef;
#define RTC                                     ((RTC_TypeDef *)(RTC_BASE))

/********************************************** IWDT **************************************************/

#define IWDT_IWDT_CR_IWDT_EN           (BIT0)
#define IWDT_IWDT_CR_RMOD             (BIT1)

#define IWDT_IWDT_STATE           (BIT0)

typedef struct
{
    __IO uint32_t IWDT_CR;
    __IO uint32_t RESERVED0;
    __I  uint32_t IWDT_CCVR;
    __O  uint32_t IWDT_CRR;
    __I  uint32_t IWDT_STAT;
    __I  uint32_t IWDT_EOI;  
    __I  uint32_t RESERVED1;
    __IO uint32_t IWDT_RLD;
//    __I  uint32_t RESERVED[53];
//    __I  uint32_t IWDT_COMP_PARAMS_1;
//    __I  uint32_t IWDT_COMP_VERSION;
//    __I  uint32_t IWDT_COMP_TYPE;
} IWDT_TypeDef;
#define IWDT                                     ((IWDT_TypeDef    *) IWATCHDOG_BASE)

/************ bit definition for TIMER CONTROL_REG REGISTER ************/


/*******************  Bit definition for TIM_CR1 register  ********************/
typedef struct
{
    
    __IO uint32_t TIMER_EN;
    __IO uint32_t TIMER_UPDATE_EN;
    __IO uint32_t TIMER_INT_EN;
    __IO uint32_t TIMER_INT_STATE;    
    __IO uint32_t TIMER_LOAD_COUNTER_0;
    __IO uint32_t TIMER_TOGSET_0;  
    __I  uint32_t TIMER_CURRENT_VAL_0;
    __IO uint32_t RESERVED0;
    __IO uint32_t TIMER_LOAD_COUNTER_1;
    __IO uint32_t TIMER_TOGSET_1;  
    __I  uint32_t TIMER_CURRENT_VAL_1;
    __IO uint32_t RESERVED1;
    __IO uint32_t TIMER_LOAD_COUNTER_2;
    __IO uint32_t TIMER_TOGSET_2;  
    __I  uint32_t TIMER_CURRENT_VAL_2;
    __IO uint32_t RESERVED2;
    __IO uint32_t TIMER_LOAD_COUNTER_3;
    __IO uint32_t TIMER_TOGSET_3;  
    __I  uint32_t TIMER_CURRENT_VAL_3;
    __IO uint32_t RESERVED3;
    __IO uint32_t TIMER_LOAD_COUNTER_4;
    __IO uint32_t TIMER_TOGSET_4;  
    __I  uint32_t TIMER_CURRENT_VAL_4;
    __IO uint32_t RESERVED4;
    __IO uint32_t TIMER_LOAD_COUNTER_5;
    __IO uint32_t TIMER_TOGSET_5;  
    __I  uint32_t TIMER_CURRENT_VAL_5;

} TIMER_TypeDef;

#define TIMER                               ((TIMER_TypeDef *) TIMER_BASE)
         
/********************************************** UART **************************************************/

#define UART_INT_EMPTY_TXIM            0x00001000  //UART FIFO empty interrupt mask;
#define UART_INT_XOFFIM                0x00000800  //XOFF interrupt mask;
#define UART_IT_RXIM                   0x00000010  //UART rx interrupt mask
#define UART_IT_TXIM                   0x00000020  //UART tx interrupt mask
#define UART_IT_RX_TIMEOUT_IM          0x00000040  //UART tx interrupt mask 




#define UART_IT_RX_STATUS               0x00000010  //UART rx interrupt status
#define UART_IT_TX_STATUS               0x00000020  //UART tx interrupt status
#define UART_IT_RX_TIMEOUT_STATUS       0x00000040  //UART rx timeout interrupt status
#define UART_IT_FRAMING_ERROR_STATUS    0x00000080  //UART framing error interrupt status(Ö¡´íÎó)

#define UART_IT_TX_EMPTY_STATUS         0x00001000  //UART tx fifo empty interrupt status

#define UART_IT_STATUS               0x1010      // tx fifo empty interrupt mask and rx interrupt status


/** @defgroup UART_Status_Flag_define
  * @{
  */
#define UART_STATUS_BUSY							((uint32_t)0x0008)
#define UART_STATUS_RX_FIFO_EMPTY				    ((uint32_t)0x0010)
#define UART_STATUS_TX_FIFO_FULL					((uint32_t)0x0020)
#define UART_STATUS_RX_FIFO_FULL				    ((uint32_t)0x0040)
#define UART_STATUS_TX_FIFO_EMPTY					((uint32_t)0x0080)

typedef struct
{
   __IO   uint32_t DR;                          //00
   __IO   uint32_t RSR_ECR;
   __IO   uint32_t DMAWM;
   __IO   uint32_t TIMEOUT;
   __I    uint32_t Reserved1[2];
   __I    uint32_t FR;
   __IO   uint32_t LCRH_RX;
   __IO   uint32_t ILPR;
   __IO   uint32_t IBRD;
   __IO   uint32_t FBRD;
   __IO   uint32_t LCRH_TX;
   __IO   uint32_t CR;
   __IO   uint32_t IFLS;
   __IO   uint32_t IMSC;
   __I    uint32_t RIS;
   __I    uint32_t MIS;
   __IO   uint32_t ICR;
   __IO   uint32_t DMACR;
   __I    uint32_t Reserved2;
   __IO   uint32_t XFCR;
   __IO   uint32_t XON1;
   __IO   uint32_t XON2;
   __IO   uint32_t XOFF1;
   __IO   uint32_t XOFF2;
   __I    uint32_t Reserved3[7];
   __IO   uint32_t ITCR;
   __IO   uint32_t ITIP;
   __IO   uint32_t ITOP;
   __IO   uint32_t TDR;
   __I    uint32_t Reserved4[4];
   __IO   uint32_t ABCR;
   __I    uint32_t ABSR;
   __I    uint32_t ABFMT;
   __I    uint32_t ABDR;
   __I    uint32_t ABDFR;
   __I    uint32_t ABMR;
   __IO   uint32_t ABIMSC;
   __I    uint32_t ABRIS;
   __I    uint32_t ABMIS;
   __IO   uint32_t ABICR;
   __I    uint32_t Reserved5[922];
   __I    uint32_t ID_PRODUCT_H_XY;
   __I    uint32_t ID_PROVIDER;
   __I    uint32_t PERIPHID0;
   __I    uint32_t PERIPHID1;
   __I    uint32_t PERIPHID2;
   __I    uint32_t PERIPHID3;
   __I    uint32_t PCELLID0;
   __I    uint32_t PCELLID1;
   __I    uint32_t PCELLID2;
   __I    uint32_t PCELLID3;
} UART_TypeDef;
#define UART0                                   ((UART_TypeDef *) UART0_BASE)

/********************************************** SPI **************************************************/
#define SPI_SR_RFNE         (BIT3)  
#define SPI_SR_TFE          (BIT2)

typedef struct
{
    __IO uint16_t CTRLR0;
    uint16_t RESERVED0;
    __IO uint16_t CTRLR1;
    uint16_t RESERVED1;
    __IO uint32_t SSIENR;
    __IO uint32_t MWCR;
    __IO uint32_t SER;
    __IO uint32_t BAUDR;
    __IO uint32_t TXFTLR;
    __IO uint32_t RXFTLR;
    __IO uint32_t TXFLR;
    __I  uint32_t RXFLR;
    __I  uint32_t SR;
    __IO uint32_t IMR;
    __I  uint32_t ISR;
    __I  uint32_t RISR;
    __I  uint32_t TXOICR;
    __I  uint32_t RXOICR;
    __I  uint32_t RXUICR;
    __I  uint32_t MSTICR;
    __I  uint32_t ICR;
    __IO uint32_t DMACR;
    __IO uint32_t DMATDLR;
    __IO uint32_t DMARDLR;
    __I  uint32_t IDR;
    __I  uint32_t SSI_COMP_VERSION;
    __IO uint32_t DR;
    __IO uint32_t DR_Array[35];
    __IO uint32_t RX_SAMPLE_DLY;    
} SPI_TypeDef;

#define SPI0                                    ((SPI_TypeDef    *) SPI0_BASE)

/********************************************** QDEC **************************************************/

typedef struct
{
    __IO uint32_t QCR;
    __IO uint32_t QSR;
    __IO uint32_t QIR;
    __IO int32_t QRW;
    __IO uint32_t QCK;
} QDEC_TypeDef;
#define QDEC                                    ((QDEC_TypeDef *)QDEC_BASE)

/********************************************** GPADC **************************************************/
typedef struct
{
    __IO uint32_t GPADC_CONF;
    __IO uint32_t GPADC_CTRL;
    __I  uint32_t GPADC_FIFOLEVEL;
    __IO uint32_t GPADC_THRESHOLD;
    __O  uint32_t GPADC_FIFOFLUSH;
    __I  uint32_t GPADC_FIFODATA;
    __IO uint32_t GPADC_STAT;
    __IO uint32_t GPADC_INT_CTRL;
    __I  uint32_t GPADC_DATA;
    __IO uint32_t DMA_CR;
    __IO uint32_t DMA_RDLR;
} GPADC_TypeDef;

#define GPADC_BASE                          (CPU_APB0_BASE + 0xF000)
#define GPADC                               ((GPADC_TypeDef *)GPADC_BASE)

/********************************************** QUADDEC **************************************************/
#define QDEC_QCR_INRC               (BIT9)              
#define QDEC_QCR_IDXL               (BIT8)        
#define QDEC_QCR_ICHB               (BIT7)        
#define QDEC_QCR_ICHA               (BIT6)        
#define QDEC_QCR_QLAT               (BIT5)        
#define QDEC_QCR_PLCT               (BIT4)        
#define QDEC_QCR_INZC               (BIT3)        
#define QDEC_QCR_INEN               (BIT2)        
#define QDEC_QCR_CTDR               (BIT1)        
#define QDEC_QCR_ECNT               (BIT0)        

#define QDEC_QSR_CMI                (BIT4)
#define QDEC_QSR_INI                (BIT3)
#define QDEC_QSR_UNI                (BIT2)
#define QDEC_QSR_OVI                (BIT1)
#define QDEC_QSR_QEI                (BIT0)

#define QDEC_QIR_CMIE               (BIT4)
#define QDEC_QIR_INIE               (BIT3)
#define QDEC_QIR_UNIE               (BIT2)
#define QDEC_QIR_OVIE               (BIT1)
#define QDEC_QIR_QEIE               (BIT0)

#define QDEC_QCK_QCK_Mask           (0x1FU)

/********************************************** KEYBOARD **************************************************/

typedef struct
{
    __IO uint32_t kpc_ctrl;
    __IO uint32_t kpc_cfg1;
    __IO uint32_t kpc_cfg2;
    __IO uint32_t kpc_stat;
    __IO uint32_t kpc_int_ctrl;
    __I  uint32_t kpc_data;
} KPC_TypeDef;


/********************************************** BLE MODLUE **************************************************/
typedef struct
{
    __IO uint32_t   BTLECNTL  ;
    __IO uint32_t   VERSION ;
    __IO uint32_t   BLECONF   ;
    __IO uint32_t   INTCNTL ;
    __IO uint32_t   INTSTAT ;
    __IO uint32_t   INTRAWSTAT  ;
    __IO uint32_t   INTACK  ;
    __IO uint32_t   BASETIMECNT ;
    __IO uint32_t   FINETIMECNT ;
    __IO uint32_t   BDADDRL ;
    __IO uint32_t   BDADDRU ;
    __IO uint32_t   CURRENTRXDESCPTR    ;
    __IO uint32_t   DEEPSLCNTL  ;
    __IO uint32_t   DEEPSLWKUP  ;
    __IO uint32_t   DEEPSLSTAT  ;
    __IO uint32_t   ENBPRESET   ;
    __IO uint32_t   FINECNTCORR ;
    __IO uint32_t   BASETIMECNTCORR ;
    __IO uint32_t   reg_4_18    ;
    __IO uint32_t   reg_4_19    ;
    __IO uint32_t   DIAGCNTL    ;
    __IO uint32_t   DIAGSTAT    ;
    __IO uint32_t   DEBUGADDMAX ;
    __IO uint32_t   DEBUGADDMIN ;
    __IO uint32_t   ERRORTYPESTAT   ;
    __IO uint32_t   SWPROFILING ;
    __IO uint32_t   reg_4_26    ;
    __IO uint32_t   reg_4_27    ;
    __IO uint32_t   RADIOCNTL0  ;
    __IO uint32_t   RADIOCNTL1  ;
    __IO uint32_t   RADIOCNTL2  ;
    __IO uint32_t   RADIOCNTL3  ;
    __IO uint32_t   RADIOPWRUPDN    ;
    __IO uint32_t   SPIPTRCNTL0 ;
    __IO uint32_t   SPIPTRCNTL1 ;
    __IO uint32_t   SPIPTRCNTL2 ;
    __IO uint32_t   ADVCHMAP    ;
    __IO uint32_t   reg_4_37    ;
    __IO uint32_t   reg_4_38    ;
    __IO uint32_t   reg_4_39    ;
    __IO uint32_t   ADVTIM  ;
    __IO uint32_t   ACTSCANSTAT ;
    __IO uint32_t   reg_4_42    ;
    __IO uint32_t   reg_4_43    ;
    __IO uint32_t   WLPUBADDPTR ;
    __IO uint32_t   WLPRIVADDPTR    ;
    __IO uint32_t   WLNBDEV ;
    __IO uint32_t   reg_4_47    ;
    __IO uint32_t   AESCNTL ;
    __IO uint32_t   AESKEY31_0  ;
    __IO uint32_t   AESKEY63_32 ;
    __IO uint32_t   AESKEY95_64 ;
    __IO uint32_t   AESKEY127_96    ;
    __IO uint32_t   AESPTR  ;
    __IO uint32_t   TXMICVAL    ;
    __IO uint32_t   RXMICVAL    ;
    __IO uint32_t   RFTESTCNTL  ;
    __IO uint32_t   RFTESTTXSTAT    ;
    __IO uint32_t   RFTESTRXSTAT    ;
    __IO uint32_t   reg_4_59    ;
    __IO uint32_t   TIMGENCNTL  ;
    __IO uint32_t   GROSSTIMTGT ;
    __IO uint32_t   FINETIMTGT  ;
    __IO uint32_t   reg_4_fc    ;
    __IO uint32_t   COEXIFCNTL0 ;
    __IO uint32_t   COEXIFCNTL1 ;
    __IO uint32_t   COEXIFCNTL2 ;
    __IO uint32_t   BLEMPRIO0   ;
    __IO uint32_t   BLEMPRIO1   ;
    __IO uint32_t   reg_4_114   ;
    __IO uint32_t   reg_4_118   ;
    __IO uint32_t   reg_4_11c   ;
    __IO uint32_t   RALPTR  ;
    __IO uint32_t   RALNBDEV    ;
    __IO uint32_t   RAL_LOCAL_RND   ;
    __IO uint32_t   RAL_PEER_RND    ;
    __IO uint32_t   BLEPRIOSCHARB   ;
   
} BLE_TypeDef;

typedef struct{
	__IO uint32_t 	modemcfg1;
	__IO uint32_t 	modemcfg2;
	__IO uint32_t 	modemcfg3;
	__IO uint32_t 	modemcfg4;
	__IO uint32_t 	modemcfg5;
	__IO uint32_t 	modemcfg6;
	__IO uint32_t 	modemrdst1;
	__IO uint32_t 	modemrdst2;
	__IO uint32_t 	rfcfg1;
	__IO uint32_t 	rfcfg2;
	__IO uint32_t 	rfcfg3;
	__IO uint32_t 	rfcfg4;
	__IO uint32_t 	rfcfg5;
	__IO uint32_t 	rfcfg6;
	__IO uint32_t 	rfcfg7;
	__IO uint32_t 	rfcfg8;
	__IO uint32_t 	rfrdst1;
	__IO uint32_t 	rfrdst2;
	__IO uint32_t reserved[22];
	__IO uint32_t modecfg7;

} RFMODEM_TypeDef;

typedef struct
{
	
	__IO uint32_t SPI_CLKDIV;
	__IO uint32_t TXUP_CMD0_04;
	__IO uint32_t TXUP_CMD1_08;
	__IO uint32_t TXUP_CMD2_0C;
	__IO uint32_t TXUP_CMD3_10;
	__IO uint32_t TXUP_CMD4_14;
	__IO uint32_t TXDOWN_CMD0_18;
	__IO uint32_t TXDOWN_CMD1_1C;
	__IO uint32_t TXDOWN_CMD2_20;
	__IO uint32_t TXDOWN_CMD3_24;
	__IO uint32_t TXDOWN_CMD4_28;
	__IO uint32_t RXUP_CMD0_2C;
	__IO uint32_t RXUP_CMD1_30;
	__IO uint32_t RXUP_CMD2_34;
	__IO uint32_t RXUP_CMD3_38;
	__IO uint32_t RXUP_CMD4_3C;
	__IO uint32_t RXDOWN_CMD0_40;
	__IO uint32_t RXDOWN_CMD1_44;
	__IO uint32_t RXDOWN_CMD2_48;
	__IO uint32_t RXDOWN_CMD3_4C;
	__IO uint32_t RXDOWN_CMD4_50;
	__IO uint32_t FREQCH_CMD0_54;
	__IO uint32_t FREQCH_CMD1_58;
	__IO uint32_t FREQCH_CMD2_5C;
	__IO uint32_t FREQCH_CMD3_60;
	__IO uint32_t FREQCH_CMD4_64;
	__IO uint32_t TXUP_RFCFG7_55008038_68;
	__IO uint32_t RXUP_RFCFG7_55008038_6C;
	__IO uint32_t RXUP_MDMCFG1_55008000_70;
	__IO uint32_t RXUP_MDMCFG3_55008008_74;
	__IO uint32_t RXUP_MDMCFG4_5500800C_78;
	__IO uint32_t RXUP_MDMCFG7_550080A0_7C;
	__IO uint32_t SW_CMD0_80;
	__IO uint32_t FREQCH_CMD0_CFG_84;
	__IO uint32_t PA_EN_CMD_88;

  __IO uint32_t INT5_CMD0_8C; //TX_UP_delay
  __IO uint32_t INT5_CMD1_90;
  __IO uint32_t INT5_CMD2_94;
  __IO uint32_t INT5_CMD3_98;
  __IO uint32_t INT5_CMD4_9C;
  __IO uint32_t INT5_CMD5_A0;
  __IO uint32_t INT6_CMD0_A4; //RX_UP_delay
  __IO uint32_t INT6_CMD1_A8;
  __IO uint32_t INT6_CMD2_AC;
  __IO uint32_t INT6_CMD3_B0;
  __IO uint32_t INT6_CMD4_B4;
  __IO uint32_t INT6_CMD5_B8;
  __IO uint32_t DATA_CONCAT_BC;

//  __IO uint32_t AFC_TXVAL_0_C0;
//  __IO uint32_t AFC_TXVAL_1_C4;
//  __IO uint32_t AFC_TXVAL_2_C8;
//  __IO uint32_t AFC_TXVAL_3_CC;
//  __IO uint32_t AFC_TXVAL_4_D0;
//  __IO uint32_t AFC_TXVAL_5_D4;
//  __IO uint32_t AFC_TXVAL_6_D8;
//  __IO uint32_t AFC_TXVAL_7_DC;
//  __IO uint32_t AFC_TXVAL_8_E0;
//  __IO uint32_t AFC_TXVAL_9_E4;
  __IO uint32_t AFC_TXVAL[10];

  __IO uint32_t RESERVED[6];

//  __IO uint32_t AFC_RXVAL_0_100;
//  __IO uint32_t AFC_RXVAL_1_104;
//  __IO uint32_t AFC_RXVAL_2_108;
//  __IO uint32_t AFC_RXVAL_3_10C;
//  __IO uint32_t AFC_RXVAL_4_110;
//  __IO uint32_t AFC_RXVAL_5_114;
//  __IO uint32_t AFC_RXVAL_6_118;
//  __IO uint32_t AFC_RXVAL_7_11C;
//  __IO uint32_t AFC_RXVAL_8_120;
//  __IO uint32_t AFC_RXVAL_9_124;  
  __IO uint32_t AFC_RXVAL[10]; 

  __IO uint32_t RESERVED1[37];
  __IO uint32_t DATA2_CONCAT_1BC;



}RF_AutoConfig_TypeDef;


#define BLE                                     ((BLE_TypeDef *) BLE_BASE)

#define RFMODEM								((RFMODEM_TypeDef *) (MHSCPU_PERIPH_BASE + 0x200))
#define RFAC							((RF_AutoConfig_TypeDef *)HW_RF_SPI0_BASE)

/********************************************** LIB_22 **************************************************/
#define LIB_22_FCU_CMD_ACCESS_REQ         (BIT0)

typedef struct
{
    __IO uint32_t FCU_CMD;
    __O  uint32_t ADDRESS;
    __IO uint32_t BYTE_NUM;
    __O  uint32_t WR_FIFO;
    __I  uint32_t RD_FIFO;
    __IO uint32_t DEVICE_PARA;
    __IO uint32_t REG_WDATA;
    __O  uint32_t REG_RDATA;
    __I  uint32_t INT_MASK;
    __IO uint32_t INT_UMSAK;
    __IO uint32_t INT_MASK_STATUS;
    __IO uint32_t INI_STATUS;
    __IO uint32_t INT_RAWSTATUS;
    __IO uint32_t INT_CLEAR;
    __IO uint32_t CACHE_INTF_CMD;
    __IO uint32_t DMA_CNTL;
    __IO uint32_t FIFO_CNTL;
} LIB_22_TypeDef;
#define LIB_22                        ((LIB_22_TypeDef *) LIB_22_BASE)

/********************************************** CHCHE **************************************************/
typedef struct
{
    __IO uint32_t CACHE_CS;
    __IO uint32_t CACHE_REF;
    __IO uint32_t RESERVED0[6];
    __IO uint32_t CACHE_BACK_DOOR;
} CACHE_TypeDef;
#define CACHE                       ((CACHE_TypeDef *)CACHE_BASE)

typedef struct
{
	__IO uint32_t IRDA_CR;
	__IO uint32_t IRDA_TX_CARR;
	__IO uint32_t IRDA_BIT_LEN;
	__I  uint32_t IRDA_RXL_SUM;
	__IO uint32_t IRDA_TX_BUF;
	__IO uint32_t IRDA_INT_STAT;
	__IO uint32_t IRDA_INT_CTRL;
} IRDA_TypeDef;

typedef struct
{
	__IO uint32_t AUADC_CTRL0;
	__IO uint32_t AUADC_CTRL1;
	__IO uint32_t AUADC_FIFOLEVEL;
	__IO uint32_t AUADC_Threshold;
	__O  uint32_t AUADC_FIFOFLUSH;
	__I  uint32_t AUADC_FIFODATA;
	__IO uint32_t AUADC_STAT;
	__IO uint32_t AUADC_INT_CTRL;
	__I  uint32_t AUADC_ADC_DATA;
	__IO uint32_t DMA_CR;
	__IO uint32_t DMA_RDLR;
} AUADC_TypeDef;

#define IRDA                  ((IRDA_TypeDef* )IRDA_BASE)
#define AUADC                  ((AUADC_TypeDef* )AUADC_BASE)
#define IRDA_BASE               (CPU_APB0_BASE + 0x51000)
#define AUADC_BASE               (CPU_APB0_BASE + 0x50000)

typedef struct {
	// comon flash API
	void (*flash_lock)(void);
	void (*flash_unlock)(void);
	uint32_t (*flash_read_id)(void);
	uint32_t (*flash_erase_chip)(void);
	uint32_t (*flash_erase_sector)(unsigned int adr);
	uint32_t (*flash_erase_page)(unsigned int adr);
	uint32_t (*flash_read)(unsigned long adr, unsigned long sz, unsigned char *buf);
	uint32_t (*flash_program)(unsigned long adr, unsigned long sz, unsigned char *buf);
	uint32_t (*flash_erase_otp)(unsigned int page);
	uint32_t (*flash_read_otp)(unsigned char page, unsigned long adr, unsigned long sz, unsigned char *buf);
	uint32_t (*flash_program_otp)(unsigned char page, unsigned long adr, unsigned long sz, unsigned char *buf);
	uint32_t (*flash_lock_otp)(unsigned int page);	
	// basic flash API
	uint32_t (*flash_command)(void *cmd, unsigned long  timeout);
	uint32_t (*flash_release_pd)(void);
	uint32_t (*flash_write_enable)(void);
	uint32_t (*flash_is_busy)(void);
	uint32_t (*flash_read_sta_reg1)(void);
	uint32_t (*flash_read_sta_reg2)(void);
	uint32_t (*flash_write_sta_reg)(unsigned int reg_val);	
	uint32_t (*flash_write_sta_reg_eon)(unsigned int reg_val);
	uint32_t (*flash_set_burst_len)(unsigned long  param);
	uint32_t (*flash_program_single)(unsigned char is_otp, unsigned long adr, unsigned long sz, unsigned char *buf) ;

}bootrom_func_t;

#include <stdio.h>
#include "cpu_uart.h"
#include "cpu_sysctrl.h"
#include "cpu_spi.h"
#include "cpu_timer.h"
#include "cpu_flash.h"
#include "cpu_gpio.h"
#include "cpu_nvic.h"
#include "cpu_qdec.h"
#include "cpu_wdt.h"
#include "system_cpu.h"
#include "cpu_irda.h"
#include "cpu_rtc.h"
#include "cpu_auadc.h"


/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed. 
  *   If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) //((void)0)
#endif /* USE_FULL_ASSERT */
#ifdef __cplusplus
}
#endif

#endif  /* <Device>_H */
