/**
 * @file    app_kv_flash.c
 * @brief   简易键值对Flash存储模块实现
 *
 * 存储格式：
 * - 每个扇区头部第一个字节标记扇区使用状态：
 *     0xAA 表示当前使用中，0xFF 表示空闲/未使用。
 * - 每个记录格式：[KEY(1B)] [CHECK(1B)] [LEN(2B, 小端)] [DATA(LEN)]
 * - 删除记录时，将记录头部的校验字节写为0。
 * - 支持两个扇区交替使用，实现磨损均衡与垃圾回收。
 */

#include "app_kv_flash.h"
#include "kv_flash_port.h"
#include <string.h>

/*===========================================================================
 * 内部宏定义
 *===========================================================================*/

/** 两个扇区起始地址 */
#define SECTOR1_ADDR                (KV_FLASH_START_ADDR)
#define SECTOR2_ADDR                (KV_FLASH_START_ADDR + KV_FLASH_SECTOR_SIZE)

/** 扇区有效数据起始地址（跳过头部） */
#define SECTOR_DATA_START(sec)      ((sec) + KV_FLASH_HEAD_SIZE)
#define SECTOR_END(sec)             ((sec) + KV_FLASH_SECTOR_SIZE)

/** 记录头部长度（KEY + 校验 + 长度字段） */
#define RECORD_HEADER_SIZE          4

/** 内部保留键值 */
#define KEY_INVALID                 0x00    /* 已删除标记 */
#define KEY_EMPTY                   0xFF    /* 空闲区域 */

/*===========================================================================
 * 静态变量
 *===========================================================================*/

static uint32_t s_used_sector = 0;      /**< 当前使用的扇区地址 */
static uint32_t s_free_sector = 0;      /**< 备用扇区地址 */
static uint8_t  s_buffer[KV_FLASH_BUFFER_SIZE];  /**< 内部缓冲区 */

/*===========================================================================
 * 内部函数声明
 *===========================================================================*/

static bool read_record_header(uint32_t addr, uint8_t *key, uint8_t *check, uint16_t *len);
static bool is_record_valid(uint8_t key, uint8_t check);
static bool garbage_collection(void);

/*===========================================================================
 * 公共函数实现
 *===========================================================================*/

/**
 * @brief 初始化KV存储模块
 */
bool kv_flash_init(void)
{
    uint8_t sector_flag;

    /* 读取第一个扇区头部标记 */
    if (!kv_port_flash_read(SECTOR1_ADDR, 1, &sector_flag)) {
        return false;
    }

    if (sector_flag == 0xAA) {
        /* 第一个扇区正在使用 */
        s_used_sector = SECTOR1_ADDR;
        s_free_sector = SECTOR2_ADDR;
    } else {
        /* 第二个扇区正在使用（或两个扇区都为空，默认使用第二个） */
        s_used_sector = SECTOR2_ADDR;
        s_free_sector = SECTOR1_ADDR;

        /* 如果两个扇区都为空，主动标记工作扇区 */
        if (!kv_port_flash_read(SECTOR2_ADDR, 1, &sector_flag)) {
            return false;
        }
        if (sector_flag != 0xAA) {
            uint8_t flag = 0xAA;
            if (!kv_port_flash_write(s_used_sector, 1, &flag)) {
                return false;
            }
        }
    }

    return true;
}

/**
 * @brief 读取键值数据
 */
