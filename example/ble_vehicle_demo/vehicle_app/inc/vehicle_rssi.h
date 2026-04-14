#ifndef _VEHICLE_RSSI_H_
#define _VEHICLE_RSSI_H_




#include "stdint.h"






/*外部声明*/
extern uint8_t pair_device_state;

/*函数声明*/
extern void connect_rssi_timer(void);

extern void rssi_process(void);



#endif








