#ifndef _LIB_API_H
#define _LIB_API_H

#include <stdint.h>
#include "bluetooth.h"

typedef enum 
{
	TX_POWER_MINUX_12,
	TX_POWER_MINUX_8,
	TX_POWER_MINUX_6,
	TX_POWER_MINUX_3,
	TX_POWER_0,
	TX_POWER_2,
	TX_POWER_4,
	TX_POWER_7,
	TX_POWER_9,
	TX_POWER_10,
}tx_power_level;

typedef enum 
{
	RSSI_RAW_DATA_ALGORITHM = 0,
	RSSI_THRESHOLD_ALGORITHM = 1,
	RSSI_AVERAGE_ALGORITHM = 2,
}rssi_mode;



typedef struct stack_timer_source {
	struct stack_timer_source *next;
    uint32_t timeout;
    void  (*process)(struct stack_timer_source *ts); 
    void * context;
	uint16_t timer_id;
}stack_timer_t;

typedef struct{
	void (*sleep_peripheral_prepare)(void);//休眠前准备
	void (*sleep_peripheral_recover)(void);//休眠后恢复
	void (*packet_handler)(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);
}stack_init_t;


void lib_bluetooth_irq4(void);
void lib_bluetooth_irq5(void);
void lib_bluetooth_irq6(void);
void lib_bluetooth_irq7(void);
void lib_bluetooth_irq8(void);
void lib_bluetooth_irq9(void);
void lib_bluetooth_irq10(void);
void lib_bluetooth_irq11(void);
void lib_bluetooth_irq13(void);
void lib_bluetooth_irq16(void);
void lib_bluetooth_irq18(void);
void lib_bluetooth_irq19(void);
void lib_bluetooth_irq20(void);

/**
 * @brief get lib version
 * @returns type 1:ble stack,2:bt stack
 */
uint8_t lib_get_stack_type(void);

/**
 * @brief get lib version
 * @returns version 高2字节:LIB VERSION,低2字节：STACK VERSION
 */
uint32_t lib_get_version(void);

/**
 * @brief get time
 * @returns ms
 */
uint32_t lib_get_ms(void);

/**
 * @brief close global irq
 */
void lib_global_irq_lock(void);

/**
 * @brief open global irq
 */
void lib_global_irq_unlock(void);

/**
 * @brief bluetooth stack init
 * @param init 
 */
void lib_bluetooth_init(stack_init_t *init);

/**
 * @brief bluetooth stack enable
 */
void lib_bluetooth_run(void);

/**
 * @brief bluetooth stack loop
 */
void lib_bluetooth_schedule(void);

/**
 * @brief new memory
 * @param size
 */
void *lib_new_malloc(uint32_t size);

/**
 * @brief free memory
 * @param mem_ptr
 */
void lib_free_malloc(void *mem_ptr);

/**
 * @brief execute event
 * @param func
 * @param arg 必须是全局参数，局部传参用lib_new_malloc申请新空间
 * @returns 0 if ok
 */
uint8_t lib_event_post(void (*func)(void*),void *arg);

/**
 * @brief bluetooth stack is sleep
 * @returns 1 if ok
 */
uint8_t lib_bluetooth_sleep(void);

/**
 * @brief bluetooth sleep prepare
 */
void lib_sleep_prepare(void);

/**
 * @brief bluetooth sleep enable
 */
__asm void lib_sleep_enter(void);

/**
 * @brief bluetooth sleep exit
 */
void lib_sleep_exit(void);

/**
 * @brief set xtal osc
 * @param time_us: N us
 */
void lib_set_xtal_osc(uint16_t time_us);

/**
 * @brief select extern 32 and execute calib
 */
void lib_extern_32k_calib(void);

/**
 * @brief setting rc accuracy
 * @param ppm: recommend 500 
 * @param jitter: recommend 20
 * @param drift: recommend 1000
 */
void lib_rc_32k_ppm(uint16_t ppm,uint16_t jitter,uint16_t drift);

/**
 * @brief rc calib period
 * @param period:	Time = N ms
 */
void lib_rc_32k_calib_period(uint16_t period);

/**
 * @brief rc stop calib
 */
void lib_rc_32k_stop_calib(void);

/**
 * @brief radio calib period
 * @param period:	Time = N ms
 */
void lib_radio_calib_period(uint16_t period);

/**
 * @brief radio stop calib
 */
void lib_radio_stop_calib(void);

/**
 * @brief radio initialization
 */
void lib_radio_configuration(uint8_t mode);

/**
 * @brief set xtal frequency deviation
 * @param freq_dev
 */
void lib_set_xtal_freq_dev(uint8_t freq_dev);

/**
 * @brief set tx power
 */
void lib_radio_set_tx_power(tx_power_level tx_power);

/**
 * @brief turn on a timer
 * @param ts
 * @param timer_handler:timeout callback func
 * @param timeout:	Time = N ms
 */
void lib_start_timer(stack_timer_t *ts, void (*timer_handler)(stack_timer_t *ts), uint32_t timeout);

/**
 * @brief close timer
 * @param ts
 */
void lib_stop_timer(stack_timer_t *ts);

/**
 * @brief reset timer
 * @param ts
 * @param timeout:	Time = N ms
 */
void lib_restart_timer(stack_timer_t *ts,uint32_t timeout);

/**
 * @brief get bd addr
 * @param addr
 */
void lib_get_bd_addr(bd_addr_t addr);

/**
 * @brief rssi_adjust
 * @param skip_count 
 * @param threshold_dbm
 */
void lib_rssi_adjust(uint8_t skip_count, uint8_t threshold_dbm,rssi_mode mode);

/**
 * @brief get connected bt rssi
 */
int lib_get_bt_rssi(void);

/**
 * @brief get connected ble rssi
 */
int lib_get_ble_rssi(void);


/**
 * @brief get rc 32k freq
 */
uint32_t lib_get_rc_freq(void);

/**
 * @brief btstack assert cb
 */
void lib_assert_init(void (*cb)(uint8_t info, const char *file, int line));


/**
 * @brief hardware_chache_init
 */
void hardware_chache_init(void (*cb)(void));

/**
 * @brief hardware_flash_idle_sched_set
 */
void hardware_flash_idle_sched_set(void (*cb)(void));

/**
 * @brief hardware_flash_erase_sector
 */
void hardware_flash_erase_sector(uint32_t addr);


/**
 * @brief hardware_flash_write
 */
void hardware_flash_write(uint32_t addr,const uint8_t *data, uint32_t size);


/**
 * @brief hardware_flash_write
 */
void flash_cache_load(uint32_t addr, uint32_t size);

/*说明:
设置关中断的优先级,如果设置为2。则每次调用
lib_global_irq_lock关闭抢占优先级2以上的
中断。函数必须放到蓝牙初始化之前调用。
*/
void lib_set_lock_irq(uint8_t num);

/* 扫描优先级设置 - 参数 0, 0, 1*/
void lib_scan_prority_set(uint8_t mode, uint8_t adv_prority, uint8_t scan_prority);

#endif
