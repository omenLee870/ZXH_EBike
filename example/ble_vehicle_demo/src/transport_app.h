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


void read_pair_info_from_flash(void);
void transport_app_init(void);
void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);
void transport_data_schedule(void);
void ble_disconnect(void);

#endif










