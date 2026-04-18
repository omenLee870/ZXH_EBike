#include "app.h"
#include "app_nfc_process.h"
#include "ws1850s_nfcdrv.h"
#include <string.h>

static nfc_work_state_t s_nfc_state = NFC_WORK_INIT;
static uint32_t s_card_stay_timer = 0;

/* 模拟系统里存储好的合法卡号 (正常应该从 Flash 里读) */
static const uint8_t saved_cards[MAX_SAVED_CARDS][UID_LEN] = {
    {0x11, 0x22, 0x33, 0x44},
    {0xAA, 0xBB, 0xCC, 0xDD}
};

/* 对比读取到的卡号是否在合法列表中 */
static int8_t check_card_valid(uint8_t *uid) {
    for (int i = 0; i < MAX_SAVED_CARDS; i++) {
        if (memcmp(uid, saved_cards[i], UID_LEN) == 0) {
            return i; // 返回匹配到的索引
        }
    }
    return -1; // 没找到
}

void app_nfc_init(void) {
    pcd_init();        // 初始化底层硬件
    pcd_antenna_on();  // 开启天线发射载波
    s_nfc_state = NFC_WORK_POLLING;
}

void app_nfc_suspend(void) {
    if (s_nfc_state != NFC_WORK_STOP) {
        // 1. 关闭 WS1850S 的天线发射（极其重要，切断最大耗电来源）
        pcd_antenna_off(); 
        
        // 2. 还可以往寄存器写指令，让芯片进入 PowerDown 模式
        // pcd_write_reg(CommandReg, PCD_RESETPHASE); 
        
        // 3. 切换状态机
        s_nfc_state = NFC_WORK_STOP;
    }
}

void app_nfc_resume(void) {
    if (s_nfc_state == NFC_WORK_STOP) {
        // 1. 重新打开天线发射
        pcd_antenna_on();
        
        // 2. 切回轮询找卡状态
        s_nfc_state = NFC_WORK_POLLING;
    }
}

void app_nfc_task_run(void* arg) {
    uint8_t tag_type[2];
    uint8_t current_uid[5]; // 存放4字节卡号+1字节校验
    
    switch (s_nfc_state) {
        case NFC_WORK_STOP:
            break;
        case NFC_WORK_INIT:
            app_nfc_init();
            break;
        case NFC_WORK_POLLING:
            // 1. 发送 0x26 请求命令寻卡
            if (pcd_request(0x26, tag_type) == MI_OK) {
                
                // 2. 发送防冲突命令读取 UID
                if (pcd_anticoll(current_uid) == MI_OK) {
                    
                    // 3. 对比 ID 
                    int8_t match_idx = check_card_valid(current_uid);
                    if (match_idx >= 0) {
                        // 【匹配成功！】这里调用解锁逻辑或仪表盘点亮逻辑
                        // vehicle_unlock(); 
                        
                        s_card_stay_timer = 0;
                        s_nfc_state = NFC_WORK_MATCHED; // 切换状态，防止重复刷卡
                    }
                }
            }
            break;
            
        case NFC_WORK_MATCHED:
            // 卡片停留在刷卡区时的逻辑，每进一次 Run() 增加停留时间
            s_card_stay_timer += 50; // 假设定时器是50ms运行一次
            
            // 尝试再次寻卡，如果找不到，说明卡拿走了，恢复轮询状态
            if (pcd_request(0x26, tag_type) != MI_OK) {
                // 卡片已离开
                s_nfc_state = NFC_WORK_POLLING; 
            }
            break;
    }
}
