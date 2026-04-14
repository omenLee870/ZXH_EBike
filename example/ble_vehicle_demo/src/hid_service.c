#include "bsp.h"



#if 0
 const uint8_t hid_descriptor_mouse_boot_mode[] = {
    0x05, 0x01,     /* Usage Page (Generic Desktop), */
    0x09, 0x06,     /* Usage (Keyboard), */
    0xA1, 0x01,     /* Collection (Application), */
    0x85, 0x01,    // Report_ID (1),
    0x05, 0x07,     /* Usage Page (Key Codes); */
    0x19, 0xE0,     /* Usage Minimum (224), */
    0x29, 0xE7,     /* Usage Maximum (231), */
    0x15, 0x00,     /* Logical Minimum (0), */
    0x25, 0x01,     /* Logical Maximum (1), */
    0x75, 0x01,     /* Report Size (1), */
    0x95, 0x08,     /* Report Count (8), */
    0x81, 0x02,     /* Input (Data, Variable, Absolute), ;Modifier byte */

    0x95, 0x01,     /* Report Count (1), */
    0x75, 0x08,     /* Report Size (8), */
    0x81, 0x01,     /* Input (Constant), ;Reserved byte */

    0x95, 0x05,     /* Report Count (5), */
    0x75, 0x01,     /* Report Size (1), */
    0x05, 0x08,     /* Usage Page (Page# for LEDs), */
    0x19, 0x01,     /* Usage Minimum (1), */
    0x29, 0x05,     /* Usage Maximum (5), */
    0x91, 0x02,     /* Output (Data, Variable, Absolute), ;LED report */
    
    0x95, 0x01,     /* Report Count (1), */
    0x75, 0x03,     /* Report Size (3), */
    0x91, 0x01,     /* Output (Constant), ;LED report padding */

    0x95, 0x06,     /* Report Count (6), */
    0x75, 0x08,     /* Report Size (8), */
    0x15, 0x00,     /* Logical Minimum (0), */
    0x25, 0x65,     /* Logical Maximum(101), */
    0x05, 0x07,     /* Usage Page (Key Codes), */
    0x19, 0x00,     /* Usage Minimum (0), */
    0x29, 0x65,     /* Usage Maximum (101), */
    0x81, 0x00,     /* Input (Data, Array), ;Key arrays (6 bytes) */
    0xC0,       /* End Collection */

    0x05, 0x01, // Usage Page (Generic Desktop),
    0x09, 0x02, // Usage (Mouse),
    0xA1, 0x01, // Collection (Application),
    0x09, 0x01, // Usage (Pointer),
    0xA1, 0x00, // Collection (Physical),
    0x85, 0x02,   // Report_ID (2),
   	
	0x05, 0x09, // Usage Page (Buttons),
    0x19, 0x01, // Usage Minimum (01),
    0x29, 0x03, // Usage Maximun (03),
    0x15, 0x00, // Logical Minimum (0),
    0x25, 0x01, // Logical Maximum (1),
    0x95, 0x03, // Report Count (3),
    0x75, 0x01, // Report Size (1),
    0x81, 0x02, // Input (Data, Variable, Absolute),    ;3 button bits

    0x95, 0x01, // Report Count (1),
    0x75, 0x05, // Report Size (5),
    0x81, 0x01, // Input (Constant),            ;5 bit padding
    0x05, 0x01, // Usage Page (Generic Desktop),
    0x09, 0x30, // Usage (X),
    0x09, 0x31, // Usage (Y),
	0x09, 0x38, // Usage (Wheel),
    0x15, 0x81, // Logical Minimum (-127),
    0x25, 0x7F, // Logical Maximum (127),
    0x75, 0x08, // Report Size (8),
    0x95, 0x03, // Report Count (3),
    0x81, 0x06, // Input (Data, Variable, Relative),    ;3 position bytes (X & Y & Wheel)
    0xC0,       // End Collection,
    0xC0        // End Collection
};

