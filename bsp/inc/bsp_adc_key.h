/**
 * @file bsp_adc_key.h
 * @brief ADC按键底层硬件抽象层
 * @note 只负责配置外设和提供电压值，不参与任何逻辑判定
 */
#ifndef _BSP_ADC_KEY_H_
#define _BSP_ADC_KEY_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化 ADC 按键相关的 GPIO 和 GPADC 外设
 */
void bsp_adc_key_init(void);

/**
 * @brief 获取当前 ADC 按键通道的真实电压值
 * @return int32_t 当前电压值 (单位: mV)
 */
int32_t bsp_adc_key_get_voltage(void);

#ifdef __cplusplus
}
#endif

#endif /* _BSP_ADC_KEY_H_ */
