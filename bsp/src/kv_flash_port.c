/**
 * @file    kv_flash_port.c
 * @brief   KV Flash 模块平台适配层（WS8300）
 */

#include "kv_flash_port.h"
#include "cpu_flash.h"   /* WS8300 官方 Flash 驱动 */

/*-----------------------------------------------------------------------------
 * 底层驱动封装
 *-----------------------------------------------------------------------------*/

bool kv_port_flash_read(uint32_t address, uint32_t size, uint8_t *buf)
{
    return (flash_read(address, size, buf) == FLASH_SUCCEED);
}

bool kv_port_flash_write(uint32_t address, uint32_t size, const uint8_t *buf)
{
    return (flash_write(address, size, (uint8_t *)buf) == FLASH_SUCCEED);
}

bool kv_port_flash_erase_sector(uint32_t address)
{
    return (flash_sector_erase(address) == FLASH_SUCCEED);
}
