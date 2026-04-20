#include "bsp.h"


#define SOFT_VERSION	"1.00"

#define BLE_STORE_ENABLE 1	//????????豸????

#define MAX_SPP_MTU				      128

static uint16_t connection_handle;
static uint16_t iot_server_tx_handle;
static uint16_t iot_server_rx_handle;

static uint16_t le_notification_enabled;
uint16_t batteryremainpower=10;
uint8_t transport_mtu = 20;
uint8_t transport_connection_type;
uint8_t transport_data[200];
uint8_t transport_complete_flag = 1;
uint8_t transport_uilen;
stack_timer_t updata_param_request_timer;//updta timer
uint8_t device_ble_addr[] = {0xAD,0xBC,0xDE,0x60,0x40,0x32};
static ble_rx_data_callback_t g_ble_rx_callback = NULL;   // 接收回调指针

typedef struct
{
	bd_addr_t addr;
	link_key_t key;
	link_key_type_t type;
}PAIR_INFO_T;

// 实现注册函数
void transport_register_rx_callback(ble_rx_data_callback_t cb)
{
    g_ble_rx_callback = cb;
}

void ble_send_data(void *arg)
{
	//???att????????ATT_EVENT_CAN_SEND_NOW????????????????÷??????
	att_server_request_can_send_now_event(connection_handle);	
}

static void att_transport_report(void)
{
	int ret = att_server_notify(connection_handle, iot_server_tx_handle, transport_data, transport_uilen);
	if(ERROR_CODE_SUCCESS == ret){
		transport_complete_flag = 1;
	}else{
		lib_event_post(ble_send_data,NULL);//????????????д????????????÷??????
	}
}

//att_read_callback helpers
static uint16_t att_read_callback_handle_blob(const uint8_t * blob, uint16_t blob_size, uint16_t offset, uint8_t * buffer, uint16_t buffer_size){
    if (buffer){
        uint16_t bytes_to_copy = host_stack_min(blob_size - offset, buffer_size);
        (void)memcpy(buffer, &blob[offset], bytes_to_copy);
        return bytes_to_copy;
    } else {
        return blob_size;
    }
}

//?????д???ATT_PROPERTY_DYNAMIC??????????????????????
static uint16_t att_read_callback(uint16_t con_handle, uint16_t att_handle, uint16_t offset, uint8_t * buffer, uint16_t buffer_size)
{
	if(att_handle != iot_server_rx_handle) return 0;
	
	const uint8_t read[6] = "012345";
	return att_read_callback_handle_blob(read,sizeof(read),offset,buffer,buffer_size);
}

//write requests
static int att_write_callback(uint16_t con_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size)
{ 

	if (transaction_mode == ATT_TRANSACTION_MODE_CANCEL) return 0;
	
    // find characteristic for handle
    if(att_handle == iot_server_tx_handle + 1)
	{
		le_notification_enabled = little_endian_read_16(buffer, 0) == GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION;
		if(le_notification_enabled){
			dbg_printk("ccc enable\n");
		}else{
			dbg_printk("ccc disable\n");
		}
	}
	//??????????????
	if(att_handle == iot_server_rx_handle)
	{
		if(buffer)
		{
			// 如果应用层注册了回调，则转发数据
            if(g_ble_rx_callback){
                g_ble_rx_callback(buffer, buffer_size);
            }
		}
	}
#if OTA_SURPORT
	ota_att_write_callback(con_handle, att_handle, transaction_mode, offset, buffer, buffer_size);
  #endif	
	return 0;
}

