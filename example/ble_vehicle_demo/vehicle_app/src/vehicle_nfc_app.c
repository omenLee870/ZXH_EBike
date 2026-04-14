#include "bsp.h"



#define BLOCKADDR 0x04;	///��д�����ĵ�ַ
#define KEYADDR 0x07;	///��д�����ĵ�ַ
uint8_t PassWd[6]={0};//������Կ
uint8_t PassWdA[6]={0x11,0x22,0x33,0x44,0x55,0x66};
uint8_t card_sn[6];

/*�Աȿ���Կ �ɹ�����1 ʧ�ܷ���0*/
uint8_t compare_card_secret_key(uint8_t* secret_key,uint8_t secret_key_len)
{
	uint8_t state=0;
	uint8_t statusA;
	uint8_t Block=BLOCKADDR;
	if(secret_key_len!=6)
		return 0;
	memcpy(PassWd,secret_key,secret_key_len);
	statusA = ComReqA();
	if(statusA!=MI_OK)
		return 0;
	card_put_down();
	state=ComReqA_Block(READ,Block);
	if(state==MI_OK)
	{
		return 1;
	}
	return 0;
}
/*������ �ɹ�����1 ʧ�ܷ���0*/
uint8_t get_card_sn(uint8_t* sn,uint8_t sn_len)
{
	uint8_t state=0,i=0;
	uint8_t statusA;
	uint8_t Block=BLOCKADDR;
	if(sn_len>16)sn_len = 16;
	statusA = ComReqA();
	if(statusA!=MI_OK)
		return 0;
	memcpy(PassWd,PassWdA,6);
	{
		state=ComReqA_Block(READ,Block);//�ȶ����ݿ�
		if(state==MI_OK)
		{
			if(KeyType==PICC_AUTHENT1A)
			{
				for(i=0;i<6;i++)
				{
					*sn = RWDATA[i];
					sn++;
				}
			}					
		}
		return !state;
	}
	return 0;
}
/*�Ա����� �ɹ�����1 ʧ�ܷ���0*/
uint8_t compare_card_sn(uint8_t* mac_addr,uint8_t addr_len)
{
LogHex("mac_addr",mac_addr,6);
	get_card_sn(card_sn,6);
LogHex("card_sn",card_sn,6);
	return (memcmp(mac_addr,card_sn,6) == 0);
}
/*д��Կд����*/
uint8_t write_card(uint8_t* secret_key,uint8_t* secret_key_new,uint8_t secret_key_len,uint8_t* mac_addr,uint8_t addr_len)
{	
	uint8_t state=0,i=0;
	uint8_t statusA;
	uint8_t Block=BLOCKADDR;
	if(secret_key_len!=6)
		return 0;
	if(addr_len>16)
		addr_len = 16;
	memcpy(PassWd,secret_key,secret_key_len);
	statusA = ComReqA();
	if(statusA!=MI_OK)
	return 0;
	state=ComReqA_Block(READ,Block);
	if(state==MI_OK)
	{
		for(i=0;i<addr_len;i++)
		{ 
			RWDATA[i] = *mac_addr;   
			mac_addr++;
		}
		i=0;
		while(ComReqA())//////д��֮���ٶ�������Ҫ���½��ж���������
		{
			if(i>=200)
				break;
			i++;
		}////����
		state=ComReqA_Block(WRITE,Block);
		if(state!=MI_OK)
		{ 
			return 0;
		}
		Block = KEYADDR;
		statusA = ComReqA();
		if(statusA!=MI_OK)
			return state;
		if(!((Block+1)%4))/////���ƿ��ַ������
		{
			state=ComReqA_Block(READ,Block);///�ȶ����ݿ�
			if(state==MI_OK)
			{
				if(KeyType==PICC_AUTHENT1A)
				{
					for(i=0;i<6;i++)
					{
						RWDATA[i] = *secret_key_new;
						secret_key_new++;
					}
					for(i=10;i<16;i++)
					{
						RWDATA[i] = *secret_key_new;
						secret_key_new++;
					}
				}
				while(ComReqA())//////д��֮���ٶ�������Ҫ���½��ж���������
				{
					if(i>=200)
						break;
					i++;
				}////д��
				state=ComReqA_Block(WRITE,Block);////д�ɹ�
			}
			return !state;
		}		
	}
	return 0;
}
void reset_card(void)
{
	LogInfo("reset_card\r\n");
	uint8_t data[16];
	memset(data,0xff,16);
	write_card(PassWdA,PassWdA,6,data,16);
}















