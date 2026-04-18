#include "ws1850s_softiic.h"

/* 简单的微秒级延时，用于控制 I2C 速率 */
static void iic_delay(void) {
    volatile uint8_t i = 10; // 根据主频微调，保证SCL速率在400KHz以下
    while (i--);
}

/* SDA 引脚方向切换 */
static void sda_out(void) {
    ws8300_gpio_init(NFC_SDA_PIN, GPIO_Mode_Output);
}
static void sda_in(void) {
    ws8300_gpio_init(NFC_SDA_PIN, GPIO_Mode_Input_Pull_Up_Resistor);
}

/* ======== 标准 I2C 时序底层实现 ======== */
static void iic_start(void) {
    sda_out();
    ws8300_gpio_set(NFC_SDA_PIN);
    ws8300_gpio_set(NFC_SCL_PIN);
    iic_delay();
    ws8300_gpio_reset(NFC_SDA_PIN); // SCL高时，SDA拉低产生Start
    iic_delay();
    ws8300_gpio_reset(NFC_SCL_PIN);
}

static void iic_stop(void) {
    sda_out();
    ws8300_gpio_reset(NFC_SDA_PIN);
    ws8300_gpio_set(NFC_SCL_PIN);
    iic_delay();
    ws8300_gpio_set(NFC_SDA_PIN); // SCL高时，SDA拉高产生Stop
    iic_delay();
}

static bool iic_wait_ack(void) {
    uint8_t timeout = 0;
    sda_in();
    ws8300_gpio_set(NFC_SDA_PIN); iic_delay();
    ws8300_gpio_set(NFC_SCL_PIN); iic_delay();
    while (ws8300_gpio_get_intput_level(NFC_SDA_PIN)) {
        timeout++;
        if (timeout > 250) {
            iic_stop();
            return false; // 等待应答超时
        }
    }
    ws8300_gpio_reset(NFC_SCL_PIN);
    return true; // 收到应答
}

static void iic_send_byte(uint8_t dat) {
    uint8_t i;
    sda_out();
    ws8300_gpio_reset(NFC_SCL_PIN);
    for (i = 0; i < 8; i++) {
        if ((dat & 0x80) >> 7) {
            ws8300_gpio_set(NFC_SDA_PIN);
        } else {
            ws8300_gpio_reset(NFC_SDA_PIN);
        }
        dat <<= 1;
        iic_delay();
        ws8300_gpio_set(NFC_SCL_PIN);
        iic_delay();
        ws8300_gpio_reset(NFC_SCL_PIN);
        iic_delay();
    }
}

static uint8_t iic_read_byte(bool ack) {
    uint8_t i, receive = 0;
    sda_in();
    for (i = 0; i < 8; i++) {
        ws8300_gpio_reset(NFC_SCL_PIN); iic_delay();
        ws8300_gpio_set(NFC_SCL_PIN);
        receive <<= 1;
        if (ws8300_gpio_get_intput_level(NFC_SDA_PIN)) receive++;
        iic_delay();
    }
    // 发送 ACK 或 NACK
    sda_out();
    if (ack) ws8300_gpio_reset(NFC_SDA_PIN); // ACK
    else     ws8300_gpio_set(NFC_SDA_PIN);   // NACK
    
    ws8300_gpio_set(NFC_SCL_PIN); iic_delay();
    ws8300_gpio_reset(NFC_SCL_PIN); iic_delay();
    return receive;
}

/* ======== 对外接口：硬件初始化 ======== */
void ws1850s_hw_init(void) {
    ws8300_gpio_remap(NFC_SDA_PIN, GPIO_Remap_GPIO);
    ws8300_gpio_remap(NFC_SCL_PIN, GPIO_Remap_GPIO);
    ws8300_gpio_remap(NFC_RST_PIN, GPIO_Remap_GPIO);

    ws8300_gpio_init(NFC_SDA_PIN, GPIO_Mode_Output);
    ws8300_gpio_init(NFC_SCL_PIN, GPIO_Mode_Output);
    ws8300_gpio_init(NFC_RST_PIN, GPIO_Mode_Output);
    
    ws8300_gpio_set(NFC_SCL_PIN);
    ws8300_gpio_set(NFC_SDA_PIN);
    ws8300_gpio_set(NFC_RST_PIN);
}

void ws1850s_hw_reset(void) {
    ws8300_gpio_reset(NFC_RST_PIN);
    for(volatile int i=0; i<50000; i++); // 模拟延迟
    ws8300_gpio_set(NFC_RST_PIN);
    for(volatile int i=0; i<50000; i++); // 模拟延迟
}

/* ======== 对外接口：寄存器读写逻辑 ======== */
// 详细解答：这就是 WS1850S 通过 I2C 读写寄存器的完整时序过程
void pcd_write_reg(uint8_t reg_addr, uint8_t val) {
    iic_start();
    iic_send_byte(WS1850S_I2C_ADDR_WRITE); // 发送设备写地址 (0x50)
    iic_wait_ack();
    iic_send_byte(reg_addr);               // 发送目标寄存器地址
    iic_wait_ack();
    iic_send_byte(val);                    // 发送写入的数据
    iic_wait_ack();
    iic_stop();
}

uint8_t pcd_read_reg(uint8_t reg_addr) {
    uint8_t val;
    iic_start();
    iic_send_byte(WS1850S_I2C_ADDR_WRITE); // 发送设备写地址 (0x50)
    iic_wait_ack();
    iic_send_byte(reg_addr);               // 写入要读取的寄存器地址
    iic_wait_ack();
    
    iic_start();                           // 重新产生 Start 信号
    iic_send_byte(WS1850S_I2C_ADDR_READ);  // 发送设备读地址 (0x51)
    iic_wait_ack();
    val = iic_read_byte(false);            // 读1字节，完毕后回复 NACK
    iic_stop();
    return val;
}

// 辅助函数：清除/设置 寄存器的某些位
void pcd_clear_bit_mask(uint8_t reg_addr, uint8_t mask) {
    uint8_t tmp = pcd_read_reg(reg_addr);
    pcd_write_reg(reg_addr, tmp & (~mask));
}

void pcd_set_bit_mask(uint8_t reg_addr, uint8_t mask) {
    uint8_t tmp = pcd_read_reg(reg_addr);
    pcd_write_reg(reg_addr, tmp | mask);
}
