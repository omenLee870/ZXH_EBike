#include "cpu.h"
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "user_gpadc.h"
//#include "app_trace.h"
#include "app_config.h"
#define MAX_READ_TIMES			    10		//一次轮询读取次数做平均，可修改

#define	HVIN_SLOPE				    1.539713
#define	HVIN_INTERCEPT		        2884.589

#define	GPIO_SLOPE				    0.317993	//gpio读取电压值转换公式的斜率
#define	GPIO_INTERCEPT		        601.50161   //gpio读取电压值转换公式的偏移值

#define	TEMP_SLOPE				    -0.186603
#define	TEMP_INTERCEPT		        107.49333

/*GPADC_Sel*/
#define GPADC_CH_SEL1(ch)		    ((uint32_t)ch)
#define GPADC_CH_SEL2(ch)		    ((uint32_t)ch)

/*AUADC_DATA_SRC*/
#define DATA_SRC_MODE1			    ((uint32_t)0x0000 << 25)
#define DATA_SRC_MODE2			    ((uint32_t)0x0001 << 25)
#define DATA_SRC_MODE3			    ((uint32_t)0x0002 << 25)
#define DATA_SRC_MODE4			    ((uint32_t)0x0003 << 25)

/*AUADC_Sample_Rate*/
#define AUADC_SAMPLERATE_8k         ((uint32_t)0x0000)

#define GPADC_VCM_060			    ((uint32_t)0x0003 << 10)



static int32_t gpadc_value_sum;

float digital_conversion_analog(gpadc_mode_t mode,int32_t value)
{
	float analog_value = 0;
	switch(mode)
	{
		case GPADC_HVIN:
		analog_value = HVIN_SLOPE * value + HVIN_INTERCEPT; //采集HVIN的GPADC值换算成电压值 单位mv
		break;
		
		case GPADC_GPIO:
		analog_value = GPIO_SLOPE * value + GPIO_INTERCEPT;//采集GPIO的GPADC值换算成电压值 单位mv
		break;
		
		case GPADC_TEMP:
		analog_value = TEMP_SLOPE * value + TEMP_INTERCEPT;//采集温度的GPADC值换算成电压值 单位mv
		break;
		
		default:
		break;
		
	}
	return analog_value;
}
/*GPADC的GPIO引脚配置*/
void gpadc_gpio_config(gpadc_gpio_t gpio) 
{
	switch(gpio)
	{
		case GPADC_GPIOA_PIN1:
		gpio_pin_remap_config(GPIOA,GPIO_PinNum_1,GPIO_Remap_ANALOG);
		break;
		case GPADC_GPIOA_PIN3:
		gpio_pin_remap_config(GPIOA,GPIO_PinNum_3,GPIO_Remap_ANALOG);
		break;
		case GPADC_GPIOA_PIN2:
		gpio_pin_remap_config(GPIOA,GPIO_PinNum_2,GPIO_Remap_ANALOG);
		break;
		case GPADC_GPIOA_PIN4:
		gpio_pin_remap_config(GPIOA,GPIO_PinNum_4,GPIO_Remap_ANALOG);
		break;
		case GPADC_GPIOB_PIN6:
		gpio_pin_remap_config(GPIOB,GPIO_PinNum_6,GPIO_Remap_ANALOG);
		break;
		case GPADC_GPIOC_PIN0:
		gpio_pin_remap_config(GPIOC,GPIO_PinNum_0,GPIO_Remap_ANALOG);
		break;
		case GPADC_GPIOC_PIN1:
		gpio_pin_remap_config(GPIOC,GPIO_PinNum_1,GPIO_Remap_ANALOG);
		break;
		case GPADC_GPIOC_PIN2:
		gpio_pin_remap_config(GPIOC,GPIO_PinNum_2,GPIO_Remap_ANALOG);
		break;
		
		default:
		break;
	}
}

