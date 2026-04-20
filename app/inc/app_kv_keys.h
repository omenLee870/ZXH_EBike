/**
 * @file    app_kv_keys.h
 * @brief   应用层 KV 存储键值枚举定义
 *
 * 使用说明：
 *   1. 所有需要持久化存储的配置项，均在此枚举中定义唯一的 KEY。
 *   2. KEY 值范围：1 ~ 254（0x00 和 0xFF 为系统保留）。
 *   3. 通过 kv_flash_read/write 接口操作，使用枚举名提高可读性。
 */

#ifndef __APP_KV_KEYS_H
#define __APP_KV_KEYS_H

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * 应用层存储键值枚举
 *===========================================================================*/
typedef enum {
    /* 设备信息 */
    KV_KEY_DEVICE_CRC           = 0x01,     /* 设备配置CRC */
    KV_KEY_FIRST_LEARN_NFC      = 0x02,     /* 首次学卡标志 */

    /* 用户设置参数 */
    KV_KEY_SETTING_LIMIT_SPEED  = 0x10,     /* 限速值 */
    KV_KEY_SETTING_UNIT_SWITCH  = 0x11,     /* 单位切换 */
    KV_KEY_SETTING_WHEEL_INCH   = 0x12,     /* 轮径英寸 */
    KV_KEY_SETTING_VOLTAGE      = 0x13,     /* 电压等级 */
    KV_KEY_SETTING_ASSIST_GEAR  = 0x14,     /* 助力档位 */
    KV_KEY_SETTING_START_MODE   = 0x15,     /* 启动模式 */
    KV_KEY_SETTING_DRIVER_MODE  = 0x16,     /* 驱动模式 */
    KV_KEY_SETTING_CRUISE_SWITCH= 0x17,     /* 巡航开关 */
    KV_KEY_SETTING_ODO_CLEAR    = 0x18,     /* 里程清零标志 */

    /* 密码 */
    KV_KEY_PASSWORD_1           = 0x20,
    KV_KEY_PASSWORD_2           = 0x21,
    KV_KEY_PASSWORD_3           = 0x22,
    KV_KEY_PASSWORD_4           = 0x23,
    KV_KEY_PASSWORD_5           = 0x24,
    KV_KEY_PASSWORD_6           = 0x25,

    /* 里程数据 */
    KV_KEY_ODO_KM               = 0x30,     /* 总里程(km) */
    KV_KEY_TRIP_KM              = 0x31,     /* 单次里程(km) */

    /* NFC UID 列表 */
    KV_KEY_NFC_UID_LIST         = 0x40,     /* NFC UID 数组 */
    KV_KEY_ADC_CALIBRATION      = 0x41,     /* ADC 校准值 */

    /* 可根据需要继续添加，最大值 254 */
} app_kv_key_t;

#ifdef __cplusplus
}
#endif

#endif /* __APP_KV_KEYS_H */
