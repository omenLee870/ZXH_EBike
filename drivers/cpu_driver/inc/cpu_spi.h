/**
******************************************************************************************************************************************************************************************
*@file                       cpu_spi.h
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
#ifndef CPU_SPI
#define CPU_SPI

#include "cpu.h"

typedef struct
{
	uint32_t SPI_Mode;
	
	uint32_t SPI_Direction;                                           
																	
	uint32_t SPI_DataSize;                                        

	uint32_t SPI_CPOL;                                            

	uint32_t SPI_CPHA;                                           

	uint32_t SPI_NSS;              

	uint32_t SPI_BaudRatePrescaler; 
	
	uint32_t SPI_RXFIFOFullThreshold;

    uint32_t SPI_TXFIFOEmptyThreshold;
	
}spi_init_type_def;


#define SPI_Mode_Slave		((uint32_t)0x0000)
#define SPI_Mode_Master		((uint32_t)0x0001)

#define SPI_Direction_2Lines_FullDuplex 		((uint32_t)0x0000)
#define SPI_Direction_1Line_Tx					((uint32_t)0x0100)
#define SPI_Direction_1Line_Rx					((uint32_t)0x0200)
#define SPI_Direction_EEPROM_Read				((uint32_t)0x0300)

#define SPI_DataSize_8b                 ((uint32_t)0x0007)
#define SPI_DataSize_16b                ((uint32_t)0x000F)

#define SPI_CPOL_Low                    ((uint32_t)0x0000)
#define SPI_CPOL_High                   ((uint32_t)0x0080)

#define SPI_CPHA_1Edge                  ((uint32_t)0x0000)
#define SPI_CPHA_2Edge                  ((uint32_t)0x0040)

#define SPI_NSS_0                    ((uint32_t)0x0001)
#define SPI_NSS_1                    ((uint32_t)0x0002)
#define SPI_NSS_2                    ((uint32_t)0x0004)
#define SPI_NSS_3                    ((uint32_t)0x0008)
#define SPI_NSS_All                  ((uint32_t)0x000F)
#define SPI_NSS_Null                 ((uint32_t)0x0000)

#define SPI_BaudRatePrescaler_2         ((uint32_t)0x0002)
#define SPI_BaudRatePrescaler_4         ((uint32_t)0x0004)
#define SPI_BaudRatePrescaler_8         ((uint32_t)0x0008)
#define SPI_BaudRatePrescaler_16        ((uint32_t)0x0010)
#define SPI_BaudRatePrescaler_32        ((uint32_t)0x0020)
#define SPI_BaudRatePrescaler_64        ((uint32_t)0x0040)
#define SPI_BaudRatePrescaler_128       ((uint32_t)0x0080)
#define SPI_BaudRatePrescaler_256       ((uint32_t)0x0100)

//**********************************************************************************************

/** @defgroup SPI_RXFIFOFullThreshold 
  * @{
  */
#define SPI_RXFIFOFullThreshold_1        ((uint32_t)0x0000)
#define SPI_RXFIFOFullThreshold_2        ((uint32_t)0x0001)
#define SPI_RXFIFOFullThreshold_3        ((uint32_t)0x0002)
#define SPI_RXFIFOFullThreshold_4        ((uint32_t)0x0003)
#define SPI_RXFIFOFullThreshold_5        ((uint32_t)0x0004)
#define SPI_RXFIFOFullThreshold_6        ((uint32_t)0x0005)
#define SPI_RXFIFOFullThreshold_7        ((uint32_t)0x0006)
#define SPI_RXFIFOFullThreshold_8        ((uint32_t)0x0007)
#define SPI_RXFIFOFullThreshold_9        ((uint32_t)0x0008)
#define SPI_RXFIFOFullThreshold_10       ((uint32_t)0x0009)
#define SPI_RXFIFOFullThreshold_11       ((uint32_t)0x000A)
#define SPI_RXFIFOFullThreshold_12       ((uint32_t)0x000B)
#define SPI_RXFIFOFullThreshold_13       ((uint32_t)0x000C)
#define SPI_RXFIFOFullThreshold_14       ((uint32_t)0x000D)
#define SPI_RXFIFOFullThreshold_15       ((uint32_t)0x000E)
#define SPI_RXFIFOFullThreshold_16       ((uint32_t)0x000F)

