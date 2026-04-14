/**
******************************************************************************************************************************************************************************************
*@file                       cpu_uart.h
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
#ifndef CPU_UART_H
#define CPU_UART_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
#include "cpu.h"
	 
     
typedef struct
{
  uint32_t UART_BaudRate;          

  uint32_t UART_WordLength;          

  uint32_t UART_StopBits;           

  uint32_t UART_Parity;   
	
} uart_init_type_def;

typedef struct
{
	functional_state FIFO_Enable;
    
	uint32_t FIFO_RX_Trigger;
    
	uint32_t FIFO_TX_Trigger;
    
	uint32_t FIFO_TX_TriggerIntEnable;
    
}uart_fifo_init_type_def;

#define UART_WordLength_5b                  (0x00000000)
#define UART_WordLength_6b                  (0x00000020)
#define UART_WordLength_7b                  (0x00000040)
#define UART_WordLength_8b                  (0x00000060)

#define UART_StopBits_1                     (0x00000000)
#define UART_StopBits_2                     (0x00000008)

#define UART_Parity_No                      (0x00000000)
#define UART_Parity_Even                    (0x00000032)
#define UART_Parity_Odd                     (0x00000002) 

//TX FIFO深度为4
#define UART_FIFO_TX_Trigger_1_2_Full       (0x00000001)
#define UART_FIFO_TX_Trigger_1_4_Full       (0x00000000)

//RX  FIFO深度为32
#define UART_FIFO_RX_Trigger_1_2_Full       (0x00000020)//RX FIFO数据 >=16 字节触发中断
#define UART_FIFO_RX_Trigger_1_4_Full       (0x00000018)
#define UART_FIFO_RX_Trigger_1_8_Full       (0x00000010)//RX FIFO数据 >=4  字节触发中断
#define UART_FIFO_RX_Trigger_1_16_Full      (0x00000008)
#define UART_FIFO_RX_Trigger_1_32_Full      (0x00000000)//RX FIFO数据 >=1  字节触发中断

#define TXFEIC_Clear                        (0x00001000)
#define XOFFIC_Clear                        (0x00000800)
#define OEIC_Clear                          (0x00000400)
#define BEIC_Clear                          (0x00000200)
#define PEIC_Clear                          (0x00000100)
#define FEIC_Clear                          (0x00000080)
#define RTIC_Clear                          (0x00000040)
#define TXIC_Clear                          (0x00000020)
#define RXIC_Clear                          (0x00000010)
#define DSRMIC_Clear                        (0x00000008)
#define DCDMIC_Clear                        (0x00000004)
#define CTSMIC_Clear                        (0x00000002)
#define RIMIC_Clear                         (0x00000001)

void uart_init(UART_TypeDef* UARTx, uart_init_type_def* uart_init_struct);
void uart_it_config(UART_TypeDef* UARTx, uint32_t UART_IT, functional_state new_state);
void uart_send_data(UART_TypeDef* UARTx, uint8_t Data);
uint8_t uart_receive_data(UART_TypeDef* UARTx);
uint32_t uart_get_it_identity(UART_TypeDef* UARTx);

boolean uart_is_rx_fifo_full(UART_TypeDef* UARTx);
boolean uart_is_rx_fifo_empty(UART_TypeDef* UARTx);
boolean uart_is_tx_fifo_empty(UART_TypeDef* UARTx);
boolean uart_is_tx_fifo_full(UART_TypeDef* UARTx);
boolean uart_is_busy(UART_TypeDef* UARTx);
boolean uart_is_receive_data_ready(UART_TypeDef * UARTx);

void uart_fifo_cmd(UART_TypeDef* UARTx,functional_state new_state);
void uart_fifo_init(UART_TypeDef* UARTx, uart_fifo_init_type_def* uart_fifo_init_struct);


#ifdef __cplusplus
}
#endif

#endif // CPU_SCU_UART_H
