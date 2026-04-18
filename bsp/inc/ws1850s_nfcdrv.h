#ifndef __DRV_WS1850S_H_
#define __DRV_WS1850S_H_

#include <stdint.h>

/* 核心寄存器地址 */
#define CommandReg      0x01
#define CommIEnReg      0x02
#define DivlEnReg       0x03
#define CommIrqReg      0x04
#define DivIrqReg       0x05
#define ErrorReg        0x06
#define Status1Reg      0x07
#define Status2Reg      0x08
#define FIFODataReg     0x09
#define FIFOLevelReg    0x0A
#define WaterLevelReg   0x0B
#define ControlReg      0x0C
#define BitFramingReg   0x0D
#define CollReg         0x0E
#define ModeReg         0x11
#define TxModeReg       0x12
#define RxModeReg       0x13
#define TxControlReg    0x14
#define TxAutoReg       0x15

/* 命令集 */
#define PCD_IDLE        0x00               
#define PCD_CALCCRC     0x03               
#define PCD_TRANSMIT    0x04               
#define PCD_NOCMDCHANGE 0x07               
#define PCD_RECEIVE     0x08               
#define PCD_TRANSCEIVE  0x0C               
#define PCD_RESETPHASE  0x0F               

/* 状态返回码 */
#define MI_OK           0
#define MI_NOTAGERR     1
#define MI_ERR          2

/* 对外驱动接口 */
void pcd_init(void);
void pcd_antenna_on(void);
void pcd_antenna_off(void);
uint8_t pcd_request(uint8_t req_code, uint8_t *pTagType);
uint8_t pcd_anticoll(uint8_t *pSnr);

#endif /* __DRV_WS1850S_H_ */

