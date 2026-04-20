/**
 * @file    app_kv_flash.h
 * @brief   简易键值对Flash存储模块（磨损均衡，支持掉电保护）
 *
 * @note    使用前请调用 kv_flash_init() 进行初始化。
 */

#ifndef __APP_KV_FLASH_H
#define __APP_KV_FLASH_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * 公共API
 *===========================================================================*/

/**
 * @brief 初始化KV存储模块
 * @return true: 成功, false: 失败（如底层驱动未就绪）
 */
bool kv_flash_init(void);

/**
 * @brief 读取键值数据
 * @param key       键值（1~255，0xFF保留）
 * @param out_buf   输出缓冲区
 * @param inout_len 输入时指示缓冲区大小，输出时返回实际数据长度
 * @return true: 成功, false: 键不存在或参数错误
 */
bool kv_flash_read(uint8_t key, uint8_t *out_buf, uint16_t *inout_len);

/**
 * @brief 写入键值数据
 * @param key   键值（1~255）
 * @param data  数据指针
 * @param len   数据长度
 * @return true: 成功, false: 失败（如空间不足）
 */
bool kv_flash_write(uint8_t key, const uint8_t *data, uint16_t len);

/**
 * @brief 删除键值数据
 * @param key   键值
 * @return true: 成功, false: 键不存在
 */
bool kv_flash_delete(uint8_t key);

/**
 * @brief 获取键值数据长度
 * @param key   键值
 * @return 数据长度（0表示键不存在）
 */
uint16_t kv_flash_get_len(uint8_t key);

/**
 * @brief 格式化整个存储区（擦除所有扇区）
 */
void kv_flash_format(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_KV_FLASH_H */
