/**
******************************************************************************************************************************************************************************************
*@file                       att_server.h
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
#ifndef ATT_SERVER_H
#define ATT_SERVER_H

#include <stdint.h>
#include "bluetooth.h"

#if defined __cplusplus
extern "C" {
#endif

// ATT Client Read Callback for Dynamic Data
// - if buffer == NULL, don't copy data, just return size of value
// - if buffer != NULL, copy data and return number bytes copied
// If ENABLE_ATT_DELAYED_READ_RESPONSE is defined, you may return ATT_READ_RESPONSE_PENDING if data isn't available yet
// @param con_handle of hci le connection
// @param attribute_handle to be read
// @param offset defines start of attribute value
// @param buffer 
// @param buffer_size
typedef uint16_t (*att_read_callback_t)(hci_con_handle_t con_handle, uint16_t attribute_handle, uint16_t offset, uint8_t * buffer, uint16_t buffer_size);

// ATT Client Write Callback for Dynamic Data
// @param con_handle of hci le connection
// @param attribute_handle to be written
// @param transaction - ATT_TRANSACTION_MODE_NONE for regular writes. For prepared writes: ATT_TRANSACTION_MODE_ACTIVE, ATT_TRANSACTION_MODE_VALIDATE, ATT_TRANSACTION_MODE_EXECUTE, ATT_TRANSACTION_MODE_CANCEL
// @param offset into the value - used for queued writes and long attributes
// @param buffer 
// @param buffer_size
// @param signature used for signed write commmands
// @returns 0 if write was ok, ATT_ERROR_PREPARE_QUEUE_FULL if no space in queue, ATT_ERROR_INVALID_OFFSET if offset is larger than max buffer
//
// Each Prepared Write Request triggers a callback with transaction mode ATT_TRANSACTION_MODE_ACTIVE.
// On Execute Write, the callback will be called with ATT_TRANSACTION_MODE_VALIDATE and allows to validate all queued writes and return an application error.
// If none of the registered callbacks return an error for ATT_TRANSACTION_MODE_VALIDATE and the callback will be called with ATT_TRANSACTION_MODE_EXECUTE.
// Otherwise, all callbacks will be called with ATT_TRANSACTION_MODE_CANCEL.
//
// If the additional validation step is not needed, just return 0 for all callbacks with transaction mode ATT_TRANSACTION_MODE_VALIDATE.
//
typedef int (*att_write_callback_t)(hci_con_handle_t con_handle, uint16_t attribute_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size);

/* API_START */
/*
 * @brief setup ATT server
 * @param db attribute database created by compile-gatt.ph
 * @param read_callback, see att_db.h, can be NULL
 * @param write_callback, see attl.h, can be NULL
 */
void att_server_init(uint8_t const * db, att_read_callback_t read_callback, att_write_callback_t write_callback);

/**
 * @brief Set ATT MTU
 * @param size
 */
void att_mtu_size(uint16_t size);

/**
 * @brief Return ATT MTU
 * @param con_handle
 * @return mtu if ok, 0 otherwise
 */
uint16_t att_server_get_mtu(hci_con_handle_t con_handle);

/*
 * @brief notify client about attribute value change
 * @param con_handle
 * @param attribute_handle
 * @param value
 * @param value_len
 * @return 0 if ok, error otherwise
 */
int att_server_notify(hci_con_handle_t con_handle, uint16_t attribute_handle, const uint8_t *value, uint16_t value_len);

/*
 * @brief indicate value change to client. client is supposed to reply with an indication_response
 * @param con_handle
 * @param attribute_handle
 * @param value
 * @param value_len
 * @return 0 if ok, error otherwise
 */
int att_server_indicate(hci_con_handle_t con_handle, uint16_t attribute_handle, const uint8_t *value, uint16_t value_len);


// the following functions will be removed soon

/*
 * @brief tests if a notification or indication can be send right now
 * @param con_handle
 * @return 1, if packet can be sent
 */
int  att_server_can_send_packet_now(hci_con_handle_t con_handle);

/**
 * @brief Request emission of ATT_EVENT_CAN_SEND_NOW as soon as possible
 * @note ATT_EVENT_CAN_SEND_NOW might be emitted during call to this function
 *       so packet handler should be ready to handle it
 * @param con_handle
 */
void att_server_request_can_send_now_event(hci_con_handle_t con_handle);
// end of deprecated functions

/* API_END */

#if defined __cplusplus
}
#endif

#endif // ATT_SERVER_H
