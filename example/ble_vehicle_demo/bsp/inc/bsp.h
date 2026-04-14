#ifndef _INCLUDE_H
#define _INCLUDE_H


//#define SCAN_ADV

/*头文件*/

/*系统头文件*/
#include <stdint.h>
#include <stdio.h>
#include "stdlib.h"
#include "stdbool.h"
#include "stdarg.h"
#include "math.h"
/*底层头文件*/
#include "cpu.h"
#include "host_stack.h"
#include "lib_api.h"
#include "cpu_sleep.h"
#include "cpu_gpio.h"
#include "cpu_spi.h"
/*宏头文件*/
#include "app_config.h"
/*ota*/
#include "ota.h"
#include "ota_service.h"
/*app*/
#include "transport_app.h"
#include "hid_service.h"
/*vehicle_app*/
#include "vehicle_app.h"
#include "vehicle_rssi.h"
#include "vehicle_simulation_spi.h"
#include "vehicle_nfc.h"
#include "vehicle_nfc_app.h"
#include "vehicle_card_manage.h"
#include "vehicle_adc.h"

/*mcu_app*/
#include "mcu_timer.h"
/*user*/
#include "user_gpadc.h"
#include "user_hardware.h"
#include "user_flash.h"
#include "user_button.h"
/*bsp.h*/
#include "bsp.h"
#include "bsp_debug.h"
#include "Delays.h"
#include "event_process.h"
/*nvs*/
#include "nvs.h"
#include "user_storage.h"















/*函数声明*/
extern void bsp_init(void);



#endif










