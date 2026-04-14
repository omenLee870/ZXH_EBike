#include "bsp.h"

uint8_t KeyType = PICC_AUTHENT1A;   //读写密钥类型 验证密钥A
uint8_t RWDATA[16];
unsigned char  uid_length=0;
unsigned char  CT[2];//???
unsigned char  IDA[10]={0};           //??A ?ID
unsigned char  IDB[10];               //??B ?ID
unsigned char  LPCDflag=0;

uint16_t ver_err_ct=0;
uint16_t ver=0;


uint8_t  NFC_ShakeDelay=0;

extern   uint8_t         VehicleWorking ;
extern   uint8_t         SpeedData      ;

extern   unsigned char   NFC_ID_Number[10];




extern void user_gpio_nvic_init(void);
extern void ws8300_gpio_remap(ws8300_gpio gpio, gpio_remap_type_def remap); 
extern void buzzer_ctrl(uint8_t enable, uint8_t period, uint32_t count);

uint8_t get_irq_pin_hight(void)
{
	return ws8300_gpio_get_intput_level(IRQ_PIN);
}

/* us延时 */
void delay_100us(unsigned int n)
{
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < 800; j++);
    }
}

void delay_ms(uint32_t n)
{
    delay_100us(10 * n);
}

unsigned char IC_ver(void)
{
	unsigned char status;
	//复位NFC芯片
	CLR_NFC_RST();
	delay_100us(5);
	SET_NFC_RST();
	//等待NFC晶振起振,再配置寄存器
	mdelay(1);

	WriteRawRC(0x37,0x55);
	status=ReadRawRC(0x37);
	CLR_NFC_RST();
	return status;
}
uint8_t get_card_id(void)
{
	uint8_t retval,statusA;
		statusA = ComReqA();//获取
	if(statusA == MI_OK){
		dbg_printk("card type:%02X %02X\r\n",CT[0],CT[1]);
		LogHex("IDA",IDA,10);
		retval = 1;
	}
	else{
		retval = 0;
	}
	//读卡完成配置NFC芯片进入硬件低功耗模式
	CLR_NFC_RST();		
	return retval;
}

/**
* @brief
* @param
* @return
*/
unsigned char ComReqA(void)
{
		unsigned char status = MI_OK;
		PcdConfig('A');
	if(PcdRequest(PICC_REQALL,CT)!=MI_OK)
			return MI_ERR;
	if(PcdAnticoll(PICC_ANTICOLL1,IDA)!=MI_OK) 	  //一次防冲突及选卡
			return MI_ERR;
#if 0  //操作扇区
	if(PcdSelect(IDA)!=MI_OK)
			return MI_ERR;
	if(PcdAuthState(PICC_AUTHENT1A,Block,PassWd,IDA)!=MI_OK)
			return MI_ERR;
	switch(rw)
	{
		case 0: status = PcdRead(Block,RWDATA);break;
		case 1: status = PcdWrite(Block,RWDATA);break;	
		default: break;
	}
#endif
	return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//          p [OUT]：读出的数据，16字节
//返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////// 
unsigned char PcdRead(unsigned char addr,unsigned char *pData)
{
    uint8_t status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
    {
        for (i=0; i<16; i++)
        {   
					*(pData+i) = ucComMF522Buf[i]; 
//Send_Data_To_UART0(ucComMF522Buf[i]);					
				}
    }
    else
    {   status = MI_ERR;   }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：写数据到M1卡一块
//参数说明: addr[IN]：块地址
//          p [IN]：写入的数据，16字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////                 
unsigned char PcdWrite(unsigned char addr,unsigned char *pData)
{
    unsigned char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pData+i);   }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    
    return status;
}
/////////////////////////////////////////////////////////////////////
//功    能：验证卡片密码
//参数说明: auth_mode[IN]: 密码验证模式
//                 0x60 = 验证A密钥
//                 0x61 = 验证B密钥 
//          addr[IN]：块地址
//          pKey[IN]：密码
//          pSnr[IN]：卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////                   
unsigned char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
    unsigned char status=MI_OK;//,status1=0;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
	
	
