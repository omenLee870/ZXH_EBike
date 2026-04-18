#include "ws1850s_nfcdrv.h"
#include "ws1850s_softiic.h"
#include <string.h>

/* 核心通信接口：单片机和卡片进行底层交互 */
static uint8_t pcd_com_mf522(uint8_t command, uint8_t *pInData, uint8_t InLenByte, uint8_t *pOutData, uint32_t *pOutLenBit) {
    uint8_t status = MI_ERR;
    uint8_t irqEn = 0x00;
    uint8_t waitIRq = 0x00;
    uint8_t lastBits, n;
    uint32_t i;

    switch (command) {
        case PCD_TRANSCEIVE:
            irqEn = 0x77;
            waitIRq = 0x30; // 接收或发送完成中断
            break;
        default:
            break;
    }

    pcd_write_reg(CommIEnReg, irqEn | 0x80); // 允许中断
    pcd_clear_bit_mask(CommIrqReg, 0x80);    // 清除中断标志
    pcd_set_bit_mask(FIFOLevelReg, 0x80);    // 清空FIFO
    pcd_write_reg(CommandReg, PCD_IDLE);     // 回到空闲

    // 1. 数据写入 FIFO
    for (i = 0; i < InLenByte; i++) {
        pcd_write_reg(FIFODataReg, pInData[i]);
    }

    // 2. 触发执行
    pcd_write_reg(CommandReg, command);
    if (command == PCD_TRANSCEIVE) {
        pcd_set_bit_mask(BitFramingReg, 0x80); // 启动发送
    }

    // 3. 等待命令完成
    i = 2000; // 超时计数器，根据时钟调整
    do {
        n = pcd_read_reg(CommIrqReg);
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

    pcd_clear_bit_mask(BitFramingReg, 0x80);

    // 4. 解析结果
    if (i != 0) {
        if (!(pcd_read_reg(ErrorReg) & 0x1B)) {
            status = MI_OK;
            if (n & irqEn & 0x01) status = MI_NOTAGERR; // 没找到卡
            
            if (command == PCD_TRANSCEIVE) {
                n = pcd_read_reg(FIFOLevelReg); // 查收到多少字节
                lastBits = pcd_read_reg(ControlReg) & 0x07;
                if (lastBits) *pOutLenBit = (n - 1) * 8 + lastBits;
                else          *pOutLenBit = n * 8;
                
                if (n == 0) n = 1;
                if (n > 64) n = 64; // 最大64字节
                
                // 读出接收到的数据
                for (i = 0; i < n; i++) {
                    pOutData[i] = pcd_read_reg(FIFODataReg);
                }
            }
        } else {
            status = MI_ERR; // 校验/协议错误
        }
    }
    return status;
}

void pcd_init(void) {
    ws1850s_hw_init();
    ws1850s_hw_reset();
    pcd_write_reg(ModeReg, 0x3D);        // 与卡片通信的设置
    pcd_write_reg(TxAutoReg, 0x40);      // 必须100%ASK
}

void pcd_antenna_on(void) {
    uint8_t i = pcd_read_reg(TxControlReg);
    if (!(i & 0x03)) {
        pcd_set_bit_mask(TxControlReg, 0x03); // 打开天线Tx1/Tx2
    }
}

void pcd_antenna_off(void) {
    // TxControlReg (0x14) 寄存器的最低两位(bit0 和 bit1) 控制着 TX1 和 TX2 引脚的发射
    // 清除这 2 位 (0x03)
    pcd_clear_bit_mask(TxControlReg, 0x03); 
}

uint8_t pcd_request(uint8_t req_code, uint8_t *pTagType) {
    uint8_t status;
    uint32_t unLen;
    pcd_write_reg(Status2Reg, 0x08); // 清除标志
    pcd_write_reg(BitFramingReg, 0x07); // 寻卡模式传输7位数据
    pTagType[0] = req_code;
    status = pcd_com_mf522(PCD_TRANSCEIVE, pTagType, 1, pTagType, &unLen);
    if ((status != MI_OK) || (unLen != 0x10)) {
        status = MI_ERR;
    }
    return status;
}

uint8_t pcd_anticoll(uint8_t *pSnr) {
    uint8_t status;
    uint8_t i, snr_check = 0;
    uint32_t unLen;
    
    pcd_write_reg(Status2Reg, 0x08);
    pcd_write_reg(BitFramingReg, 0x00);
    pcd_clear_bit_mask(CollReg, 0x80);

    pSnr[0] = 0x93; // 防冲突命令1
    pSnr[1] = 0x20; // 防冲突命令2
    status = pcd_com_mf522(PCD_TRANSCEIVE, pSnr, 2, pSnr, &unLen);
    
    if (status == MI_OK) {
        // UID 校验：前4字节异或应该等于第5字节
        for (i = 0; i < 4; i++) {
            snr_check ^= pSnr[i];
        }
        if (snr_check != pSnr[4]) status = MI_ERR;
    }
    return status;
}
