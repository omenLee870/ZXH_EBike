/**
******************************************************************************************************************************************************************************************
*@file                       heart_rate_service_server.h
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
#ifndef HEART_RATE_SERVICE_SERVER_H
#define HEART_RATE_SERVICE_SERVER_H

#include <stdint.h>

#if defined __cplusplus
extern "C" {
#endif

/**
 * Implementation of the GATT Heart Rate Service Server 
 */

// *****************************************************************************
/* GATT_SERVICE_SERVER_START(heart_rate_service_server){Heart Rate Service} 
 *
 * The heart rate service server provides heart rate measurements via 
 * notifications.
 *
 * Each notification reports the heart rate measurement in beats per minute, and if enabled, 
 * the total energy expended in kilo Joules, as well as RR-intervals in 1/1024 seconds resolution.
 *
 * The Energy Expended field represents the accumulated energy expended
 * in kilo Joules since the last time it was reset. If the maximum value of 65535
 * kilo Joules is reached, it will remain at this value, until a reset command
 * from the client is received.
 *  
 * The RR-Interval represents the time between two consecutive R waves in 
 * an Electrocardiogram (ECG) waveform. If needed, the RR-Intervals are sent in
 * multiple notifications.
 * 
 * To use with your application, add `#import <heart_rate_service.gatt>` to your .gatt file.
 * After adding it to your .gatt file, you call *heart_rate_server_init(body_sensor_location, energy_expended_supported)*
 * with the intended sensor location, and a flag indicating if energy expanded is supported.
 * 
 * If heart rate measurement changes, you can call 
 * *heart_rate_service_server_update_heart_rate_values(heart_rate_bpm, service_sensor_contact_status, rr_interval_count, rr_intervals)*. 
 * This function will trigger sending Notifications if the client enables them.
 * 
 * If energy expanded is supported, you can call *heart_rate_service_add_energy_expended(energy_expended_kJ)* 
 * with the newly expanded energy. The accumulated energy expended value
 * will be emitted with the next heart rate measurement.
 *
 */
// *****************************************************************************
/* GATT_SERVICE_SERVER_END */

/* API_START */

typedef enum {
	HEART_RATE_SERVICE_BODY_SENSOR_LOCATION_OTHER = 0,
	HEART_RATE_SERVICE_BODY_SENSOR_LOCATION_CHEST,
	HEART_RATE_SERVICE_BODY_SENSOR_LOCATION_WRIST,
	HEART_RATE_SERVICE_BODY_SENSOR_LOCATION_FINGER,
	HEART_RATE_SERVICE_BODY_SENSOR_LOCATION_HAND,
	HEART_RATE_SERVICE_BODY_SENSOR_LOCATION_EAR_LOBE,
	HEART_RATE_SERVICE_BODY_SENSOR_LOCATION_FOOT
} heart_rate_service_body_sensor_location_t;

typedef enum {
	HEART_RATE_SERVICE_SENSOR_CONTACT_UNKNOWN = 0,
	HEART_RATE_SERVICE_SENSOR_CONTACT_UNSUPPORTED,
	HEART_RATE_SERVICE_SENSOR_CONTACT_NO_CONTACT,
	HEART_RATE_SERVICE_SENSOR_CONTACT_HAVE_CONTACT
} heart_rate_service_sensor_contact_status_t;


/**
 * @brief Init Battery Service Server with ATT DB
 * @param body_sensor_location
 * @param energy_expended_supported
 */
void heart_rate_service_server_init(heart_rate_service_body_sensor_location_t body_sensor_location, int energy_expended_supported);


/**
 * @brief Add Energy Expended to the internal accumulator.
 * @param energy_expended_kJ   energy expended in kilo Joules since the last update
 */
void heart_rate_service_add_energy_expended(uint16_t energy_expended_kJ);

/**
 * @brief Update heart rate (unit: beats per minute)
 * @note triggers notifications if subscribed
 * @param heart_rate_bpm 		beats per minute
 * @param contact    
 * @param rr_interval_count 
 * @param rr_intervals      resolution in 1/1024 seconds
 */
void heart_rate_service_server_update_heart_rate_values(uint16_t heart_rate_bpm, 
	heart_rate_service_sensor_contact_status_t contact, int rr_interval_count, uint16_t * rr_intervals);

/* API_END */

#if defined __cplusplus
}
#endif

#endif

