/**
******************************************************************************************************************************************************************************************
*@file                       cpu_GPADC.c
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
#include "cpu_irda.h"


void irda_init(irda_init_type_def* irda_init_struct)
{
	
	IRDA->IRDA_CR = irda_init_struct->IRDA_RXCTRL << 16 | irda_init_struct->IRDA_RXL_SUM_UPDT_EN | \
					irda_init_struct->IRDA_IN_INV | irda_init_struct->IRDA_OUT_INV | \
					irda_init_struct->IRDA_TX_MODE | irda_init_struct->IRDA_RX_NODE | \
					irda_init_struct->IRDA_HARDWARE | irda_init_struct->IRDA_TX_STP;

	IRDA->IRDA_TX_CARR = irda_init_struct->IRDA_TX_CARR_LT << 16 | irda_init_struct->IRDA_TX_CARR_HT;

	IRDA->irda_bit_len = irda_init_struct->irda_bit_len;	
}

void irda_rx_cmd(functional_state new_state)
{
	if (new_state != DISABLE)
	{
		
		IRDA->IRDA_CR |= IRDA_RX_EN;
	}
	else
	{
		
		IRDA->IRDA_CR &= ~IRDA_RX_EN;		
	}
}

void irda_tx_cmd(functional_state new_state)
{
	if (new_state != DISABLE)
	{
		
		IRDA->IRDA_CR |= IRDA_TX_EN;
	}
	else
	{
		
		IRDA->IRDA_CR &= ~IRDA_TX_EN;		
	}
}

void irda_tx_stop_en(functional_state new_state)
{
	if (new_state != DISABLE)
	{
		
		IRDA->IRDA_CR |= IRDA_TX_STP_OPEN;
	}
	else
	{
		
		IRDA->IRDA_CR &= IRDA_TX_STP_DOWN;		
	}
}

void irda_rxl_sum_updt(functional_state new_state)
{
	if (new_state != DISABLE)
	{
		
		IRDA->IRDA_CR |= IRDA_RX_SUM_SIZE_OPEN;
	}
	else
	{
		
		IRDA->IRDA_CR &= ~IRDA_RX_SUM_SIZE_OPEN;		
	}
}

void irda_carr_lt(uint32_t carr_lt)
{
	IRDA->IRDA_TX_CARR |= (carr_lt << 16) & 0x07FF0000;
}

void irda_carr_ht(uint32_t carr_ht)
{
	IRDA->IRDA_TX_CARR |= carr_ht & 0x000007FF;
}

void irda_bit_len(uint32_t bit_len)
{
	IRDA->irda_bit_len = bit_len;
}


void irda_it_config(uint32_t irda_it_en,functional_state new_state)
{
	if(new_state != DISABLE)
		IRDA->IRDA_INT_CTRL = irda_it_en;
	else
		IRDA->IRDA_INT_CTRL &= ~irda_it_en;
}

uint8_t irda_rx_receive_data(void)
{
	/* Return the data in the DATA register */
	return ((IRDA->IRDA_CR & 0x400) >> 10);
}

uint32_t irda_rx_sum_size(void)
{
	/* Return the data in the DATA register */
	return IRDA->IRDA_RXL_SUM;
}

void irda_tx_data(uint8_t tx_buf)
{
	IRDA->IRDA_TX_BUF = tx_buf;
}

uint32_t irda_read_stat_pending_bit(uint32_t irda_it)
{
	uint32_t tmpreg = 0;
	tmpreg = IRDA->IRDA_INT_STAT;
	return (tmpreg & irda_it);
}						

void irda_set_it_pending_bit(uint32_t irda_it)
{
	
	if (IRP_INTP_STATE & irda_it)
	{
		IRDA->IRDA_INT_STAT |= irda_it;
	}
	if (IRN_INTP_STATE & irda_it)
	{
		IRDA->IRDA_INT_STAT |= irda_it;
	}
	if (RXL_VF_INTP_STATE & irda_it)
	{
		IRDA->IRDA_INT_STAT |= irda_it;
	}
	if (RXL_SUM_VF_INTP_STATE & irda_it)
	{
		IRDA->IRDA_INT_STAT |= irda_it;
	}
	if (TX_STP_INTP_STATE & irda_it)
	{
		IRDA->IRDA_INT_STAT |= irda_it;
	}
	if (TX_STR_INTP_STATE & irda_it)
	{
		IRDA->IRDA_INT_STAT |= irda_it;
	}
}						
