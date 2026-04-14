#ifndef __USER_STORAGE__
#define __USER_STORAGE__

#include "types.h"


#define CARD_FLASH_ID   										  	 1
#define VEHICLE_MATCH_DEVICE_FLASH_ID   				 2

#define USER_FLASH_DATA_OFFSET									 0x0007A000	//数据区起始偏移地址
#define USER_FLASH_SECTOR_NUM										 3					//数据区扇区个数，不能小于2个


int storage_init(void);
int storage_save(uint16_t id, const void *data, uint32_t size);
int storage_read(uint16_t id, void *data, uint32_t data_size, uint32_t *read_size);
int storage_id_delete(u16_t id);
int storage_all_delete(void);
#endif