//	status1 = ReadRawRC(Status2Reg);
//Send_Data_To_UART0(0x29);
//Send_Data_To_UART0(status1);	
    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
		
//Send_Data_To_UART0(0x29);
//for (i=0; i<MAXRLEN; i++)		
//{Send_Data_To_UART0(ucComMF522Buf[i]);}
			
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
//Send_Data_To_UART0(status);
//Send_Data_To_UART0(0x27);
//status1 = ReadRawRC(Status2Reg) & 0x08;
//Send_Data_To_UART0(0x29);
//Send_Data_To_UART0(status1);
//Send_Data_To_UART0(0x30);		
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
// 	Send_Data_To_UART0(status);
//	Send_Data_To_UART0(0x27);   
    return status;
}
unsigned char ComReqA_Block(unsigned char rw,unsigned char Block)////读取A卡数据块  成功返回0
{
	unsigned char sak;
	unsigned char status=MI_OK;
	if(PcdSelect(PICC_ANTICOLL1,IDA,&sak)!=MI_OK)
			return MI_ERR;
	
		if(PcdAuthState(KeyType,Block,PassWd,IDA)!=MI_OK)
			return MI_ERR;
	switch(rw)
	{
		case 0: status = PcdRead(Block,RWDATA);break;
		case 1: status = PcdWrite(Block,RWDATA);break;	
		default: break;
	}

	return status;
}
/**
* @brief 
* @param type:
* @return 
*/
#define NFC_HARD_RESET
void PcdConfig(unsigned char type)
{
#ifdef NFC_HARD_RESET
	//读卡前复位NFC芯片，重新配置，提高稳定性
	CLR_NFC_RST();
	delay_100us(5);
	SET_NFC_RST();
#else
	WriteRawRC(CommandReg, 0x0F);// 软复位芯片
#endif
	//等待NFC晶振起振,再配置寄存器
	mdelay(1);
	
    if ('A' == type)
    {
        ClearBitMask(Status2Reg, BIT3);
        ClearBitMask(ComIEnReg, BIT7); // 高电平触发中断
        WriteRawRC(ModeReg,0x3D);	// 11 // CRC seed:6363
        WriteRawRC(RxSelReg, 0x88);//RxWait
        WriteRawRC(TxASKReg, 0x40);//15  //typeA
        WriteRawRC(TxModeReg, 0x00);//12 //Tx Framing A
        WriteRawRC(RxModeReg, 0x00);//13 //Rx framing A
        WriteRawRC(AutoTestReg,0);
        WriteRawRC(0x0C, 0x00);	//^_^
        //以下寄存器必须按顺序配置
        {   
            unsigned char backup;
            backup = ReadRawRC(0x37);
            WriteRawRC(0x37, 0x00);	

            if(backup == 0x15)
            {		
                WriteRawRC(0x37, 0x5E);
                WriteRawRC(0x26, 0x48);
                WriteRawRC(0x17, 0x88);
                WriteRawRC(0x35, 0xED);
                WriteRawRC(0x3b, 0xA5);
                WriteRawRC(0x37, 0xAE);
                WriteRawRC(0x3b, 0x72);	
            }
            /*兼容配置，T板芯片打开*/
            if(backup == 0x18)
            {	  
                WriteRawRC(0x1d, 0x04); // 
                WriteRawRC(0x37, 0xA5);
                WriteRawRC(0x32, 0xC9);
                WriteRawRC(0x33, 0x24);
                WriteRawRC(0x37, 0xAE);
                WriteRawRC(0x33, 0x59);
                WriteRawRC(0x31, 0x08);
                WriteRawRC(0x37, 0x5E);
                WriteRawRC(0x35, 0xED);
                WriteRawRC(0x3a, 0x10);
            }
            WriteRawRC(0x37, backup);
        }		

    }
    else if ('B' == type)
    {
        WriteRawRC(Status2Reg, 0x00);	//清MFCrypto1On
        ClearBitMask(ComIEnReg, BIT7);// 高电平触发中断
        WriteRawRC(ModeReg, 0x3F);	// CRC seed:FFFF
        WriteRawRC(RxSelReg, 0x88);	//RxWait
        WriteRawRC(0x0C, 0x00);	//^_^	
        //Tx
        WriteRawRC(ModGsPReg, 0x32);	//调制指数，29h值越大，B卡调制深度越小，反之。 、//30
        WriteRawRC(AutoTestReg, 0x00);
        WriteRawRC(TxASKReg, 0x00);	// typeB
        WriteRawRC(TypeBReg, 0x13);
        WriteRawRC(TxModeReg, 0x83);	//Tx Framing B
        WriteRawRC(RxModeReg, 0x83);	//Rx framing B
        WriteRawRC(BitFramingReg, 0x00);	//TxLastBits=0
        {
            unsigned char backup;
            backup = ReadRawRC(0x37);
            WriteRawRC(0x37, 0x00);
            if(backup == 0x15)
            {	
                WriteRawRC(0x37, 0x5E);
                WriteRawRC(0x26, 0x58);
                WriteRawRC(0x17, 0x88);
                WriteRawRC(0x35, 0xED);
                WriteRawRC(0x3b, 0xE5);
            }
            /*兼容配置，T板芯片打开*/
            if(backup == 0x18)
            {	   
                WriteRawRC(0x1d, 0x04); // 
                WriteRawRC(0x37, 0xA5);
                WriteRawRC(0x32, 0xC9);
                WriteRawRC(0x33, 0x24);
                WriteRawRC(0x37, 0xAE);
                WriteRawRC(0x33, 0x59);
                WriteRawRC(0x31, 0x08);
                WriteRawRC(0x37, 0x5E);
                WriteRawRC(0x35, 0xED);
                WriteRawRC(0x3a, 0x10);
            }
            WriteRawRC(0x37, backup);
        }
    }
        
	PcdAntennaOn();
    
	//打开天线后等待卡上电
	mdelay(8);
}

