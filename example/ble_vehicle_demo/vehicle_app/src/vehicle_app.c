#include "bsp.h"




vehicle_param_t  s_vehicle_param =
{
	.vehicle_state = POWER_OFF,
};







/****************************************************************************/
void set_vehicle_state(vehicle_state_e state)
{
	s_vehicle_param.vehicle_state = state;
}
uint8_t  is_cur_vehicle_state(vehicle_state_e state)
{
	return (s_vehicle_param.vehicle_state == state);
}





