static void gpadc_auadc_init(auadc_init_type_def* auadc_init_struct)
{

	AUADC->AUADC_CTRL0 = 0x01 << 26 | auadc_init_struct->AUADC_GAINADJ_L << 20 | auadc_init_struct->AUADC_GAINADJ_H << 16 | 0x1 << 15 | \
					   ((auadc_init_struct->AUADC_AGC_SET_IDX &(uint32_t)0x0f) << 11) | auadc_init_struct->AUADC_GPIO_CFG_AGC |\
						auadc_init_struct->AUADC_GPIO_CFG_AGC | ((auadc_init_struct->AUADC_GAIN_VAL &(uint32_t)0x3f) << 4) | \
						auadc_init_struct->AUADC_Sample_Rate << 1;

	AUADC->AUADC_CTRL1 = auadc_init_struct->AUADC_DATA_SRC | auadc_init_struct->AUADC_BIAS_EN | auadc_init_struct->GPADC_SEL2 << 19| \
						auadc_init_struct->GPADC_SEL1 << 16| auadc_init_struct->GPADC_VCM_SEL| \
						auadc_init_struct->GPADC_CH_SEL | auadc_init_struct->AUADC_AUADC_DIFF_EN | 0x04;

	AUADC->AUADC_Threshold = auadc_init_struct->AUADC_FIFO_Threshold;
	
}

void gapdc_configuration(gpadc_mode_t mode,gpadc_gpio_t gpio)
{
	auadc_init_type_def AUADC_InitStructure;

	memset(&AUADC_InitStructure, 0, sizeof(AUADC_InitStructure));
	
	crm_cmd_periph_clock(CRM_CG_CTRL_GPADC  | CRM_CG_CTRL_AUADC,ENABLE); 
	crm_reset_periph(CRM_SOFT_RST_GPADC     | CRM_SOFT_RST_AUADC);
	
	if (mode == GPADC_GPIO) {
        gpadc_gpio_config(gpio);
	}
    
	AUADC_InitStructure.AUADC_GAINADJ_H         =   0xC;
	AUADC_InitStructure.AUADC_GAINADJ_L         =   0x0;
	AUADC_InitStructure.AUADC_AGC_SET_IDX       =   0xC;
	AUADC_InitStructure.AUADC_GPIO_CFG_AGC      =   AUADC_GPIO_CFG_AGC_DOWN;
	AUADC_InitStructure.AUADC_GAIN_VAL          =   0x0;
	AUADC_InitStructure.AUADC_Sample_Rate       =   AUADC_SAMPLERATE_8k;        // GPADC采样率配置
	AUADC_InitStructure.AUADC_AUADC_DIFF_EN     =   AUADC_AUADC_DIFF_DOWN;      // 关闭差分采集配置
	AUADC_InitStructure.AUADC_FIFO_Threshold    =   8;
	AUADC_InitStructure.AUADC_BIAS_EN           =   AUADC_BIAS_DOWN;
	AUADC_InitStructure.AUADC_DATA_SRC          =   DATA_SRC_MODE3;             // GPADC返回值模式选择
	AUADC_InitStructure.GPADC_SEL1              =   GPADC_CH_SEL1(gpio);        // 采集电压值的+GPIO引脚配置
	AUADC_InitStructure.GPADC_SEL2              =   GPADC_CH_SEL2(gpio);        // 采集电压值的—GPIO引脚配置
	AUADC_InitStructure.GPADC_CH_SEL            =   mode;                       // GPADC采集模式选择
	AUADC_InitStructure.GPADC_VCM_SEL           =   GPADC_VCM_060;
    
    /* 非校准芯片auadc初始化 */
	gpadc_auadc_init(&AUADC_InitStructure);	
    
	auadc_cmd(ENABLE);
	gpadc_cmd(ENABLE);
    
	auadc_it_config(AUADC_IT_ALERT_EN,DISABLE);
}

int get_gpadc_value(gpadc_mode_t mode,gpadc_gpio_t gpio,uint32_t *adc_value)
{
	uint32_t i;
	
	uint32_t r3 = CRM->CG_CTRL;
	uint32_t r0 = AUADC->AUADC_CTRL0;
	uint32_t r1 = AUADC->AUADC_CTRL1;
	uint32_t r2 = AUADC->AUADC_Threshold;
	uint32_t r4 = AUADC->AUADC_INT_CTRL;

/* 关中断 */	
__set_PRIMASK(1);

	gapdc_configuration(mode, gpio);

    /* 配置使能后不能立即读取，需要延时等待。32m：317us */
//	for(i = 0; i < 2000; i++);
	for(i = 0; i < 5000; i++);
    
	gpadc_value_sum = 0;
	
	for(i = 0; i < MAX_READ_TIMES; i++)
	{
		gpadc_value_sum += (int16_t)auadc_receive_data();
	}
	gpadc_value_sum = gpadc_value_sum / MAX_READ_TIMES;

	crm_cmd_periph_clock(CRM_CG_CTRL_GPADC | CRM_CG_CTRL_AUADC,DISABLE);
	
	CRM->CG_CTRL            =   r3;
	AUADC->AUADC_CTRL0      =   r0;
	AUADC->AUADC_CTRL1      =   r1;
	AUADC->AUADC_Threshold  =   r2;
	AUADC->AUADC_INT_CTRL   =   r4;

/* 开中断 */    
__set_PRIMASK(0);
	
	if (adc_value != NULL) {
        *adc_value = gpadc_value_sum;
    }

	return (digital_conversion_analog(mode,gpadc_value_sum));

}

