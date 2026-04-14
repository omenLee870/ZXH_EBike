#ifndef _VEHICLE_CARD_MANAGE_H
#define _VEHICLE_CARD_MANAGE_H

#include "stdint.h"
#include "lib_api.h"

#define CARD_TOTAL_NUM   5//5张ic卡 max 400张ic卡
#define CARD_OVER_TIME   (30*1000)//30s
#define CHECK_CARD_TIME  (1*1000)

#define CARD_STATE_SWITH_TIME 200
//#define CHECK_CARD_TIME  (1*150)
typedef enum
{
	CHECK_CARD_MODE,
	ADD_CARD_MODE,
	DEL_CARD_MODE,
}card_mode_e;
typedef enum
{
	INVAILD_CARD,
	VAILD_CARD,
}card_type_e;
typedef enum
{
	CARD_PUT_DOWN,//卡片放下
	CARD_PUT_UP,	//卡片抬起
}card_operate_state_e;
#pragma pack(1)
typedef struct
{
	uint8_t card_num;
	uint8_t card_id[CARD_TOTAL_NUM][10];
}card_param_t;
#pragma pack()

/*变量外部声明*/
extern card_mode_e g_card_mode_e;
extern card_operate_state_e  g_card_operate_state_e;

/*函数声明*/
extern void card_mode_operation_time(card_mode_e mode,uint16_t time);
extern void card_operation_time(uint16_t time);
extern void set_card_mode(card_mode_e mode);
extern void card_manage(void);
extern void nfc_process(void);
extern void card_put_down(void);
/**************************************card flash**************************************************/
extern void read_card_flash(void);
extern void write_card_flash(void);


#endif

