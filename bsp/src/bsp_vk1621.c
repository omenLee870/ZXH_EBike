/**
 * @file bsp_vk1621.c
 * @brief VK1621 硬件层通信时序实现
 * @note  底层完全适配 WS8300 GPIO 库
 */
#include "bsp_vk1621.h"
#include "cpu_gpio.h" 

/* * 【重要修改说明】：
 * 你的 WS8300 芯片每个 GPIO Port 只有 8 个引脚 (Pin0 ~ Pin7)。
 * 下面的宏定义使用了合法的引脚，请务必根据你的实际硬件原理图，
 * 将下面三个宏修改为你真实连接到 VK1621 的引脚。
 */
#define VK1621_CS_PIN   GPIO_GPIOB_Pin3   // 修改为真实的 CS 引脚
#define VK1621_WR_PIN   GPIO_GPIOB_Pin4   // 修改为真实的 WR 引脚
#define VK1621_DAT_PIN  GPIO_GPIOB_Pin5   // 修改为真实的 DAT/DATA 引脚

/* --- GPIO 快速操作宏，直接调用你的底层库 --- */
#define VK1621_CS_H()   ws8300_gpio_set(VK1621_CS_PIN)
#define VK1621_CS_L()   ws8300_gpio_reset(VK1621_CS_PIN)
#define VK1621_WR_H()   ws8300_gpio_set(VK1621_WR_PIN)
#define VK1621_WR_L()   ws8300_gpio_reset(VK1621_WR_PIN)
#define VK1621_DAT_H()  ws8300_gpio_set(VK1621_DAT_PIN)
#define VK1621_DAT_L()  ws8300_gpio_reset(VK1621_DAT_PIN)

/* 软件微秒级延时，决定通信速率。根据你的系统主频微调 */
static void bsp_delay_us(uint32_t us) {
    uint32_t i;
    while(us--) {
        i = 10;
        while(i--) { __asm("nop"); }
    }
}

/* 模拟写时钟信号 */
static void bsp_vk1621_clk(void) {
    VK1621_WR_L(); 
    bsp_delay_us(5); 
    VK1621_WR_H();	
    bsp_delay_us(5);	
}

void bsp_vk1621_init_io(void) {
    // 1. 设置为普通 GPIO 功能 (复用解除)
    ws8300_gpio_remap(VK1621_CS_PIN, GPIO_Remap_GPIO);
    ws8300_gpio_remap(VK1621_WR_PIN, GPIO_Remap_GPIO);
    ws8300_gpio_remap(VK1621_DAT_PIN, GPIO_Remap_GPIO);

    // 2. 配置为输出模式
    ws8300_gpio_init(VK1621_CS_PIN, GPIO_Mode_Output);
    ws8300_gpio_init(VK1621_WR_PIN, GPIO_Mode_Output);
    ws8300_gpio_init(VK1621_DAT_PIN, GPIO_Mode_Output);

    // 3. 默认空闲状态为高电平
    VK1621_CS_H(); 
    VK1621_WR_H();  
    VK1621_DAT_H(); 
}

/* 写命令: CS低 -> 标志码100 -> 8位命令 -> 一位填充位 -> CS高 */
void bsp_vk1621_write_cmd(uint8_t cmd) {
    uint8_t shift = 0x80; 
    uint8_t i;
    
    VK1621_CS_L();   
    bsp_delay_us(2);
    
    // 发送命令标志 100
    VK1621_DAT_H(); bsp_vk1621_clk();
    VK1621_DAT_L(); bsp_vk1621_clk();
    VK1621_DAT_L(); bsp_vk1621_clk();

    // 发送8位命令码
    for(i = 0; i < 8; i++) {
        if(shift & cmd) VK1621_DAT_H(); 
        else VK1621_DAT_L();	  
        bsp_vk1621_clk();
        shift >>= 1;
    }
    
    VK1621_DAT_L(); 
    bsp_vk1621_clk(); // 填充命令最后一位 '0'
    
    VK1621_CS_H();  
    bsp_delay_us(2);
    VK1621_DAT_H(); 
}

/* 连续写数据: CS低 -> 标志码101 -> 6位地址 -> N个4位数据 -> CS高 */
void bsp_vk1621_write_data(uint8_t addr, uint8_t *data, uint8_t cnt) {
    uint8_t i, j; 
    uint8_t shift, dataval; 
    
    VK1621_CS_L(); 
    bsp_delay_us(2);

    // 发送写入标志 101
    VK1621_DAT_H(); bsp_vk1621_clk();  
    VK1621_DAT_L(); bsp_vk1621_clk();
    VK1621_DAT_H(); bsp_vk1621_clk();

    // 发送6位起始地址 A5-A0
    shift = 0x20;
    for(i = 0; i < 6; i++) {   		 
        if (addr & shift) VK1621_DAT_H(); 		
        else VK1621_DAT_L();
        bsp_vk1621_clk();		
        shift >>= 1; 
    }
    
    // 连续写入 Cnt 个 4-bit 数据
    for (j = 0; j < cnt; j++) {
        shift = 0x01;
        dataval = *data++;
        for (i = 0; i < 4; i++) {
            if(dataval & shift) VK1621_DAT_H();		 
            else VK1621_DAT_L();
            bsp_vk1621_clk();
            shift <<= 1;
        }   
    }
    VK1621_CS_H(); 
    bsp_delay_us(2);	 
    VK1621_DAT_H(); 
}