/************************************** 校准芯片adc采集代码 **************************************/

#define ADC_CAL_MAX     139

int16_t adc_val[ADC_CAL_MAX];
static int32_t adc_delay = 100;

void b_gapdc_configuration(gpadc_gpio_t gpio)
{
	auadc_init_type_def AUADC_InitStructure;
	AUADC_CH_SEL mode = AUADC_CH_GPIO;
	memset(&AUADC_InitStructure, 0, sizeof(AUADC_InitStructure));
	crm_cmd_periph_clock(CRM_CG_CTRL_GPADC | CRM_CG_CTRL_AUADC,ENABLE);
	crm_reset_periph(CRM_SOFT_RST_AUADC | CRM_SOFT_RST_GPADC);
	
	if(gpio == GPADC_TEMP_MODE){
		mode = AUADC_CH_TEMP;
	}else if(gpio == GPADC_HVIN_MODE){
		mode = AUADC_CH_HVIN;
	}else{ 
		gpadc_gpio_config(gpio);
		mode = AUADC_CH_GPIO;
	}
	
	AUADC_InitStructure.AUADC_GAINADJ_H 		= 0xC;
	AUADC_InitStructure.AUADC_GAINADJ_L 		= 0x0;
	AUADC_InitStructure.AUADC_AGC_SET_IDX 		= 0xC;
	AUADC_InitStructure.AUADC_GPIO_CFG_AGC 		= AUADC_GPIO_CFG_AGC_DOWN;
	AUADC_InitStructure.AUADC_GAIN_VAL 			= 0x0;
	AUADC_InitStructure.AUADC_Sample_Rate 		= AUADC_SAMPLE_RATE_32K;
	AUADC_InitStructure.AUADC_AUADC_DIFF_EN 	= AUADC_AUADC_DIFF_DOWN;
	AUADC_InitStructure.AUADC_FIFO_Threshold 	= 8;
	AUADC_InitStructure.AUADC_BIAS_EN 			= AUADC_BIAS_DOWN;
	AUADC_InitStructure.AUADC_DATA_SRC 			= AUADC_SAMPLE_ORG_DATA_10BIT;				// 1024点平均值

	adc_delay = 300;
	if(AUADC_InitStructure.AUADC_DATA_SRC == AUADC_SAMPLE_ORG_DATA_10BIT){
		adc_delay = 300;
	}else if(AUADC_InitStructure.AUADC_DATA_SRC == AUADC_SAMPLE_RATE_1024_AVERAGE_10BIT){
		adc_delay = 2048;
	}else if(AUADC_InitStructure.AUADC_DATA_SRC == AUADC_SAMPLE_RATE_8192_AVERAGE_10BIT){
		adc_delay = 3072;
	}
	
	if(mode == AUADC_CH_GPIO){
		AUADC_InitStructure.GPADC_SEL1 			= gpio;
		AUADC_InitStructure.GPADC_SEL2 			= gpio;
	}
	
	AUADC_InitStructure.GPADC_CH_SEL 			= mode;
	AUADC_InitStructure.GPADC_VCM_SEL 			= AUADC_VCM_060;
//	dbg_block_printk("GPADC_SEL1 %d GPADC_SEL2 %d mode %d\n",AUADC_InitStructure.GPADC_SEL1,AUADC_InitStructure.GPADC_SEL2,mode);

	auadc_init(&AUADC_InitStructure);	
	auadc_cmd(ENABLE);
	gpadc_cmd(ENABLE);
	auadc_agc_cmd(DISABLE);
	auadc_it_config(AUADC_IT_ALERT_EN,DISABLE);
}

void bubble_sort(int16_t a[], uint8_t n)   
{
    int16_t i,j,temp;   
    for (j=0;j<n-1;j++)    
    {                           
        for (i=0;i<n-1-j;i++)
        {
            if(a[i]>a[i+1]) 
            {
                temp=a[i];    
                a[i]=a[i+1];  
                a[i+1]=temp;
            }
        }
    }    
}