void updata_connection_param(stack_timer_t *ts)
{
	gap_request_connection_parameter_update(connection_handle,DEFAULT_UPDATE_MIN_CONN_INTERVAL,DEFAULT_UPDATE_MAX_CONN_INTERVAL,DEFAULT_UPDATE_SLAVE_LATENCY,DEFAULT_UPDATE_CONN_TIMEOUT);
}
#if (BLE_STORE_ENABLE)
#define MAX_STORE_NUM		8
#pragma pack(1)
typedef struct
{
	uint8_t nvram[16*MAX_STORE_NUM];
	uint8_t nvram_index;	
}store_pair_info_t;
#pragma pack()
store_pair_info_t g_store_pair_info;
uint8_t match_index;
void sm_irk_match_cb(uint8_t reason);
uint8_t is_pair_info_exist(const uint8_t*pair_info)
{
	for(uint8_t i = 0;i < MAX_STORE_NUM;i++){
		if(memcmp(&g_store_pair_info.nvram[i * 16],pair_info,16) == 0){
			return i+1;
		}
	}
	return 0;
}
void store_pair_info_to_flash(void)
{
	storage_save(VEHICLE_MATCH_DEVICE_FLASH_ID,&g_store_pair_info,sizeof(store_pair_info_t));
	LogInfo("store_pair_info_to_flash\r\n");
}
void read_pair_info_from_flash(void)
{
	uint8_t len = sizeof(store_pair_info_t);
	memset(&g_store_pair_info,0x00,len);
	storage_read(VEHICLE_MATCH_DEVICE_FLASH_ID,&g_store_pair_info,len,(uint32_t*)&len);
	LogHex("g_store_pair_info",(uint8_t*)&g_store_pair_info,sizeof(store_pair_info_t));
}
void sm_irk_match_process(void *arg)
{
	uint8_t key[16];

	for(int i = 0; i < 16; i++)
	{
		key[i] = g_store_pair_info.nvram[15 + (match_index * 16) - i];
	}
	if(match_index == MAX_STORE_NUM)
	{
		match_index = 0;
		return;
	}
	match_index++;
	sm_irk_match(key,sm_irk_match_cb);
}

