#ifndef _VEHICLE_NFC_APP_H
#define _VEHICLE_NFC_APP_H

#include "stdint.h"



/*变量声明*/
extern uint8_t PassWd[6];
extern uint8_t PassWdA[6];

/*函数声明*/
extern uint8_t compare_card_secret_key(uint8_t* secret_key,uint8_t secret_key_len);
extern uint8_t get_card_sn(uint8_t* mac_addr,uint8_t addr_len);
extern uint8_t compare_card_sn(uint8_t* mac_addr,uint8_t addr_len);
extern uint8_t write_card(uint8_t* secret_key,uint8_t* secret_key_new,uint8_t secret_key_len,uint8_t* mac_addr,uint8_t addr_len);
extern void reset_card(void);









#endif
