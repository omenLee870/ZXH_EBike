/**
******************************************************************************************************************************************************************************************
*@file                       sdp_client_rfcomm.h
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

#ifndef SDP_QUERY_RFCOMM_H
#define SDP_QUERY_RFCOMM_H

#include "host_stack_util.h"

#define SDP_SERVICE_NAME_LEN 20

#if defined __cplusplus
extern "C" {
#endif

/* API_START */

/** 
 * @brief Searches SDP records on a remote device for RFCOMM services with a given 16-bit UUID.
 * @note calls sdp_service_search_pattern_for_uuid16 that uses global buffer
 */
uint8_t sdp_client_query_rfcomm_channel_and_name_for_uuid(host_stack_packet_handler_t callback, bd_addr_t remote, uint16_t uuid);

/** 
 * @brief Searches SDP records on a remote device for RFCOMM services with a given 128-bit UUID.
 * @note calls sdp_service_search_pattern_for_uuid128 that uses global buffer
 */
uint8_t sdp_client_query_rfcomm_channel_and_name_for_uuid128(host_stack_packet_handler_t callback, bd_addr_t remote, const uint8_t * uuid128);

/** 
 * @brief Searches SDP records on a remote device for RFCOMM services with a given service search pattern.
 */
uint8_t sdp_client_query_rfcomm_channel_and_name_for_search_pattern(host_stack_packet_handler_t callback, bd_addr_t remote, const uint8_t * des_serviceSearchPattern);

/* API_END */

#if defined __cplusplus
}
#endif

#endif // SDP_QUERY_RFCOMM_H