#else
const uint8_t hid_descriptor_mouse_boot_mode[] =
{
    //************mouse buttons+sensor+wheel**********************//
       0x05, 0x01,                                        // Usage Page (Generic Desktop)
       0x09, 0x02,                                        // Usage (Mouse)
       0xA1, 0x01,                                        // Collection (Application)
       0x85, 0x01,
       0x09, 0x01,                                        //   Usage (Pointer)
       0xA1, 0x00,                                        //   Collection (Physical)
	     /*--------buttons---------*/
       0x05, 0x09,                                        //     Usage Page (Buttons)
       0x19, 0x01,                                        //     Usage Minimum (1 buttons)
       0x29, 0x06,                                        //     Usage Maximum (6 buttons)
       0x15, 0x00,                                        //     Logic Minimum (0)
       0x25, 0x01,                                        //     Logic Maximum (1)
			
       0x75, 0x01,                                        //     Report Size (1bit)
       0x95, 0x06,                                        //     Report Count (6)
       0x81, 0x02,                                        //     Input (Data, Variable, Absolute)     - Button Byte
 
       0x06, 0x00, 0xFF,                                  //     Usage Page (Reserved)
       0x75, 0x01,                                        //     Report Size (1bit)
       0x95, 0x02,                                        //     Report Count (2)
       0x81, 0x03,                                        //     Input (Constant, Variable, Absolute) - Button Padding
	     /*----------sensor boot--------*/
       0x09, 0x40,                                        //     Usage (X, Y)
       0x75, 0x08,                                        //     Report Size (8bits)
       0x95, 0x02,                                        //     Report Count (2)
       0x15, 0x81,                                        //     Logical Minimum (-127)
       0x25, 0x7F,                                        //     Logical Maximum (+127)
       0x81, 0x02,                                        //     Input (Data, Variable, Absolute)     - X, Y Bytes
       /*---------scroll wheel+sensor--------*/
       0x05, 0x01,                                        //     Usage Page (Generic Desktop)
       0x09, 0x38,                                        //     Usage (Scroll Wheel)
       0x15, 0x81,                                        //     Logical Minimum (-127)
       0x25, 0x7F,                                        //     Logical Maximum (+127)
       0x75, 0x08,                                        //     Report Size (8)
       0x95, 0x01,                                        //     Report Count (1)
       0x81, 0x06,                                        //     Input (Data, Variable, Relative)     - Z Bytes
       0x09, 0x30,                                        //     Usage (X)
       0x09, 0x31,                                        //     Usage (Y)
       0x16, 0x00, 0x80,                                  //     Logical Minimum (-32767)
       0x26, 0xFF, 0x7F,                                  //     Logical Maximum (+32767)
       0x75, 0x10,                                        //     Report Size (16)
       0x95, 0x02,                                        //     Report Count (2)
       0x81, 0x06,                                        //     Input (Data, Variable, Relative)     - X, Y Words
       0xC0,                                              //   End Collection (Physical)
       0xC0,                                              //   End Collection (Application)
};
#endif
void ble_initilazed(char *device_name,uint8_t dev_name_len,char *apperace,uint8_t apperace_len)
{
	att_db_util_init();
	att_db_util_add_service_uuid16(ORG_BLUETOOTH_SERVICE_GENERIC_ACCESS);
	att_db_util_add_characteristic_uuid16(0x2A00,ATT_PROPERTY_READ,ATT_SECURITY_NONE,ATT_SECURITY_NONE,device_name,sizeof(dev_name_len));
	att_db_util_add_characteristic_uuid16(0x2A01,ATT_PROPERTY_READ,ATT_SECURITY_NONE,ATT_SECURITY_NONE,apperace,sizeof(apperace_len));
	
}


