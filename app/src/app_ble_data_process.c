#include "app.h"
#include "app_ble_data_process.h"
#include "transport_app.h"      // 包含 transport_register_rx_callback 声明
#include "lib_api.h"


/* ========== 内部 FIFO 缓冲区实现（用于接收蓝牙原始数据） ========== */
#define BLE_RX_FIFO_SIZE   512

typedef struct {
    uint8_t buffer[BLE_RX_FIFO_SIZE];
    volatile uint16_t write_idx;
    volatile uint16_t read_idx;
} fifo_t;

static fifo_t ble_rx_fifo;

/* 初始化 FIFO */
static void fifo_init(fifo_t *fifo)
{
    fifo->write_idx = 0;
    fifo->read_idx = 0;
}

/* 向 FIFO 写入数据（返回实际写入字节数） */
static uint16_t fifo_write(fifo_t *fifo, const uint8_t *data, uint16_t len)
{
    uint16_t i;
    uint16_t write_idx, next_write;

    lib_global_irq_lock(); 

    for (i = 0; i < len; i++) {
        write_idx = fifo->write_idx;
        next_write = (write_idx + 1) % BLE_RX_FIFO_SIZE;
        if (next_write == fifo->read_idx) {
            // FIFO 满，停止写入
            break;
        }
        fifo->buffer[write_idx] = data[i];
        fifo->write_idx = next_write;
    }

    lib_global_irq_unlock(); 
    return i;  // 返回实际写入字节数
}

/* 从 FIFO 读取数据（返回实际读取字节数） */
static uint16_t fifo_read(fifo_t *fifo, uint8_t *buf, uint16_t max_len)
{
    uint16_t i;
    uint16_t read_idx, write_idx;

    lib_global_irq_lock();

    read_idx = fifo->read_idx;
    write_idx = fifo->write_idx;

    for (i = 0; i < max_len; i++) {
        if (read_idx == write_idx) {
            break;  // FIFO 空
        }
        buf[i] = fifo->buffer[read_idx];
        read_idx = (read_idx + 1) % BLE_RX_FIFO_SIZE;
    }

    fifo->read_idx = read_idx;

     lib_global_irq_unlock();
    return i;
}

/* 检查 FIFO 是否为空 */
static uint8_t fifo_is_empty(fifo_t *fifo)
{
    uint8_t empty;
    lib_global_irq_lock();
    empty = (fifo->read_idx == fifo->write_idx);
     lib_global_irq_unlock();
    return empty;
}

/* ========== 蓝牙数据接收回调（由协议栈在中断/高优先级上下文调用） ========== */
static void ble_raw_data_handler(uint8_t *data, uint16_t len)
{
    fifo_write(&ble_rx_fifo, data, len);
}

/* ========== 帧解析器 ========== */
static uint8_t parse_frame(uint8_t *buf, uint16_t len, ble_packet_t *pkt)
{
    uint16_t i;
    uint8_t cmd, data_len;
    uint16_t frame_len;
    uint8_t calc_sum = 0;
    uint8_t recv_sum;
    uint8_t j;

    if (len < 5) return 0;  // 至少需要帧头2+命令1+长度1+校验1=5字节

    // 查找帧头
    for (i = 0; i < len - 1; i++) {
        if (buf[i] == BLE_FRAME_HEADER1 && buf[i+1] == BLE_FRAME_HEADER2) {
            break;
        }
    }
    if (i >= len - 1) return 0;  // 未找到帧头

    if (len - i < 5) return 0;   // 剩余数据不足最小帧长

    cmd      = buf[i + 2];
    data_len = buf[i + 3];

    if (data_len > BLE_MAX_DATA_LEN) {
        // 非法长度，丢弃一个字节后继续尝试
        return i + 1;
    }

    frame_len = 2 + 1 + 1 + data_len + 1;  // 帧头+命令+长度+数据+校验
    if (len - i < frame_len) return 0;     // 数据未收全

    // 计算累加和（校验范围不包括校验字节本身）
    for (j = 0; j < data_len + 4; j++) {
        calc_sum += buf[i + j];
    }
    recv_sum = buf[i + frame_len - 1];
    if (calc_sum != recv_sum) {
        // 校验错误，跳过帧头字节
        return i + 1;
    }

    // 填充解析后的包
    pkt->cmd = (ble_cmd_t)cmd;
    pkt->data_len = data_len;
    memcpy(pkt->data, &buf[i + 4], data_len);

    return i + frame_len;  // 返回消耗的字节数
}

