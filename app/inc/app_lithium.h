/**
 * @file app_lithium.h
 * @brief 锂电二号通讯协议模块（适配 GS 锂电 2 号协议 V1.1）
 * @details 基于 user_uart 抽象层，不依赖特定硬件，可移植性强。
 */

#ifndef _APP_LITHIUM_H_
#define _APP_LITHIUM_H_

#include <stdint.h>

/* ========== 协议常量 ========== */
#define LITHIUM_TX_FRAME_LEN        20      /* 仪表发送帧长度 */
#define LITHIUM_RX_FRAME_LEN        14      /* 控制器应答帧长度 */

#define LITHIUM_TX_INTERVAL_MS      90      /* 发送间隔（协议要求≥50ms） */
#define LITHIUM_RX_TIMEOUT_MS       5       /* 帧间超时判断（协议建议10-20ms，取5ms更灵敏） */

/* ========== 控制器状态结构体（解析后数据） ========== */
typedef struct {
    /* 帧头信息 */
    uint8_t  addr;
    uint8_t  len;
    uint8_t  cmd;

    /* 状态位域（控制器状态1） */
    uint8_t  motor_phase_fault  :1; /* bit0: 电机缺相故障 */
    uint8_t  reserved1           :1; /* bit1 */
    uint8_t  cruising           :1; /* bit2: 巡航中 */
    uint8_t  undervoltage_protect:1; /* bit3: 欠压保护 */
    uint8_t  controller_fault   :1; /* bit4: 控制器故障 */
    uint8_t  throttle_fault     :1; /* bit5: 转把故障 */
    uint8_t  hall_fault         :1; /* bit6: 霍尔故障 */
    uint8_t  cruising_6km       :1; /* bit7: 6km/h 巡航 */

    /* 状态位域（控制器状态2） */
    uint8_t  reserved2_0         :1; /* bit0 */
    uint8_t  reserved2_1         :1; /* bit1 */
    uint8_t  reserved2_2         :1; /* bit2 */
    uint8_t  charging            :1; /* bit3: 充电中 */
    uint8_t  comm_fault          :1; /* bit4: 通讯故障（收不到仪表） */
    uint8_t  brake_active        :1; /* bit5: 断电刹把启动 */
    uint8_t  pas_sensor_fault    :1; /* bit6: 助力传感器故障 */
    uint8_t  horizontal          :1; /* bit7: 水平状态 */

    /* 运行电流 */
    uint16_t current_raw;           /* 原始值（可能带单位标志） */
    float    current;               /* 转换后电流值（单位：A） */

    uint8_t  current_ratio;         /* 电流比例值（1~100%） */
    uint16_t speed_pulse_ms;        /* 轮子转一圈的毫秒数（用于计算速度） */
    uint8_t  battery_capacity;      /* 电池容量百分比（0~100） */
    uint16_t remain_mileage;        /* 剩余里程（单位：km） */
    uint8_t  checksum;              /* 接收帧校验和 */

    /* 通讯状态标志 */
    uint8_t  packet_ok;             /* 本次任务循环收到有效包 */
    uint8_t  miss_cnt;              /* 连续丢包计数 */
} lithium_status_t;

/* ========== 发送帧结构体（仪表发往控制器） ========== */
typedef struct {
    uint8_t  addr;                  /* 固定 0x01 */
    uint8_t  len;                   /* 固定 20 */
    uint8_t  cmd;                   /* 固定 0x00 */

    uint8_t  drive_mode;            /* 0:助力，1:电驱动，2:共存 */
    uint8_t  assist_level;          /* 助力档位值（见协议） */
    uint8_t  ctrl_set1;             /* 控制器控制设定1（见协议） */
    uint8_t  speed_magnet_num;      /* 测速磁钢数 */
    uint16_t wheel_diameter;        /* 轮径（0.1英寸） */
    uint8_t  assist_sensitivity;    /* 助力灵敏度（1~24） */
    uint8_t  assist_start_strength; /* 助力启动强度（0~5） */
    uint8_t  inner_magnet_speed_limit; /* 内测速磁钢数+限速值高4位？按协议处理 */
    uint8_t  speed_limit;           /* 限速值（km/h） */
    uint8_t  current_limit;         /* 控制器限流值（A） */
    uint16_t undervoltage_brake_ad; /* 欠压值（0.1V）或刹车AD值 */
    uint16_t throttle_pwm;          /* 转把PWM占空比（0~1000） */
    uint8_t  ctrl_set2;             /* 控制器控制设定2 */
    uint8_t  checksum;              /* 校验和（模块自动计算） */
} lithium_tx_frame_t;

/* ========== 对外接口 ========== */

/**
 * @brief 模块初始化
 * @note  配置UART引脚复用，初始化内部缓冲区。应在任务初始化阶段调用。
 */
void app_lithium_init(void);

/**
 * @brief 模块任务执行函数（建议10ms周期调用）
 * @param arg 未使用
 * @note  处理接收超时、帧解析、周期性发送。
 */
void app_lithium_task_run(void* arg);

/**
 * @brief 获取最新的控制器状态（只读）
 * @return 指向内部状态结构体的常量指针
 */
const lithium_status_t* app_lithium_get_status(void);

/**
 * @brief 设置发送帧参数（由其他业务模块调用以更新配置）
 * @param param 指向发送帧结构体的指针，若为 NULL 则不操作
 * @note  只需填充业务字段，固定字段（addr/len/cmd/checksum）模块内部处理。
 */
void app_lithium_set_tx_param(const lithium_tx_frame_t* param);

#endif /* _APP_LITHIUM_H_ */