#define IS_SPI_RX_FIFO_FULL_THRESHOLD(THRESHOLD)	(((THRESHOLD) == SPI_RXFIFOFullThreshold_1) || \
													((THRESHOLD) == SPI_RXFIFOFullThreshold_2) || \
													((THRESHOLD) == SPI_RXFIFOFullThreshold_3) || \
													((THRESHOLD) == SPI_RXFIFOFullThreshold_4) || \
													((THRESHOLD) == SPI_RXFIFOFullThreshold_5) || \
													((THRESHOLD) == SPI_RXFIFOFullThreshold_6) || \
													((THRESHOLD) == SPI_RXFIFOFullThreshold_7) || \
													((THRESHOLD) == SPI_RXFIFOFullThreshold_8) || \
													((THRESHOLD) == SPI_RXFIFOFullThreshold_9) || \
													((THRESHOLD) == SPI_RXFIFOFullThreshold_10) || \
													((THRESHOLD) == SPI_RXFIFOFullThreshold_11) || \
													((THRESHOLD) == SPI_RXFIFOFullThreshold_12) || \
													((THRESHOLD) == SPI_RXFIFOFullThreshold_13) || \
													((THRESHOLD) == SPI_RXFIFOFullThreshold_14) || \
													((THRESHOLD) == SPI_RXFIFOFullThreshold_15) || \
													((THRESHOLD) == SPI_RXFIFOFullThreshold_16))

/** @defgroup SPI_TXFIFOEmptyThreshold
  * @{
  */
#define SPI_TXFIFOEmptyThreshold_0		  ((uint32_t)0x0000)
#define SPI_TXFIFOEmptyThreshold_1        ((uint32_t)0x0001)
#define SPI_TXFIFOEmptyThreshold_2        ((uint32_t)0x0002)
#define SPI_TXFIFOEmptyThreshold_3        ((uint32_t)0x0003)
#define SPI_TXFIFOEmptyThreshold_4        ((uint32_t)0x0004)
#define SPI_TXFIFOEmptyThreshold_5        ((uint32_t)0x0005)
#define SPI_TXFIFOEmptyThreshold_6        ((uint32_t)0x0006)
#define SPI_TXFIFOEmptyThreshold_7        ((uint32_t)0x0007)
#define SPI_TXFIFOEmptyThreshold_8        ((uint32_t)0x0008)
#define SPI_TXFIFOEmptyThreshold_9        ((uint32_t)0x0009)
#define SPI_TXFIFOEmptyThreshold_10       ((uint32_t)0x000A)
#define SPI_TXFIFOEmptyThreshold_11       ((uint32_t)0x000B)
#define SPI_TXFIFOEmptyThreshold_12       ((uint32_t)0x000C)
#define SPI_TXFIFOEmptyThreshold_13       ((uint32_t)0x000D)
#define SPI_TXFIFOEmptyThreshold_14       ((uint32_t)0x000E)
#define SPI_TXFIFOEmptyThreshold_15       ((uint32_t)0x000F)

#define IS_SPI_TX_FIFO_EMPTY_THRESHOLD(THRESHOLD)	(((THRESHOLD) == SPI_TXFIFOEmptyThreshold_0) || \
													((THRESHOLD) == SPI_TXFIFOEmptyThreshold_1) || \
													((THRESHOLD) == SPI_TXFIFOEmptyThreshold_2) || \
													((THRESHOLD) == SPI_TXFIFOEmptyThreshold_3) || \
													((THRESHOLD) == SPI_TXFIFOEmptyThreshold_4) || \
													((THRESHOLD) == SPI_TXFIFOEmptyThreshold_5) || \
													((THRESHOLD) == SPI_TXFIFOEmptyThreshold_6) || \
													((THRESHOLD) == SPI_TXFIFOEmptyThreshold_7) || \
													((THRESHOLD) == SPI_TXFIFOEmptyThreshold_8) || \
													((THRESHOLD) == SPI_TXFIFOEmptyThreshold_9) || \
													((THRESHOLD) == SPI_TXFIFOEmptyThreshold_10) || \
													((THRESHOLD) == SPI_TXFIFOEmptyThreshold_11) || \
													((THRESHOLD) == SPI_TXFIFOEmptyThreshold_12) || \
													((THRESHOLD) == SPI_TXFIFOEmptyThreshold_13) || \
													((THRESHOLD) == SPI_TXFIFOEmptyThreshold_14) || \
													((THRESHOLD) <= SPI_TXFIFOEmptyThreshold_15))
