#ifndef _APP_CONFIG_H
#define _APP_CONFIG_H

#include "gap.h"
#include "cpu_sysctrl.h"
#include "user_uart.h"

/* select xtal or rc clock */
#define USE_EXT_XTAL_32M				1	//use external 32m xtal
#define USE_EXT_XTAL_16M				0	//use external 16m xtal
#define USE_EXT_XTAL_32K				0	//use external 32k xtal
#define USE_INTER_RC_32K				1	//use internal 32k rc

/* define xtal frequency deviation */
//#define XTAL_FREQ_DEV					  0x78 //0xAD	
#define XTAL_FREQ_DEV					    0x98	
/* define rc32k default param */
#define RC32K_DEFAULT_PPM				  500
#define RC32K_DEFAULT_JITTER			20
#define RC32K_DEFAULT_DRIFFT			1000
#define RC32K_CALIB_TIME				  30000	//Time = N ms

/* define radion default param */
#define RADIO_CALIB_TIME				60000	//Time = N ms


/* 1: enable sdk sleep 0: disable sdk sleep */
#define SDK_ENABLE_SLEEP		0

/* ���󰴼� */
#define MATRIX_BUTTON_ENABLE			0
/* uart config */
#define UART_ENABLE					  1
#define LOG_ENABLE					  1
#define DEFAULT_UART_BAUD			1000000
#define MAX_UART_TX_BUF_SIZE  200		//512
#define MAX_UART_RX_BUF_SIZE	200		//512
#define OTA_SURPORT				    1

#if (LOG_ENABLE)
#define dbg_printk(fmt, ...) 				user_print(fmt, ##__VA_ARGS__)
#define dbg_block_printk(fmt, ...) 		user_block_print(fmt, ##__VA_ARGS__)
#define dbg_block_printk_hex(fmt, ...) 	user_block_printk_hex(fmt, ##__VA_ARGS__)
#else
#define dbg_printk(fmt, ...)
#define dbg_block_printk(fmt, ...)
#define dbg_block_printk_hex(fmt, ...) 
#endif


/* wdt config */
#define WDT_ENABLE				1
#define WDT_TIMEOUT				10	//Time = N s


/* ble default param */
#define DEFAULT_ADVERTISING_INTERVAL			  160	//Time = N * 0.625 ms
#define DEFAULT_ADVERTISING_TYPE				    GAP_ADTYPE_ADV_IND
#define DEFAULT_UPDATE_MIN_CONN_INTERVAL		12	// Time = N * 1.25 ms
#define DEFAULT_UPDATE_MAX_CONN_INTERVAL		12	// Time = N * 1.25 ms
#define DEFAULT_UPDATE_SLAVE_LATENCY			  0
#define DEFAULT_UPDATE_CONN_TIMEOUT				  1000	//Time = N * 10 ms
#define DEFAULT_UPDATE_PAUSE_TIME				    2000	//Time = N ms
#define DEFAULT_ENABLE_UPDATE_REQUEST			  0


/* br default param */
#define DEFAULT_INQUIRY_SCAN_INTERVAL			0x0258	//Time = N * 0.625 ms
#define DEFAULT_INQUIRY_SCAN_WINDOW				0x12	//Time = N * 0.625 ms
#define DEFAULT_PAGE_SCAN_INTERVAL				0x0258	//Time = N * 0.625 ms
#define DEFAULT_PAGE_SCAN_WINDOW				  0x12	//Time = N * 0.625 ms
#define DEFAULT_SNIFF_INTERVAL					  0x0320	//Time = N * 0.625 ms
#define DEFAULT_SNIFF_ATTEMPT					    0x04	//Time = N * 1.25 ms
#define DEFAULT_SNIFF_TIMEOUT					    0x04	//Time = N * 1.25 ms
#define DEFAULT_SNIFF_PAUSE_TIME				  2000	//Time = N ms
#define DEFAULT_ENABLE_SNIFF_REQUEST			1

#endif

