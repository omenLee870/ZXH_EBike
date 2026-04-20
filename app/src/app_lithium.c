/**
 * @file app_lithium.c
 * @brief 锂电二号通讯协议模块实现（基于 user_uart 抽象层）
 */

#include "app_lithium.h"
#include "user_uart.h"           /* 提供 user_uart_read/write/get_len */
#include "cpu_gpio.h"            /* 提供 ws8300_gpio_remap/init */
#include <string.h>

/* ========== 硬件引脚配置（请根据实际电路修改） ========== */
#define LITHIUM_TX_PIN      GPIO_GPIOA_Pin2      /* 示例：PA2 */
#define LITHIUM_RX_PIN      GPIO_GPIOA_Pin3      /* 示例：PA3 */

/* ========== 本地接收缓冲区（简易环形队列） ========== */
#define LOCAL_RX_BUF_SIZE   256

typedef struct {
    uint8_t  buffer[LOCAL_RX_BUF_SIZE];
    uint16_t head;
    uint16_t tail;
    uint16_t count;
} local_ring_t;

static local_ring_t local_rx;

extern uint32_t get_sys_tick_ms(void); 

static void local_ring_init(local_ring_t* r) {
    r->head = r->tail = r->count = 0;
}

static uint16_t local_ring_count(local_ring_t* r) {
    return r->count;
}

static void local_ring_push(local_ring_t* r, uint8_t data) {
    if (r->count < LOCAL_RX_BUF_SIZE) {
        r->buffer[r->head] = data;
        r->head = (r->head + 1) % LOCAL_RX_BUF_SIZE;
        r->count++;
    } else {
        /* 缓冲区满：丢弃最旧的一个字节，为新字节腾出空间 */
        r->tail = (r->tail + 1) % LOCAL_RX_BUF_SIZE;
        r->buffer[r->head] = data;
        r->head = (r->head + 1) % LOCAL_RX_BUF_SIZE;
    }
}

static uint8_t local_ring_pop(local_ring_t* r) {
    uint8_t data = 0;
    if (r->count > 0) {
        data = r->buffer[r->tail];
        r->tail = (r->tail + 1) % LOCAL_RX_BUF_SIZE;
        r->count--;
    }
    return data;
}

static void local_ring_clear(local_ring_t* r) {
    r->head = r->tail = r->count = 0;
}

/* ========== 内部状态 ========== */
static lithium_status_t g_status;
static lithium_tx_frame_t g_tx_frame = {
    .addr = 0x01,
    .len  = LITHIUM_TX_FRAME_LEN,
    .cmd  = 0x00,
    .drive_mode = 1,                /* 默认电驱动 */
    .assist_level = 5,
    .wheel_diameter = 305,          /* 30.5英寸 */
    .speed_limit = 25,
    .current_limit = 12,
    .throttle_pwm = 0,
};

static uint32_t last_rx_tick = 0;   /* 最后一次收到字节的系统tick */
static uint8_t  rx_pending = 0;     /* 是否有数据正在接收（用于超时判断） */
static uint32_t last_tx_tick = 0;   /* 上一次发送的系统tick */

/* ========== 私有函数声明 ========== */
static void lithium_gpio_init(void);
static void lithium_parse_frame(const uint8_t* data);
static void lithium_build_tx_frame(uint8_t* out_buf);
static uint8_t lithium_calc_checksum(const uint8_t* data, uint8_t len);

/* ========== 对外接口实现 ========== */

/**
 * @brief 模块初始化
 */
void app_lithium_init(void)
{
    /* 清零状态 */
    memset(&g_status, 0, sizeof(g_status));
    g_status.packet_ok = 0;
    g_status.miss_cnt = 0;

    /* 初始化本地环形缓冲区 */
    local_ring_init(&local_rx);

    /* 配置GPIO复用为UART0 */
    lithium_gpio_init();

    /* 注意：user_uart_init() 应在系统初始化时调用一次（通常在main中）。
       如果确保main中已调用，此处无需重复；若不确定，可在此调用并传入NULL。 */
    // user_uart_init(NULL);

    /* 初始化时间戳 */
    uint32_t now = get_sys_tick_ms();
    last_rx_tick = now;
    last_tx_tick = now;
}

/**
 * @brief 任务执行函数（建议每10ms调用一次）
 */
