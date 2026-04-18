#ifndef __APP_NFC_PROCESS_H_
#define __APP_NFC_PROCESS_H_

#include <stdint.h>

/* 卡片 ID 长度为 4字节 */
#define UID_LEN 4
#define MAX_SAVED_CARDS 5

typedef enum {
    NFC_WORK_INIT = 0,
    NFC_WORK_POLLING,
    NFC_WORK_MATCHED,
    NFC_WORK_STOP,
} nfc_work_state_t;

void app_nfc_init(void);
void app_nfc_task_run(void* arg);
void app_nfc_suspend(void); // 挂起/停止 NFC
void app_nfc_resume(void);  // 唤醒/恢复 NFC

#endif /* __APP_NFC_PROCESS_H_ */
