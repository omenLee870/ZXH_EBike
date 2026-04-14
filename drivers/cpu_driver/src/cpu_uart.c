/**
******************************************************************************************************************************************************************************************
*@file                       cpu_uart.c
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
#include "cpu_uart.h"
#include "cpu_sysctrl.h"

void uart_init(UART_TypeDef* UARTx, uart_init_type_def* uart_init_struct)
{
    uint32_t 	BaudRateI,BaudRateF;
    uint32_t    g_baud_rate = uart_init_struct->UART_BaudRate ;

    BaudRateI =16000000/8/g_baud_rate;
    BaudRateF= ((1600000000/8/g_baud_rate-BaudRateI*100)*64+50)/100;

    UARTx->IBRD = BaudRateI;
    UARTx->FBRD = BaudRateF;
    
    UARTx->LCRH_RX=uart_init_struct->UART_WordLength|uart_init_struct->UART_Parity|uart_init_struct->UART_StopBits;    
    UARTx->LCRH_TX=uart_init_struct->UART_WordLength|uart_init_struct->UART_Parity|uart_init_struct->UART_StopBits; 
    
    UARTx->TIMEOUT=0xff; //duration =TIMEOUT/(ovsp*buad_rate).ovsp is 8 or 16,now it is about 4 char transfer time;
//    UARTx->IFLS =0x00;  //set rx and tx level 1 character;
//    UARTx->CR =BIT3; //set oversamoling factor is 1--8 uartclk clock cycles,波特率可以更高
//    UARTx->IMSC=UART_IT_RXIM;//|BIT12; //enable rx interrupt mask and disable tx fifo empty interrupt mask; 
    
    UARTx->ICR= 0x1FFF;  //clear all interrupt;
    UARTx->CR =(BIT0|BIT3|BIT8|BIT9);//enable uart、tx、rx;
}

boolean uart_is_tx_fifo_empty(UART_TypeDef *UARTx)
{
    // Wait TX FIFO empty
	if (UARTx->FR & UART_STATUS_TX_FIFO_EMPTY)
		return TRUE;
	else
		return FALSE;
}

boolean uart_is_rx_fifo_empty(UART_TypeDef *UARTx)
{
	if(UARTx->FR  & UART_STATUS_RX_FIFO_EMPTY)
		return TRUE;
	else
		return FALSE;
}

boolean uart_is_tx_fifo_full(UART_TypeDef *UARTx)
{
   	if (UARTx->FR & UART_STATUS_TX_FIFO_FULL)
		return TRUE;
	else
		return FALSE;
}

boolean uart_is_rx_fifo_full(UART_TypeDef *UARTx)
{
	// Wait RX FIFO Full
   	if (UARTx->FR & UART_STATUS_RX_FIFO_FULL)
		return TRUE;
	else
		return FALSE;
}

void uart_it_config(UART_TypeDef * UARTx, uint32_t Type, functional_state new_state)
{
    if(new_state)
    {
     UARTx->IMSC |=Type;
    }
    else
    {
     UARTx->IMSC &=~Type;
    }
}

uint8_t uart_receive_data(UART_TypeDef *UARTx)
{
	return (uint8_t)(UARTx->DR & 0xFF);   
}

void uart_send_data(UART_TypeDef *UARTx,uint8_t Data)
{
	UARTx->DR = (Data & 0xFF);
}

uint32_t uart_get_it_identity(UART_TypeDef *UARTx)
{
   if( UARTx->MIS & UART_IT_RX_STATUS)
	   return UART_IT_RX_STATUS;
   
   if(UARTx->MIS & UART_IT_TX_STATUS)      
	   return UART_IT_TX_STATUS;
   
   if(UARTx->MIS & UART_IT_RX_TIMEOUT_STATUS)
       return UART_IT_RX_TIMEOUT_STATUS;
   
   return UARTx->MIS;
   
}

boolean uart_is_busy(UART_TypeDef* UARTx)
{
   	if (UARTx->FR & UART_STATUS_BUSY)
		return TRUE;
	else
		return FALSE;
}

boolean uart_is_receive_data_ready(UART_TypeDef * UARTx)
{
	if(UARTx->FR & BIT6)
		return TRUE;
	else
		return FALSE;
}

void uart_fifo_cmd(UART_TypeDef* UARTx,functional_state new_state)
{
	if (DISABLE != new_state)
    {
		UARTx->LCRH_RX |= BIT4;
        UARTx->LCRH_TX |= BIT4;
    }
	else
    {
		UARTx->LCRH_RX &= ~BIT4;
        UARTx->LCRH_TX &= ~BIT4;
    }

}

void uart_fifo_init(UART_TypeDef* UARTx, uart_fifo_init_type_def* uart_fifo_init_struct)
{
    if (ENABLE == uart_fifo_init_struct->FIFO_Enable)//FIFO使能
    {
		UARTx->LCRH_RX |= BIT4;
        UARTx->LCRH_TX |= BIT4;
    }
	else
    {
		UARTx->LCRH_RX &= ~BIT4;
        UARTx->LCRH_TX &= ~BIT4;
    }
    //配置FIFOTrigger
    UARTx->IFLS &= ~0x3F;
    UARTx->IFLS |= ((uart_fifo_init_struct->FIFO_RX_Trigger)|(uart_fifo_init_struct->FIFO_TX_Trigger));
       
}
