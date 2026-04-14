#ifndef __2P4G_H__
#define __2P4G_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include"cpu.h"

#define BB2P4GCFG1   0x40007000
#define BB2P4GCFG2   0x40007004
#define BB2P4GCFG3   0x40007008
#define BB2P4GCFG4   0x4000700C
#define BB2P4GCFG5   0x40007010
#define BB2P4GCFG6   0x40007014
#define BB2P4GCFG7   0x40007018
#define BB2P4GCFG8   0x4000701C
#define BB2P4GCFG9   0x40007020
#define BB2P4GCFG10 0x40007024
#define BB2P4GCFG11 0x40007028
#define BB2P4GCFG12 0x4000702C
#define BB2P4GCFG13 0x40007030
#define BB2P4GCFG14 0x40007034
#define BB2P4GCFG15 0x40007038
#define BB2P4GCFG16 0x4000703C
#define BB2P4GCFG17 0x40007040
#define BB2P4GCFG18 0x40007044
#define BB2P4GCFG19 0x40007048
#define BB2P4GCFG20 0x4000704C
#define BB2P4GCFG21 0x40007050
#define BB2P4GCFG22 0x40007054
#define BB2P4GCFG23 0x40007058
#define BB2P4GCFG24 0x4000705C
#define BB2P4GCFG25 0x40007060
#define BB2P4GCFG26 0x40007064
#define BB2P4GCFG27 0x40007068
#define BB2P4GCFG28 0x4000706C
#define BB2P4GCFG29 0x40007070




//#define DCDC_CALIB

void delay_10us(uint32_t t);

void _2p4g_rf_cfg(void);

void _2p4g_trx_efficiency_effect(void);
void _2p4g_idle_efficiency_effect(void);
/*
2400~2483.5Mhz, channel=0->2400Mhz，channal=1->2400.5Mhz
*/
void _2p4g_channel_set(uint8_t channel);

/*
power =[0, 9]
0: -18.5dbm
1: -13.5dbm
2: -11dbm
3: -7dbm
4: -4dbm
5:   0dbm
6:   2dbm
7:   5dbm
8:   7dbm
9:   8dbm
*/
void _2p4g_power_set(uint8_t power);
void _2p4g_carrier_on(void);
void _2p4g_carrier_off(void);

/*
使用场景:①用于温度骤变环境;②温度在0度上下变化时;
使用方式:定时调用该温度补偿校准函数,
*/
void _2p4g_radio_calc_to_req(void);


int _2p4g_get_rssi(void);



#define write_reg(addr, val) *(volatile uint32_t *)addr=val
#define read_reg(addr) *(volatile uint32_t *)(addr)

uint16_t read_reg_analog(uint8_t addr);
void write_reg_analog(uint8_t addr, uint16_t dat);

void _2p4g_fd_160k(void);
void _2p4g_fd_250k(void);

void _2p4g_static_normal_init(void);
void _2p4g_dynamic_normal_init(void);
void _2p4g_static_enhanced_init(void);
void _2p4g_dynamic_enhanced_init(void);

void _2p4g_static_normal_mode_set(uint16_t pkt_len);
void _2p4g_dynamic_normal_mode_set(uint16_t pkt_len);
void _2p4g_static_enhanced_mode_set(uint16_t pkt_len);
void _2p4g_dynamic_enhanced_mode_set(uint16_t pkt_len);


void _2p4g_rx_idcode_set(uint32_t rxid);
void _2p4g_tx_idcode_set(uint32_t txid);
void _2p4g_rate_set(uint16_t rate);


void _2p4g_static_normal_txpld_len_set(uint16_t txpld_len);
void _2p4g_dynamic_normal_txpld_len_set(uint16_t txpld_len);
void _2p4g_static_enhanced_txpld_len_set(uint16_t txpld_len);
void _2p4g_static_enhanced_txackpld_len_set(uint16_t txackpld_len);
void _2p4g_dynamic_enhanced_txpld_len_set(uint16_t txpld_len);
void _2p4g_dynamic_enhanced_txackpld_len_set(uint16_t txackpld_len);

void _2p4g_static_normal_rxpld_len_set(uint16_t rxpld_len);
uint16_t _2p4g_dynamic_normal_rxpld_len_get(void);
void _2p4g_static_enhanced_rxpld_len_set(uint16_t rxpld_len);
void _2p4g_static_enhanced_rxackpld_len_set(uint16_t rxackpld_len);
uint16_t _2p4g_dynamic_enhanced_rxpld_len_get(void);
uint16_t _2p4g_dynamic_enhanced_rxackpld_len_get(void);


boolean _2p4g_staitc_normal_tx(uint8_t ch, uint16_t rate, uint32_t tx_id, uint8_t *txpld_p, uint16_t txpld_len);
boolean _2p4g_static_normal_rx(uint8_t ch, uint16_t rate, uint32_t rx_id, uint8_t *rxpld_p, uint16_t rxpld_len);

boolean _2p4g_dynamic_normal_tx(uint8_t ch, uint16_t rate, uint32_t tx_id, uint8_t *txpld_p, uint16_t txpld_len);
boolean _2p4g_dynamic_normal_rx(uint8_t ch, uint16_t rate, uint32_t rx_id, uint8_t *rxpld_p, uint16_t *rxpld_len_p);


boolean _2p4g_static_enhanced_tx(uint8_t ch, uint16_t rate, uint32_t tx_id, uint32_t rx_id, uint8_t *txpld_p, uint16_t txpld_len, uint8_t *rxpld_p, uint16_t rxpld_len);
boolean _2p4g_static_enhanced_rx(uint8_t ch, uint16_t rate, uint32_t tx_id, uint32_t rx_id, uint8_t *txackpld_p, uint16_t txackpld_len, uint8_t *rxpld_p, uint16_t rxpld_len);

boolean _2p4g_dynamic_enhanced_tx(uint8_t ch, uint16_t rate, uint32_t tx_id, uint32_t rx_id, uint8_t *txpld_p, uint16_t txpld_len, uint8_t *rxackpld_p, uint16_t *rxackpld_len_p);
boolean _2p4g_dynamic_enhanced_rx(uint8_t ch, uint16_t rate, uint32_t tx_id, uint32_t rx_id, uint8_t *txackpld_p, uint16_t txackpld_len, uint8_t *rxpld_p, uint16_t *rxpld_len_p);


#ifdef __cplusplus
}
#endif
#endif