/**
* @brief
* @param 
* @return
*/
void PcdAntennaOff(void)
{
  WriteRawRC(TxControlReg, ReadRawRC(TxControlReg) & (~0x03));
}

/**
* @brief1ms
* @param 
* @return 
*/
void PcdAntennaOn()
{
	WriteRawRC(TxControlReg, ReadRawRC(TxControlReg) | 0x03); //Tx1RFEn=1 Tx2RFEn=1
}

/**
* @brief  
* @param  
					req_code[IN]:
					0x52 = 14443A
					0x26 = 
					pTagType[OUT]:
					0x4400 = Mifare_UltraLight
					0x0400 = Mifare_One(S50)
					0x0200 = Mifare_One(S70)
					0x0800 = Mifare_Pro(X)
					0x4403 = Mifare_DESFire
* @return ????MI_OK
*/
unsigned char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
   unsigned char  status; 
   unsigned char	i;
   unsigned int   unLen;
   unsigned char  ucComMF522Buf[MAXRLEN]; 

   ClearBitMask(Status2Reg,0x08);
   WriteRawRC(BitFramingReg,0x07);
{//Ginger
 //  SetBitMask(TxControlReg,0x03);
	 i = ReadRawRC(0x37);
	 if( i == 0x12 |i == 0x15 || i == 0x18 )
	 {
		 mdelay(1);
   }
	 else
	 {
		 WriteRawRC(TxModeReg,0x30);
		 WriteRawRC(0x27,0x20);
     WriteRawRC(0x28,0x03);
   }
 }
