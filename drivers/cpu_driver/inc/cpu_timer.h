/**
******************************************************************************************************************************************************************************************
*@file                       cpu_timer.h
*@author         
*@data                       
*@version                    v1.0
*@par Copyright(c):
*
*@par History:
*     version:author,date,desc
*     v1.0   :create this file
*
*******************************************************************************************************************************************************************************************
*/
#ifndef CPU_TIMER_H
#define CPU_TIMER_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "cpu.h"

#define     timer0      0
#define     timer1      1
#define     timer2      2
#define     timer3      3
#define     timer4      4     
#define     timer5      5  
     
typedef struct
{
    
    uint32_t TIMER_prescaler; //timer 预分频值
    
    uint32_t TIMER_pwm_pol; //pwm极性 
    
    uint32_t TIMER_reload_cnt_val;  //timer cnt装载值
           

} timer_init_type_def;    

//timer_it_type
#define TIMER0_int_cfg_0           (0x00<<8)     //计数到0
#define TIMER0_int_cfg_toggle      (0x01<<8)     //计数到toggle_set_val
#define TIMER1_int_cfg_0           (0x00<<9)     //计数到0
#define TIMER1_int_cfg_toggle      (0x01<<9)     //计数到toggle_set_val
#define TIMER2_int_cfg_0           (0x00<<10)    //计数到0
#define TIMER2_int_cfg_toggle      (0x01<<10)    //计数到toggle_set_val
#define TIMER3_int_cfg_0           (0x00<<11)    //计数到0
#define TIMER3_int_cfg_toggle      (0x01<<11)    //计数到toggle_set_val
#define TIMER4_int_cfg_0           (0x00<<12)    //计数到0
#define TIMER4_int_cfg_toggle      (0x01<<12)    //计数到toggle_set_val
#define TIMER5_int_cfg_0           (0x00<<13)    //计数到0
#define TIMER5_int_cfg_toggle      (0x01<<13)    //计数到toggle_set_val

//timer极性
#define TIMER_PWM_POL_HIGHT        (0x01)    //pwm极性，初始化状态为高
#define TIMER_PWM_POL_LOW          (0x00)    //pwm极性，初始化状态为高

void timer_cmd(uint8_t timerx , functional_state new_state);

void timer_init(uint8_t timerx , timer_init_type_def * timer_init_type_def);

uint32_t timer_get_current_count(uint8_t timerx);

void timer_it_config(uint8_t timerx,uint32_t timer_it_type,functional_state new_state);

itstatus timer_get_it_state(uint8_t timerx);

void timer_it_clear(uint8_t timerx);

void timer_update_en(uint8_t timerx , functional_state new_state);

void timer_toggle_set(uint8_t timerx,uint16_t toggle_val);

#ifdef __cplusplus
}
#endif

#endif

