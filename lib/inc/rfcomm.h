/**
******************************************************************************************************************************************************************************************
*@file                       rfcomm.h
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

#ifndef RFCOMM_H
#define RFCOMM_H
 
#include "host_stack_util.h"
#include <stdint.h>
#include "gap.h"

#if defined __cplusplus
extern "C" {
#endif
    
/* API_START */

/** 
 * @brief Set up RFCOMM.
 */
void rfcomm_init(void);

/** 
 * @brief Set security level required for incoming connections, need to be called before registering services.
 */
void rfcomm_set_required_security_level(gap_security_level_t security_level);

/* 
 * @brief Create RFCOMM connection to a given server channel on a remote deivce.
 * This channel will automatically provide enough credits to the remote side.
 * @param addr
 * @param server_channel
 * @param out_cid
 * @result status
 */
uint8_t rfcomm_create_channel(host_stack_packet_handler_t packet_handler, bd_addr_t addr, uint8_t server_channel, uint16_t * out_cid);

/* 
 * @brief Create RFCOMM connection to a given server channel on a remote deivce.
 * This channel will use explicit credit management. During channel establishment, an initial  amount of credits is provided.
 * @param addr
 * @param server_channel
 * @param initial_credits
 * @param out_cid
 * @result status
 */
uint8_t rfcomm_create_channel_with_initial_credits(host_stack_packet_handler_t packet_handler, bd_addr_t addr, uint8_t server_channel, uint8_t initial_credits, uint16_t * out_cid);

/** 
 * @brief Disconnects RFCOMM channel with given identifier. 
 */
void rfcomm_disconnect(uint16_t rfcomm_cid);

/** 
 * @brief Registers RFCOMM service for a server channel and a maximum frame size, and assigns a packet handler.
 * This channel provides credits automatically to the remote side -> no flow control
 * @param packet handler for all channels of this service
 * @param channel 
 * @param max_frame_size
 */
uint8_t rfcomm_register_service(host_stack_packet_handler_t packet_handler, uint8_t channel, uint16_t max_frame_size);

/** 
 * @brief Registers RFCOMM service for a server channel and a maximum frame size, and assigns a packet handler. 
 * This channel will use explicit credit management. During channel establishment, an initial amount of credits is provided.
 * @param packet handler for all channels of this service
 * @param channel 
 * @param max_frame_size
 * @param initial_credits
 */
uint8_t rfcomm_register_service_with_initial_credits(host_stack_packet_handler_t packet_handler, uint8_t channel, uint16_t max_frame_size, uint8_t initial_credits);

/** 
 * @brief Unregister RFCOMM service.
 */
void rfcomm_unregister_service(uint8_t service_channel);

/** 
 * @brief Accepts incoming RFCOMM connection.
 */
void rfcomm_accept_connection(uint16_t rfcomm_cid);

/** 
 * @brief Deny incoming RFCOMM connection.
 */
void rfcomm_decline_connection(uint16_t rfcomm_cid);

/** 
 * @brief Grant more incoming credits to the remote side for the given RFCOMM channel identifier.
 */
void rfcomm_grant_credits(uint16_t rfcomm_cid, uint8_t credits);

/** 
 * @brief Checks if RFCOMM can send packet. 
 * @param rfcomm_cid
 * @result != 0 if can send now
 */
int rfcomm_can_send_packet_now(uint16_t rfcomm_cid);

/** 
 * @brief Request emission of RFCOMM_EVENT_CAN_SEND_NOW as soon as possible
 * @note RFCOMM_EVENT_CAN_SEND_NOW might be emitted during call to this function
 *       so packet handler should be ready to handle it
 * @param rfcomm_cid
 */
void rfcomm_request_can_send_now_event(uint16_t rfcomm_cid);

/** 
 * @brief Sends RFCOMM data packet to the RFCOMM channel with given identifier.
 * @param rfcomm_cid
 */
int  rfcomm_send(uint16_t rfcomm_cid, uint8_t *data, uint16_t len);

/** 
 * @brief Sends Local Line Status, see LINE_STATUS_..
 * @param rfcomm_cid
 * @param line_status
 */
int rfcomm_send_local_line_status(uint16_t rfcomm_cid, uint8_t line_status);

/** 
 * @brief Send local modem status. see MODEM_STAUS_..
 * @param rfcomm_cid
 * @param modem_status
 */
int rfcomm_send_modem_status(uint16_t rfcomm_cid, uint8_t modem_status);

/** 
 * @brief Configure remote port 
 * @param rfcomm_cid
 * @param baud_rate
 * @param data_bits
 * @param stop_bits
 * @param parity
 * @param flow_control
 */
int rfcomm_send_port_configuration(uint16_t rfcomm_cid, rpn_baud_t baud_rate, rpn_data_bits_t data_bits, rpn_stop_bits_t stop_bits, rpn_parity_t parity, rpn_flow_control_t flow_control);

/** 
 * @brief Query remote port 
 * @param rfcomm_cid
 */
int rfcomm_query_port_configuration(uint16_t rfcomm_cid);

/** 
 * @brief Query max frame size
 * @param rfcomm_cid
 */
uint16_t  rfcomm_get_max_frame_size(uint16_t rfcomm_cid);

/** 
 * @brief set max mtu size
 */
void rfcomm_mtu_size(uint16_t size);

/** 
 * @brief Allow to create RFCOMM packet in outgoing buffer.
 * if (rfcomm_can_send_packet_now(cid)){
 *     rfcomm_reserve_packet_buffer();
 *     uint8_t * buffer = rfcomm_get_outgoing_buffer();
 *     uint16_t buffer_size = rfcomm_get_max_frame_size(cid);
 *     .. setup data in buffer with len ..
 *     rfcomm_send_prepared(cid, len)
 * }
 */
int       rfcomm_reserve_packet_buffer(void);
uint8_t * rfcomm_get_outgoing_buffer(void);
int       rfcomm_send_prepared(uint16_t rfcomm_cid, uint16_t len);
void      rfcomm_release_packet_buffer(void);

/* API_END */

#if defined __cplusplus
}
#endif

#endif // RFCOMM_H
