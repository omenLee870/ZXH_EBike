/**
******************************************************************************************************************************************************************************************
*@file                       host_stack_util.h
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

#ifndef BTSTACK_UTIL_H
#define BTSTACK_UTIL_H


#if defined __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

#include "bluetooth.h"
#include "host_stack_defines.h"
	
// hack: compilation with the android ndk causes an error as there's a reverse_64 macro
#ifdef reverse_64
#undef reverse_64
#endif

// will be moved to daemon/host_stack_device_name_db.h


/**
 * @brief The device name type
 */
#define DEVICE_NAME_LEN 248
typedef uint8_t device_name_t[DEVICE_NAME_LEN+1]; 

/* API_START */

/**
 * @brief Minimum function for uint32_t
 * @param a
 * @param b
 * @return value
 */
uint32_t host_stack_min(uint32_t a, uint32_t b);

/**
 * @brief Maximum function for uint32_t
 * @param a
 * @param b
 * @return value
 */
uint32_t host_stack_max(uint32_t a, uint32_t b);

/**
 * @brief Calculate delta between two points in time
 * @returns time_a - time_b - result > 0 if time_a is newer than time_b
 */
int32_t host_stack_time_delta(uint32_t time_a, uint32_t time_b);

/** 
 * @brief Read 16/24/32 bit little endian value from buffer
 * @param buffer
 * @param position in buffer
 * @return value
 */
uint16_t little_endian_read_16(const uint8_t * buffer, int position);
uint32_t little_endian_read_24(const uint8_t * buffer, int position);
uint32_t little_endian_read_32(const uint8_t * buffer, int position);

/** 
 * @brief Write 16/32 bit little endian value into buffer
 * @param buffer
 * @param position in buffer
 * @param value
 */
void little_endian_store_16(uint8_t *buffer, uint16_t position, uint16_t value);
void little_endian_store_24(uint8_t *buffer, uint16_t position, uint32_t value);
void little_endian_store_32(uint8_t *buffer, uint16_t position, uint32_t value);

/** 
 * @brief Read 16/24/32 bit big endian value from buffer
 * @param buffer
 * @param position in buffer
 * @return value
 */
uint32_t big_endian_read_16( const uint8_t * buffer, int pos);
uint32_t big_endian_read_24( const uint8_t * buffer, int pos);
uint32_t big_endian_read_32( const uint8_t * buffer, int pos);

/** 
 * @brief Write 16/32 bit big endian value into buffer
 * @param buffer
 * @param position in buffer
 * @param value
 */
void big_endian_store_16(uint8_t *buffer, uint16_t pos, uint16_t value);
void big_endian_store_24(uint8_t *buffer, uint16_t pos, uint32_t value);
void big_endian_store_32(uint8_t *buffer, uint16_t pos, uint32_t value);


/**
 * @brief Swap bytes in 16 bit integer
 */
static inline uint16_t host_stack_flip_16(uint16_t value){
    return (uint16_t)((value & 0xff) << 8) | (value >> 8);
}

/** 
 * @brief Check for big endian system
 * @returns 1 if on big endian
 */
static inline int host_stack_is_big_endian(void){
	uint16_t sample = 0x0100;
	return *(uint8_t*) &sample;
}

/** 
 * @brief Check for little endian system
 * @returns 1 if on little endian
 */
static inline int host_stack_is_little_endian(void){
	uint16_t sample = 0x0001;
	return *(uint8_t*) &sample;
}

/**
 * @brief Copy from source to destination and reverse byte order
 * @param src
 * @param dest
 * @param len
 */
void reverse_bytes  (const uint8_t *src, uint8_t * dest, int len);

/**
 * @brief Wrapper around reverse_bytes for common buffer sizes
 * @param src
 * @param dest
 */
void reverse_24 (const uint8_t *src, uint8_t * dest);
void reverse_48 (const uint8_t *src, uint8_t * dest);
void reverse_56 (const uint8_t *src, uint8_t * dest);
void reverse_64 (const uint8_t *src, uint8_t * dest);
void reverse_128(const uint8_t *src, uint8_t * dest);
void reverse_256(const uint8_t *src, uint8_t * dest);

void reverse_bd_addr(const bd_addr_t src, bd_addr_t dest);

/** 
 * @brief ASCII character for 4-bit nibble
 * @return character
 */
char char_for_nibble(int nibble);

/**
 * @brif 4-bit nibble from ASCII character
 * @return value
 */
int nibble_for_char(char c);

/**
 * @brief Compare two Bluetooth addresses
 * @param a
 * @param b
 * @return 0 if equal
 */
int bd_addr_cmp(const bd_addr_t a, const bd_addr_t b);

/**
 * @brief Copy Bluetooth address
 * @param dest
 * @param src
 */
void bd_addr_copy(bd_addr_t dest, const bd_addr_t src);

/**
 * @brief Create human readable representation for UUID128
 * @note uses fixed global buffer
 * @return pointer to UUID128 string
 */
char * uuid128_to_str(const uint8_t * uuid);

/**
 * @brief Create human readable represenationt of Bluetooth address
 * @note uses fixed global buffer
 * @return pointer to Bluetooth address string
 */
char * bd_addr_to_str(const bd_addr_t addr);

/**
 * @brief Parse unsigned number 
 * @param str to parse
 * @return value
 */
uint32_t host_stack_atoi(const char *str);

/* API_END */

#if defined __cplusplus
}
#endif
		
#endif // BTSTACK_UTIL_H