void sm_irk_match_cb(uint8_t reason)
{
	if(reason == 0)//match sucess
	{
		dbg_printk("match success\n");
		pair_device_state = 1;
		match_index = 0;
	}
	else
	{
		if(match_index == (MAX_STORE_NUM - 1))
			dbg_printk("match fail\n");
		lib_event_post(sm_irk_match_process,NULL);
	}
}
#endif
void adv_report_process(uint8_t *packet)
{
	uint8_t adv_pos = 0;
	bd_addr_t peer_event_addr;
	uint8_t adr_type = gap_event_advertising_report_get_address_type(packet);
	uint8_t len      = gap_event_advertising_report_get_data_length(packet);
	int8_t  rssi 		 = gap_event_advertising_report_get_rssi(packet);
	const uint8_t *adv_data = gap_event_advertising_report_get_data(packet);
	
	if(len > 31)return;
	while(adv_pos < len)
	{
		uint8_t adv_len = adv_data[adv_pos++];
		uint8_t adv_type = adv_data[adv_pos];
		if(adv_len > 30)break;
		switch(adv_type) 
		{ 
#if 1
			case 0x09:{
			}break;
			case 0xFF:{	/* ?????AD Type */	

			}
			break;	
#endif			
			default:
			break;
		}
		adv_pos += adv_len;
	}
}
void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
	UNUSED(channel);
    UNUSED(size);
    uint16_t conn_interval;
	bd_addr_t event_addr;

    switch (packet_type) {
        case HCI_EVENT_PACKET:
            switch (hci_event_packet_get_type(packet)) {
                case HCI_EVENT_DISCONNECTION_COMPLETE:
					transport_connection_type = NONE_CONNECTED;	
					pair_device_state = 0;								
					dbg_printk("Disconected Reason %02X\n",packet[5]);
				break;
				case HCI_EVENT_LINK_KEY_NOTIFICATION:
						reverse_bd_addr(&packet[2], event_addr);
						PAIR_INFO_T pair_info;
						pair_info.type = (link_key_type_t)packet[24];
						memcpy(pair_info.addr,event_addr,sizeof(bd_addr_t));
						memcpy(pair_info.key,&packet[8],sizeof(link_key_t));;
				break;
				case SM_EVENT_JUST_WORKS_REQUEST:
						sm_just_works_confirm(sm_event_just_works_request_get_handle(packet));
						break;
				case SM_EVENT_NUMERIC_COMPARISON_REQUEST:
						sm_numeric_comparison_confirm(sm_event_passkey_display_number_get_handle(packet));
						break;
				case SM_EVENT_PASSKEY_DISPLAY_NUMBER:
						dbg_printk("Display Passkey: %u\n", sm_event_passkey_display_number_get_passkey(packet));
						break;
				case L2CAP_EVENT_CONNECTION_PARAMETER_UPDATE_RESPONSE:
                    dbg_printk("L2CAP Connection Parameter Update Complete, response: %x\n", l2cap_event_connection_parameter_update_response_get_result(packet));
                    break;
				case SM_EVENT_PASSKEY_INPUT_NUMBER:
					sm_passkey_input(sm_event_passkey_input_number_get_handle(packet), 12346);
					break;
	case SM_EVENT_PAIRING_COMPLETE:
					switch (sm_event_pairing_complete_get_status(packet)){
						case ERROR_CODE_SUCCESS:
						{
							dbg_printk("Pairing complete, success\n");
							#if (BLE_STORE_ENABLE)
							uint8_t addr_type = sm_event_pairing_complete_get_type(packet);
							const uint8_t *data = sm_event_pairing_complete_get_data(packet);
							switch(addr_type)
							{
								case GAP_RANDOM_ADDRESS_TYPE_OFF:
								case GAP_RANDOM_ADDRESS_TYPE_STATIC:
								memcpy(&g_store_pair_info.nvram[g_store_pair_info.nvram_index * 16],data,6);	//?洢??6????豸???
								g_store_pair_info.nvram_index = (g_store_pair_info.nvram_index + 1) % MAX_STORE_NUM;
								break;
								
								case GAP_RANDOM_ADDRESS_NON_RESOLVABLE:
								memcpy(&g_store_pair_info.nvram[g_store_pair_info.nvram_index * 16],data,8);//?洢??8????rand
								g_store_pair_info.nvram_index = (g_store_pair_info.nvram_index + 1) % MAX_STORE_NUM;
								break;
								
								case GAP_RANDOM_ADDRESS_RESOLVABLE:
								pair_device_state = 1;
								if(!is_pair_info_exist(data)){
									memcpy(&g_store_pair_info.nvram[g_store_pair_info.nvram_index * 16],data,16);//?洢??16????irk
									store_pair_info_to_flash();
									g_store_pair_info.nvram_index = (g_store_pair_info.nvram_index + 1) % MAX_STORE_NUM;
								}							
								break;
							}
							dbg_printk("peer device type = %d,",addr_type);
							for(int i = 0; i < 16; i++)
							{
								dbg_printk("%02X ",data[i]);
							}
							#endif
						}
							break;
						case ERROR_CODE_CONNECTION_TIMEOUT:
							break;
						case ERROR_CODE_REMOTE_USER_TERMINATED_CONNECTION:
							break;
						case ERROR_CODE_AUTHENTICATION_FAILURE:
							break;
						default:
							break;
					}
					break;
					#if (BLE_STORE_ENABLE)
					case SM_EVENT_LTK_REQUEST:
					{
						uint8_t type  = sm_event_ltk_request_get_addr_type(packet);
						const uint8_t *data = sm_event_ltk_request_get_data(packet);
						switch(type)
						{
							case GAP_RANDOM_ADDRESS_TYPE_OFF:
							case GAP_RANDOM_ADDRESS_TYPE_STATIC://???????????豸????????洢???豸????????????????豸
							for(int i = 0; i < MAX_STORE_NUM; i++)
							{
								if(memcmp(data,&g_store_pair_info.nvram[i * 16],6) == 0)
								{//????豸
									
								}
							}
							break;
							
							case GAP_RANDOM_ADDRESS_NON_RESOLVABLE://????rand?????豸????????????rand??????????????豸
							for(int i = 0; i < MAX_STORE_NUM; i++)
							{
								if(memcmp(data,&g_store_pair_info.nvram[i * 16],8) == 0)
								{//????豸
									
								}
							}
							break;
							
							case GAP_RANDOM_ADDRESS_RESOLVABLE://????irk????豸???????????irk?????sm_irk_match???????????????豸
							sm_irk_match_process(NULL);
							break;
						}
						
					}break;
					#endif
			
                case HCI_EVENT_LE_META:
                    switch (hci_event_le_meta_get_subevent_code(packet)) {
                        case HCI_SUBEVENT_LE_CONNECTION_COMPLETE://??????????
						// print connection parameters (without using float operations)
						if(hci_subevent_le_connection_complete_get_status(packet) != 0x00) return;
						connection_handle = hci_subevent_le_connection_complete_get_connection_handle(packet);
						conn_interval = hci_subevent_le_connection_complete_get_conn_interval(packet);

						
						break;
                        case HCI_SUBEVENT_LE_CONNECTION_UPDATE_COMPLETE://?????????????????
						// print connection parameters (without using float operations)
                        conn_interval = hci_subevent_le_connection_update_complete_get_conn_interval(packet);
						dbg_printk("- Connection Update Interval: %u.%02u ms\n", conn_interval * 125 / 100, 25 * (conn_interval & 3));
						break;
						
                        default:
                            break;
                    }
                    break; 
				case ATT_EVENT_CONNECTED:
				{
					transport_connection_type = BLE_CONNECTED;
					transport_mtu = att_server_get_mtu(connection_handle) - 3;
					#if (DEFAULT_ENABLE_UPDATE_REQUEST)
					lib_start_timer(&updata_param_request_timer,updata_connection_param,DEFAULT_UPDATE_PAUSE_TIME);
					#endif
					dbg_printk("ATT_EVENT_CONNECTED DEFAULT MTU %d\n",att_server_get_mtu(connection_handle));
				}break;
				
				case ATT_EVENT_MTU_EXCHANGE_COMPLETE://????MTU?????
				 {
					  transport_mtu = att_event_mtu_exchange_complete_get_MTU(packet) - 3;
#if OTA_SURPORT
						 updata_ota_mtu(transport_mtu);
#endif		
					 dbg_printk("ATT MTU EXCHANGE COMPLETE %d\n",att_event_mtu_exchange_complete_get_MTU(packet));
				 }break;

		    	case ATT_EVENT_CAN_SEND_NOW://att????????????????
				{
#if OTA_SURPORT
					if(get_ota_requir_send_once()){
							ota_send_data();
					}
					else
#endif					
					{
						att_transport_report();
					}

				}
				break;
				case GAP_EVENT_ADVERTISING_REPORT://??????
#ifdef SCAN_ADV			
			adv_report_process(packet);
#endif
				break;
			   case HCI_EVENT_ENCRYPTION_CHANGE: //???????
				break;

				default:
					break;

		}break;
		
		default: 
			break;

	}
}
uint8_t stroge[200];
ring_buffer_t g_ring_buffer;
void transport_data_schedule(void)
{
	if(user_ring_buffer_empty(&g_ring_buffer))
		return;
	transport_uilen = user_ring_buffer_read(&g_ring_buffer,transport_data,transport_mtu);
	switch(transport_connection_type)
	{
		case BLE_CONNECTED:
		transport_complete_flag = 0;
		lib_event_post(ble_send_data,NULL);//????????????д????????????????÷??????
		break;
		default:	
		break;
	}
}