void app_lithium_task_run(void* arg)
{
    uint32_t now = get_sys_tick_ms();
    uint8_t ch;
    (void)arg;

    /* ----- 步骤1：从user_uart接收缓冲区读取所有可用字节，存入本地缓冲区 ----- */
    while (user_get_uart_len() > 0) {
        if (user_uart_read(&ch, 1) == 1) {
            local_ring_push(&local_rx, ch);
            last_rx_tick = now;      /* 更新最后接收时刻 */
            rx_pending = 1;          /* 标记有数据正在接收 */
        }
    }

    /* ----- 步骤2：接收超时判断（5ms内无新数据则视为一帧结束） ----- */
    if (rx_pending && ((int32_t)(now - last_rx_tick) >= LITHIUM_RX_TIMEOUT_MS)) {
        rx_pending = 0;

        /* 检查本地缓冲区中是否至少有一帧完整数据 */
        if (local_ring_count(&local_rx) >= LITHIUM_RX_FRAME_LEN) {
            uint8_t frame[LITHIUM_RX_FRAME_LEN];
            for (int i = 0; i < LITHIUM_RX_FRAME_LEN; i++) {
                frame[i] = local_ring_pop(&local_rx);
            }
            lithium_parse_frame(frame);
            g_status.packet_ok = 1;
            g_status.miss_cnt = 0;
        } else {
            /* 数据不完整，丢弃并计数丢包 */
            local_ring_clear(&local_rx);
            if (g_status.miss_cnt < 255) {
                g_status.miss_cnt++;
            }
            g_status.packet_ok = 0;
        }
    }

    /* ----- 步骤3：周期性发送配置帧（90ms间隔） ----- */
    if ((int32_t)(now - last_tx_tick) >= LITHIUM_TX_INTERVAL_MS) {
        last_tx_tick = now;
        uint8_t tx_buf[LITHIUM_TX_FRAME_LEN];
        lithium_build_tx_frame(tx_buf);
        user_uart_write(tx_buf, LITHIUM_TX_FRAME_LEN);
    }
}

/**
 * @brief 获取控制器状态
 */
const lithium_status_t* app_lithium_get_status(void)
{
    return &g_status;
}

/**
 * @brief 设置发送参数
 */
void app_lithium_set_tx_param(const lithium_tx_frame_t* param)
{
    if (param == NULL) {
        return;
    }

    /* 只拷贝业务字段，固定字段由模块维护 */
    g_tx_frame.drive_mode = param->drive_mode;
    g_tx_frame.assist_level = param->assist_level;
    g_tx_frame.ctrl_set1 = param->ctrl_set1;
    g_tx_frame.speed_magnet_num = param->speed_magnet_num;
    g_tx_frame.wheel_diameter = param->wheel_diameter;
    g_tx_frame.assist_sensitivity = param->assist_sensitivity;
    g_tx_frame.assist_start_strength = param->assist_start_strength;
    g_tx_frame.inner_magnet_speed_limit = param->inner_magnet_speed_limit;
    g_tx_frame.speed_limit = param->speed_limit;
    g_tx_frame.current_limit = param->current_limit;
    g_tx_frame.undervoltage_brake_ad = param->undervoltage_brake_ad;
    g_tx_frame.throttle_pwm = param->throttle_pwm;
    g_tx_frame.ctrl_set2 = param->ctrl_set2;

    /* 确保固定字段正确 */
    g_tx_frame.addr = 0x01;
    g_tx_frame.len  = LITHIUM_TX_FRAME_LEN;
    g_tx_frame.cmd  = 0x00;
}

/* ========== 私有函数实现 ========== */

/**
 * @brief 配置UART引脚复用及GPIO属性
 */
static void lithium_gpio_init(void)
{
    /* 将引脚复用为UART0功能 */
    ws8300_gpio_remap(LITHIUM_TX_PIN, GPIO_Remap_UART0_TX);
    ws8300_gpio_remap(LITHIUM_RX_PIN, GPIO_Remap_UART0_RX);

    /* TX为推挽输出，RX为输入上拉 */
    ws8300_gpio_init(LITHIUM_TX_PIN, GPIO_Mode_Output);
    ws8300_gpio_init(LITHIUM_RX_PIN, GPIO_Mode_Input_Pull_Up_Resistor);
}

/**
 * @brief 计算异或校验和
 */
static uint8_t lithium_calc_checksum(const uint8_t* data, uint8_t len)
{
    uint8_t sum = 0;
    while (len--) {
        sum ^= *data++;
    }
    return sum;
}

/**
 * @brief 构建发送帧（大端格式，自动计算校验和）
 */