void ble_hid_service_register(void)
{
//【1】注册pin信息
	att_db_util_add_service_uuid16(ORG_BLUETOOTH_SERVICE_DEVICE_INFORMATION);
	uint8_t pinid_string[7]={0x02,0xAC,0x05,0x52,0x02,0xDB,0x07};
	att_db_util_add_characteristic_uuid16(ORG_BLUETOOTH_CHARACTERISTIC_PNP_ID,ATT_PROPERTY_READ,ATT_SECURITY_ENCRYPTED,ATT_SECURITY_NONE,pinid_string,sizeof(pinid_string));

//【2】注册人机接口设备
	att_db_util_add_service_uuid16(ORG_BLUETOOTH_SERVICE_HUMAN_INTERFACE_DEVICE);
	uint8_t val_hal[2]={0,0};
	uint8_t mode = 1;
	
//【3】注册协议、报告、map、
	att_db_util_add_characteristic_uuid16(ORG_BLUETOOTH_CHARACTERISTIC_PROTOCOL_MODE,ATT_PROPERTY_READ | ATT_PROPERTY_DYNAMIC | ATT_PROPERTY_WRITE_WITHOUT_RESPONSE,ATT_SECURITY_ENCRYPTED,ATT_SECURITY_NONE,&mode,1);

	att_db_util_add_characteristic_uuid16(ORG_BLUETOOTH_CHARACTERISTIC_REPORT,ATT_PROPERTY_READ | ATT_PROPERTY_NOTIFY | ATT_PROPERTY_WRITE | ATT_PROPERTY_DYNAMIC,ATT_SECURITY_ENCRYPTED,ATT_SECURITY_NONE,val_hal,2);
	uint8_t val1[2]={1,1};
	att_db_util_add_attribute_uuid16(ORG_BLUETOOTH_DESCRIPTOR_REPORT_REFERENCE,ATT_PROPERTY_READ,val1,2);
	
	att_db_util_add_characteristic_uuid16(ORG_BLUETOOTH_CHARACTERISTIC_REPORT,ATT_PROPERTY_READ | ATT_PROPERTY_NOTIFY | ATT_PROPERTY_WRITE | ATT_PROPERTY_DYNAMIC,ATT_SECURITY_ENCRYPTED,ATT_SECURITY_NONE,val_hal,2);
	uint8_t val2[2]={2,1};
	att_db_util_add_attribute_uuid16(ORG_BLUETOOTH_DESCRIPTOR_REPORT_REFERENCE,ATT_PROPERTY_READ,val2,2);
	
	att_db_util_add_characteristic_uuid16(ORG_BLUETOOTH_CHARACTERISTIC_REPORT,ATT_PROPERTY_READ | ATT_PROPERTY_NOTIFY | ATT_PROPERTY_WRITE | ATT_PROPERTY_DYNAMIC,ATT_SECURITY_ENCRYPTED,ATT_SECURITY_NONE,val_hal,2);
	uint8_t val3[2]={1,2};
	att_db_util_add_attribute_uuid16(ORG_BLUETOOTH_DESCRIPTOR_REPORT_REFERENCE,ATT_PROPERTY_READ,val3,2);

	att_db_util_add_characteristic_uuid16(ORG_BLUETOOTH_CHARACTERISTIC_REPORT_MAP,ATT_PROPERTY_READ | ATT_PROPERTY_DYNAMIC,ATT_SECURITY_ENCRYPTED,ATT_SECURITY_NONE,val_hal,2);
	att_db_util_add_characteristic_uuid16(ORG_BLUETOOTH_CHARACTERISTIC_BOOT_KEYBOARD_INPUT_REPORT,ATT_PROPERTY_READ |  ATT_PROPERTY_NOTIFY | ATT_PROPERTY_WRITE| ATT_PROPERTY_DYNAMIC,ATT_SECURITY_ENCRYPTED,ATT_SECURITY_NONE,val_hal,2);
	att_db_util_add_characteristic_uuid16(ORG_BLUETOOTH_CHARACTERISTIC_BOOT_KEYBOARD_OUTPUT_REPORT,ATT_PROPERTY_READ |  ATT_PROPERTY_WRITE_WITHOUT_RESPONSE | ATT_PROPERTY_WRITE| ATT_PROPERTY_DYNAMIC,ATT_SECURITY_ENCRYPTED,ATT_SECURITY_NONE,val_hal,2);
	att_db_util_add_characteristic_uuid16(ORG_BLUETOOTH_CHARACTERISTIC_BOOT_MOUSE_INPUT_REPORT,ATT_PROPERTY_READ |  ATT_PROPERTY_NOTIFY | ATT_PROPERTY_WRITE| ATT_PROPERTY_DYNAMIC,ATT_SECURITY_ENCRYPTED,ATT_SECURITY_NONE,val_hal,2);

	uint8_t hid_information[4]={0x01,0x01,0x00,0x02};
	att_db_util_add_characteristic_uuid16(ORG_BLUETOOTH_CHARACTERISTIC_HID_INFORMATION,ATT_PROPERTY_READ,ATT_SECURITY_ENCRYPTED,ATT_SECURITY_NONE,hid_information,4);
	att_db_util_add_characteristic_uuid16(ORG_BLUETOOTH_CHARACTERISTIC_HID_CONTROL_POINT,ATT_PROPERTY_WRITE_WITHOUT_RESPONSE| ATT_PROPERTY_DYNAMIC,ATT_SECURITY_NONE,ATT_SECURITY_NONE,val_hal,2);

	att_server_init(att_db_util_get_address(), NULL, NULL); 
  // setup HID Device service
  hids_device_init(0, hid_descriptor_mouse_boot_mode, sizeof(hid_descriptor_mouse_boot_mode));
	hids_device_register_packet_handler(packet_handler);

}






























