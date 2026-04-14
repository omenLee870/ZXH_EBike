#ifndef __USER_FLASH__
#define __USER_FLASH__




//UserFlash提供了如下API供应用程序使用
void user_flash_init(void);	
int  user_flash_read(unsigned char KEY, unsigned char * buf, unsigned char * len);
int  user_flash_write(unsigned char KEY, unsigned char * buf, unsigned char len);

#endif
