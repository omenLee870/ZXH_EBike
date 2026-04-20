#ifndef _APP_BLE_DATA_PROCESS_H
#define _APP_BLE_DATA_PROCESS_H

#include <stdint.h>
#include <string.h>

/* 协议帧定义 */
#define BLE_FRAME_HEADER1   0xAA
#define BLE_FRAME_HEADER2   0x55
#define BLE_MAX_DATA_LEN    128

/* 命令码枚举 */
typedef enum {
    BLE_CMD_LED_CONTROL = 0x01,
    BLE_CMD_MOTOR_SPEED  = 0x02,
    BLE_CMD_READ_SENSOR  = 0x03,
    
    BLE_RSP_LED_STATUS   = 0x81,
    BLE_RSP_TEMPERATURE  = 0x82,
} ble_cmd_t;

/* 解析后的数据包 */
typedef struct {
    ble_cmd_t cmd;
    uint8_t   data_len;
    uint8_t   data[BLE_MAX_DATA_LEN];
} ble_packet_t;

/* 初始化蓝牙桥接模块 */
void app_ble_bridge_init(void);

/* 周期性处理任务（由调度器调用） */
void app_ble_process_task_run(void);

/* ========== 新增：发送数据接口 ========== */
/**
 * @brief 发送一包 BLE 数据（自动封装帧头、校验）
 * @param pkt 指向待发送数据包的指针
 * @return 0:成功，-1:数据超长，-2:蓝牙未连接
 */
int ble_send_packet(ble_packet_t *pkt);

#endif
