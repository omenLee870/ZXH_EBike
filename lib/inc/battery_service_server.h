/**
******************************************************************************************************************************************************************************************
*@file                       battery_service_server.h
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
#ifndef BATTERY_SERVICE_SERVER_H
#define BATTERY_SERVICE_SERVER_H

#include <stdint.h>

#if defined __cplusplus
extern "C" {
#endif

/**
 * Implementation of the GATT Battery Service Server 
 * To use with your application, add '#import <battery_service.gatt' to your .gatt file
 */

/* API_START */

/**
 * @brief Init Battery Service Server with ATT DB
 * @param battery_value in range 0-100
 */
void battery_service_server_init(uint8_t battery_value);

/**
 * @brief Update battery value
 * @note triggers notifications if subscribed
 * @param battery_value in range 0-100
 */
void battery_service_server_set_battery_value(uint8_t battery_value);

/* API_END */

#if defined __cplusplus
}
#endif

#endif

