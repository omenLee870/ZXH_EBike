#include "bsp.h"

#define MAX_RSSI   (-72)

/* 0-配对失败   1-配对成功 */
uint8_t pair_device_state = 0;

int lock_rssi[3] = {-80,-84,-88};
uint8_t open_lock_rssi_counter;
uint8_t close_lock_rssi_counter;

void user_con_timer(void)
{
	int rssi = lib_get_ble_rssi();
	if(rssi > -15 || rssi < -95) return;
	LogInfo("rssi = %d\r\n",rssi);
	if(is_cur_vehicle_state(POWER_ON)){
		open_lock_rssi_counter = 0;
		if(rssi < lock_rssi[0]){
			close_lock_rssi_counter++;
		}
		else close_lock_rssi_counter = 0;
		if(close_lock_rssi_counter == 7){ 
			set_vehicle_state(POWER_OFF);
			dbg_printk("clock lock\r\n");									 
		}
	}
	else{
		close_lock_rssi_counter = 0;
		if(rssi > MAX_RSSI){
			open_lock_rssi_counter++;
		}
		else open_lock_rssi_counter = 0;
		if(open_lock_rssi_counter == 3){
			set_vehicle_state(POWER_ON);
			dbg_printk("open lock\r\n");
		}
	}
}


void rssi_process(void)
{
	if ((transport_connection_type == BLE_CONNECTED)&&
        (pair_device_state)) {  
           
		user_con_timer();	
    }
}

