/**
******************************************************************************************************************************************************************************************
*@file                       cpu_timer.c
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
#include "cpu_timer.h"

void timer_init(uint8_t timerx , timer_init_type_def * timer_init_type_def)
{
    timer_cmd(timerx,DISABLE);
    
    if(timerx == timer0)
    {
        TIMER->TIMER_LOAD_COUNTER_0 = (timer_init_type_def->TIMER_prescaler | timer_init_type_def->TIMER_reload_cnt_val<<16 ) ;
        //TIMER->TIMER_EN &=  ~(0x01<<8);
        TIMER->TIMER_EN |= (timer_init_type_def->TIMER_pwm_pol << 8);
    }        
    else if(timerx == timer1)
    {
         TIMER->TIMER_LOAD_COUNTER_1 = (timer_init_type_def->TIMER_prescaler | timer_init_type_def->TIMER_reload_cnt_val<<16 ) ;
         TIMER->TIMER_EN &=  ~(0x01<<9);
         TIMER->TIMER_EN |= (timer_init_type_def->TIMER_pwm_pol << 9);
    }
    else if(timerx == timer2)
    {
         TIMER->TIMER_LOAD_COUNTER_2 = (timer_init_type_def->TIMER_prescaler | timer_init_type_def->TIMER_reload_cnt_val<<16 ) ;
         TIMER->TIMER_EN &=  ~(0x01<<10);
         TIMER->TIMER_EN |= (timer_init_type_def->TIMER_pwm_pol << 10);        
    }
    else if(timerx == timer3)
    {
         TIMER->TIMER_LOAD_COUNTER_3 = (timer_init_type_def->TIMER_prescaler | timer_init_type_def->TIMER_reload_cnt_val<<16 ) ;
         TIMER->TIMER_EN &=  ~(0x01<<11);
         TIMER->TIMER_EN |= (timer_init_type_def->TIMER_pwm_pol << 11);        
    }
    else if(timerx == timer4)
    {
         TIMER->TIMER_LOAD_COUNTER_4 = (timer_init_type_def->TIMER_prescaler | timer_init_type_def->TIMER_reload_cnt_val<<16 ) ;
         TIMER->TIMER_EN &=  ~(0x01<<12);
         TIMER->TIMER_EN |= (timer_init_type_def->TIMER_pwm_pol << 12);        
    }
    else if(timerx == timer5)
    {
         TIMER->TIMER_LOAD_COUNTER_5 = (timer_init_type_def->TIMER_prescaler | timer_init_type_def->TIMER_reload_cnt_val<<16 ) ;
         TIMER->TIMER_EN &=  ~(0x01<<13);
         TIMER->TIMER_EN |= (timer_init_type_def->TIMER_pwm_pol << 13);        
    }
    
//    timer_cmd(timerx,ENABLE);
    
}

void timer_cmd(uint8_t timerx , functional_state new_state)
{
    if(new_state != DISABLE)
    {
        if(timerx == timer0)
        {
            TIMER->TIMER_EN |= BIT0 ;
        }        
        else if(timerx == timer1)
        {
            TIMER->TIMER_EN |= BIT1 ;
        }
        else if(timerx == timer2)
        {
            TIMER->TIMER_EN |= BIT2 ;
        }
        else if(timerx == timer3)
        {
            TIMER->TIMER_EN |= BIT3 ;
        }
        else if(timerx == timer4)
        {
            TIMER->TIMER_EN |= BIT4 ;
        }
        else if(timerx == timer5)
        {
            TIMER->TIMER_EN |= BIT5 ;
        }        
    }
    else
    {
        if(timerx == timer0)
        {
            TIMER->TIMER_EN &= ~BIT0 ;
        }        
        else if(timerx == timer1)
        {
            TIMER->TIMER_EN &= ~BIT1 ;
        }
        else if(timerx == timer2)
        {
            TIMER->TIMER_EN &= ~BIT2 ;
        }
        else if(timerx == timer3)
        {
            TIMER->TIMER_EN &= ~BIT3 ;
        }
        else if(timerx == timer4)
        {
            TIMER->TIMER_EN &= ~BIT4 ;
        }
        else if(timerx == timer5)
        {
            TIMER->TIMER_EN &= ~BIT5 ;
        } 
    }
}

//获得当前计数值
uint32_t timer_get_current_count(uint8_t timerx)
{
    if(timerx == timer0)
    {
        return (TIMER->TIMER_LOAD_COUNTER_0);
    } 
    else if(timerx == timer1)
    {
        return (TIMER->TIMER_LOAD_COUNTER_1);
    }
    else if(timerx == timer2)
    {
        return (TIMER->TIMER_LOAD_COUNTER_2);
    }
    else if(timerx == timer3)
    {
        return (TIMER->TIMER_LOAD_COUNTER_3);
    }
    else if(timerx == timer4)
    {
        return (TIMER->TIMER_LOAD_COUNTER_4);
    }
    else if(timerx == timer5)
    {
        return (TIMER->TIMER_LOAD_COUNTER_5);
    }
	return 0;
}

void timer_toggle_set(uint8_t timerx,uint16_t toggle_val)
{
    
     if(timerx == timer0)
    {
        TIMER->TIMER_TOGSET_0 = (uint16_t)toggle_val;
    } 
    else if(timerx == timer1)
    {
        TIMER->TIMER_TOGSET_1 = toggle_val;
    }
    else if(timerx == timer2)
    {
        TIMER->TIMER_TOGSET_2 = toggle_val;
    }
    else if(timerx == timer3)
    {
        TIMER->TIMER_TOGSET_3 = toggle_val;
    }
    else if(timerx == timer4)
    {
        TIMER->TIMER_TOGSET_4 = toggle_val;
    }
    else if(timerx == timer5)
    {
        TIMER->TIMER_TOGSET_5 = toggle_val;
    }   
    
}

//选择中断类型，使能中断
void timer_it_config(uint8_t timerx,uint32_t timer_it_type,functional_state new_state)
{
    if(new_state != DISABLE)
    {
        if(timerx == timer0)
        {
            TIMER->TIMER_INT_EN |= (BIT0 | timer_it_type);
        }        
        else if(timerx == timer1)
        {
            TIMER->TIMER_INT_EN |= (BIT1 | timer_it_type) ;
        }
        else if(timerx == timer2)
        {
            TIMER->TIMER_INT_EN |= (BIT2 | timer_it_type) ;
        }
        else if(timerx == timer3)
        {
            TIMER->TIMER_INT_EN |= (BIT3 | timer_it_type) ;
        }
        else if(timerx == timer4)
        {
            TIMER->TIMER_INT_EN |= (BIT4 | timer_it_type) ;
        }
        else if(timerx == timer5)
        {
            TIMER->TIMER_INT_EN |= (BIT5 | timer_it_type) ;
        }        
    }
    else
    {
        if(timerx == timer0)
        {
            TIMER->TIMER_INT_EN &= ~(BIT0 | timer_it_type) ;
        }        
        else if(timerx == timer1)
        {
            TIMER->TIMER_INT_EN &= ~(BIT1 | timer_it_type) ;
        }
        else if(timerx == timer2)
        {
            TIMER->TIMER_INT_EN &= ~(BIT2 | timer_it_type) ;
        }
        else if(timerx == timer3)
        {
            TIMER->TIMER_INT_EN &= ~(BIT3 | timer_it_type) ;
        }
        else if(timerx == timer4)
        {
            TIMER->TIMER_INT_EN &= ~(BIT4 | timer_it_type) ;
        }
        else if(timerx == timer5)
        {
            TIMER->TIMER_INT_EN &= ~(BIT5 | timer_it_type) ;
        } 
    }
}

itstatus timer_get_it_state(uint8_t timerx)
{
    if(timerx == timer0)
    {
        if((TIMER->TIMER_INT_STATE & BIT0) == BIT0 )
        {
            return SET;
        }
        else
        {
            return RESET;
        }
    }
    else if(timerx == timer1)
    {
        if((TIMER->TIMER_INT_STATE & BIT1) == BIT1 )
        {
            return SET;
        }
        else
        {
            return RESET;
        }
    }
    else if(timerx == timer2)
    {
        if((TIMER->TIMER_INT_STATE & BIT2) == BIT2 )
        {
            return SET;
        }
        else
        {
            return RESET;
        }
    }
    else if(timerx == timer3)
    {
        if((TIMER->TIMER_INT_STATE & BIT3) == BIT3 )
        {
            return SET;
        }
        else
        {
            return RESET;
        }
    }
    else if(timerx == timer4)
    {
        if((TIMER->TIMER_INT_STATE & BIT4) == BIT4 )
        {
            return SET;
        }
        else
        {
            return RESET;
        }
    }
    else if(timerx == timer5)
    {
        if((TIMER->TIMER_INT_STATE & BIT5) == BIT5 )
        {
            return SET;
        }
        else
        {
            return RESET;
        }
    }
	return RESET;
}

void timer_it_clear(uint8_t timerx)
{
    if(timerx == timer0)
    {
        TIMER->TIMER_INT_STATE &= ~BIT0 ;
    }        
    else if(timerx == timer1)
    {
        TIMER->TIMER_INT_STATE &= ~BIT1 ;
    }
    else if(timerx == timer2)
    {
        TIMER->TIMER_INT_STATE &= ~BIT2 ;
    }
    else if(timerx == timer3)
    {
        TIMER->TIMER_INT_STATE &= ~BIT3 ;
    }
    else if(timerx == timer4)
    {
        TIMER->TIMER_INT_STATE &= ~BIT4 ;
    }
    else if(timerx == timer5)
    {
        TIMER->TIMER_INT_STATE &= ~BIT5 ;
    } 
}

void timer_update_en(uint8_t timerx , functional_state new_state)
{
    if(new_state != DISABLE)
    {
        if(timerx == timer0)
        {
            TIMER->TIMER_UPDATE_EN |= BIT0 ;
        }        
        else if(timerx == timer1)
        {
            TIMER->TIMER_UPDATE_EN |= BIT1 ;
        }
        else if(timerx == timer2)
        {
            TIMER->TIMER_UPDATE_EN |= BIT2 ;
        }
        else if(timerx == timer3)
        {
            TIMER->TIMER_UPDATE_EN |= BIT3 ;
        }
        else if(timerx == timer4)
        {
            TIMER->TIMER_UPDATE_EN |= BIT4 ;
        }
        else if(timerx == timer5)
        {
            TIMER->TIMER_UPDATE_EN |= BIT5 ;
        }        
    }
    else
    {
        if(timerx == timer0)
        {
            TIMER->TIMER_UPDATE_EN &= ~BIT0 ;
        }        
        else if(timerx == timer1)
        {
            TIMER->TIMER_UPDATE_EN &= ~BIT1 ;
        }
        else if(timerx == timer2)
        {
            TIMER->TIMER_UPDATE_EN &= ~BIT2 ;
        }
        else if(timerx == timer3)
        {
            TIMER->TIMER_UPDATE_EN &= ~BIT3 ;
        }
        else if(timerx == timer4)
        {
            TIMER->TIMER_UPDATE_EN &= ~BIT4 ;
        }
        else if(timerx == timer5)
        {
            TIMER->TIMER_UPDATE_EN &= ~BIT5 ;
        } 
    }    
}