//mdelay(2);	
   ucComMF522Buf[0] = req_code;

   status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
   if ((status == MI_OK) && (unLen == 0x10))
   {    
       *pTagType     = ucComMF522Buf[0];
       *(pTagType+1) = ucComMF522Buf[1];
   }
   else
   {   status = MI_ERR;   }
   
   return status;
}
/**
* @brief  
* @param  pSnr[OUT]:4
* @return MI_OK
*/
unsigned char PcdAnticoll(uint8_t mode, uint8_t *pSnr) 
{
    char  status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
    ucComMF522Buf[0] = mode;
    ucComMF522Buf[1] = 0x20;
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);
    if (status == MI_OK)
    {
		 for (i=0; i<4; i++)
			 {   
					 *(pSnr+i)  = ucComMF522Buf[i];
					 snr_check ^= ucComMF522Buf[i];
			 }
			 if (snr_check != ucComMF522Buf[i])
			 {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

/**
* @brief  ????
* @param  pSnr[IN]:?????,4??
* @return ????MI_OK
*/
unsigned char PcdSelect(uint8_t Mode,uint8_t *pSnr,unsigned char *SAK)
{
    char  status;
    unsigned char  i;
    unsigned int   unLen;
    unsigned char  ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = Mode;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   
      SAK[0]=ucComMF522Buf[0];
      status = MI_OK;  
    
    }
    else
    {   status = MI_ERR;    }

    return status;
}

/**
* @brief  ?MF522??CRC16??
* @param  
* @return 
*/
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char  i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}
/**
* @brief  ?ISO14443???
* @param  Command[IN]:???
					pInData[IN]:????????
					InLenByte[IN]:?????????
					pOutData[OUT]:??????????
					*pOutLenBit[OUT]:????????
* @return status:???????
*/
unsigned char PcdComMF522(signed char Command,
								unsigned char *pInData,
								unsigned char InLenByte,
								unsigned char *pOutData,
								unsigned int *pOutLenBit)
{
	unsigned char  recebyte=0;
	unsigned char  val;
	unsigned char  err=0;	
	unsigned char  irq_inv;
	unsigned char  len_rest=0;
	unsigned char  len=0;
	unsigned char  status;
	unsigned char  irqEn   = 0x00;
	unsigned char  waitFor = 0x00;
	unsigned char  lastBits;
	uint16_t i;

	switch (Command)
	{
	  case PCD_IDLE:
	     irqEn   = 0x00;
	     waitFor = 0x00;
	     break;
	  case PCD_AUTHENT:    
		irqEn = IdleIEn | TimerIEn;
		waitFor = IdleIRq;
		break;
	  case PCD_RECEIVE:
	     irqEn   = RxIEn | IdleIEn;
	     waitFor = RxIRq;
	     recebyte=1;
	     break;
	  case PCD_TRANSMIT:
	     irqEn   = TxIEn | IdleIEn;
	     waitFor = TxIRq;
	     break;
	  case PCD_TRANSCEIVE:   
		 irqEn = RxIEn | IdleIEn | TimerIEn | TxIEn;
	     waitFor = RxIRq;
	     recebyte=1;
//UART0_write(irqEn);
	     break;
	  default:
	     Command = MI_UNKNOWN_COMMAND;
	     break;
	}
   
	if (Command != MI_UNKNOWN_COMMAND
		&& (((Command == PCD_TRANSCEIVE || Command == PCD_TRANSMIT) && InLenByte > 0)
		|| (Command != PCD_TRANSCEIVE && Command != PCD_TRANSMIT))
		)
	{		
		WriteRawRC(CommandReg, PCD_IDLE);
		
		irq_inv = ReadRawRC(ComIEnReg) & BIT7;
		WriteRawRC(ComIEnReg, irq_inv |irqEn | BIT0);//??Timer ?????
		WriteRawRC(ComIrqReg, 0x7F); //Clear INT
		WriteRawRC(DivIrqReg, 0x7F); //Clear INT
		//Flush Fifo
		SetBitMask(FIFOLevelReg, BIT7);
		if (Command == PCD_TRANSCEIVE || Command == PCD_TRANSMIT || Command == PCD_AUTHENT)
		{
			len_rest = InLenByte;
			if (len_rest >= FIFO_SIZE)
			{
				len = FIFO_SIZE;
			}else
			{
				len = len_rest;
			}		
			for (i = 0; i < len; i++)
			{
				WriteRawRC(FIFODataReg, pInData[i]);
			}
			len_rest -= len;//Rest bytes
			if (len_rest != 0)
			{
				WriteRawRC(ComIrqReg, BIT2); // clear LoAlertIRq
				SetBitMask(ComIEnReg, BIT2);// enable LoAlertIRq
			}

			WriteRawRC(CommandReg, Command);
			if (Command == PCD_TRANSCEIVE)
		    {    
				SetBitMask(BitFramingReg,0x80);  
			}
	
			while (len_rest != 0)
			{
			  mdelay(1);		
				if (len_rest > (FIFO_SIZE - WATER_LEVEL))
				{
					len = FIFO_SIZE - WATER_LEVEL;
				}
				else
				{
					len = len_rest;
				}
				for (i = 0; i < len; i++)
				{
					WriteRawRC(FIFODataReg, pInData[InLenByte - len_rest + i]);
				}

				WriteRawRC(ComIrqReg, BIT2);//?write fifo??,??????????
				len_rest -= len;//Rest bytes
				if (len_rest == 0)
				{
					ClearBitMask(ComIEnReg, BIT2);// disable LoAlertIRq
				}	
			}
			//Wait TxIRq
			mdelay(1);
                        //while (INT_PIN == 0);
			val = ReadRawRC(ComIrqReg);
			if (val & TxIRq)
			{
				WriteRawRC(ComIrqReg, TxIRq);
				{//Ginger
					WriteRawRC(RxThresholdReg, CollLevel);  
					WriteRawRC(RFCfgReg, GainLevel );
				}
			}
		}
		if (PCD_RECEIVE == Command)
		{	
			SetBitMask(ControlReg, BIT6);// TStartNow
		}
	
		len_rest = 0; // bytes received
		WriteRawRC(ComIrqReg, BIT3); // clear HoAlertIRq
		SetBitMask(ComIEnReg, BIT3); // enable HoAlertIRq
	
		mdelay(2);
	
		while(1)
		{
      mdelay(2);
			val = ReadRawRC(ComIrqReg);	
			if ((val & BIT3) && !(val & BIT5))
			{
				if (len_rest + FIFO_SIZE - WATER_LEVEL > 255)
				{
					break;
				}
		    for (i = 0; i <FIFO_SIZE - WATER_LEVEL; i++)
		    {
					pOutData[len_rest + i] = ReadRawRC(FIFODataReg);
		    }
				WriteRawRC(ComIrqReg, BIT3);//?read fifo??,??????????
				len_rest += FIFO_SIZE - WATER_LEVEL; 
			}
			else
			{
				ClearBitMask(ComIEnReg, BIT3);//disable HoAlertIRq
				break;
			}			
		}

		val = ReadRawRC(ComIrqReg);

		
		
		WriteRawRC(ComIrqReg, val);// ???
		//val = ReadRawRC(ComIrqReg);
		
		if (val & BIT0)
		{//????
			status = MI_NOTAGERR;
//			UART0_write(0X22);
		}
		else			//20190911??
		{
//			UART0_write(0X11);
//			err = ReadRawRC(ErrorReg);
			
			status = MI_COM_ERR;
			if ((val & waitFor) && (val & irqEn))
			{
				if (!(val & ErrIRq))
				 {//??????
				    status = MI_OK;

				    if (recebyte)
				    {
						val = 0x7F & ReadRawRC(FIFOLevelReg);
				      	lastBits = ReadRawRC(ControlReg) & 0x07;
						if (len_rest + val > MAX_TRX_BUF_SIZE)
						{//????????
							status = MI_COM_ERR;
						}
						else
						{	
							if (lastBits && val) //??spi??? val-1????
							{
								*pOutLenBit = (val-1)*8 + lastBits;
							}
							else
							{
								*pOutLenBit = val*8;
							}
							*pOutLenBit += len_rest*8;


							if (val == 0)
							{
								val = 1;
							}
							for (i = 0; i < val; i++)
							{
								pOutData[len_rest + i] = ReadRawRC(FIFODataReg);
							}					
						}
				   }
				 }					
				 else if ((err & CollErr) && (!(ReadRawRC(CollReg) & BIT5)))
				 {//a bit-collision is detected				 	
				    status = MI_COLLERR;
				    if (recebyte)
				    {
								val = 0x7F & ReadRawRC(FIFOLevelReg);
				      	lastBits = ReadRawRC(ControlReg) & 0x07;
						if (len_rest + val > MAX_TRX_BUF_SIZE)
						{//????????
							;
						}
						else
						{
					     if (lastBits && val) //??spi??? val-1????
					     {
					        *pOutLenBit = (val-1)*8 + lastBits;
					     }
					     else
					     {
					        *pOutLenBit = val*8;
					     }		
							*pOutLenBit += len_rest*8;
					     if (val == 0)
					     {
					        val = 1;
					     }
							for (i = 0; i < val; i++)
					    {
								pOutData[len_rest + i +1] = ReadRawRC(FIFODataReg);				
					    }				
						}
				    }
					pOutData[0] = (ReadRawRC(CollReg) & CollPos);
					if (pOutData[0] == 0)
					{
						pOutData[0] = 32;
					}
				
					pOutData[0]--;// ???????????,?????????,??????;

				}
				else if ((err & CollErr) && (ReadRawRC(CollReg) & BIT5))
				{
					;		
				}
				//else if (err & (CrcErr | ParityErr | ProtocolErr))
				else if (err & (ProtocolErr))
				{
					status = MI_FRAMINGERR;				
				}
				else if ((err & (CrcErr | ParityErr)) && !(err &ProtocolErr))
				{
					//EMV  parity err EMV 307.2.3.4		
					val = 0x7F & ReadRawRC(FIFOLevelReg);
			      	lastBits = ReadRawRC(ControlReg) & 0x07;
					if (len_rest + val > MAX_TRX_BUF_SIZE)
					{//????????
						status = MI_COM_ERR;
					}
					else
					{
				        if (lastBits && val)
				        {
				           *pOutLenBit = (val-1)*8 + lastBits;
				        }
				        else
				        {
				           *pOutLenBit = val*8;
				        }
						*pOutLenBit += len_rest*8;
					}
					status = MI_INTEGRITY_ERR;
				}				
				else
				{
					status = MI_INTEGRITY_ERR;
				}
			}
			else
			{   
				status = MI_COM_ERR;
			}
		}	
 		SetBitMask(ControlReg, BIT7);// TStopNow =1,???;
		WriteRawRC(ComIrqReg, 0x7F);// ???0
		WriteRawRC(DivIrqReg, 0x7F);// ???1
		ClearBitMask(ComIEnReg, 0x7F);//?????,???????
		ClearBitMask(DivIEnReg, 0x7F);//?????,???????
		WriteRawRC(CommandReg, PCD_IDLE);
	}
	else
	{
		status = USER_ERROR;
	}
	return status;
}