/**
  * @}
  */ 
#define SPI_DMAReceiveLevel_1				    ((uint32_t)0x0000)
#define SPI_DMAReceiveLevel_2			        ((uint32_t)0x0001)
#define SPI_DMAReceiveLevel_3					((uint32_t)0x0002)
#define SPI_DMAReceiveLevel_4					((uint32_t)0x0003)
#define SPI_DMAReceiveLevel_5					((uint32_t)0x0004)
#define SPI_DMAReceiveLevel_6					((uint32_t)0x0005)
#define SPI_DMAReceiveLevel_7					((uint32_t)0x0006)
#define SPI_DMAReceiveLevel_8					((uint32_t)0x0007)
#define SPI_DMAReceiveLevel_9					((uint32_t)0x0008)
#define SPI_DMAReceiveLevel_10					((uint32_t)0x0009)
#define SPI_DMAReceiveLevel_11					((uint32_t)0x000A)
#define SPI_DMAReceiveLevel_12					((uint32_t)0x000B)
#define SPI_DMAReceiveLevel_13					((uint32_t)0x000C)
#define SPI_DMAReceiveLevel_14					((uint32_t)0x000D)
#define SPI_DMAReceiveLevel_15					((uint32_t)0x000E)
#define SPI_DMAReceiveLevel_16					((uint32_t)0x000F)

#define IS_SPI_DMA_RECEIVE_LEVEL(LEVEL)			((((int32_t)(LEVEL)) >= SPI_DMAReceiveLevel_1) && \
											(((int32_t)(LEVEL)) <= SPI_DMAReceiveLevel_16))

/** @defgroup SPI_DMATransmitLevel 
  * @{
  */
#define SPI_DMATransmitLevel_0					((uint32_t)0x0000)
#define SPI_DMATransmitLevel_1					((uint32_t)0x0001)
#define SPI_DMATransmitLevel_2					((uint32_t)0x0002)
#define SPI_DMATransmitLevel_3					((uint32_t)0x0003)
#define SPI_DMATransmitLevel_4					((uint32_t)0x0004)
#define SPI_DMATransmitLevel_5					((uint32_t)0x0005)
#define SPI_DMATransmitLevel_6					((uint32_t)0x0006)
#define SPI_DMATransmitLevel_7					((uint32_t)0x0007)
#define SPI_DMATransmitLevel_8					((uint32_t)0x0008)
#define SPI_DMATransmitLevel_9					((uint32_t)0x0009)
#define SPI_DMATransmitLevel_10					((uint32_t)0x000A)
#define SPI_DMATransmitLevel_11					((uint32_t)0x000B)
#define SPI_DMATransmitLevel_12					((uint32_t)0x000C)
#define SPI_DMATransmitLevel_13					((uint32_t)0x000D)
#define SPI_DMATransmitLevel_14					((uint32_t)0x000E)
#define SPI_DMATransmitLevel_15					((uint32_t)0x000F)

#define IS_SPI_DMA_TRANSMIT_LEVEL(LEVEL)	((((int32_t)(LEVEL)) >= SPI_DMATransmitLevel_0) && \
											(((int32_t)(LEVEL)) <= SPI_DMATransmitLevel_15))
/** @defgroup SPI_DMA_transfer_requests 
  * @{
  */

#define SPI_DMAReq_Rx               ((uint32_t)0x0001)
#define SPI_DMAReq_Tx               ((uint32_t)0x0002)
#define IS_SPI_DMAREQ(DMAREQ)		((((DMAREQ) & (uint32_t)0xFFFC) == (uint32_t)0x00 ) && \
									((DMAREQ) != (uint32_t)0x00))


