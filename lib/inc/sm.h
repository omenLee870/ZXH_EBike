/**
******************************************************************************************************************************************************************************************
*@file                      sm.h
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

#ifndef SM_H
#define SM_H

 #if defined __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "host_stack_util.h"
#include "host_stack_defines.h"


/* API_START */

/**
 * @brief Initializes the Security Manager, connects to L2CAP
 */
void sm_init(void);

/**
 * @brief Set secret ER key for key generation as described in Core V4.0, Vol 3, Part G, 5.2.2
 * @note If not set and host_stack_tlv is configured, ER key is generated and stored in TLV by SM
 * @param er
 */
void sm_set_er(sm_key_t er);

/**
 * @brief Set secret IR key for key generation as described in Core V4.0, Vol 3, Part G, 5.2.2
 * @note If not set and host_stack_tlv is configured, IR key is generated and stored in TLV by SM
 */
void sm_set_ir(sm_key_t ir);

/**
 *
 * @brief Registers OOB Data Callback. The callback should set the oob_data and return 1 if OOB data is availble
 * @param get_oob_data_callback
 */
void sm_register_oob_data_callback( int (*get_oob_data_callback)(uint8_t address_type, bd_addr_t addr, uint8_t * oob_data));


/**
 * @brief Limit the STK generation methods. Bonding is stopped if the resulting one isn't in the list
 * @param OR combination of SM_STK_GENERATION_METHOD_ 
 */
void sm_set_accepted_stk_generation_methods(uint8_t accepted_stk_generation_methods);

/**
 * @brief Set the accepted encryption key size range. Bonding is stopped if the result isn't within the range
 * @param min_size (default 7)
 * @param max_size (default 16)
 */
void sm_set_encryption_key_size_range(uint8_t min_size, uint8_t max_size);

/**
 * @brief Sets the requested authentication requirements, bonding yes/no, MITM yes/no, SC yes/no, keypress yes/no
 * @param OR combination of SM_AUTHREQ_ flags
 */
void sm_set_authentication_requirements(uint8_t auth_req);

/**
 * @brief Sets the available IO Capabilities
 * @param IO_CAPABILITY_
 */
void sm_set_io_capabilities(io_capability_t io_capability);

/**
 * @brief Let Peripheral request an encrypted connection right after connecting
 * @note Not used normally. Bonding is triggered by access to protected attributes in ATT Server
 */
void sm_set_request_security(int enable);

/** 
 * @brief Trigger Security Request
 * @note Not used normally. Bonding is triggered by access to protected attributes in ATT Server
 */
void sm_send_security_request(hci_con_handle_t con_handle);

/**
 * @brief Decline bonding triggered by event before
 * @param con_handle
 */
void sm_bonding_decline(hci_con_handle_t con_handle);

/**
 * @brief Confirm Just Works bonding 
 * @param con_handle
 */
void sm_just_works_confirm(hci_con_handle_t con_handle);

/**
 * @brief Confirm value from SM_EVENT_NUMERIC_COMPARISON_REQUEST for Numeric Comparison bonding 
 * @param con_handle
 */
void sm_numeric_comparison_confirm(hci_con_handle_t con_handle);

/**
 * @brief Reports passkey input by user
 * @param con_handle
 * @param passkey in [0..999999]
 */
void sm_passkey_input(hci_con_handle_t con_handle, uint32_t passkey);

/**
 * @brief Send keypress notification for keyboard only devices
 * @param con_handle
 * @param action see SM_KEYPRESS_* in bluetooth.h
 */
void sm_keypress_notification(hci_con_handle_t con_handle, uint8_t action);

/**
 * @brief Used by att_server.c to request user authorization.
 * @param con_handle
 */
void sm_request_pairing(hci_con_handle_t con_handle);

/**
 * @brief Report user authorization decline.
 * @param con_handle
 */
void sm_authorization_decline(hci_con_handle_t con_handle);

/**
 * @brief Report user authorization grant.
 * @param con_handle
 */
void sm_authorization_grant(hci_con_handle_t con_handle);

/**
 * @brief Support for signed writes, used by att_server.
 * @returns ready
 */
int sm_cmac_ready(void);

/**
 * @brief Support for signed writes, used by att_server.
 * @note Message is in little endian to allows passing in ATT PDU without flipping. 
 * @note signing data: [opcode, attribute_handle, message, sign_counter]
 * @note calculated hash in done_callback is big endian and has 16 byte. 
 * @param key
 * @param opcde
 * @param attribute_handle
 * @param message_len
 * @param message
 * @param sign_counter
 */
void sm_cmac_signed_write_start(const sm_key_t key, uint8_t opcode, uint16_t attribute_handle, uint16_t message_len, const uint8_t * message, uint32_t sign_counter, void (*done_callback)(uint8_t * hash));

/*
 * @brief Match address against bonded devices
 * @return 0 if successfully added to lookup queue
 * @note Triggers SM_IDENTITY_RESOLVING_* events
 */
int sm_address_resolution_lookup(uint8_t addr_type, bd_addr_t addr);

/*
 * @brief Match device irk
 * @param irk
 * @param callback reason 0 is match
 */
void sm_irk_match(uint8_t irk[16],void (*sm_handle_encryption_result_cb)(uint8_t reason));
/* API_END */
/*bill*/
void sm_use_fixed_passkey_in_display_role(uint32_t passkey);
void sm_test_set_pairing_failure(int reason);
void sm_pairing_test(uint8_t enable);
#if defined __cplusplus
}
#endif

#endif // SM_H
