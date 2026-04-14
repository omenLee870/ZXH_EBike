/**
******************************************************************************************************************************************************************************************
*@file                       cpu_irda.h
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
#ifndef _CPU_IRDA_
#define _CPU_IRDA_

#include "cpu.h"

typedef struct
{
	uint32_t IRDA_RXCTRL;
	
	uint32_t IRDA_IN;
	
	uint32_t IRDA_RXL_SUM_UPDT_EN;
	
	uint32_t IRDA_IN_INV;                                           
																	
	uint32_t IRDA_OUT_INV;                                        

	uint32_t IRDA_TX_MODE;                                            

	uint32_t IRDA_RX_NODE; 

	uint32_t IRDA_HARDWARE;

	uint32_t IRDA_RX_EN;              
	
	uint32_t IRDA_TX_STP;
	
	uint32_t IRDA_TX_EN;
	
	uint32_t IRDA_TX_CARR_LT;
	
	uint32_t IRDA_TX_CARR_HT;
	
	uint32_t IRDA_BIT_LEN;
	
	uint32_t IRDA_RXL_SUM;
	
	uint32_t IRDA_TX_BUF;
	
}irda_init_type_def;

/*IRDA_RXL_SUM_UPDT_EN*/
#define IRDA_RX_SUM_SIZE_OPEN				((uint32_t)0x0001 << 9)
#define IRDA_RX_SUM_SIZE_DOWN				((uint32_t)0x0000 << 9)

/*IRDA_IN_INV*/
#define IRDA_IN_TAKE_BACK_OPEN				((uint32_t)0x0001 << 8)
#define IRDA_IN_TAKE_BACK_DOWN				((uint32_t)0x0000 << 8)

/*IRDA_OUT_INV*/
#define IRDA_OUT_TAKE_BACK_OPEN				((uint32_t)0x0001 << 7)
#define IRDA_OUT_TAKE_BACK_DOWN				((uint32_t)0x0000 << 7)

/*IRDA_TX_MODE*/
#define IRDA_CODED_DATA_MODE 				((uint32_t)0x0000 << 5)
#define IRDA_SOFT_CTRL_MODE					((uint32_t)0x0001 << 5)
#define IRDA_TIME2_PWM_MODE					((uint32_t)0x0002 << 5)
#define IRDA_TIME3_PWM_MODE					((uint32_t)0x0003 << 5)

/*IRDA_RX_MODE*/
#define IRDA_INTERIOR_MODE 					((uint32_t)0x0000 << 3)
#define IRDA_OUTSIDE_IO_MODE				((uint32_t)0x0001 << 3)

#define IRDA_HARDWARE_COUNT					((uint32_t)0x0000 << 4)
#define IRDA_HARDWARE_NOT_COUNT				((uint32_t)0x0001 << 4)

/*IRDA_RX_EN*/
#define IRDA_RX_EN							((uint32_t)0x0001 << 2)

/*IRDA_TX_STP*/
#define IRDA_TX_STP_OPEN					((uint32_t)0x0001 << 1)
#define IRDA_TX_STP_DOWN					~((uint32_t)0x0001 << 1)

/*IRDA_TX_EN*/
#define IRDA_TX_EN							((uint32_t)0x0001)

/*INT_STAT*/
#define RXL_VF_INTP_STATE					((uint32_t)0x0020)
#define RXL_SUM_VF_INTP_STATE				((uint32_t)0x0010)
#define TX_STP_INTP_STATE					((uint32_t)0x0008)
#define TX_STR_INTP_STATE					((uint32_t)0x0004)
#define IRN_INTP_STATE						((uint32_t)0x0002)
#define IRP_INTP_STATE						((uint32_t)0x0001)


#define TP_IN_IE_EN							((uint32_t)0x0001)				
#define TS_IP_IE_EN							((uint32_t)0x0002)				
#define RXL_SUM_VF_IE_EN					((uint32_t)0x0004)				
#define RXL_VF_IE_EN						((uint32_t)0x0008)				
#define TP_IN_MASK							((uint32_t)0x0100)				
#define TS_IP_MASK							((uint32_t)0x0200)				
#define RXL_SUM_VF_MASK						((uint32_t)0x0400)				
#define RXL_VF_MASK							((uint32_t)0x0800)		

void irda_init(irda_init_type_def* irda_init_struct);
void irda_rx_cmd(functional_state new_state);
void irda_tx_cmd(functional_state new_state);
void irda_it_config(uint32_t irda_it_en,functional_state new_state);
uint8_t irda_rx_receive_data(void);
uint32_t irda_rx_sum_size(void);
void irda_tx_data(uint8_t tx_buf);
uint32_t irda_read_stat_pending_bit(uint32_t irda_it);
void irda_set_it_pending_bit(uint32_t irda_it);
void irda_rxl_sum_updt(functional_state new_state);
void irda_tx_stop_en(functional_state new_state);
void irda_bit_len(uint32_t bit_len);
void irda_carr_ht(uint32_t carr_ht);
void irda_carr_lt(uint32_t carr_lt);
void irdar_configuration(void);
void irda_cmd(uint8_t en);
void call_data_len(void);
#endif
