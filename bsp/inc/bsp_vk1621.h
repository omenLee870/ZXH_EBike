/**
 * @file bsp_vk1621.h
 * @brief VK1621 LCD 驱动底层 BSP 层
 */
#ifndef _BSP_VK1621_H_
#define _BSP_VK1621_H_

#include <stdint.h>

/* --- VK1621 基础控制指令 (提取自原厂代码) --- */
#define VK1621_CMD_OSC_OFF     0x00     // 关闭震荡器
#define VK1621_CMD_OSC_ON      0x01     // 开启震荡器
#define VK1621_CMD_DISP_OFF    0x02     // 关LCD Bias
#define VK1621_CMD_DISP_ON     0x03     // 开LCD Bias
#define VK1621_CMD_COM_1_3__4  0x29     // 1/3bias 4com (最常用)
#define VK1621_CMD_TIMER_DIS   0x04     // 禁止时基输出
#define VK1621_CMD_WDT_DIS     0x05     // 禁止WDT输出
#define VK1621_CMD_BUZZ_OFF    0x08     // 关闭蜂鸣器

/* --- BSP 对外提供的纯硬件接口 --- */
void bsp_vk1621_init_io(void);
void bsp_vk1621_write_cmd(uint8_t cmd);
void bsp_vk1621_write_data(uint8_t addr, uint8_t *data, uint8_t cnt);

#endif /* _BSP_VK1621_H_ */
