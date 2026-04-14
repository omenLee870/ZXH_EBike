#include "lib_api.h"
#include "nvs.h"
#include "cpu.h"
#include "user_storage.h"

uint32_t hardware_flash_erase(uint32_t address, uint32_t size)
{
	uint16_t i          = 0;
	uint16_t num_sector = 0;

	if(address < FLASH_VALID_ADDR || address >= FLASH_END_ADDR)
			return FLASH_ERROR_ADDR_INVALID;
	if((address - FLASH_ADDR_BASE)%(FLASH_SECTOR_SIZE))
			return FLASH_ERROR_ADDR_INVALID;
	if((address + size) > FLASH_END_ADDR)
			return FLASH_ERROR_ADDR_OUT;

	if(size % FLASH_SECTOR_SIZE){
		 num_sector = size / (FLASH_SECTOR_SIZE) + 1;
	}else{
		num_sector = size /(FLASH_SECTOR_SIZE);
	}
	for(i=0; i<num_sector; i++)
			hardware_flash_erase_sector(address+(i*FLASH_SECTOR_SIZE));
	return FLASH_SUCCEED;
}

static void user_flash_irq_dis(void)
{
		lib_global_irq_lock();
}

static void user_flash_irq_en(void)
{
		lib_global_irq_unlock();
}

static int user_flash_read(u32_t offset, void *data, u32_t len)
{
		flash_read(offset,len,data);
    return 0;
}

static int user_flash_write(u32_t offset,const void *data, u32_t len)
{
		hardware_flash_write(offset,data,len);
    return 0;
}

static int user_flash_erase(u32_t offset, u32_t size)
{
		hardware_flash_erase(offset,size);
    return 0;
}

static struct nvs_fs fs = {
	.offset = USER_FLASH_DATA_OFFSET,
	.sector_size = 4096,
	.sector_count = USER_FLASH_SECTOR_NUM,
	.write_block_size = 4,
	.disable_irq = user_flash_irq_dis,
	.enable_irq = user_flash_irq_en,
	.flash_read = user_flash_read,
	.flash_write = user_flash_write,
	.flash_erase = user_flash_erase
};


/* 存储初始化 */
int storage_init(void)
{
   int err;
	
	 err = nvs_init(&fs);
   
   return err;
}

/* 保存数据 */
int storage_save(uint16_t id, const void *data, uint32_t size)
{
    int err;

    /* 用户的存储ID不能超过0x2000 */
    if (id >= 0x2000) {
        return -90;
    }

    /* 写NVS */
    err = nvs_write(&fs, id, data, size);

    if (err < 0) {
        return err;
    }

    return 0;
}

/* 读取数据 */
int storage_read(uint16_t id, void *data, uint32_t data_size, uint32_t *read_size)
{
    /* 用户的存储ID不能超过0x2000 */
    if (id >= 0x2000) {
        return -90;
    }

    uint32_t tmp;
    read_size = read_size ? read_size : &tmp;

    /* 读NVS */
    *read_size = nvs_read(&fs, id, data, data_size);
    
    return 0;
}

/* 删除某一个id数据 */
int storage_id_delete(u16_t id)
{
	return nvs_delete(&fs,id);
}

/* 删除所有的数据区数据 */
int storage_all_delete(void)
{
	return nvs_clear(&fs);
}
