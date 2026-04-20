#ifndef _TRANSPORT_APP_H
#define _TRANSPORT_APP_H
#include "user_ring_buffer.h"
#include "bluetooth.h"
enum{
	NONE_CONNECTED = 0,
	BLE_CONNECTED,
};
typedef enum
{
	STATE_DISCONNECTED,
	STATE_CONNNECTED,
}connected_t;
extern uint8_t transport_connection_type;
extern ring_buffer_t g_ring_buffer;
extern uint8_t stroge[200];
extern void ble_send_data(void *arg);
extern bd_addr_t otp_addr;

/* 新增：蓝牙接收数据回调函数类型 */
typedef void (*ble_rx_data_callback_t)(uint8_t *data, uint16_t len);
/* 新增：允许应用层注册一个接收回调 */
void transport_register_rx_callback(ble_rx_data_callback_t cb);
void read_pair_info_from_flash(void);
void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);
void transport_app_init(void);
void transport_data_schedule(void);
void ble_disconnect(void);
void ble_gatt_send_data(uint8_t *p_data,uint16_t len);

#endif










