/**
 * @file    kv_flash_port.h
 * @brief   KV Flash存储模块移植配置文件
 */

#ifndef __KV_FLASH_PORT_H
#define __KV_FLASH_PORT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * 用户配置区（请根据实际硬件修改）
 *===========================================================================*/

/**
 * Flash起始地址（两个扇区组成的存储区首地址）
 * WS8300 UserData1 起始物理地址为 0x0107A000
 * 若底层驱动使用偏移地址，可改为 0x0007A000
 */
#define KV_FLASH_START_ADDR         0x0107A000UL

/** Flash扇区大小（字节），WS8300 为 4KB */
#define KV_FLASH_SECTOR_SIZE        4096UL

/** 扇区头大小（字节），用于标记扇区状态 */
#define KV_FLASH_HEAD_SIZE          32UL

/** 内部缓冲区大小，建议不小于 3 + 最大单条数据长度 */
#define KV_FLASH_BUFFER_SIZE        258UL

/*===========================================================================
 * 底层Flash驱动接口（需用户实现）
 *===========================================================================*/

/**
 * @brief 从Flash读取数据
 * @param address   Flash地址
 * @param size      读取字节数
 * @param buf       输出缓冲区
 * @return true: 成功, false: 失败
 */
bool kv_port_flash_read(uint32_t address, uint32_t size, uint8_t *buf);

/**
 * @brief 向Flash写入数据
 * @param address   Flash地址
 * @param size      写入字节数
 * @param buf       数据缓冲区
 * @return true: 成功, false: 失败
 */
bool kv_port_flash_write(uint32_t address, uint32_t size, const uint8_t *buf);

/**
 * @brief 擦除Flash扇区
 * @param address   扇区起始地址
 * @return true: 成功, false: 失败
 */
bool kv_port_flash_erase_sector(uint32_t address);

#ifdef __cplusplus
}
#endif

#endif /* __KV_FLASH_PORT_H */
