#include "cpu.h"
#include "stdint.h"
#include "stdarg.h"
#include "user_ring_buffer.h"
#include "cpu_sleep.h"
#include "user_uart.h"
#include "app_config.h"

#include "lib_api.h"
#if (UART_ENABLE)
#if LOG_ENABLE
char print_string[255];
#endif
typedef struct{
	struct{
		ring_buffer_t ring_buffer;
	}uart_tx;
	struct{
		ring_buffer_t ring_buffer;
	}uart_rx;
	void (*uart_cb)(void);
}uart_param_t;

uint8_t uart_tx_buffer[MAX_UART_TX_BUF_SIZE];
uint8_t uart_rx_buffer[MAX_UART_RX_BUF_SIZE];

uart_param_t user_uart;

//nvic init
void uart_nvic_init(void)
{
	NVIC_SetPriority(UART0_IRQn,NVIC_EncodePriority(0x05, 0x02,0x00));
	NVIC_EnableIRQ(UART0_IRQn);
}

//uart init
void user_uart_configuration(void)
{
	uart_init_type_def UART_InitStructure;
    uart_fifo_init_type_def UART_FIFO_Structure;
	
	UART_InitStructure.UART_BaudRate = DEFAULT_UART_BAUD;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
	UART_InitStructure.UART_StopBits = UART_StopBits_1;
	UART_InitStructure.UART_Parity = UART_Parity_No;
           
	UART_FIFO_Structure.FIFO_Enable = ENABLE;
    UART_FIFO_Structure.FIFO_RX_Trigger = UART_FIFO_RX_Trigger_1_2_Full; //4×Ö½Ú
    UART_FIFO_Structure.FIFO_TX_Trigger = UART_FIFO_TX_Trigger_1_2_Full; //2×Ö½Ú

	uart_init(UART0, &UART_InitStructure);
	uart_fifo_init(UART0,&UART_FIFO_Structure);    	
    uart_it_config(UART0,UART_IT_RXIM|UART_IT_RX_TIMEOUT_IM ,ENABLE);
}

void user_uart_resume(void)
{
	user_uart_configuration();
}

void user_uart_init(void (*recv_irq_cb)(void))
{
	user_uart.uart_cb = recv_irq_cb;
	user_ring_buffer_init(&user_uart.uart_tx.ring_buffer,uart_tx_buffer,MAX_UART_TX_BUF_SIZE);
	user_ring_buffer_init(&user_uart.uart_rx.ring_buffer,uart_rx_buffer,MAX_UART_RX_BUF_SIZE);
	uart_nvic_init();
	user_uart_configuration();
}

uint32_t user_get_uart_len(void)
{
	return user_ring_buffer_bytes_avaiable(&user_uart.uart_rx.ring_buffer);
}

uint32_t user_uart_read(uint8_t *pbuf, uint32_t len)
{
	return user_ring_buffer_read(&user_uart.uart_rx.ring_buffer,pbuf,len);
}

void user_uart_write(uint8_t *pbuf,uint32_t len)
{
	lib_global_irq_lock();
	int ret = user_ring_buffer_write(&user_uart.uart_tx.ring_buffer,pbuf,len);
	lib_global_irq_unlock();
	if(ERROR_NO == ret)
	{
		cpu_sleep_event_set(UART0_ONGOING);
		uart_it_config(UART0, UART_IT_TXIM, ENABLE);
	}
}

void user_block_write(uint8_t *pbuf,uint32_t len)
{
	uint32_t i;
	for(i=0;i<len;i++){
		if(pbuf[i] == '\n'){
			uart_send_data(UART0, '\r');
			while(!uart_is_tx_fifo_empty(UART0));
		}
		
		uart_send_data(UART0, pbuf[i]);
		while(!uart_is_tx_fifo_empty(UART0));
	}
}

void UART0_IRQHandler(void)
{
	uint32_t iir;
    iir = uart_get_it_identity(UART0);
	
	if((iir & UART_IT_RX_STATUS) || (iir & UART_IT_RX_TIMEOUT_STATUS))
	{
		while(!uart_is_rx_fifo_empty(UART0))
		{
			uint8_t ch = uart_receive_data(UART0);		
			user_ring_buffer_write(&user_uart.uart_rx.ring_buffer,&ch,1);
		}
		if(user_uart.uart_cb) user_uart.uart_cb();
	}
	
	if(iir & UART_IT_TX_STATUS)
	{
		uint8_t ch;
		uint8_t len = user_ring_buffer_read(&user_uart.uart_tx.ring_buffer,&ch,1);
		if(len) uart_send_data(UART0, ch);
		
		if(user_ring_buffer_empty(&user_uart.uart_tx.ring_buffer))
		{
			while(!uart_is_tx_fifo_empty(UART0));
			uart_it_config(UART0, UART_IT_TXIM, DISABLE);
			cpu_sleep_event_clear(UART0_ONGOING);
		}
	}
	
	NVIC_ClearPendingIRQ(UART0_IRQn);
}

#if LOG_ENABLE
int user_print(char *fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	vsprintf(print_string,fmt,ap);
	va_end(ap);
	user_uart_write((uint8_t *)print_string,strlen(print_string));
	return 0;
}

int user_block_print(char *fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	vsprintf(print_string,fmt,ap);
	va_end(ap);
	user_block_write((uint8_t *)print_string,strlen(print_string));
	return 0;
}

int user_block_printk_hex(char* lable,char *buf,unsigned char len)
{
	unsigned char i;
    user_block_write((unsigned char *)lable, strlen(lable));
	if(len > 50)
		len = 50;

	memset(print_string,0,sizeof(print_string));
	for(i=0;i<len;i++)
	{
		if((buf[i] & 0xF0) > 0x90){
			print_string[ i*3 + 0 ]= ((buf[i] & 0xF0) >> 4) - 10 + 'A';
		}else{
			print_string[ i*3 + 0 ]= ((buf[i] & 0xF0) >> 4) + '0';
		}

		if((buf[i] & 0x0F) > 0x09){
			print_string[ i*3 + 1 ]= (buf[i] & 0x0F) - 10 + 'A';
		}else{
			print_string[ i*3 + 1 ]= (buf[i] & 0x0F) + '0';
		}
		print_string[ i*3 + 2 ]=' ';
	}
	
	print_string[len*3 + 0]='\n';
	print_string[len*3 + 1]='\0';
	user_block_write((uint8_t *)print_string, strlen(print_string));	
    return 0 ;
}

#endif
#else
void user_uart_write(uint8_t *pbuf,uint32_t len){}
int user_print(char *fmt,...){}
int user_block_print(char *fmt,...){}

#endif