void transport_uart_init(void)
{
	gpio_pin_remap_config(GPIOD,GPIO_PinNum_2,GPIO_Remap_UART0_RX);
	gpio_pin_remap_config(GPIOD,GPIO_PinNum_3,GPIO_Remap_UART0_TX);	  
}

void transport_uart_sleep(void)
{
	gpio_pin_remap_config(GPIOD,GPIO_PinNum_2,GPIO_Remap_GPIO);
	gpio_pin_remap_config(GPIOD,GPIO_PinNum_3,GPIO_Remap_GPIO);	
	gpio_init(GPIOD,GPIO_Pin_2,GPIO_Mode_Input_Pull_Up_Resistor);
	gpio_init(GPIOD,GPIO_Pin_3,GPIO_Mode_Input_Pull_Up_Resistor);
}

//prepare sleep hardware
void hardware_sleep_prepare(void)
{
	#if (UART_ENABLE)
	transport_uart_sleep();
	#endif
	
}

//recover hardware
void hardware_sleep_recover(void)
{
	#if (UART_ENABLE)
	transport_uart_init();
	user_uart_resume();
	#endif
}

const stack_init_t ble_stack_init = {
	.sleep_peripheral_prepare = hardware_sleep_prepare,
	.sleep_peripheral_recover = hardware_sleep_recover,
	.packet_handler = packet_handler,
};

