/* Include ------------------------------------------------------------------*/
#include "cpu_sysctrl.h"
#include "define.h"
#include "user_gpio.h"
#include "user_spi.h"
#include "mh523.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/	

/* Private macro ------------------------------------------------------------*/

#define	NFC_PD_GPIOX	  GPIOB
#define	NFC_PD_GPIO_PIN	  GPIO_Pin_2

/* Ptivate function prototypes ----------------------------------------------*/	

/******************************************************************************
* Function Name  : gpio_config
* Description    : config gpio for led and nrstpd
* Input          : NONE
* Output         : NONE
* Return         : NONE
******************************************************************************/
void gpio_config(void)
{ 
	gpio_init(NFC_SPI_CS_GPIOx,NFC_SPI_CS_PIN,GPIO_Mode_Output);
//	gpio_pin_remap_config(GPIOB,NFC_PD_GPIO_PIN,GPIO_Remap_GPIO);
	gpio_pin_remap_config(NFC_SPI_CS_GPIOx,GPIO_PinNum_7,GPIO_Remap_GPIO);
	
//	gpio_init(GPIOD,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6,GPIO_Mode_Output);
	
	gpio_init(GPIOB,NFC_INT_GPIO_PIN,GPIO_Mode_Input_Pull_Up_Resistor);
	gpio_pin_remap_config(GPIOB,GPIO_PinNum_7,GPIO_Remap_GPIO);
}

/******************************************************************************
* Function Name  : led_success_on
* Description    : len on
* Input          : NONE
* Output         : NONE
* Return         : NONE
******************************************************************************/
void led_success_on(void)
{
    write_reg(TestPinEnReg, 0x9E);
    write_reg(TestPinValueReg, 0x80);	 
}

/******************************************************************************
* Function Name  : led_success_off
* Description    : len off
* Input          : NONE
* Output         : NONE
* Return         : NONE
******************************************************************************/
void led_success_off(void)
{
    write_reg(TestPinEnReg, 0x9E);
    write_reg(TestPinValueReg, 0x9E);		   
}

/******************************************************************************
* Function Name  : pcd_poweron
* Description    : pcd power on
* Input          : NONE
* Output         : NONE
* Return         : NONE
******************************************************************************/
void pcd_poweron(void)
{
	int i;
	
	gpio_set(NFC_PD_GPIOX, NFC_PD_GPIO_PIN); 
	for (i = 0; i < 2000; i++);
}

/******************************************************************************
* Function Name  : pcd_powerdown
* Description    : pcd power down
* Input          : NONE
* Output         : NONE
* Return         : NONE
******************************************************************************/
void pcd_powerdown(void)
{
   gpio_reset(NFC_PD_GPIOX, NFC_PD_GPIO_PIN); 
}


/********************** (C) COPYRIGHT 2014 Megahuntmicro ********************/
