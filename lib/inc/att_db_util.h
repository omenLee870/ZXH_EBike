/**
******************************************************************************************************************************************************************************************
*@file                       att_db_util.h
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
#ifndef __ATT_DB_UTIL
#define __ATT_DB_UTIL


#include <stdint.h>

#if defined __cplusplus
extern "C" {
#endif

/* API_START */

/** 
* @brief Init ATT DB storage 
*/
void att_db_util_init(void);

/**
 * @brief Add primary service for 16-bit UUID
 * @param uuid16
 * @returns attribute handle for the new service definition
 */
uint16_t att_db_util_add_service_uuid16(uint16_t uuid16);

/**
* @brief Add 16-bit UUID, properties, and data
* @param uuid16
* @param flags
* @param data returned in read operations if ATT_PROPERTY_DYNAMIC is not specified
* @param data_len
*/
void att_db_util_add_attribute_uuid16(uint16_t uuid16, uint16_t flags, uint8_t * data, uint16_t data_len);
/**
 * @brief Add primary service for 128-bit UUID
 * @param uuid128
 * @returns attribute handle for the new service definition
 */
uint16_t att_db_util_add_service_uuid128(const uint8_t * uuid128);

/**
 * @brief Add secondary service for 16-bit UUID
 * @param uuid16
 * @returns attribute handle for the new service definition
 */
uint16_t att_db_util_add_secondary_service_uuid16(uint16_t uuid16);

/**
 * @brief Add secondary service for 128-bit UUID
 * @param uuid128
 * @returns attribute handle for the new service definition
 */
uint16_t att_db_util_add_secondary_service_uuid128(const uint8_t * uuid128);

/**
 * @brief Add included service with 16-bit UUID
 * @param start_group_handle
 * @param end_group_handle
 * @param uuid16
 * @returns attribute handle for the new service definition
 */
uint16_t att_db_util_add_included_service_uuid16(uint16_t start_group_handle, uint16_t  end_group_handle, uint16_t uuid16);

/**
 * @brief Add Characteristic with 16-bit UUID, properties, and data
 * @param uuid16
 * @param properties        - see ATT_PROPERTY_* in src/bluetooth.h
 * @param read_permissions  - see ATT_SECURITY_* in src/bluetooth.h
 * @param write_permissions - see ATT_SECURITY_* in src/bluetooth.h
 * @param data returned in read operations if ATT_PROPERTY_DYNAMIC is not specified
 * @param data_len
 * @returns attribute handle of the new characteristic value declaration
 * @note If properties contains ATT_PROPERTY_NOTIFY or ATT_PROPERTY_INDICATE flags, a Client Configuration Characteristic Descriptor (CCCD)
 *       is created as well. The attribute value handle of the CCCD is the attribute value handle plus 1
 */
uint16_t att_db_util_add_characteristic_uuid16(uint16_t uuid16, uint16_t properties, uint8_t read_permission, uint8_t write_permission, uint8_t * data, uint16_t data_len);

/**
 * @brief Add Characteristic with 128-bit UUID, properties, and data
 * @param uuid128
 * @param properties        - see ATT_PROPERTY_* in src/bluetooth.h
 * @param read_permissions  - see ATT_SECURITY_* in src/bluetooth.h
 * @param write_permissions - see ATT_SECURITY_* in src/bluetooth.h
 * @param data returned in read operations if ATT_PROPERTY_DYNAMIC is not specified
 * @param data_len
 * @returns attribute handle of the new characteristic value declaration
 * @note If properties contains ATT_PROPERTY_NOTIFY or ATT_PROPERTY_INDICATE flags, a Client Configuration Characteristic Descriptor (CCCD)
 *       is created as well. The attribute value handle of the CCCD is the attribute value handle plus 1
 */
uint16_t att_db_util_add_characteristic_uuid128(const uint8_t * uuid128, uint16_t properties, uint8_t read_permission, uint8_t write_permission, uint8_t * data, uint16_t data_len);

/**
* @brief Add descriptor with 16-bit UUID, properties, and data
* @param uuid16
* @param properties        - see ATT_PROPERTY_* in src/bluetooth.h
* @param read_permissions  - see ATT_SECURITY_* in src/bluetooth.h
* @param write_permissions - see ATT_SECURITY_* in src/bluetooth.h
* @param data returned in read operations if ATT_PROPERTY_DYNAMIC is not specified
* @param data_len
* @returns attribute handle of the new characteristic descriptor declaration
*/
uint16_t att_db_util_add_descriptor_uuid16(uint16_t uuid16, uint16_t properties, uint8_t read_permission, uint8_t write_permission, uint8_t * data, uint16_t data_len);

/**
* @brief Add descriptor with 128-bit UUID, properties, and data
* @param uuid128
* @param properties        - see ATT_PROPERTY_* in src/bluetooth.h
* @param read_permissions  - see ATT_SECURITY_* in src/bluetooth.h
* @param write_permissions - see ATT_SECURITY_* in src/bluetooth.h
* @param data returned in read operations if ATT_PROPERTY_DYNAMIC is not specified
* @param data_len
* @returns attribute handle of the new characteristic descriptor declaration
*/
uint16_t att_db_util_add_descriptor_uuid128(const uint8_t * uuid128, uint16_t properties, uint8_t read_permission, uint8_t write_permission, uint8_t * data, uint16_t data_len);

/**
 * @brief Get address of constructed ATT DB
 */
uint8_t * att_db_util_get_address(void);

/**
 * @brief Get size of constructed ATT DB 
 */
uint16_t att_db_util_get_size(void);

/* API_END */

#if defined __cplusplus
}
#endif
#endif
