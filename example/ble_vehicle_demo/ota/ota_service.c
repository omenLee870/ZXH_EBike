#include <stdint.h>
#include <string.h>
#include "host_stack.h"

#include "cpu_sleep.h"
#include "cpu.h"
#include "user_hardware.h"
#include "app_config.h"

#include "ota.h"
#include "ota_service.h"

#define SOFT_VERSION	"1.00"

static uint16_t ota_con_handle;
static uint16_t le_notification_enabled;

uint16_t ota_mtu 				= 20;
uint8_t ota_buf_data[300];
uint16_t ota_buf_offset;
uint16_t ota_buf_len;
uint16_t ota_require_send = 0;

stack_timer_t ota_updata_param_request_timer;//updta timer

/* OTA_server */
#define APP_ATT_UUID_OTA_SERVICE						0xFAB0
#define APP_ATT_UUID_OTA_CTRL_PORT                  	0xFAB1 //CMD命令发送
#define APP_ATT_UUID_OTA_DOWNLOAD_PORT				0xFAB2 //固件下载
#define APP_ATT_UUID_OTA_EVENT_PORT					0xFAB3 //事件上报

static uint16_t ota_server_event_handle;
static uint16_t ota_server_cmd_handle;


static void updata_connection_param(stack_timer_t *ts)
{
	gap_request_connection_parameter_update(ota_con_handle,DEFAULT_UPDATE_MIN_CONN_INTERVAL,DEFAULT_UPDATE_MAX_CONN_INTERVAL,DEFAULT_UPDATE_SLAVE_LATENCY,DEFAULT_UPDATE_CONN_TIMEOUT);
}

static void updata_connect_param(void)
{
	lib_start_timer(&ota_updata_param_request_timer,updata_connection_param,DEFAULT_UPDATE_PAUSE_TIME);
}

void ota_ble_send_data(void *arg)
{
	//上报att数据会产生ATT_EVENT_CAN_SEND_NOW事件，在事件中再调用发送函数
	att_server_request_can_send_now_event(ota_con_handle);	
}


void ota_send_data(void)
{
	uint16_t uilen;
	if(ota_buf_len >= ota_mtu)
	{
		uilen = ota_mtu;
	}
	else
	{
		uilen = ota_buf_len;
	}
	int ret = att_server_notify(ota_con_handle, ota_server_event_handle, ota_buf_data + ota_buf_offset, uilen);
	if(ERROR_CODE_SUCCESS == ret)
	{
		ota_buf_offset += uilen;
		ota_buf_len -= uilen;
		if(ota_buf_len != 0)
		{
			lib_event_post(ota_ble_send_data,NULL);//提交到事件队列中处理，不直接调用发送函数
		}
		else
		{
			ota_buf_offset = 0;
		}
	}
	else
	{
		lib_event_post(ota_ble_send_data,NULL);//提交到事件队列中处理，不直接调用发送函数
	}
}


//write requests
int ota_att_write_callback(uint16_t con_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size)
{
	if (transaction_mode == ATT_TRANSACTION_MODE_CANCEL) 
		return 0;
	
    // find characteristic for handle
    if(att_handle == ota_server_event_handle + 1)
	{
		ota_con_handle = con_handle;
		le_notification_enabled = little_endian_read_16(buffer, 0) == GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION;
		if(le_notification_enabled){
			dbg_printk("ccc enable\n");
		}else{
			dbg_printk("ccc disable\n");
		}
		updata_connect_param();
//		att_exchange_mut_request(con_handle,247);
		return 1;
	}
	//接收到手机的数据
	if(att_handle == ota_server_cmd_handle)
	{
		if(buffer)
		{
			ota_cmd_ctl(buffer,buffer_size);
		}
		return 2;
	}
	return 0;
}

uint8_t get_ota_requir_send_once(void)
{
	uint8_t ota_require = ota_require_send;
	ota_require_send = 0;
	return ota_require;
}

void ota_bt_notify(uint8_t *data,uint16_t len) 
{
	ota_buf_len 		= len;
	ota_buf_offset 	= 0;
	memcpy(ota_buf_data,data,len);
	ota_require_send 	= 1;
	lib_event_post(ota_ble_send_data,NULL);//提交到事件队列中处理，不直接调用发送函数
}

void updata_ota_mtu(uint16_t mtu)
{
	ota_mtu = mtu - 3;
}

void ota_app_init(void)
{
	//事件上报：发送
	att_db_util_add_service_uuid16(APP_ATT_UUID_OTA_SERVICE);
	ota_server_event_handle = att_db_util_add_characteristic_uuid16(APP_ATT_UUID_OTA_EVENT_PORT,ATT_PROPERTY_NOTIFY,ATT_SECURITY_NONE,ATT_SECURITY_NONE,NULL,0);
	ota_server_cmd_handle = att_db_util_add_characteristic_uuid16(APP_ATT_UUID_OTA_CTRL_PORT, ATT_PROPERTY_READ | ATT_PROPERTY_DYNAMIC | ATT_PROPERTY_WRITE | ATT_PROPERTY_WRITE_WITHOUT_RESPONSE,ATT_SECURITY_NONE,ATT_SECURITY_NONE,NULL,0);
	att_db_util_add_characteristic_uuid16(APP_ATT_UUID_OTA_DOWNLOAD_PORT, ATT_PROPERTY_READ | ATT_PROPERTY_DYNAMIC | ATT_PROPERTY_WRITE | ATT_PROPERTY_WRITE_WITHOUT_RESPONSE,ATT_SECURITY_NONE,ATT_SECURITY_NONE,NULL,0);
}

