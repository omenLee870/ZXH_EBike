
#ifndef _NVS_H_
#define _NVS_H_

#include "types.h"
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Non-volatile Storage
 * @defgroup nvs Non-volatile Storage
 * @ingroup file_system_storage
 * @{
 * @}
 */

/**
 * @brief Non-volatile Storage Data Structures
 * @defgroup nvs_data_structures Non-volatile Storage Data Structures
 * @ingroup nvs
 * @{
 */

/**
 * @brief Non-volatile Storage File system structure
 *
 * @param offset File system offset in flash
 * @param ate_wra: Allocation table entry write address. Addresses are stored
 * as u32_t: high 2 bytes are sector, low 2 bytes are offset in sector,
 * @param data_wra: Data write address.
 * @param sector_size File system is divided into sectors each sector should be
 * multiple of pagesize
 * @param sector_count Amount of sectors in the file systems
 * @param write_block_size Alignment size
 * @param nvs_lock Mutex
 * @param flash_device Flash Device
 */
struct nvs_fs {
	u32_t offset;		/* filesystem offset in flash */
	u32_t ate_wra;		/* next alloc table entry write address */
	u32_t data_wra;		/* next data write address */
	u16_t sector_size;	/* filesystem is divided into sectors,
				 * sector size should be multiple of pagesize
				 */
	u16_t sector_count;	/* amount of sectors in the filesystem */
	u8_t write_block_size;  /* write block size for alignment */
	bool ready;		/* is the filesystem initialized ? */

	void (*disable_irq)(void);
	void (*enable_irq)(void);
	int (*flash_read)(u32_t offset, void *data,
			      u32_t len);
	int (*flash_write)(u32_t offset,
			       const void *data, u32_t len);
	int (*flash_erase)(u32_t offset, u32_t size);
};

/**
 * @}
 */

/**
 * @brief Non-volatile Storage APIs
 * @defgroup nvs_high_level_api Non-volatile Storage APIs
 * @ingroup nvs
 * @{
 */

/**
 * @brief nvs_init
 *
 * Initializes a NVS file system in flash.
 *
 * @param fs Pointer to file system
 * @param dev_name Pointer to flash device name
 * @retval 0 Success
 * @retval -ERRNO errno code if error
 */
int nvs_init(struct nvs_fs *fs);

/**
 * @brief nvs_clear
 *
 * Clears the NVS file system from flash.
 * @param fs Pointer to file system
 * @retval 0 Success
 * @retval -ERRNO errno code if error
 */
int nvs_clear(struct nvs_fs *fs);

/**
 * @brief nvs_write
 *
 * Write an entry to the file system.
 *
 * @param fs Pointer to file system
 * @param id Id of the entry to be written
 * @param data Pointer to the data to be written
 * @param len Number of bytes to be written
 *
 * @return Number of bytes written. On success, it will be equal to the number
 * of bytes requested to be written. On error returns -ERRNO code.
 */

int nvs_write(struct nvs_fs *fs, u16_t id, const void *data, u32_t len);

/**
 * @brief nvs_delete
 *
 * Delete an entry from the file system
 *
 * @param fs Pointer to file system
 * @param id Id of the entry to be deleted
 * @retval 0 Success
 * @retval -ERRNO errno code if error
 */
int nvs_delete(struct nvs_fs *fs, u16_t id);

/**
 * @brief nvs_read
 *
 * Read an entry from the file system.
 *
 * @param fs Pointer to file system
 * @param id Id of the entry to be read
 * @param data Pointer to data buffer
 * @param len Number of bytes to be read
 *
 * @return Number of bytes read. On success, it will be equal to the number
 * of bytes requested to be read. When the return value is larger than the
 * number of bytes requested to read this indicates not all bytes were read,
 * and more data is available. On error returns -ERRNO code.
 */
int nvs_read(struct nvs_fs *fs, u16_t id, void *data, u32_t len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif 
