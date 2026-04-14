/* Include ------------------------------------------------------------------*/
#include <stdio.h>
#include "cpu_sysctrl.h"
#include "user_spi.h"
#include "app_config.h"
/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/	

/* Private macro ------------------------------------------------------------*/	
/* Private variables --------------------------------------------------------*/
static SPI_TypeDef *NFC_SPI = SPI0;
/* Ptivate function prototypes ----------------------------------------------*/	

/******************************************************************************
* Function Name  : spi_config
* Description    : config SPI 
* Input          : NONE
* Output         : NONE
* Return         : NONE 
******************************************************************************/
void spi_config(void)
{
	SPI_TypeDef *SPIx = SPI0;
    /// Mode 0, Polling
	spi_init_type_def spi_init_structure;	
	crm_cmd_periph_clock(CRM_CG_CTRL_SPI0,ENABLE);
    //SPI管脚映射配置
    if( SPIx == SPI0 )
	{
		gpio_pin_remap_config(GPIOC,GPIO_PinNum_6,GPIO_Remap_SPI0_CLK);
		gpio_pin_remap_config(GPIOC,GPIO_PinNum_4,GPIO_Remap_SPI0_MISO);
		gpio_pin_remap_config(GPIOC,GPIO_PinNum_5,GPIO_Remap_SPI0_MOSI);
	}
    
    spi_init_structure.SPI_Mode = SPI_Mode_Master;//配置SPI master模式
	spi_init_structure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//设置SPI通信方式：全双工模式
	spi_init_structure.SPI_DataSize = SPI_DataSize_8b;//设置数据帧大小：8bit
	spi_init_structure.SPI_CPOL = SPI_CPOL_Low;//设置时钟极性:空闲状态时，时钟保持低电平
	spi_init_structure.SPI_CPHA = SPI_CPHA_2Edge;//设置时钟相位：数据采样从第二个时钟边沿开始
	spi_init_structure.SPI_NSS = SPI_NSS_0;//NSS信号控制选择：开启
	spi_init_structure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//SPI波特率分频：256 ，SPI速率=16M/256 =62.5KHz
	spi_init_structure.SPI_RXFIFOFullThreshold = SPI_RXFIFOFullThreshold_1;//接收FIFO空中断阈值：接收FIFO中数据1个及以上触发中断
	spi_init_structure.SPI_TXFIFOEmptyThreshold = SPI_TXFIFOEmptyThreshold_0;//发送FIFO满中断阈值：发送FIFO中数据数量为0触发中断
	
	spi_init(SPIx, &spi_init_structure);//SPI初始化
	spi_cmd(SPIx, ENABLE);//SPI使能
}

/******************************************************************************
* Function Name  : spi_write_byte
* Description    : SPI write byte 
* Input          : write_data:write data value
* Output         : NONE
* Return         : state
******************************************************************************/
uint8_t spi_write_byte(uint8_t write_data)
{ 
    uint8_t state = 0;
    uint16_t u16_time_out_counter = 200;
    
    spi_send_data(SPI0, write_data);
    
    while((RESET == spi_get_flag_status(SPI0, SPI_FLAG_RXNE)) && u16_time_out_counter)
    {
        u16_time_out_counter--;
    }
    
    if (!u16_time_out_counter)
    {
        state = 1;
    }
    
    spi_receive_data(SPI0);
	
    return state;
}

/******************************************************************************
* Function Name  : spi_read_byte
* Description    : SPI read byte 
* Input          : NONE
* Output         : NONE
* Return         : read data value 
******************************************************************************/
uint8_t spi_read_byte(void)
{ 
    uint8_t read_byte = 0;
    uint16_t u16_time_out_counter = 500;
    
    spi_send_data(SPI0, 0xFF);
    
    while((RESET == spi_get_flag_status(SPI0, SPI_FLAG_RXNE)) && u16_time_out_counter)
    {
        u16_time_out_counter--;
    }
    
    if (!u16_time_out_counter)
    {
        dbg_printk("spi_read_byte failed!\r\n");
    }
    
    read_byte = spi_receive_data(SPI0);
    
    return read_byte;
}

/**
 ****************************************************************
 * @brief write_reg() 
 *
 * 写芯片的寄存器
 *
 * @param:  addr 寄存器地址
 ****************************************************************
 */
void write_reg(uint8_t addr, uint8_t val)
{
	uint8_t c;

	//最低位空闲，有效数据域为bit1~bit6
	addr <<= 1;
	
	//地址最高位为1代表读，为0代表写；
	c = addr & ~(READ_REG_CTRL);

	SPI_CS_LOW();						
	spi_write_byte(c);
	spi_write_byte(val);
	SPI_CS_HIGH();
}

/**
 ****************************************************************
 * @brief read_reg() 
 *
 * 读芯片的寄存器
 *
 * @param: addr 寄存器地址
 * @return: c 寄存器的值
 ****************************************************************
 */
uint8_t read_reg(uint8_t addr)
{
	uint8_t c;
	
	//最低位空闲，有效数据域为bit1~bit6
	addr <<= 1;
	
	//地址最高位为1代表读，为0代表写；
	c = addr | READ_REG_CTRL;

	SPI_CS_LOW();	
	spi_write_byte(c);	
	c = spi_read_byte();	
	SPI_CS_HIGH();	

	return c;
}


/************************ (C) COPYRIGHT 2014 Megahuntmicro **********************/