static void lithium_build_tx_frame(uint8_t* out_buf)
{
    uint8_t idx = 0;

    out_buf[idx++] = g_tx_frame.addr;                               /* [0] */
    out_buf[idx++] = g_tx_frame.len;                                /* [1] */
    out_buf[idx++] = g_tx_frame.cmd;                                /* [2] */
    out_buf[idx++] = g_tx_frame.drive_mode;                         /* [3] */
    out_buf[idx++] = g_tx_frame.assist_level;                       /* [4] */
    out_buf[idx++] = g_tx_frame.ctrl_set1;                          /* [5] */
    out_buf[idx++] = g_tx_frame.speed_magnet_num;                   /* [6] */
    out_buf[idx++] = (g_tx_frame.wheel_diameter >> 8) & 0xFF;       /* [7] 轮径高字节 */
    out_buf[idx++] = g_tx_frame.wheel_diameter & 0xFF;              /* [8] 轮径低字节 */
    out_buf[idx++] = g_tx_frame.assist_sensitivity;                 /* [9] */
    out_buf[idx++] = g_tx_frame.assist_start_strength;              /* [10] */
    out_buf[idx++] = g_tx_frame.inner_magnet_speed_limit;           /* [11] */
    out_buf[idx++] = g_tx_frame.speed_limit;                        /* [12] */
    out_buf[idx++] = g_tx_frame.current_limit;                      /* [13] */
    out_buf[idx++] = (g_tx_frame.undervoltage_brake_ad >> 8) & 0xFF;/* [14] 高字节 */
    out_buf[idx++] = g_tx_frame.undervoltage_brake_ad & 0xFF;       /* [15] 低字节 */
    out_buf[idx++] = (g_tx_frame.throttle_pwm >> 8) & 0xFF;         /* [16] 高字节 */
    out_buf[idx++] = g_tx_frame.throttle_pwm & 0xFF;                /* [17] 低字节 */
    out_buf[idx++] = g_tx_frame.ctrl_set2;                          /* [18] */

    /* 计算校验和（前19字节异或） */
    out_buf[idx] = lithium_calc_checksum(out_buf, 19);              /* [19] */
}

/**
 * @brief 解析接收帧并更新全局状态
 */
static void lithium_parse_frame(const uint8_t* data)
{
    /* 基本帧头校验 */
    if (data[0] != 0x02 || data[1] != 14 || data[2] != 0x01) {
        return;
    }

    /* 校验和检查 */
    if (lithium_calc_checksum(data, 13) != data[13]) {
        return;
    }

    /* 更新状态结构体 */
    g_status.addr = data[0];
    g_status.len  = data[1];
    g_status.cmd  = data[2];

    /* 解析状态1 */
    uint8_t s1 = data[3];
    g_status.motor_phase_fault   = (s1 >> 0) & 0x01;
    g_status.reserved1           = (s1 >> 1) & 0x01;
    g_status.cruising            = (s1 >> 2) & 0x01;
    g_status.undervoltage_protect= (s1 >> 3) & 0x01;
    g_status.controller_fault    = (s1 >> 4) & 0x01;
    g_status.throttle_fault      = (s1 >> 5) & 0x01;
    g_status.hall_fault          = (s1 >> 6) & 0x01;
    g_status.cruising_6km        = (s1 >> 7) & 0x01;

    /* 解析状态2 */
    uint8_t s2 = data[4];
    g_status.reserved2_0         = (s2 >> 0) & 0x01;
    g_status.reserved2_1         = (s2 >> 1) & 0x01;
    g_status.reserved2_2         = (s2 >> 2) & 0x01;
    g_status.charging            = (s2 >> 3) & 0x01;
    g_status.comm_fault          = (s2 >> 4) & 0x01;
    g_status.brake_active        = (s2 >> 5) & 0x01;
    g_status.pas_sensor_fault    = (s2 >> 6) & 0x01;
    g_status.horizontal          = (s2 >> 7) & 0x01;

    /* 电流（2字节） */
    g_status.current_raw = (data[5] << 8) | data[6];
    /* 判断电流单位：bit6为1则单位0.1A，否则为1A */
    if (data[5] & 0x40) {
        g_status.current = (g_status.current_raw & 0x3FFF) * 0.1f;
    } else {
        g_status.current = (float)(g_status.current_raw & 0x3FFF);
    }

    g_status.current_ratio = data[7];
    g_status.speed_pulse_ms = (data[8] << 8) | data[9];
    g_status.battery_capacity = data[10];
    g_status.remain_mileage = (data[11] << 8) | data[12];
    g_status.checksum = data[13];
}
