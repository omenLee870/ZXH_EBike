/**
******************************************************************************************************************************************************************************************
*@file                       hid_device.h
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

#ifndef HID_DEVICE_H
#define HID_DEVICE_H

#include <stdint.h>
#include "host_stack_defines.h"
#include "bluetooth.h"

#if defined __cplusplus
extern "C" {
#endif

typedef enum {
    HID_REPORT_TYPE_RESERVED = 0,
    HID_REPORT_TYPE_INPUT,
    HID_REPORT_TYPE_OUTPUT,
    HID_REPORT_TYPE_FEATURE
} hid_report_type_t;

/* API_START */
/**
 * @brief Create HID Device SDP service record. 
 * @param service Empty buffer in which a new service record will be stored.
 * @param have_remote_audio_control 
 * @param service
 * @param service_record_handle
 * @param hid_device_subclass
 * @param hid_country_code
 * @param hid_virtual_cable
 * @param hid_reconnect_initiate
 * @param hid_boot_device
 * @param hid_descriptor
 * @param hid_descriptor_size size of hid_descriptor
 * @param device_name
 */
void hid_create_sdp_record(
    uint8_t *       service, 
    uint32_t        service_record_handle,
    uint16_t        hid_device_subclass,
    uint8_t         hid_country_code,
    uint8_t         hid_virtual_cable,
    uint8_t         hid_reconnect_initiate,
    uint8_t         hid_boot_device,
    const uint8_t * hid_descriptor,
    uint16_t 		hid_descriptor_size,
    const char *    device_name);


/**
 * @brief Set up HID Device 
 * @param boot_protocol_mode_supported
 * @param hid_descriptor_len
 * @param hid_descriptor
 */
void hid_device_init(uint8_t boot_protocol_mode_supported, uint16_t hid_descriptor_len, const uint8_t * hid_descriptor);

/** 
 * @brief Unregister service record internally.
 */
void hid_unregister_service(void);

/**
 * @brief Register callback for the HID Device client. 
 * @param callback
 */
void hid_device_register_packet_handler(host_stack_packet_handler_t callback);

/**
 * @brief Register get report callback for the HID Device client. 
 * @param callback
 */
void hid_device_register_report_request_callback(int (*callback)(uint16_t hid_cid, hid_report_type_t report_type, uint16_t report_id, int * out_report_size, uint8_t * out_report));

/**
 * @brief Register set report callback for the HID Device client. 
 * @param callback
 */
void hid_device_register_set_report_callback(void (*callback)(uint16_t hid_cid, hid_report_type_t report_type, int report_size, uint8_t * report));

/**
 * @brief Register callback to receive report data for the HID Device client. 
 * @param callback
 */
void hid_device_register_report_data_callback(void (*callback)(uint16_t cid, hid_report_type_t report_type, uint16_t report_id, int report_size, uint8_t * report));

/*
 * @brief Create HID connection to HID Host
 * @param addr
 * @param hid_cid to use for other commands
 * @result status
 */
uint8_t hid_device_connect(bd_addr_t addr, uint16_t * hid_cid);

/*
 * @brief Disconnect from HID Host
 * @param hid_cid
 */
void hid_device_disconnect(uint16_t hid_cid);

/**
 * @brief Request can send now event to send HID Report
 * Generates an HID_SUBEVENT_CAN_SEND_NOW subevent
 * @param hid_cid
 */
void hid_device_request_can_send_now_event(uint16_t hid_cid);

/**
 * @brief Send HID messageon interrupt channel
 * @param hid_cid
 */
int hid_device_send_interrupt_message(uint16_t hid_cid, const uint8_t * message, uint16_t message_len);

/**
 * @brief Send HID messageon control channel
 * @param hid_cid
 */
int hid_device_send_control_message(uint16_t hid_cid, const uint8_t * message, uint16_t message_len);

/**
 * @brief Retutn 1 if boot protocol mode active
 * @param hid_cid
 */
int hid_device_in_boot_protocol_mode(uint16_t hid_cid);

/* API_END */

/* Only needed for PTS Testing */
void hid_device_disconnect_interrupt_channel(uint16_t hid_cid);
void hid_device_disconnect_control_channel(uint16_t hid_cid);

#if defined __cplusplus
}
#endif

#endif
