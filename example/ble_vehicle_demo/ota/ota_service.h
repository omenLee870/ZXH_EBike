/******************************************************************************

                  版权所有 (C), 2019-2039, 深圳前海维晟智能技术有限公司

 ******************************************************************************
  文 件 名   : ota_service.h
  版 本 号   : 初始版本
  作    者   : QingYang
  生成日期   : 2021年12月23日 星期四
  最近修改   :
  功能描述   : ota_service.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2021年12月23日 星期四
    作    者   : QingYang
    修改内容   : 创建文件

******************************************************************************/

#ifndef __OTA_SERVICE_H__
#define __OTA_SERVICE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern uint8_t get_ota_requir_send_once(void);
extern void ota_app_init(void);
extern int ota_att_write_callback(uint16_t con_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size);
extern void ota_ble_send_data(void *arg);
extern void ota_bt_notify(uint8_t *data,uint16_t len);
extern void ota_send_data(void);
extern void updata_ota_mtu(uint16_t mtu);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __OTA_SERVICE_H__ */

