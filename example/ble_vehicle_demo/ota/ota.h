#ifndef __OTA_H__
#define __OTA_H__

#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "cpu.h"

#define WS_OTA_OP_VER_REQ           0xDF01    /* OTA版本获取请求 */
#define WS_OTA_OP_VER_RSP           0xDF02    /* OTA版本获取响应 */
#define WS_OTA_OP_MODE_SET_REQ      0xD709    /* OTA重启到指定工作模式（正常模式，升级模式，裂变模式，强制裂变模式）请求 */
#define WS_OTA_OP_MODE_SET_RSP      0xD70A    /* OTA重启到指定工作模式响应 */
#define WS_OTA_OP_MODE_GET_REQ      0xD50B    /* OTA获取当前工作模式 */
#define WS_OTA_OP_MODE_GET_RSP      0xD50C    /* OTA获取当前工作模式响应 */

#define WS_OTA_ERR_SUCCESS              0     /* 成功 */
#define WS_OTA_ERR_IMG_TYPE_NO_EXIST    1     /* 镜像类型不存在 */
#define WS_OTA_ERR_NO_SUPPORT           12    /* 不支持的操作（不在空中返回）*/ 

#ifndef MIN
#define MIN(a,b) (((uint32_t)(a) > (uint32_t)(b)) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a,b) (((uint32_t)(a) > (uint32_t)(b)) ? (a) : (b))
#endif

//工作模式
typedef enum  {
    NORMAL_MODE = 0,//正常模式
    OTA_MODE,//升级模式
}work_mode_t;

//固件类型
typedef enum {
     APP_IMG = 0 ,//app固件
     STACK_IMG ,//stack固件
}img_type_t;

typedef struct {
    uint16_t opcode;            /* 操作码(WS_OTA_OP_VER_REQ) */
    uint16_t img_type;           /* 固件类型 */
    uint16_t msg_crc;            /* 消息CRC */
} ws_ota_ver_req_t; 
#define WS_OTA_VER_REQ_MSG_SIZE 4

typedef struct {    
    uint16_t opcode;           /* 操作码(WS_OTA_OP_VER_RSP) */
    uint16_t state;              /* 状态码 */
    uint16_t img_type;          /* 固件类型 */
    uint16_t fw_ver;            /* 固件版本 */
    uint16_t msg_crc;           /* 消息CRC */
} ws_ota_ver_rsp_t; 
#define WS_OTA_VER_RSP_MSG_SIZE 8

typedef struct {
    uint16_t opcode;            /* 操作码(WS_OTA_OP_MODE_SET_REQ) */
    uint16_t mode;              /* 目标模式 */
    uint16_t msg_crc;           /* 消息CRC */
} ws_ota_mode_set_req_t;
#define WS_OTA_MODE_SET_REQ_MSG_SIZE    4

typedef struct {
    uint16_t opcode;            /* 操作码(WS_OTA_OP_MODE_SET_RSP) */
    uint16_t err;               /* 错误码 */
    uint16_t msg_crc;           /* 消息CRC */
} ws_ota_mode_set_rsp_t;
#define WS_OTA_MODE_SET_RSP_MSG_SIZE    4

typedef struct {
    uint16_t opcode;            /* 操作码(WS_OTA_OP_MODE_GET_REQ) */
    uint16_t msg_crc;           /* 消息CRC */
} ws_ota_mode_get_req_t;
#define WS_OTA_MODE_GET_REQ_MSG_SIZE    2

typedef struct {
    uint16_t opcode;            /* 操作码(WS_OTA_OP_MODE_GET_RSP) */
    uint16_t mode;             /* 当前模式 */
    uint16_t msg_crc;           /* 消息CRC */
} ws_ota_mode_get_rsp_t;
#define WS_OTA_MODE_GET_RSP_MSG_SIZE    4

uint8_t ota_cmd_ctl(uint8_t *buf,uint16_t len);

static inline short crc16(unsigned char ch, unsigned short crc)
{
	unsigned short crch;
	unsigned short sd = ch;
	
	crch = crc >> 8;
	crch ^= sd;
	crch >>= 4;
	crch *= 0x1021;
	crc <<= 4;
	crc ^= crch;
	crch = crc >> 12;
	crch ^= sd;
	crch &= 0x0F;
	crch *= 0x1021;
	crc <<= 4;
	crc ^= crch;

	return crc;
}

static inline unsigned short calc_crc16(unsigned char *data, unsigned int data_len, unsigned short crc)
{
	unsigned int i = 0;
	
	for (i = 0; i < data_len; i++)
	{
		crc = crc16(data[i], crc);
	}
	
	return crc;
}

extern uint16_t ota_check_msg(uint8_t *msg,uint16_t len);
extern uint8_t ota_cmd_ctl(uint8_t *buf,uint16_t len);
extern void ota_event_respones(uint8_t *msg,uint16_t len);
extern uint16_t ota_get_version(uint16_t img_type,uint16_t *ver,uint32_t size );
extern uint16_t ota_key_crc_calc(uint16_t crc);
extern uint16_t  ota_mode_set(uint16_t mode);
extern uint16_t ota_msg_crc_calc(uint8_t *msg,uint16_t len);
extern void set_ota_addr(uint8_t *addr);




#endif
