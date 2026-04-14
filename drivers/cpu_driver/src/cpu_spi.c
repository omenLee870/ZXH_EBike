/**
******************************************************************************************************************************************************************************************
*@file                       cpu_spi.c
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
#include "cpu_spi.h"

void spi_init(SPI_TypeDef* SPIx, spi_init_type_def* spi_init_struct)
{
	spi_cmd(SPIx,DISABLE);
	spi_it_config(SPIx,SPI_IT_All,DISABLE);
	SPIx->CTRLR0 = (spi_init_struct->SPI_Direction | \
                            spi_init_struct->SPI_CPOL | \
                            spi_init_struct->SPI_CPHA | \
                            spi_init_struct->SPI_DataSize);	
	SPIx->BAUDR = spi_init_struct->SPI_BaudRatePrescaler;
	SPIx->SER = spi_init_struct->SPI_NSS;
	SPIx->RXFTLR = spi_init_struct->SPI_RXFIFOFullThreshold;
	SPIx->TXFTLR = spi_init_struct->SPI_TXFIFOEmptyThreshold;
}

void spi_cmd(SPI_TypeDef* SPIx, functional_state new_state)
{
	if (new_state != DISABLE)
	{
		/* Enable the selected SPI peripheral */
		SPIx->SSIENR = 1;		//ENABLE current SPI
	}
	else
	{
		/* Disable the selected SPI peripheral */
		SPIx->SSIENR = 0;		//DISABLE current SPI
	}
}

void spi_it_config(SPI_TypeDef* SPIx, uint32_t spi_it, functional_state new_state)
{

    if(new_state != DISABLE)
        SPIx->IMR |= spi_it;
    else
        SPIx->IMR &= ~spi_it;
}

boolean spi_is_rx_no_empty(SPI_TypeDef* SPIx)
{
	if(SPIx->SR & SPI_SR_RFNE)
		return TRUE;
	else
		return FALSE;
}

boolean spi_is_tx_empty(SPI_TypeDef* SPIx)
{
	if(SPIx->SR & SPI_SR_TFE)
		return TRUE;
	else
		return FALSE;
}

void spi_send_data(SPI_TypeDef* SPIx, uint16_t Data)
{
    (((SPI_TypeDef*)((uint32_t)SPIx + 0x0000)))->DR = Data;
}

uint16_t spi_receive_data(SPI_TypeDef* SPIx)
{
    return (uint16_t)(((SPI_TypeDef*)((uint32_t)SPIx + 0x0000)))->DR;
}

itstatus spi_get_it_status(SPI_TypeDef* SPIx, uint32_t spi_it)
{
	itstatus bitstatus = RESET; 
	if (SPIx->ISR & spi_it)
	{
			bitstatus = SET;
	}
	else
	{
			bitstatus = RESET;
	}
	return bitstatus;
}

void spi_clear_it_pending_bit(SPI_TypeDef* SPIx, uint32_t spi_it)
{
	uint32_t tmpreg = 0;

	if (SPI_IT_MST | spi_it)
	{
			tmpreg = SPIx->MSTICR;
	}
	if (SPI_IT_RXU | spi_it)
	{
			tmpreg = SPIx->RXUICR;
	}
	if (SPI_IT_RXO | spi_it)
	{
			tmpreg = SPIx->RXOICR;
	}
	if (SPI_IT_TXO | spi_it)
	{
			tmpreg = SPIx->TXOICR;
	}
	tmpreg = tmpreg;
}

flagstatus spi_get_flag_status(SPI_TypeDef* SPIx, uint32_t spi_flag)
{
    flagstatus bitstatus = RESET; 

    if (((((SPI_TypeDef*)((uint32_t)SPIx + 0x0000)))->SR & spi_flag) != RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;

}
