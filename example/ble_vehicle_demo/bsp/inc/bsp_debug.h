#ifndef _BSP_DEBUG_H
#define _BSP_DEBUG_H


#include "stdint.h"







/*º¯ÊýÉùÃ÷*/

extern void LogInfo(const char*fmt,...);
extern void LogHex(char *ucpName,uint8_t *ucpHex, uint32_t ulLen);







#endif

