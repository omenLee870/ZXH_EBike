#include "bsp.h"
#include "adc_lib.h"


/* 芯片是否校准 0-未校准 1-已校准*/
#define CALIBRATED_CHIP         1

/* 定义电动车采电压管脚 */
#define VEHICLE_ADC_PIN         GPADC_GPIOA_PIN1



/* 获取电动车分压后的电压值 */
int get_vehicle_adc_val(void)
{
    uint32_t adc_val    =   0;
    int voltage_val     =   0;

#if(CALIBRATED_CHIP)
    adc_val     =   get_adc_value(VEHICLE_ADC_PIN, 9, NULL, 14);
    voltage_val =   get_adc_volum(adc_val,GPIO_ADC);
#else
    /* vehicle_voltage_val范围0-1200对应0-1.2v */
    voltage_val = get_gpadc_value(GPADC_GPIO, VEHICLE_ADC_PIN, &adc_val);
#endif
    
    LogInfo("voltage_val = %d\r\n", voltage_val);

    return voltage_val;
}

