#define IOT_UUID_SERVICE   0xFF00
#define IOT_UUID_RX        0xFF01
#define IOT_UUID_TX        0xFF02
uint8_t adv_data[31] = {
    // Flags general discoverable, BR/EDR not supported
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, 0x06,
    // Name 
    0x0c, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME,'w','s','8','3','0','0','_','b','i','k','e',
    // 16-bit Service UUIDs
    0x03, BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS, ORG_BLUETOOTH_SERVICE_HUMAN_INTERFACE_DEVICE & 0xff, ORG_BLUETOOTH_SERVICE_HUMAN_INTERFACE_DEVICE >> 8,
//    // Appearance HID - keyboard (Category 15, Sub-Category 2)
//    0x03, BLUETOOTH_DATA_TYPE_APPEARANCE, 0x80, 0x01,
    // Appearance HID - Mouse (Category 15, Sub-Category 2)
    0x03, BLUETOOTH_DATA_TYPE_APPEARANCE, 0xC2, 0x00,
		0x06,0xff,0xff,0xff,0xff,0xff,0xff,//???????????????31?????????Щ?????????????	
};

uint8_t scan_resp_data[31] = {
    // Name 
    0x0c, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'w','s','8','3','0','0','_','b','i','k','e'
};
void ble_gatt_send_data(uint8_t *p_data,uint16_t len)
{
	user_ring_buffer_write(&g_ring_buffer,(uint8_t*)p_data,len);//д????
}
static stack_timer_t notify_timer;
static void notify_event_handler(stack_timer_t *ts)
{
	uint8_t notify_data[6] = {0x01,0x02,0x03,0x04,0x05,0x06,};
	lib_stop_timer(&notify_timer);
	if(transport_connection_type == BLE_CONNECTED)
		ble_gatt_send_data(notify_data,6);
	lib_start_timer(&notify_timer,notify_event_handler,1000);	
}
/*????????????*/
extern uint8_t IRQ_FLAG;
stack_timer_t adv_check_timer;
void adv_check_timer_handler(stack_timer_t *ts)
{
	if(IRQ_FLAG == 1){
		LogInfo("reboot_adv_func\r\n");
		crm_reset_periph(CRM_SOFT_RST_GLB);
	}
	IRQ_FLAG = 1;
	lib_start_timer(&adv_check_timer,adv_check_timer_handler,10000);//10s??????
}
bd_addr_t otp_addr;
void transport_app_init(void)
{
	#if (UART_ENABLE)
	transport_uart_init();
	user_uart_init(NULL);
	dbg_printk("version = %s,%d-%d-%d,%s%s\n",SOFT_VERSION,lib_get_version() >> 16 & 0xFFFF,lib_get_stack_type(),lib_get_version() & 0xFFFF,__DATE__,__TIME__);
	#endif
	

	//init ble stack
	lib_bluetooth_init((void *)&ble_stack_init);	
	bsp_init();

	#if (USE_INTER_RC_32K)
	//rc calib period 30s
	lib_rc_32k_calib_period(RC32K_CALIB_TIME);
	#endif

	//radio auto calib period 60s
	lib_radio_calib_period(RADIO_CALIB_TIME);

	// setup SM: Display only
	sm_init();
	// LE Legacy Pairing, Just Works
	sm_set_io_capabilities(IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
	sm_set_authentication_requirements(SM_AUTHREQ_BONDING);
	sm_set_request_security(1);

    /* ???????????? */
    sm_pairing_test(1);
    /* ?????0x00??????? 0x08?????? */
    sm_test_set_pairing_failure(0x08);//??????
    
	//register att server ATT_PROPERTY_WRITE?????ATT_PROPERTY_WRITE_WITHOUT_RESPONSE??????????????ATT_PROPERTY_DYNAMIC???????????????
	att_db_util_init();
	
	uint8_t apperace[]={0xC2,0x00};
	uint8_t device_name[] = "ws8300_bike";
	att_db_util_add_service_uuid16(ORG_BLUETOOTH_SERVICE_GENERIC_ACCESS);
	att_db_util_add_characteristic_uuid16(ORG_BLUETOOTH_CHARACTERISTIC_GAP_DEVICE_NAME,ATT_PROPERTY_READ,ATT_SECURITY_NONE,ATT_SECURITY_NONE,(uint8_t *)device_name,sizeof(device_name));
	att_db_util_add_characteristic_uuid16(ORG_BLUETOOTH_CHARACTERISTIC_GAP_APPEARANCE,ATT_PROPERTY_READ,ATT_SECURITY_NONE,ATT_SECURITY_NONE,(uint8_t *)apperace,sizeof(apperace));

    uint8_t service_change_val[4] = {0x00, 0x00, 0xff, 0xff};
	att_db_util_add_service_uuid16(ORG_BLUETOOTH_SERVICE_GENERIC_ATTRIBUTE);
	att_db_util_add_characteristic_uuid16(ORG_BLUETOOTH_CHARACTERISTIC_GATT_SERVICE_CHANGED,ATT_PROPERTY_INDICATE,ATT_SECURITY_NONE,ATT_SECURITY_NONE,(uint8_t *)service_change_val,4);
        
	ble_hid_service_register();	
	
	att_db_util_add_service_uuid16(IOT_UUID_SERVICE);
	iot_server_rx_handle  = att_db_util_add_characteristic_uuid16(IOT_UUID_RX,ATT_PROPERTY_WRITE_WITHOUT_RESPONSE  | ATT_PROPERTY_DYNAMIC,ATT_SECURITY_NONE,ATT_SECURITY_NONE,NULL,0);
	iot_server_tx_handle  = att_db_util_add_characteristic_uuid16(IOT_UUID_TX, ATT_PROPERTY_NOTIFY,ATT_SECURITY_NONE,ATT_SECURITY_NONE,NULL,0);
	att_server_init(att_db_util_get_address(), att_read_callback, att_write_callback);    

	att_mtu_size(23);
	lib_get_bd_addr(otp_addr);
//	gap_set_ble_bd_addr(device_ble_addr);
	gap_set_ble_bd_addr(otp_addr);//??chip ID??mac???
#if OTA_SURPORT
	ota_app_init();
	set_ota_addr(otp_addr);	
#endif
	bd_addr_t null_addr;
	memset(null_addr, 0, 6);
	gap_advertisements_set_params(DEFAULT_ADVERTISING_INTERVAL, DEFAULT_ADVERTISING_INTERVAL, DEFAULT_ADVERTISING_TYPE, 0, null_addr, 0x07, 0x00);
	gap_advertisements_set_data(sizeof(adv_data), (uint8_t*) adv_data);
	gap_advertisements_enable(1);
    
    lib_start_timer(&adv_check_timer,adv_check_timer_handler,10);
    
#ifdef SCAN_ADV
	gap_set_scan_parameters(0x00,0x35,0x25,0x00,0x00);
	gap_start_scan();
#endif
	lib_bluetooth_run();
    
#ifdef SCAN_ADV
    /* ??????????????????????????*/
    lib_scan_prority_set(0, 0, 1);
#endif

	user_ring_buffer_init(&g_ring_buffer,stroge,200);//?????200??????λ???
	
//	notify_event_handler(NULL);//??????notify????_??????????????????????
//	nfc_card_init();//nfc?????
}


/* ??????? */
void ble_disconnect(void)
{
    gap_disconnect(connection_handle);
}