void SetLPCDMode(void)////LPCD??
{
	dbg_printk("退出LPCD模式\n");
	//CLR_NFC_RST();
	//delay_100us(5);	
	//SET_NFC_RST();
	//delay_100us(5);
	pcd_lpcd_application();
	CLR_NFC_RST();
	delay_100us(5);
	SET_NFC_RST();
	delay_100us(5); 
	dbg_printk("进入LPCD模式\n");
	pcd_lpcd_start(SENSITIVITY,PROBE);//??LPCD??
	return;
}

void pcd_lpcd_start(unsigned char delta,unsigned char swingscnt)
{
	dbg_printk("LPCD start\n");
	WriteRawRC(0x01,0x0F); 	//
	WriteRawRC(0x14, 0x23);	// Tx2CW = 1 ,使能TX1，TX2的13.56MHz的能量载波信号
	WriteRawRC(0x37, 0x5e);	//  
	WriteRawRC(0x3c,0X30|delta);//Delta[3:0]
	WriteRawRC(0x3d, 0x0d);	//休眠时间	100MS
	WriteRawRC(0x3e, 0x90|swingscnt);//探测时间
	WriteRawRC(0x37, 0x00);	// 关闭私有寄存器保护开关

	WriteRawRC(0x37, 0x5a);//打开私有寄存器保护开关
	WriteRawRC(0x38, 0x30);//设置LPCD 发射功率
	WriteRawRC(0x39, 0x1c);//设置LPCD 发射功率
	WriteRawRC(0x33, 0x60);//调整步长,20,60,A0,E0
	WriteRawRC(0x36, 0x80);
	WriteRawRC(0x37, 0x00);//关闭私有寄存器保护开关

	ClearBitMask(0x02, 0x80); //配置IRQ为高电平中断
	//SetBitMask(0x02, 0x80);//配置为低电平中断   与Status1Reg的IRq位相反
	WriteRawRC(0x03, 0xA0);	//打开卡探测中断,IRQ 为CMOS 输出
	WriteRawRC(0x01, 0x10);	//PCD soft powerdown	
}
void pcd_lpcd_application(void)
{
}
//******WS1850结束LPCD模式*******************
void pcd_lpcd_end(void)
{
	WriteRawRC(0x01,0x0f); //lpcd
}

