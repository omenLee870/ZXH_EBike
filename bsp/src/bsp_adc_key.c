#include "bsp_adc_key.h"
#include "cpu_gpadc.h"
#include "cpu_gpio.h"
#include "adc_lib.h"

/* 假设你的 ADC 按键接在 GPIOA_Pin2，请根据原理图修改此处宏定义 */
#define ADC_KEY_PORT_PIN    GPIO_GPIOA_Pin6

void bsp_adc_key_init(void) {
    GPADC_InitTypeDef gpadc_init;

    // 1. 将按键引脚复用为模拟输入模式 (调用 cpu_gpio.h 接口)
    ws8300_gpio_remap(ADC_KEY_PORT_PIN, GPIO_Remap_ANALOG);
    
    // 2. 配置 GPADC 参数 (调用 cpu_gpadc.h 接口)
    gpadc_init.GPADC_Gain         = GPADC_Gain_1;        // 增益设置为1
    gpadc_init.GPADC_Vcm_Sel      = GPADC_VCM_075;       // 共模电压选择
    gpadc_init.GPADC_InMode       = GPADC_SINGLE_IN;     // 单端输入
    gpadc_init.GPADC_Ch_Sel       = GPADC_CH_GPIO;       // 选择 GPIO 模拟通道
    gpadc_init.GPADC_Sample_Rate  = GPADC_SAMPLERATE_8k; // 采样率 8k
    gpadc_init.GPADC_WriteIn_Mode = GPADC_MODE_SINGLE;   // 单次转换模式
    gpadc_init.GPADC_EN           = GPADC_OPEN;          // 开启 GPADC
    
    GPADC_Init(&gpadc_init);
}

int32_t bsp_adc_key_get_voltage(void) {
    uint16_t raw_data = 0;
    
    // 1. 获取 GPADC 的原始数字量
    raw_data = GPADC_ReceiveData();
    
    // 2. 调用原厂 ADC 库，将数字量转换为真实电压值 (mV)
    // 根据 adc_lib.h 提供的接口，GPIO_ADC 枚举代表普通GPIO采压
    return get_adc_volum((int16_t)raw_data, GPIO_ADC);
}
