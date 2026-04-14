#include "bsp.h"




#ifdef SPI_MODE

//------------------------------------------
// 读SPI数据 
//------------------------------------------
unsigned char SPIReadByte(void)
{
  unsigned char SPICount;                                       // Counter used to clock out the data
  unsigned char SPIData;                  

  SPIData = 0;
  for (SPICount = 0; SPICount < 8; SPICount++)                  // Prepare to clock in the data to be read
  {
    SPIData <<=1;                                               // Rotate the data
    SET_SPI_CK;
    if(STU_SPI_MISO)
    {
      SPIData|=0x01;
    }    	
 		CLR_SPI_CK;
  }                                                             // and loop back
  return (SPIData);                              // Finally return the read data
} 

//------------------------------------------
// 写SPI数据 
//------------------------------------------
void SPIWriteByte(unsigned char SPIData)
{
  unsigned char SPICount;                                       // Counter used to clock out the data

  for(SPICount = 0; SPICount < 8; SPICount++)
  {
    if (SPIData & 0x80)
    {
      SET_SPI_MOSI;
    }
    else
    {
      CLR_SPI_MOSI;
    } 
 
    SET_SPI_CK;
    CLR_SPI_CK;

    
    SPIData <<= 1;
  }          	
} 
/////////////////////////////////////////////////////////////////////
//功    能：读寄存器
//参数说明：Address[IN]:寄存器地址
//返    回：读出的值
/////////////////////////////////////////////////////////////////////
unsigned char ReadRawRC(unsigned char Address)
{
  unsigned char ucAddr;
  unsigned char ucResult=0;
  CLR_SPI_CS;
  ucAddr = ((Address<<1)&0x7E)|0x80;

  SPIWriteByte(ucAddr);
  ucResult=SPIReadByte();
  SET_SPI_CS;
  return ucResult;
}

/////////////////////////////////////////////////////////////////////
//功    能：写寄存器
//参数说明：Address[IN]:寄存器地址
//          value[IN]:写入的值
/////////////////////////////////////////////////////////////////////
void WriteRawRC(unsigned char Address, unsigned char value)   
{  
  unsigned char ucAddr;

  CLR_SPI_CS;
  ucAddr = ((Address<<1)&0x7E);

  SPIWriteByte(ucAddr);
  SPIWriteByte(value);
  SET_SPI_CS;
}

/////////////////////////////////////////////////////////////////////
//功    能：置寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:置位值
/////////////////////////////////////////////////////////////////////
void SetBitMask(unsigned char reg,unsigned char mask)  
{
  char tmp = 0x0;
  tmp = ReadRawRC(reg);
  WriteRawRC(reg,tmp | mask);  // set bit mask
}

/////////////////////////////////////////////////////////////////////
//功    能：清寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:清位值
/////////////////////////////////////////////////////////////////////
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
  char tmp = 0x0;
  tmp = ReadRawRC(reg);
  WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 




#endif










































