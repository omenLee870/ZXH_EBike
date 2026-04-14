/**
******************************************************************************************************************************************************************************************
*@file                       cpuflash_burst_set.c
*@author         
*@data                       
*@version                    v1.0
*@par Copyright(c):
*
*@par History:
*     version:author,date,desc
*     v1.0   :create this file
*
*******************************************************************************************************************************************************************************************
*/
#include <string.h>
#include <stdint.h>
#include "cpu.h"
#include "cpu_flash.h"
#include "user_gpadc.h"
//#include "app_config.h"
#define PROTECTION_VOLTAGE			2400	//保护电压2.4v

const bootrom_func_t *flash_pboot_func = (const bootrom_func_t *)0x00001000;

static void flash_cache_clear()
{
    CACHE->CACHE_REF = 0x40000000; 
	CACHE->CACHE_REF |= 0x80000000;
	while ((CACHE->CACHE_REF & 0x80000000));
}

uint32_t flash_read(uint32_t address, uint32_t size, uint8_t *buf)
{
    uint32_t status = FLASH_SUCCEED;
	if(address < FLASH_VALID_ADDR || address >= FLASH_END_ADDR)
        return FLASH_ERROR_ADDR_INVALID;

	memcpy(buf,(uint8_t *)(FLASH_BASE + address),size);
    return (status);
}


uint8_t volum_fall_down(void)
{
	uint8_t down = 1;
	int16_t voltage;
	if(get_gpadc_value(GPADC_HVIN_MODE,GPADC_GPIOA_PIN1,NULL))
	{
		if(voltage > PROTECTION_VOLTAGE){
			down = 0;
		}
	}
	return down;
}

uint32_t flash_write(uint32_t address, uint32_t size, uint8_t *buf)
{
		uint32_t total_len = size;
		uint32_t offset = 0;
	  uint32_t uilen;
	  unsigned long tmp;
	
	  if(volum_fall_down()){
			return 0;
	  }
		
	  __set_PRIMASK(1);
    	flash_pboot_func->flash_unlock();
		while(total_len > 0)
		{
			if(total_len > 0x100)
			{
				uilen = 0x100;
			}
			else
			{
				uilen = total_len;
			}
			tmp = address & 0xff;
			total_len -= uilen;
			if(tmp + uilen > 0x100) //跨扇区写入
			{
					flash_pboot_func->flash_program(address, 0x100 - tmp, buf + offset);

					tmp = (0x100 - tmp);

					uilen -= tmp;

					buf += tmp;

					address &= 0xffffff00;

					address += 0x100;
			}
			flash_pboot_func->flash_program(address, uilen, buf + offset);
			address += uilen;
			offset += uilen;
		}
		flash_pboot_func->flash_lock();
    flash_cache_clear();
   	 __set_PRIMASK(0);
    return 0;
}

uint32_t flash_erase(uint32_t address, uint32_t size)
{
    uint16_t i          = 0;
    uint16_t num_sector = 0;
    uint32_t status     = FLASH_SUCCEED;

	
    if(address < FLASH_VALID_ADDR || address >= FLASH_END_ADDR)
        return FLASH_ERROR_ADDR_INVALID;
    if((address - FLASH_ADDR_BASE)%(FLASH_SECTOR_SIZE))
        return FLASH_ERROR_ADDR_INVALID;
    if((address + size) > FLASH_END_ADDR)
        return FLASH_ERROR_ADDR_OUT;
	
	if(volum_fall_down()){
		  return 0;
	}



    num_sector = size/(FLASH_SECTOR_SIZE);
     __set_PRIMASK(1);
    flash_pboot_func->flash_unlock();
    for(i=0; i<num_sector; i++)
        status = flash_pboot_func->flash_erase_sector(address+(i*FLASH_SECTOR_SIZE));
    flash_pboot_func->flash_lock();
    flash_cache_clear();
     __set_PRIMASK(0);
    return (status);
}

uint32_t flash_sector_erase(uint32_t address)
{
	if(volum_fall_down()){
		  return 0;
	}
	
    __set_PRIMASK(1);
    flash_pboot_func->flash_unlock();
    flash_pboot_func->flash_erase_sector(address);
    flash_pboot_func->flash_lock();
    flash_cache_clear();
   __set_PRIMASK(0);
	
    return 0;
}

uint32_t flash_otp_read(uint32_t addr, uint32_t size, uint8_t *buf)
{
   uint32_t result = 0;
	__set_PRIMASK(1);
   result = flash_pboot_func->flash_read_otp(1,addr,size,buf);
	__set_PRIMASK(0);
   return result;
}


uint32_t flash_page_otp_read(uint32_t page,uint32_t addr, uint32_t size, uint8_t *buf)
{
   uint32_t result = 0;
	__set_PRIMASK(1);
//	dbg_block_printk(" read page %d addr %d size %d\n",page,addr,size);
   result = flash_pboot_func->flash_read_otp(page,addr,size,buf);
	__set_PRIMASK(0);
   return result;
}


uint32_t flash_page_otp_write(uint32_t page,uint32_t addr, uint32_t size, uint8_t *buf)
{
	uint32_t result = 0;
	uint8_t buffer[255];
	__set_PRIMASK(1);
	result = flash_pboot_func->flash_read_otp(page,0,255,buffer);
	memcpy(&buffer[addr],buf,size);
	flash_pboot_func->flash_erase_otp(page);
	flash_pboot_func->flash_unlock();
	result = flash_pboot_func->flash_program_otp(page,0,255,buffer);
	flash_pboot_func->flash_lock();
	__set_PRIMASK(0);
   return result;
}
