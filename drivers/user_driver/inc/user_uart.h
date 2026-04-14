/**
******************************************************************************************************************************************************************************************
*@file                       user_uart.h
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
#ifndef _UI_UART_H_
#define _UI_UART_H_
#include "cpu.h"

void user_uart_resume(void);
void user_uart_init(void (*recv_irq_cb)(void));
uint32_t user_get_uart_len(void);
int32_t uart_getbytes(void);
uint32_t user_uart_read(uint8_t *pbuf, uint32_t len);
int32_t uart_RecvChar(void);
void user_uart_write(uint8_t *pbuf,uint32_t len);
int32_t uart_SendChar(uint8_t ch);
int user_print(char *fmt,...);

#endif