bool kv_flash_read(uint8_t key, uint8_t *out_buf, uint16_t *inout_len)
{
    uint32_t addr;
    uint8_t  cur_key, cur_check;
    uint16_t cur_len;
    uint16_t buf_size;

    if (key == KEY_INVALID || key == KEY_EMPTY) {
        return false;
    }

    if (inout_len == NULL) {
        return false;
    }

    buf_size = *inout_len;
    addr = SECTOR_DATA_START(s_used_sector);

    while (addr < SECTOR_END(s_used_sector) - RECORD_HEADER_SIZE) {
        /* 读取记录头部 */
        if (!kv_port_flash_read(addr, RECORD_HEADER_SIZE, s_buffer)) {
            return false;
        }

        cur_key  = s_buffer[0];
        cur_check = s_buffer[1];
        cur_len  = s_buffer[2] | (s_buffer[3] << 8);

        /* 遇到空闲区域则停止搜索 */
        if (cur_key == KEY_EMPTY) {
            break;
        }

        /* 跳过无效记录 */
        if (cur_key == KEY_INVALID || !is_record_valid(cur_key, cur_check)) {
            addr += RECORD_HEADER_SIZE + cur_len;
            continue;
        }

        if (cur_key == key) {
            /* 找到目标键值 */
            if (out_buf == NULL) {
                /* 仅返回长度 */
                *inout_len = cur_len;
                return true;
            } else {
                /* 读取数据 */
                if (buf_size < cur_len) {
                    *inout_len = cur_len;
                    return false;   /* 缓冲区不足 */
                }
                if (!kv_port_flash_read(addr + RECORD_HEADER_SIZE, cur_len, out_buf)) {
                    return false;
                }
                *inout_len = cur_len;
                return true;
            }
        }

        /* 移动到下一条记录 */
        addr += RECORD_HEADER_SIZE + cur_len;
    }

    return false;   /* 未找到 */
}

/**
 * @brief 写入键值数据
 */
bool kv_flash_write(uint8_t key, const uint8_t *data, uint16_t len)
{
    uint32_t addr;
    uint8_t  cur_key, cur_check;
    uint16_t cur_len;
    uint16_t dirty_space = 0;
    bool     found = false;

    if (key == KEY_INVALID || key == KEY_EMPTY || data == NULL) {
        return false;
    }

    if (len + RECORD_HEADER_SIZE > KV_FLASH_BUFFER_SIZE) {
        return false;   /* 数据过长 */
    }

    /* 先尝试删除旧数据（如果存在） */
    kv_flash_delete(key);

    addr = SECTOR_DATA_START(s_used_sector);

    while (addr < SECTOR_END(s_used_sector) - RECORD_HEADER_SIZE) {
        /* 检查剩余空间是否足够写入新记录 */
        if (addr + RECORD_HEADER_SIZE + len > SECTOR_END(s_used_sector)) {
            /* 空间不足，尝试垃圾回收 */
            if (dirty_space > 0) {
                if (!garbage_collection()) {
                    return false;   /* GC 失败 */
                }
                /* 重新从新扇区开始写入 */
                return kv_flash_write(key, data, len);
            }
            return false;   /* 无脏数据可回收，空间确实不足 */
        }

        /* 读取记录头部 */
        if (!kv_port_flash_read(addr, RECORD_HEADER_SIZE, s_buffer)) {
            return false;
        }

        cur_key  = s_buffer[0];
        cur_check = s_buffer[1];
        cur_len  = s_buffer[2] | (s_buffer[3] << 8);

        /* 遇到空闲区域，直接写入新记录 */
        if (cur_key == KEY_EMPTY) {
            found = true;
            break;
        }

        /* 跳过无效记录，并累加脏数据长度 */
        if (cur_key == KEY_INVALID || !is_record_valid(cur_key, cur_check)) {
            dirty_space += RECORD_HEADER_SIZE + cur_len;
            addr += RECORD_HEADER_SIZE + cur_len;
            continue;
        }

        /* 有效记录，继续遍历 */
        addr += RECORD_HEADER_SIZE + cur_len;
    }

    if (!found) {
        /* 未找到合适的空闲位置（理论上不会进入这里） */
        return false;
    }

    /* 构造记录并写入 */
    s_buffer[0] = key;
    s_buffer[1] = key ^ 0xFF;
    s_buffer[2] = len & 0xFF;
    s_buffer[3] = (len >> 8) & 0xFF;
    memcpy(&s_buffer[RECORD_HEADER_SIZE], data, len);

    return kv_port_flash_write(addr, RECORD_HEADER_SIZE + len, s_buffer);
}

/**
 * @brief 删除键值数据
 */
