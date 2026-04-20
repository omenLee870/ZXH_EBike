/**
 * @file app.h
 * @brief 业务应用层全局头文件
 * @details 仅包含业务层相关的头文件，隔离底层驱动，实现模块解耦
 */

#ifndef _APP_H_
#define _APP_H_

#include <stdint.h>
#include <string.h>

/* --- 业务层模块集中包含 --- */
#include "app_task.h"
#include "app_debug.h"
#include "app_lcd.h"
#include "app_key.h"
#include "app_nfc_process.h"
#include "app_ble_data_process.h"
#include "app_kv_flash.h"


/* --- 业务频率全局宏定义 --- */


/* --- 项目宏 --- */
#define ZXH_EBike 1

#endif /* _APP_H_ */
