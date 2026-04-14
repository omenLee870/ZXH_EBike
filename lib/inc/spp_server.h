/**
******************************************************************************************************************************************************************************************
*@file                       spp_server.h
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

#ifndef SPP_SERVER_H
#define SPP_SERVER_H

#include <stdint.h>
 
#if defined __cplusplus
extern "C" {
#endif

/* API_START */

/**
 * @brief Create SDP record for SPP service with official SPP Service Class
 * @param service buffer - needs to large enough
 * @param service_record_handle
 * @param rfcomm_channel
 * @param name
 */
void spp_create_sdp_record(uint8_t *service, uint32_t service_record_handle, int rfcomm_channel, const char *name);

/**
 * @brief Create SDP record for SPP service with custom service UUID (e.g. for use with Android)
 * @param service buffer - needs to large enough
 * @param service_record_handle
 * @param service_uuid128 buffer
 * @param rfcomm_channel
 * @param name
 */
void spp_create_custom_sdp_record(uint8_t *service, uint32_t service_record_handle, const uint8_t * service_uuid128, int rfcomm_channel, const char *name);

/* API_END */

#if defined __cplusplus
}
#endif
#endif // SPP_SERVER_H
