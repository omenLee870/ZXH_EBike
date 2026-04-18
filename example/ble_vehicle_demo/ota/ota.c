#include "ota.h"
#include "lib_api.h"
#include "app_config.h"
#define APP_VERSION         1
#define STACK_VERSION       1

typedef struct {
    unsigned short company_id;
    unsigned short product_id;
    char password[12];
} ws_ota_pid_key_t;

//Keyֵ�Զ���
#define OTA_VENDOR_ID       0x81AB
#define OTA_PRODUCT_ID      0x0001

#define OTA_PRODUCT_PASS    "rs#58da+s1@0"

ws_ota_pid_key_t Key = {OTA_VENDOR_ID, OTA_PRODUCT_ID, OTA_PRODUCT_PASS};
uint8_t ota_addr[6] = {0xFC,0xFC,0xDC,0x81,0x00,0xE4};
uint16_t crc_init = 0x0000; //CRC��ʼֵ

typedef struct{
    uint16_t company_id;
    uint16_t product_id;
    uint8_t password[12];
    uint8_t app_ble_addr[6];
    uint8_t addr_type;
    uint8_t freq_offset;
    uint8_t xtal_select;
	uint8_t tx_power;
    uint16_t work_mode_flag; 
	uint8_t ota_indicate_pin;//����OTA ָʾpin �� 0 - 32
	uint8_t ota_toggle_type; //���� ָʾIO��LED�� ����  1:�ߵ�ƽ��   0���͵�ƽ��
	uint16_t ota_io_toggle_time;//����IO ��ת��Ƶ�� ������ʱ��˸Ƶ��
    uint16_t crc;    
}ota_flag_t;
#define ota_flag_size 32
ota_flag_t * ota_flag = (ota_flag_t *)0x20004FDE;

//typedef struct{
//    uint16_t company_id;
//    uint16_t product_id;
//    uint8_t password[12];
//    uint8_t app_ble_addr[6];
//    uint8_t addr_type;
//    uint16_t work_mode_flag;    
//    uint16_t crc;    
//}ota_flag_t;
//#define ota_flag_size 25
//ota_flag_t * ota_flag = (ota_flag_t *)0x20003FE2;

void set_ota_addr(uint8_t *addr)
{
	uint8_t i;
	for(i=0;i<6;i++)
	{
		ota_addr [i] = addr[i]  + 1;
	}
}
//����Keyֵcrc
uint16_t ota_key_crc_calc(uint16_t crc)
{
    return calc_crc16((unsigned char*)&Key,sizeof(Key),crc);
}

//crcУ��
uint16_t ota_msg_crc_calc(uint8_t *msg,uint16_t len)
{
    uint16_t crc=0x0000;
    
    crc = calc_crc16(msg,len,crc);
    crc = ota_key_crc_calc(crc);
    
    return crc;
}

void system_reset()//ȫ������λ
{
	
    ota_flag->company_id =  OTA_VENDOR_ID;
    ota_flag->product_id =  OTA_PRODUCT_ID;
    memcpy(ota_flag->password,OTA_PRODUCT_PASS,sizeof(ota_flag->password));    
    
	extern uint8_t device_ble_addr[];
	memcpy(ota_flag->app_ble_addr,(void *)&ota_addr,sizeof(ota_flag->app_ble_addr));
   ota_flag->addr_type = 0; //0:��̬��ַ  1:random ��ַ ���ݳ���ʵ�ʵ�ַ���;���
	#if(USE_EXT_XTAL_32M)
	ota_flag->freq_offset = 0xAD;//����Ƶƫ
    ota_flag->xtal_select = 0x20;//0x10:16M   0x20:32M   
	#else
    ota_flag->freq_offset = 0x78;//����Ƶƫ
    ota_flag->xtal_select = 0x10;//0x10:16M   0x20:32M    
	#endif
	ota_flag->tx_power = TX_POWER_9 + 1; //power 0dbm
	
	ota_flag->ota_indicate_pin = 2 ; //PA2 : ��Χ 0 - 31
	ota_flag->ota_toggle_type = 0 ; //�͵�ƽ��
	ota_flag->ota_io_toggle_time = 100 ;//100m��תһ��
	
    ota_flag->crc = calc_crc16((uint8_t *)ota_flag,ota_flag_size,crc_init);	
    CRM->SOFT_RST |= CRM_SOFT_RST_GLB;
}

extern void ota_bt_notify(uint8_t *data,uint16_t len) ;
void ota_event_respones(uint8_t *msg,uint16_t len)
{
    uint16_t crc = ota_msg_crc_calc(msg,len);
    
    ((uint8_t *)msg)[len] = crc &0xFF;
    ((uint8_t *)msg)[len+1] = crc >> 8;
    	
    ota_bt_notify(msg,len+2);     
}

