/**
******************************************************************************************************************************************************************************************
*@file                       hids_device.h
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
#ifndef HIDS_DEVICE_H
#define HIDS_DEVICE_H

#include <stdint.h>
#include "host_stack_defines.h"
#include "bluetooth.h"

#if defined __cplusplus
extern "C" {
#endif

/**
 * Implementation of the GATT HIDS Device
 * To use with your application, add '#import <hids.gatt>' to your .gatt file
 */

/**
 * @brief Set up HIDS Device
 */
void hids_device_init(uint8_t hid_country_code, const uint8_t * hid_descriptor, uint16_t hid_descriptor_size);

/**
 * @brief Register callback for the HIDS Device client.
 * @param callback
 */
void hids_device_register_packet_handler(host_stack_packet_handler_t callback);

/**
 * @brief Request can send now event to send HID Report
 * Generates an HIDS_SUBEVENT_CAN_SEND_NOW subevent
 * @param hid_cid
 */
void hids_device_request_can_send_now_event(hci_con_handle_t con_handle);

/**
 * @brief Send HID Report_ID (1)
 */
int hids_device_send_report_id_1(hci_con_handle_t con_handle, const uint8_t * report, uint16_t report_len);

/**
 * @brief Send HID Report_ID (2)
 */
int hids_device_send_report_id_2(hci_con_handle_t con_handle, const uint8_t * report, uint16_t report_len);

/**
 * @brief Send HID Report_ID (3)
 */
int hids_device_send_report_id_3(hci_con_handle_t con_handle, const uint8_t * report, uint16_t report_len);

/**
 * @brief Send HID Boot Mouse Input Report
 */
int hids_device_send_boot_mouse_input_report(hci_con_handle_t con_handle, const uint8_t * report, uint16_t report_len);

/**
 * @brief Send HID Boot Mouse Input Report
 */
int hids_device_send_boot_keyboard_input_report(hci_con_handle_t con_handle, const uint8_t * report, uint16_t report_len);

#if defined __cplusplus
}
#endif

#endif