bool kv_flash_delete(uint8_t key)
{
    uint32_t addr;
    uint8_t  cur_key, cur_check;
    uint16_t cur_len;
    uint8_t  zero = 0;

    if (key == KEY_INVALID || key == KEY_EMPTY) {
        return false;
    }

    addr = SECTOR_DATA_START(s_used_sector);

    while (addr < SECTOR_END(s_used_sector) - RECORD_HEADER_SIZE) {
        if (!kv_port_flash_read(addr, RECORD_HEADER_SIZE, s_buffer)) {
            return false;
        }

        cur_key  = s_buffer[0];
        cur_check = s_buffer[1];
        cur_len  = s_buffer[2] | (s_buffer[3] << 8);

        if (cur_key == KEY_EMPTY) {
            break;
        }

        if (cur_key == KEY_INVALID || !is_record_valid(cur_key, cur_check)) {
            addr += RECORD_HEADER_SIZE + cur_len;
            continue;
        }

        if (cur_key == key) {
            /* 将校验字节写为0，标记为删除 */
            return kv_port_flash_write(addr + 1, 1, &zero);
        }

        addr += RECORD_HEADER_SIZE + cur_len;
    }

    return false;   /* 键不存在 */
}

/**
 * @brief 获取键值数据长度
 */
uint16_t kv_flash_get_len(uint8_t key)
{
    uint16_t len = 0;
    kv_flash_read(key, NULL, &len);
    return len;
}

/**
 * @brief 格式化整个存储区
 */
void kv_flash_format(void)
{
    kv_port_flash_erase_sector(SECTOR1_ADDR);
    kv_port_flash_erase_sector(SECTOR2_ADDR);
    s_used_sector = SECTOR2_ADDR;
    s_free_sector = SECTOR1_ADDR;
}

/*===========================================================================
 * 内部函数实现
 *===========================================================================*/

/**
 * @brief 读取记录头部信息
 */
static bool read_record_header(uint32_t addr, uint8_t *key, uint8_t *check, uint16_t *len)
{
    if (!kv_port_flash_read(addr, RECORD_HEADER_SIZE, s_buffer)) {
        return false;
    }

    *key   = s_buffer[0];
    *check = s_buffer[1];
    *len   = s_buffer[2] | (s_buffer[3] << 8);

    return true;
}

/**
 * @brief 判断记录是否有效（未被删除）
 */
static bool is_record_valid(uint8_t key, uint8_t check)
{
    return (check == (key ^ 0xFF));
}

/**
 * @brief 垃圾回收：将有效数据复制到空闲扇区，擦除旧扇区
 * @return true: 成功, false: 失败
 */
static bool garbage_collection(void)
{
    uint32_t src_addr, dst_addr;
    uint8_t  key, check;
    uint16_t len;
    uint8_t  sector_flag = 0xAA;

    src_addr = SECTOR_DATA_START(s_used_sector);
    dst_addr = SECTOR_DATA_START(s_free_sector);

    /* 遍历旧扇区，复制有效记录 */
    while (src_addr < SECTOR_END(s_used_sector) - RECORD_HEADER_SIZE) {
        if (!read_record_header(src_addr, &key, &check, &len)) {
            return false;
        }

        if (key == KEY_EMPTY) {
            break;  /* 到达空闲区域 */
        }

        if (key != KEY_INVALID && is_record_valid(key, check)) {
            /* 有效记录：复制头部+数据 */
            if (!kv_port_flash_read(src_addr, RECORD_HEADER_SIZE + len, s_buffer)) {
                return false;
            }
            if (!kv_port_flash_write(dst_addr, RECORD_HEADER_SIZE + len, s_buffer)) {
                return false;
            }
            dst_addr += RECORD_HEADER_SIZE + len;
        }

        src_addr += RECORD_HEADER_SIZE + len;
    }

    /* 标记新扇区为使用中 */
    if (!kv_port_flash_write(s_free_sector, 1, &sector_flag)) {
        return false;
    }

    /* 擦除旧扇区 */
    if (!kv_port_flash_erase_sector(s_used_sector)) {
        return false;
    }

    /* 交换扇区角色 */
    uint32_t temp = s_used_sector;
    s_used_sector = s_free_sector;
    s_free_sector = temp;

    return true;
}