void user_gpio_cbk2(uint8_t intter_num)
{
	if(get_irq_pin_hight()){
		//buzzer_ctrl(ENABLE, 200, 2);	
		//cpu_prevent_sleep_set(USER_INT1_ONGOING);	
	}else{
		//cpu_prevent_sleep_clear(USER_INT1_ONGOING);
	}
}

void nfc_card_init(void)
{
	ws8300_gpio_remap(NFC_CS_PIN,GPIO_Remap_GPIO);
	ws8300_gpio_remap(NFC_CLK_PIN,GPIO_Remap_GPIO);
	ws8300_gpio_remap(NFC_MISO_PIN,GPIO_Remap_GPIO);
	ws8300_gpio_remap(NFC_MOSI_PIN,GPIO_Remap_GPIO);
	ws8300_gpio_remap(NFC_NREST_PIN,GPIO_Remap_GPIO);
	
	ws8300_gpio_init(NFC_NREST_PIN,GPIO_Mode_Output);
	ws8300_gpio_init(NFC_CS_PIN,GPIO_Mode_Output);
	ws8300_gpio_init(NFC_CLK_PIN,GPIO_Mode_Output);
	ws8300_gpio_init(NFC_MOSI_PIN,GPIO_Mode_Output);
	ws8300_gpio_init(NFC_MISO_PIN,GPIO_Mode_Input);
	SET_NFC_RST();
#if 1
	dbg_printk("start read ver\n");	
	do{
		ver = IC_ver(); 			
	}while((ver != 0x12) && (ver != 0x15) && (ver != 0x18)); //Ginger
	dbg_printk("read ver Ok == :: %02x \n",ver);
#endif
}

uint8_t card_process_flag = 0;
void set_card_process_flag(uint8_t flag)
{
	card_process_flag = flag;
}
void nfc_card_process(void)
{	
	if(card_process_flag){
		card_process_flag = 0;
		//电动车方案直接寻卡
		PcdAntennaOn();	
		card_manage();	
		/*关闭天线*/
		PcdAntennaOff();
	}
}