uint16_t ota_check_msg(uint8_t *msg,uint16_t len)
{    
        if(len < 4 || len > 20)
             return FALSE;
        
        uint16_t msg_crc = (uint16_t)(msg[len - 2]) + (uint16_t)(msg[len - 1] << 8);
        uint16_t calc_crc = ota_msg_crc_calc(msg ,len-2);
        
         
        return (msg_crc == calc_crc);
}

uint16_t ota_get_version(uint16_t img_type,uint16_t *ver,uint32_t size )
{
    if(img_type == APP_IMG)
    {
        *ver = APP_VERSION;

        return TRUE;
    }
    else if(img_type == STACK_IMG)
    {
        *ver = STACK_VERSION;
        return TRUE;
    } 
	return FALSE;
}

//���ù���ģʽ
uint16_t  ota_mode_set(uint16_t mode)
{ 
    if(mode == NORMAL_MODE)
    {     
        ota_flag->work_mode_flag  = NORMAL_MODE;  
		        
        return WS_OTA_ERR_SUCCESS;
    } 
    if(mode == OTA_MODE)
    {
        ota_flag->work_mode_flag  = OTA_MODE;   
		        
        return WS_OTA_ERR_SUCCESS;
    }
	return WS_OTA_ERR_NO_SUPPORT;
}

uint16_t ota_mode_get()
{
	return ota_flag->work_mode_flag ; 
}


uint8_t ota_cmd_ctl(uint8_t *buf,uint16_t len)
{      
    uint16_t msg[10] ;
//	uint32_t i;
    if(ota_check_msg(buf,len))
    {
        memcpy(msg,buf,MIN(sizeof(msg),len));
        len -= 2;
        
        switch(msg[0])
        {
            case WS_OTA_OP_VER_REQ://��ȡ�汾��
			{
				if(len == WS_OTA_VER_REQ_MSG_SIZE)
                {
                	dbg_printk("get version msg %x\n",msg[0]);
                    ws_ota_ver_req_t *ota_ver_req = (void *)msg;
                    ws_ota_ver_rsp_t ota_ver_rsp ;
					
                    ota_ver_rsp.opcode 		= WS_OTA_OP_VER_RSP;
                    ota_ver_rsp.img_type 	= ota_ver_req->img_type;
                                      
                    if( ota_get_version(ota_ver_req->img_type,&ota_ver_rsp.fw_ver,NULL) == TRUE )
                        ota_ver_rsp.state = WS_OTA_ERR_SUCCESS;
                    else
                        ota_ver_rsp.state = WS_OTA_ERR_IMG_TYPE_NO_EXIST;  
                    
					ota_event_respones((uint8_t *)&ota_ver_rsp,WS_OTA_VER_RSP_MSG_SIZE);  
                }				                           
			}            
            break; 
            		
			case WS_OTA_OP_MODE_SET_REQ:
			{
				if(len == WS_OTA_MODE_SET_REQ_MSG_SIZE) 
				{
					
	                ws_ota_mode_set_req_t *ota_mode_set_req = (void *)msg;
dbg_printk("set mode msg %x mode %d\n",msg[0],ota_mode_set_req->mode);
	                ws_ota_mode_set_rsp_t ota_mode_set_rsp ; 

					ota_mode_set_rsp.err = ota_mode_set(ota_mode_set_req->mode);					
					ota_mode_set_rsp.opcode = WS_OTA_OP_MODE_SET_RSP ;

					ota_event_respones((uint8_t *)&ota_mode_set_rsp,WS_OTA_MODE_SET_RSP_MSG_SIZE); 
					if(ota_mode_set_rsp.err == WS_OTA_ERR_SUCCESS || ota_mode_set_rsp.err == WS_OTA_ERR_NO_SUPPORT)
					{      
						static stack_timer_t reset_timer;
						lib_start_timer(&reset_timer,(void (*)(stack_timer_t *))system_reset,1500);						
					}										
				}
			}
            break;
            
            case WS_OTA_OP_MODE_GET_REQ:
            {
				if(len == WS_OTA_MODE_GET_REQ_MSG_SIZE) 
				{	                
                	dbg_printk("get mode msg %x\n",msg[0]);
	                ws_ota_mode_get_rsp_t ota_mode_get_rsp ; 
                    ota_flag->work_mode_flag = 0;//����ģʽ
                    ota_mode_get_rsp.mode = ota_mode_get();
                    ota_mode_get_rsp.opcode = WS_OTA_OP_MODE_GET_RSP;
                    ota_event_respones((uint8_t *)&ota_mode_get_rsp,WS_OTA_MODE_GET_RSP_MSG_SIZE);                     
                }                                    
            }
            break;        
        }                    
    }
    return 0;
}

