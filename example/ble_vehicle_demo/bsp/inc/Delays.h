#ifndef _DELAYS_H
#define _DELAYS_H
 

extern uint32_t timer_tick;    
    
/*声明函数*/   
extern uint32_t get_tick(void);
extern uint8_t is_timeout(uint32_t start_tick, uint32_t interval);    
extern void mdelay(uint32_t mSec);
extern void secDelay(uint16_t sec);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
#endif    
    