int16_t get_meadian(int16_t *buf,uint8_t n )
{
	bubble_sort(buf,n );
	return buf[n/2];
}

/*****************************************************************************
 函 数 名  : gpadc_configure_init
 功能描述  : ADC初始化
 参数  : 	gpadc_gpio_t gpio  
             REGIST_INIT init   
                 	RIGISTER_INIT	:	首次初始化，io 切换调用
    				RIGISTER_REINIT	: 	第二次初始化，从寄存器读取即可
 输出参数  : 无
 返 回 值  : 
 示例  : 
*****************************************************************************/
void gpadc_configure_init(gpadc_gpio_t gpio)
{
	static uint32_t store_r0;
	static uint32_t store_r1;
	static uint32_t store_r2;
	static uint32_t store_r3;
	static uint32_t store_r4;
	
	static gpadc_gpio_t last_gpio=GPADC_INVALID; 
	REGIST_INIT init = (last_gpio == gpio)?RIGISTER_REINIT:RIGISTER_INIT;
	gpadc_gpio_config(gpio);
	last_gpio = gpio;
	
	if(init == RIGISTER_REINIT){
		
// GPIO 复用
// ADC 使用上一次备份值
		CRM->CG_CTRL 			= store_r3;
		AUADC->AUADC_CTRL0 		= store_r0;
		AUADC->AUADC_CTRL1 		= store_r1;
		AUADC->AUADC_Threshold 	= store_r2;
		AUADC->AUADC_INT_CTRL 	= store_r4;
		auadc_cmd(ENABLE);
		gpadc_cmd(ENABLE);
		
	}else{
// ADC 初始化	
		b_gapdc_configuration(gpio);

// ADC 寄存器备份
		store_r3 = CRM->CG_CTRL;
		store_r0 = AUADC->AUADC_CTRL0;
		store_r1 = AUADC->AUADC_CTRL1;
		store_r2 = AUADC->AUADC_Threshold;
		store_r4 = AUADC->AUADC_INT_CTRL;
//		dbg_block_printk("0x%08X  0x%08X  0x%08X  0x%08X  0x%08X \n",
//			store_r0,store_r1,store_r2,store_r3,store_r4);

	}
	
}

int16_t get_adc_value(gpadc_gpio_t gpio,uint16_t cycle,int16_t *adc_value,uint16_t adc_cnt)
{
	uint32_t i,j;
	uint16_t delay = 0;
	int16_t adc;
	
//底层用到了ADC需要备份，	
	uint32_t r3 = CRM->CG_CTRL;
	uint32_t r0 = AUADC->AUADC_CTRL0;
	uint32_t r1 = AUADC->AUADC_CTRL1;
	uint32_t r2 = AUADC->AUADC_Threshold;
	uint32_t r4 = AUADC->AUADC_INT_CTRL;

	gpadc_configure_init(gpio);

	if(cycle > ADC_CAL_MAX)
		cycle = ADC_CAL_MAX;

	gpadc_value_sum = 0;
	for(i=0;i<adc_cnt;i++){
		for(j=0;j<cycle;j++)
		{
			auadc_cmd(ENABLE);
			gpadc_cmd(ENABLE);
			for(delay = 0; delay < adc_delay; delay++);			//配置使能后不能立即读取，需要延时等待。32m：317us
			for(delay = 0; delay < 500; delay++);					//配置使能后不能立即读取，需要延时等待。32m：317us
			adc_val[j] = (int16_t)auadc_receive_data();
//			user_block_print("%4d ",adc_val[j]);

		}
//		user_block_print("\n");
		adc = get_meadian(adc_val,cycle);
		if(adc_value != NULL) 
			adc_value[i] = adc;
		gpadc_value_sum += adc;
	}
	gpadc_value_sum/=adc_cnt;
	crm_cmd_periph_clock(CRM_CG_CTRL_GPADC | CRM_CG_CTRL_AUADC,DISABLE);
//ADC底层用到了，还原	

	CRM->CG_CTRL 			= r3;
	AUADC->AUADC_CTRL0 		= r0;
	AUADC->AUADC_CTRL1 		= r1;
	AUADC->AUADC_Threshold 	= r2;
	AUADC->AUADC_INT_CTRL 	= r4;
	return gpadc_value_sum;
}















