#ifndef CPU_FLASH_H
#define CPU_FLASH_H

#include <stdint.h>

enum
{
    FLASH_OPERATE_FAIL             =0x00,
    FLASH_SUCCEED                  = 0x01,
        /*flash error*/
    FLASH_ERROR_ADDR_INVALID              = 0x02,
    FLASH_ERROR_ADDR_OUT                  = 0x03,
};
#define FLASH_SECTOR_SIZE       (0x1000)
#define FALSH_PAGE_SIZE         (0x100)
#define FLASH_ADDR_BASE         (0x00000000) //max size 1M flash
#define FLASH_PART_SIZE         (0x1000)
#define FLASH_BOOT_SIZE         (0x1000)
#define FLASH_VALID_ADDR        (FLASH_ADDR_BASE+FLASH_PART_SIZE+FLASH_BOOT_SIZE)
#define FLASH_END_ADDR          (0x00100000)

/*
* brief: flash read api
* param[0]: flash address (0x00002000~0x01100000)
* param[1]: read flash data size
* param[2]: buf for save data from flash.
* return  : true succeed or fail
*/
uint32_t flash_read(uint32_t address, uint32_t size, uint8_t *buf);
/*
* brief: flash write api
* param[0]: flash address (0x00002000~0x01100000)
* param[1]: read flash data size
* param[2]: buf for save data into flash.
* return  : true succeed or fail
*/
uint32_t flash_write(uint32_t address, uint32_t size, uint8_t *buf);
/*
* brief: flash erase
* param[0]: flash address (0x7A000~0x7F000)
* param[1]: read flash data size
* return  : true succeed or fail
*/
uint32_t flash_erase(uint32_t address, uint32_t size);

/*
* brief: flash sector erase
* param[0]: flash address (0x7A000~0x7F000)
* return  : 0
*/
uint32_t flash_sector_erase(uint32_t address);

uint32_t flash_page_otp_read(uint32_t page,uint32_t addr, uint32_t size, uint8_t *buf);

#endif

