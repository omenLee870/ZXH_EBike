#include "bsp.h"



void LogSendBuff(uint8_t* _ucpStr, uint32_t _ulLen)
{
	user_uart_write(_ucpStr,_ulLen);
}
/*打印字符串:LogInfo = printf*/
void LogInfo(const char*fmt,...)
{
#if LOG_ENABLE
	int n;
	char LogStr[512];
	va_list args;
	va_start(args,fmt);
	n = vsnprintf(LogStr,(sizeof(LogStr) -1),fmt,args);
	va_end(args);
	if( n > 0)
	{
			LogSendBuff((uint8_t*)LogStr,strlen(LogStr));
	}
#endif
}
/*打印Hex信息*/
void LogHex(char *ucpName,uint8_t *ucpHex, uint32_t ulLen)
{
#if LOG_ENABLE
	LogInfo("%s : [%d]\r\n",ucpName,ulLen);
	LogInfo("[");
	for(uint32_t i = 0; i < ulLen; i++)
	{
		 LogInfo(" %02X",ucpHex[i]); 
		 if((i%16 == 15)&&(i+1)!= ulLen)
			LogInfo("\r\n");
	}
	LogInfo(" ]\r\n");
#endif
}
