#define SPI_FLAG_BSY	                ((uint32_t)0x0001)
#define SPI_FLAG_TXNF	                ((uint32_t)0x0002)				//clear by hardware
#define SPI_FLAG_TXE					((uint32_t)0x0004)				//clear by hardware
#define SPI_FLAG_RXNE	                ((uint32_t)0x0008)				//clear by hardware
#define SPI_FLAG_RXF					((uint32_t)0x0010)				//clear by hardware
#define SPI_FLAG_TXERR					((uint32_t)0x0020)				//read clear
#define SPI_FLAG_DCERR					((uint32_t)0x0040)				//read clear
#define IS_SPI_GET_FLAG(FLAG)			(((FLAG) == SPI_SR_RFNE) || \
										((FLAG) == SPI_SR_RFF) || \
										((FLAG) == SPI_SR_TFE) || \
										((FLAG) == SPI_SR_TFNF) || \
										((FLAG) == SPI_SR_BUSY))

/**
  * @}
  */ 

/** @defgroup NSM_Direction 
  * @{
  */
#define NSM_Direction_Data_Transmit			((uint32_t)(0x0001))
#define NSM_Direction_Data_Receive			((uint32_t)(0x0002))
#define IS_NSM_DIRECTION_MODE(MODE)			(((MODE) == NSM_Direction_Data_Transmit) || \
											((MODE) == NSM_Direction_Data_Receive))
/**
  * @}
  */ 


/** @defgroup NSM_TransferMode 
  * @{
  */											
#define NSM_TransferMode_Non_Sequential		((uint32_t)(0x0001))
#define NSM_TransferMode_Sequential			((uint32_t)(0x0002))
#define IS_NSM_TRANSFER_MODE(MODE)			(((MODE) == NSM_TransferMode_Non_Sequential) || \
											((MODE) == NSM_TransferMode_Sequential))											
/**
  * @}
  */ 


/** @defgroup NSM_DataSize 
  * @{
  */
#define NSM_DataSize_8b                 ((uint32_t)0x0007)
#define NSM_DataSize_16b                ((uint32_t)0x000F)
#define IS_NSM_DATASIZE(DATASIZE)		(((DATASIZE) == NSM_DataSize_8b) || \
										((DATASIZE) == NSM_DataSize_16b))
/**
  * @}
  */ 


/** @defgroup NSM_ControlDataSize 
  * @{
  */
#define NSM_ControlDataSize_8b           ((uint32_t)0x0007)
#define NSM_ControlDataSize_16b          ((uint32_t)0x000F)
#define IS_NSM_CONTROL_DATASIZE(DATASIZE)		(((DATASIZE) == NSM_ControlDataSize_8b) || \
												((DATASIZE) == NSM_ControlDataSize_16b))



//**********************************************************************************************

#define SPI_IT_TXE						((uint32_t)0x0001)				//clear by hardware
#define SPI_IT_TXO						((uint32_t)0x0002)				//clear by read TXOICR register
#define SPI_IT_RXU						((uint32_t)0x0004)				//clear by read RXURCR register
#define SPI_IT_RXO						((uint32_t)0x0008)				//clear by read RXOICR register
#define SPI_IT_RXF						((uint32_t)0x0010)				//clear by hardware
#define SPI_IT_MST						((uint32_t)0x0020)				//clear by read MSTICR register
#define SPI_IT_All						((uint32_t)0x003F)



void spi_init(SPI_TypeDef* SPIx, spi_init_type_def* spi_init_struct);
void spi_cmd(SPI_TypeDef* SPIx, functional_state new_state);
void spi_it_config(SPI_TypeDef* SPIx, uint32_t spi_it, functional_state new_state);
boolean spi_is_rx_no_empty(SPI_TypeDef* SPIx);
boolean spi_is_tx_empty(SPI_TypeDef* SPIx);
void spi_send_data(SPI_TypeDef* SPIx, uint16_t Data);
uint16_t spi_receive_data(SPI_TypeDef* SPIx);
itstatus spi_get_it_status(SPI_TypeDef* SPIx, uint32_t spi_it);

//*************************************
flagstatus spi_get_flag_status(SPI_TypeDef* SPIx, uint32_t spi_flag);
void spi_clear_it_pending_bit(SPI_TypeDef* SPIx, uint32_t spi_it);




#endif
