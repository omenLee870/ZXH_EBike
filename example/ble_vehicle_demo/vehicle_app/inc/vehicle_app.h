#ifndef _VEHICLE_APP_H
#define _VEHICLE_APP_H

#include "stdint.h"





typedef enum
{
	POWER_OFF,
	POWER_ON,
}vehicle_state_e;






typedef struct
{
	vehicle_state_e vehicle_state;
}vehicle_param_t;













/*****************************************************************************************************/
extern void set_vehicle_state(vehicle_state_e state);
extern uint8_t  is_cur_vehicle_state(vehicle_state_e state);








#endif

