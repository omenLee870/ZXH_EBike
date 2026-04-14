/**
******************************************************************************************************************************************************************************************
*@file                       sdp_server.h
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
#ifndef SDP_H
#define SDP_H

#include <stdint.h>

#if defined __cplusplus
extern "C" {
#endif
    

/* API_START */

/** 
 * @brief Set up SDP.
 */
void sdp_init(void);

/**
 * @brief Register Service Record with database using ServiceRecordHandle stored in record
 * @pre AttributeIDs are in ascending order
 * @pre ServiceRecordHandle is first attribute and valid
 * @param record is not copied!
 * @result status
 */
uint8_t sdp_register_service(const uint8_t * record);

/** 
 * @brief Unregister service record internally.
 */
void sdp_unregister_service(uint32_t service_record_handle);

/**
 * @brief gets record for handle
 * @result record
 */

uint8_t * sdp_get_record_for_handle(uint32_t handle);

/**
 * @brief gets service record handle from record
 * @resutl service record handle or 0
 */
uint32_t sdp_get_service_record_handle(const uint8_t * record);

/* API_END */

#if defined __cplusplus
}
#endif
#endif // SDP_H