/* ========== 业务处理函数（在任务上下文中调用，可稍耗时） ========== */
static void process_ble_packet(ble_packet_t *pkt)
{
    // 此处添加你的业务逻辑，例如控制 GPIO、更新变量、发送响应等
    // 示例：仅打印信息（实际项目中可用调试串口输出）
    // dbg_printk("CMD:0x%02X LEN:%d\n", pkt->cmd, pkt->data_len);
    // ble_packet_t tx_pkt;

    switch (pkt->cmd) {
        case BLE_CMD_LED_CONTROL:
            if (pkt->data_len >= 1) {
                // uint8_t state = pkt->data[0];
                // 控制 LED 的硬件操作
                
                // 回复状态
                // tx_pkt.cmd = BLE_RSP_LED_STATUS;
                // tx_pkt.data_len = 2;
                // tx_pkt.data[0] = led_id;
                // tx_pkt.data[1] = state;
                // ble_send_packet(&tx_pkt);
            }
            break;

        case BLE_CMD_MOTOR_SPEED:
            if (pkt->data_len >= 2) {
                // uint16_t speed = (pkt->data[0] << 8) | pkt->data[1];
                // 设置电机速度
            }
            break;

        case BLE_CMD_READ_SENSOR:
            // 处理传感器读取命令，可通过蓝牙发送数据返回
            break;

        default:
            // 未知命令
            break;
    }
}

/* ========== 模块初始化 ========== */
void app_ble_bridge_init(void)
{
    fifo_init(&ble_rx_fifo);
    transport_register_rx_callback(ble_raw_data_handler);
}

/* ========== 周期性任务处理（由调度器调用，例如每10ms） ========== */
void app_ble_process_task_run(void)
{
    uint8_t temp_buf[64];
    uint16_t read_len;
    static uint8_t parse_buf[256];
    static uint16_t parse_len = 0;
    ble_packet_t pkt;
    uint16_t consumed;

    // 从 FIFO 中取出所有原始数据，合并到 parse_buf
    while ((read_len = fifo_read(&ble_rx_fifo, temp_buf, sizeof(temp_buf))) > 0) {
        for (uint16_t i = 0; i < read_len; i++) {
            if (parse_len < sizeof(parse_buf)) {
                parse_buf[parse_len++] = temp_buf[i];
            } else {
                // 解析缓冲区溢出，清空
                parse_len = 0;
            }
        }
    }

    // 循环解析帧
    while ((consumed = parse_frame(parse_buf, parse_len, &pkt)) > 0) {
        process_ble_packet(&pkt);
        // 移除已解析的数据
        memmove(parse_buf, parse_buf + consumed, parse_len - consumed);
        parse_len -= consumed;
    }

    // 可选：如果 parse_buf 中长时间残留无效数据，可设置超时清空
}

/**
 * @brief 计算累加和校验
 * @param buf 数据缓冲区
 * @param len 数据长度
 * @return 累加和（uint8_t）
 */
static uint8_t calc_checksum(uint8_t *buf, uint8_t len)
{
    uint8_t sum = 0;
    for (uint8_t i = 0; i < len; i++) {
        sum += buf[i];
    }
    return sum;
}

/**
 * @brief 发送一包 BLE 数据
 * @param pkt 待发送的数据包（cmd, data_len, data）
 * @return 0:成功，-1:数据超长，-2:蓝牙未连接
 */
int ble_send_packet(ble_packet_t *pkt)
{
    extern uint8_t transport_connection_type;
    extern uint8_t transport_mtu;

    // 检查数据长度是否超限
    if (pkt->data_len > BLE_MAX_DATA_LEN) {
        return -1;
    }

    // 检查蓝牙连接状态（可选，若未连接可缓存或直接返回错误）
    if (transport_connection_type != BLE_CONNECTED) {
        return -2;
    }

    // 构建完整帧: 帧头(2) + 命令(1) + 长度(1) + 数据(N) + 校验(1)
    uint8_t frame_len = 2 + 1 + 1 + pkt->data_len + 1;
    uint8_t tx_buffer[BLE_MAX_DATA_LEN + 5];  // 最大帧长

    uint8_t idx = 0;
    tx_buffer[idx++] = BLE_FRAME_HEADER1;
    tx_buffer[idx++] = BLE_FRAME_HEADER2;
    tx_buffer[idx++] = pkt->cmd;
    tx_buffer[idx++] = pkt->data_len;
    memcpy(&tx_buffer[idx], pkt->data, pkt->data_len);
    idx += pkt->data_len;

    // 计算校验和（校验范围不包括校验字节本身）
    uint8_t checksum = calc_checksum(tx_buffer, idx);
    tx_buffer[idx++] = checksum;

    // 调用底层发送接口
    ble_gatt_send_data(tx_buffer, frame_len);

    return 0;
}
