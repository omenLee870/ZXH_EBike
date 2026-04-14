#include <stdio.h>
#include <string.h>
#include "cpu.h"
#include "cpu_2p4g.h"
#include "lib_api.h"


#if(__SYSTEM_CLOCK == 16000000UL)
#define  RB_1M    (0x07<<2)
#define  RB_500K (0x0F<<2)
#define  RB_250K (0x1F<<2)
#define  RB_100K (0x4F<<2)
#else//<32M
#define  RB_1M    (0x0F<<2)
#define  RB_500K (0x1F<<2)
#define  RB_250K (0x3F<<2)
#define  RB_100K (0x9F<<2)
#endif


void _2p4g_static_normal_init(void)
{
    lib_radio_configuration(4);
    
    ///MODEM
    write_reg(0x300080A4, 0x00044000);
    write_reg(0x300080B0, read_reg(0x300080B0) | (BIT8+BIT13));//<open tx_en delay and rx_en delay
    write_reg(0x300080F8, 0x00010035);

    ///2.4G

    ///crc_error_retx: enhance mode, if crc error, no remain tx.
    ///rx_drop_ena:Turn off rxfifo regression.
    ///pid_sel: pid 2bit.
    ///enhance_mode: normal mode.
    ///en_dpl:static pktlenth.
    ///pay_msb_en: payload msb.
    ///en_noack: Turn off ack.
    ///en_2p4g_flag: En 2.4G
    ///data_rate_set: RB_500Kbps
    write_reg(BB2P4GCFG1, 0x0001347C);

    ///exit tx flow:close the RF after close the baseband  and delay 0us;
    ///enter tx flow:open the baseband after open the baseband  and delay 384us; 
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG2, 0x00001800);
#else//<32M
    write_reg(BB2P4GCFG2, 0x00003000);
#endif

    ///exit rx flow:close the RF after close the baseband and delya 0us;
    ///enter rx flow:open the baseband after open the baseband and delay 50.6us;
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG3, 0x00000326);
#else//<32M
    write_reg(BB2P4GCFG3, 0x000064C);
#endif

    ///preamble_length: 4Bytes.
    ///mv_preamble_flag: contain id code for txpkt.
    write_reg(BB2P4GCFG4, 0x0000006);

    ///ID_sync_miss_bit_cnt: miss 0bits for id code.
    ///mv_id_code_flag: contain id code for txpkt.
    write_reg(BB2P4GCFG5, 0x00000000);

    ///rx_sync_word: RX ID code value=0x5ABCDEFA.
    write_reg(BB2P4GCFG6, 0x5ABCDEFA);    

    ///rx_ack payload lenght == 0bytes
    ///tx_id_length: tx id code length 4bytes.
    ///tx_mv_id_code_flag: contain id code for txpkt.
    write_reg(BB2P4GCFG7, 0x00000006);

    ///tx_sync_word: TX id code value=0x5ABCDEFA.
    write_reg(BB2P4GCFG8, 0x5ABCDEFA);

    ///tx_ack payload_length xBytes
    ///tx_payload_length: payload_length xBytes.
    write_reg(BB2P4GCFG9, 0x1A);

    ///rx_data_timeout_en: disable data timeout check.
    ///sign_length: 0bits.
    ///rx_payload_length: data pipe0 PAYLOAD_LENGTH Bytes.
    write_reg(BB2P4GCFG10, 0x0000001A);

    ///crc_out_inv: CRC out not inversion.
    ///crc_sel: CRC 1021 x16+x12+x5+1
    ///crc_error_exit: if crc err, exit rx mode.
    ///crc_mv_payload: CRC contains payload fields.
    ///crc_mv_sign: CRC not contains sign fields.
    ///crc_mv_id: CRC not contains id code fields.
    ///mv_crc_flag: enable CRC check.
    ///crc_init_data: CRC init value 0x1D0F.
    write_reg(BB2P4GCFG11, 0x004C1D0F);//<crc_out_inv, only check payload, CRC_init=0x1D0F

    ///whitening_init_pn7: whitening init value  1010101'b.
    ///whitening_mv_crc: disable crc whitening.
    ///whitening_mv_payload: disable payload whitening.
    ///whitening_mv_sign: disable sign whitening.
    ///whitening_mv_id: disable id code whitening.
    //write_reg(BB2P4GCFG12, 0x050A0553);//< enable scrambling code;payload+crc;  0x050A0000:配置txup(pll打开)拉高后，再等待80us后打开pa，避免pa对pll锁定的影响
    write_reg(BB2P4GCFG12, 0x050A055F);//<disable scrambling

    ///dly2rx_cnt_cfg: event2 delay times (3us*3+500ns) to event6
    ///dly2tx_cnt_cfg: event0 delay times (3us*3+500ns) to event5
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG13, 0x00A800A8);
#else//<32M
    write_reg(BB2P4GCFG13, 0x01500150);
#endif

    ///max_rt_msk: mask rt int.
    ///rx_fifo_full_error_int_msk: mask rxfifo full err int.
    ///rx_fifo_empty_error_int_msk: mask rxfifo empty err int.
    ///tx_fifo_full_error_int_msk: mask txfifo full err int.
    ///tx_fifo_empty_error_int_msk: mask txfifo empyt err int.
    ///rx_done_int_msk: mask rxdone int.
    ///tx_done_int_msk: mask txdone int.
    ///crc_error_int_msk: mask crc err int.
    ///id_match_int_msk: mask id code match int.
    write_reg(BB2P4GCFG14, 0x0020AAAA);

    ///tx_fifo_depth: 1204 bytes
    ///tx_fifo_em_base_addr: txfifo at exchange mem addr 0x000
    ///rx_fifo_depth: 1204 bytes
    ///rx_fifo_em_base_addr: rxfifo at exchange mem addr 0x400
    write_reg(BB2P4GCFG16, 0x20003000);

    ///tx_fifo_clean: clear TX FIFO.
    ///rx_fifo_clean: clear RX FIFO.
    write_reg(BB2P4GCFG17, 0x00000003);

    ///tx_cic_stage: bypass tx cic.
    ///rx_fd_sel: fd 160KHz.
    ///rx_data_rate: CIC 500K bit/s.
    write_reg(BB2P4GCFG22, 0x00000005);

    ///state_change_dly: enhance mode state change delay time 0.
    ///ard: auto retransmission delay 250us.
    ///arc: normal mode.
    ///tx_send_more_bit_num: tx end more 4bits.
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG25, 0x00000004);
#else//<32M
    write_reg(BB2P4GCFG25, 0x00000004);
#endif

    ///tx_preamble: 0x55555555
    write_reg(BB2P4GCFG26, 0x55555555);//<MSB

    _2p4g_rate_set(500);//<distinguish 16M and 32M
    
    _2p4g_power_set(0x09);//<set default power is  9dbm
    _2p4g_channel_set(20);
}


void _2p4g_dynamic_normal_init(void)
{
    lib_radio_configuration(4);
    
    ///MODEM
    write_reg(0x300080A4, 0x00044000);//<解码用硬判方式
    write_reg(0x300080B0, read_reg(0x300080B0) | (BIT8+BIT13));//<open tx_en delay and rx_en delay
    write_reg(0x300080F8, 0x00010035);//<解决AGC增益问题

    ///2.4G

    ///crc_error_retx: enhance mode, if crc error, no remain tx.
    ///rx_drop_ena:Turn off rxfifo regression.
    ///pid_sel: pid 2bit.
    ///enhance_mode: normal mode.
    ///en_dpl:dyncmic pktlenth.
    ///pay_msb_en: payload msb.
    ///en_noack: Turn off ack.
    ///en_2p4g_flag: En 2.4G
    ///data_rate_set: RB_500Kbps
    write_reg(BB2P4GCFG1, 0x0001747C);

    ///exit tx flow:close the RF after close the baseband  and delay 0us;
    ///enter tx flow:open the baseband after open the baseband  and delay 384us; 
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG2, 0x00001800);
#else//<32M
    write_reg(BB2P4GCFG2, 0x00003000);
#endif

    ///exit rx flow:close the RF after close the baseband and delya 0us;
    ///enter rx flow:open the baseband after open the baseband and delay 50.6us;
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG3, 0x00000326);
#else//<32M
    write_reg(BB2P4GCFG3, 0x000064C);
#endif

    ///preamble_length: 4Bytes.
    ///mv_preamble_flag: contain id code for txpkt.
    write_reg(BB2P4GCFG4, 0x0000006);

    ///ID_sync_miss_bit_cnt: miss 0bits for id code.
    ///mv_id_code_flag: contain id code for txpkt.
    write_reg(BB2P4GCFG5, 0x00000000);

    ///rx_sync_word: RX ID code value=0x5ABCDEFA.
    write_reg(BB2P4GCFG6, 0x5ABCDEFA);    

    ///rx_ack payload lenght == 0bytes
    ///tx_id_length: tx id code length 4bytes.
    ///tx_mv_id_code_flag: contain id code for txpkt.
    write_reg(BB2P4GCFG7, 0x00000006);

    ///tx_sync_word: TX id code value=0x5ABCDEFA.
    write_reg(BB2P4GCFG8, 0x5ABCDEFA);

    ///tx_ack payload_length == 0Bytes
    ///tx_payload_length: payload_length 8Bytes.
    write_reg(BB2P4GCFG9, 0x08);

    ///rx_data_timeout_en: disable data timeout check.
    ///sign_length: 10bits.
    ///rx_payload_length: data pipe0 PAYLOAD_LENGTH Bytes.
    write_reg(BB2P4GCFG10, 0x00070008);

    ///crc_out_inv: CRC out not inversion.
    ///crc_sel: CRC 1021 x16+x12+x5+1
    ///crc_error_exit: if crc err, exit rx mode.
    ///crc_mv_payload: CRC contains payload fields.
    ///crc_mv_sign: CRC not contains sign fields.
    ///crc_mv_id: CRC not contains id code fields.
    ///mv_crc_flag: enable CRC check.
    ///crc_init_data: CRC init value 0x1D0F.
    write_reg(BB2P4GCFG11, 0x004C1D0F);//<crc_out_inv, only check payload, CRC_init=0x1D0F

    ///whitening_init_pn7: whitening init value  1010101'b.
    ///whitening_mv_crc: disable crc whitening.
    ///whitening_mv_payload: disable payload whitening.
    ///whitening_mv_sign: disable sign whitening.
    ///whitening_mv_id: disable id code whitening.
    write_reg(BB2P4GCFG12, 0x050A0551);//< enable scrambling code;sign+payload+crc;  0x050A0000:配置txup(pll打开)拉高后，再等待80us后打开pa，避免pa对pll锁定的影响

    ///dly2rx_cnt_cfg: event2 delay times (3us*3+500ns) to event6
    ///dly2tx_cnt_cfg: event0 delay times (3us*3+500ns) to event5
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG13, 0x00A800A8);
#else//<32M
    write_reg(BB2P4GCFG13, 0x01500150);
#endif

    ///max_rt_msk: mask rt int.
    ///rx_fifo_full_error_int_msk: mask rxfifo full err int.
    ///rx_fifo_empty_error_int_msk: mask rxfifo empty err int.
    ///tx_fifo_full_error_int_msk: mask txfifo full err int.
    ///tx_fifo_empty_error_int_msk: mask txfifo empyt err int.
    ///rx_done_int_msk: mask rxdone int.
    ///tx_done_int_msk: mask txdone int.
    ///crc_error_int_msk: mask crc err int.
    ///id_match_int_msk: mask id code match int.
    write_reg(BB2P4GCFG14, 0x0020AAAA);

    ///tx_fifo_depth: 1204 bytes
    ///tx_fifo_em_base_addr: txfifo at exchange mem addr 0x000
    ///rx_fifo_depth: 1204 bytes
    ///rx_fifo_em_base_addr: rxfifo at exchange mem addr 0x400
    write_reg(BB2P4GCFG16, 0x20003000);

    ///tx_fifo_clean: clear TX FIFO.
    ///rx_fifo_clean: clear RX FIFO.
    write_reg(BB2P4GCFG17, 0x00000003);

    ///tx_cic_stage: bypass tx cic.
    ///rx_fd_sel: fd 160KHz.
    ///rx_data_rate: CIC 500K bit/s.
    write_reg(BB2P4GCFG22, 0x00000005);

    ///state_change_dly: enhance mode state change delay time 0.
    ///ard: auto retransmission delay 250us.
    ///arc: normal mode.
    ///tx_send_more_bit_num: tx end more 4bits.
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG25, 0x00000004);
#else//<32M
    write_reg(BB2P4GCFG25, 0x00000004);
#endif

    ///tx_preamble: 0x710F5555
    write_reg(BB2P4GCFG26, 0x710F5555);//<MSB

    _2p4g_rate_set(1000);//<distinguish 16M and 32M
    
    //_2p4g_power_set(0x09);//<set default power is  9dbm
    _2p4g_channel_set(04);
		_2p4g_fd_250k();
}

void _2p4g_static_enhanced_init(void)
{
    lib_radio_configuration(4);
    
    ///MODEM
    write_reg(0x300080A4, 0x00044000);//<解码用硬判方式
    write_reg(0x300080B0, read_reg(0x300080B0) | (BIT8+BIT13));//<open tx_en delay and rx_en delay
    write_reg(0x300080F8, 0x00010035);//<解决AGC增益问题

    ///2.4G

    ///crc_error_retx: enhance mode, if crc error, no remain tx.
    ///rx_drop_ena:Turn on rxfifo regression.
    ///pid_sel: pid 2bit.
    ///enhance_mode: static  mode.
    ///en_dpl:dynamic  pktlenth.
    ///pay_msb_en: payload msb.
    ///en_noack: Turn on ack.
    ///en_2p4g_flag: En 2.4G
    ///data_rate_set: RB_500Kbps
    write_reg(BB2P4GCFG1, 0x000FA47C);

    ///exit tx flow:close the RF after close the baseband  and delay 0us;
    ///enter tx flow:open the baseband after open the baseband  and delay 384us; 
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG2, 0x00001800);
#else//<32M
    write_reg(BB2P4GCFG2, 0x00003000);
#endif

    ///exit rx flow:close the RF after close the baseband and delya 0us;
    ///enter rx flow:open the baseband after open the baseband and delay 50.6us;
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG3, 0x00000326);
#else//<32M
    write_reg(BB2P4GCFG3, 0x0000064C);
#endif

    ///preamble_length: 4Bytes.
    ///mv_preamble_flag: contain id code for txpkt.
    write_reg(BB2P4GCFG4, 0x0000006);

    ///ID_sync_miss_bit_cnt: miss 0bits for id code.
    ///mv_id_code_flag: contain id code for txpkt.
    write_reg(BB2P4GCFG5, 0x00000000);

    ///rx_sync_word: RX ID code value=0x5ABCDEFA.
    write_reg(BB2P4GCFG6, 0x5ABCDEFA);    

    ///rx_ack payload lenght == 1bytes
    ///tx_id_length: tx id code length 4bytes.
    ///tx_mv_id_code_flag: contain id code for txpkt.
    write_reg(BB2P4GCFG7, 0x00010006);

    ///tx_sync_word: TX id code value=0x5ABCDEFA.
    write_reg(BB2P4GCFG8, 0x5ABCDEFA);

    ///tx_ack payload_length == 1Bytes
    ///tx_payload_length: payload_length 8Bytes.
    write_reg(BB2P4GCFG9, 0x00010008);

    ///rx_data_timeout_en: disable data timeout check.
    ///sign_length:1 0bits.
    ///rx_payload_length: data pipe0 PAYLOAD_LENGTH Bytes.
    write_reg(BB2P4GCFG10, 0x00090008);

    ///crc_out_inv: CRC out not inversion.
    ///crc_sel: CRC 1021 x16+x12+x5+1
    ///crc_error_exit: if crc err, not exit rx mode.
    ///crc_mv_payload: CRC contains payload fields.
    ///crc_mv_sign: CRC not contains sign fields.
    ///crc_mv_id: CRC not contains id code fields.
    ///mv_crc_flag: enable CRC check.
    ///crc_init_data: CRC init value 0x1D0F.
    write_reg(BB2P4GCFG11, 0x000C1D0F);//<crc_out_inv, only check payload, CRC_init=0x1D0F

    ///whitening_init_pn7: whitening init value  1010101'b.
    ///whitening_mv_crc: disable crc whitening.
    ///whitening_mv_payload: disable payload whitening.
    ///whitening_mv_sign: disable sign whitening.
    ///whitening_mv_id: disable id code whitening.
    write_reg(BB2P4GCFG12, 0x050A0551);//< enable scrambling code;sign+payload+crc;  0x050A0000:配置txup(pll打开)拉高后，再等待80us后打开pa，避免pa对pll锁定的影响

    ///dly2rx_cnt_cfg: event2 delay times (3us*3+500ns) to event6
    ///dly2tx_cnt_cfg: event0 delay times (3us*3+500ns) to event5
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG13, 0x00A800A8);
#else//<32M
    write_reg(BB2P4GCFG13, 0x01500150);
#endif

    ///max_rt_msk: mask rt int.
    ///rx_fifo_full_error_int_msk: mask rxfifo full err int.
    ///rx_fifo_empty_error_int_msk: mask rxfifo empty err int.
    ///tx_fifo_full_error_int_msk: mask txfifo full err int.
    ///tx_fifo_empty_error_int_msk: mask txfifo empyt err int.
    ///rx_done_int_msk: mask rxdone int.
    ///tx_done_int_msk: mask txdone int.
    ///crc_error_int_msk: mask crc err int.
    ///id_match_int_msk: mask id code match int.
    write_reg(BB2P4GCFG14, 0x0020AAAA);

    ///tx_fifo_depth: 1204 bytes
    ///tx_fifo_em_base_addr: txfifo at exchange mem addr 0x000
    ///rx_fifo_depth: 1204 bytes
    ///rx_fifo_em_base_addr: rxfifo at exchange mem addr 0x400
    write_reg(BB2P4GCFG16, 0x20003000);

    ///tx_fifo_clean: clear TX FIFO.
    ///rx_fifo_clean: clear RX FIFO.
    write_reg(BB2P4GCFG17, 0x00000003);

    ///tx_cic_stage: bypass tx cic.
    ///rx_fd_sel: fd 160KHz.
    ///rx_data_rate: CIC 500K bit/s.
    write_reg(BB2P4GCFG22, 0x00000005);

    ///state_change_dly: enhance mode state change delay time 0.
    ///ard: auto retransmission delay 250us.
    ///arc: normal mode.
    ///tx_send_more_bit_num: tx end more 4bits.
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG25, 0x01E307F4);//<hw_spi 切换时间30us
#else//<32M
    write_reg(BB2P4GCFG25, 0x03C60DF4);
#endif

    ///tx_preamble: 0x55555555
    write_reg(BB2P4GCFG26, 0x55555555);//<MSB

    _2p4g_rate_set(1000);//<distinguish 16M and 32M
    
    //_2p4g_power_set(0x09);//<set default power is  9dbm
    _2p4g_channel_set(4);
		_2p4g_fd_250k();
		
}

void _2p4g_dynamic_enhanced_init(void)
{
    lib_radio_configuration(4);

    ///MODEM
    write_reg(0x300080A4, 0x00044000);//<解码用硬判方式
    write_reg(0x300080B0, read_reg(0x300080B0) | (BIT8+BIT13));//<open tx_en delay and rx_en delay
    write_reg(0x300080F8, 0x00010035);//<解决AGC增益问题

    ///2.4G

    ///crc_error_retx: enhance mode, if crc error, no remain tx.
    ///rx_drop_ena:Turn on rxfifo regression.
    ///pid_sel: pid 2bit.
    ///enhance_mode: enhanced  mode.
    ///en_dpl:dynamic  pktlenth.
    ///pay_msb_en: payload msb.
    ///en_noack: Turn on ack.
    ///en_2p4g_flag: En 2.4G
    ///data_rate_set: RB_500Kbps
    write_reg(BB2P4GCFG1, 0x000FE47C);

    ///exit tx flow:close the RF after close the baseband  and delay 0us;
    ///enter tx flow:open the baseband after open the baseband  and delay 384us; 
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG2, 0x00001800);
#else//<32M
    write_reg(BB2P4GCFG2, 0x00003000);
#endif

    ///exit rx flow:close the RF after close the baseband and delya 0us;
    ///enter rx flow:open the baseband after open the baseband and delay 50.6us;
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG3, 0x00000326);
#else//<32M
    write_reg(BB2P4GCFG3, 0x000064C);
#endif

    ///preamble_length: 4Bytes.
    ///mv_preamble_flag: contain id code for txpkt.
    write_reg(BB2P4GCFG4, 0x0000006);

    ///ID_sync_miss_bit_cnt: miss 0bits for id code.
    ///mv_id_code_flag: contain id code for txpkt.
    write_reg(BB2P4GCFG5, 0x00000000);

    ///rx_sync_word: RX ID code value=0x5ABCDEFA.
    write_reg(BB2P4GCFG6, 0x5ABCDEFA);    

    ///rx_ack payload lenght == 1bytes
    ///tx_id_length: tx id code length 4bytes.
    ///tx_mv_id_code_flag: contain id code for txpkt.
    write_reg(BB2P4GCFG7, 0x00010006);

    ///tx_sync_word: TX id code value=0x5ABCDEFA.
    write_reg(BB2P4GCFG8, 0x5ABCDEFA);

    ///tx_ack payload_length == 1Bytes
    ///tx_payload_length: payload_length 8Bytes.
    write_reg(BB2P4GCFG9, 0x00010008);

    ///rx_data_timeout_en: disable data timeout check.
    ///sign_length:1 0bits.
    ///rx_payload_length: data pipe0 PAYLOAD_LENGTH Bytes.
    write_reg(BB2P4GCFG10, 0x00090008);

    ///crc_out_inv: CRC out not inversion.
    ///crc_sel: CRC 1021 x16+x12+x5+1
    ///crc_error_exit: if crc err, not exit rx mode.
    ///crc_mv_payload: CRC contains payload fields.
    ///crc_mv_sign: CRC not contains sign fields.
    ///crc_mv_id: CRC not contains id code fields.
    ///mv_crc_flag: enable CRC check.
    ///crc_init_data: CRC init value 0x1D0F.
    write_reg(BB2P4GCFG11, 0x000C1D0F);//<crc_out_inv, only check payload, CRC_init=0x1D0F

    ///whitening_init_pn7: whitening init value  1010101'b.
    ///whitening_mv_crc: disable crc whitening.
    ///whitening_mv_payload: disable payload whitening.
    ///whitening_mv_sign: disable sign whitening.
    ///whitening_mv_id: disable id code whitening.
    write_reg(BB2P4GCFG12, 0x050A0551);//< enable scrambling code;sign+payload+crc;  0x050A0000:配置txup(pll打开)拉高后，再等待80us后打开pa，避免pa对pll锁定的影响

    ///dly2rx_cnt_cfg: event2 delay times (3us*3+500ns) to event6
    ///dly2tx_cnt_cfg: event0 delay times (3us*3+500ns) to event5
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG13, 0x00A800A8);
#else//<32M
    write_reg(BB2P4GCFG13, 0x01500150);
#endif

    ///max_rt_msk: mask rt int.
    ///rx_fifo_full_error_int_msk: mask rxfifo full err int.
    ///rx_fifo_empty_error_int_msk: mask rxfifo empty err int.
    ///tx_fifo_full_error_int_msk: mask txfifo full err int.
    ///tx_fifo_empty_error_int_msk: mask txfifo empyt err int.
    ///rx_done_int_msk: mask rxdone int.
    ///tx_done_int_msk: mask txdone int.
    ///crc_error_int_msk: mask crc err int.
    ///id_match_int_msk: mask id code match int.
    write_reg(BB2P4GCFG14, 0x0020AAAA);

    ///tx_fifo_depth: 1204 bytes
    ///tx_fifo_em_base_addr: txfifo at exchange mem addr 0x000
    ///rx_fifo_depth: 1204 bytes
    ///rx_fifo_em_base_addr: rxfifo at exchange mem addr 0x400
    write_reg(BB2P4GCFG16, 0x20003000);

    ///tx_fifo_clean: clear TX FIFO.
    ///rx_fifo_clean: clear RX FIFO.
    write_reg(BB2P4GCFG17, 0x00000003);

    ///tx_cic_stage: bypass tx cic.
    ///rx_fd_sel: fd 160KHz.
    ///rx_data_rate: CIC 500K bit/s.
    write_reg(BB2P4GCFG22, 0x00000005);

    ///state_change_dly: enhance mode state change delay time 0.
    ///ard: auto retransmission delay 250us.
    ///arc: normal mode.
    ///tx_send_more_bit_num: tx end more 4bits.
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG25, 0x01E307F4);//<hw_spi 切换时间30us
#else//<32M
    write_reg(BB2P4GCFG25, 0x03C60DF4);
#endif

    ///tx_preamble: 0x55555555
    write_reg(BB2P4GCFG26, 0x55555555);//<MSB

    _2p4g_rate_set(1000);//<distinguish 16M and 32M
    
    //_2p4g_power_set(0x09);//<set default power is  9dbm
    _2p4g_channel_set(20);
}


void _2p4g_static_normal_mode_set(uint16_t pkt_len)
{
    write_reg(BB2P4GCFG1, read_reg(BB2P4GCFG1) & (~(BIT15|BIT14)) | BIT12);//<normal static pktlen mode
    write_reg(BB2P4GCFG10, read_reg(BB2P4GCFG10) & (~(0xF<<16)));//<pktlen field 8bits

    write_reg(BB2P4GCFG11, read_reg(BB2P4GCFG11) | BIT22);//<crc_error  exit
}


void _2p4g_dynamic_normal_mode_set(uint16_t pkt_len)
{
    write_reg(BB2P4GCFG1, read_reg(BB2P4GCFG1) & (~(BIT15|BIT14)) | BIT14 | BIT12);//<normal dynamic pktlen mode
    write_reg(BB2P4GCFG10, read_reg(BB2P4GCFG10) & (~(0xF<<16)) | (0x07<<16));//<pktlen field 8bits

    write_reg(BB2P4GCFG11, read_reg(BB2P4GCFG11) | BIT22);//<crc_error  exit
}


void _2p4g_static_enhanced_mode_set(uint16_t pkt_len)
{
    write_reg(BB2P4GCFG1, read_reg(BB2P4GCFG1) | (BIT16+BIT17+BIT18+BIT19));//<enhanced    
    write_reg(BB2P4GCFG1,read_reg(BB2P4GCFG1) & (~BIT12));//< enable ACK respnse mode
    write_reg(BB2P4GCFG1, read_reg(BB2P4GCFG1) | BIT15);//<enhanced  pktlen mode
    write_reg(BB2P4GCFG1, read_reg(BB2P4GCFG1) & (~BIT14));//< static pktlen mode
    write_reg(BB2P4GCFG10, read_reg(BB2P4GCFG10) & (~(0xF<<16)) | (0x09<<16));//<pktlen field 10bits
    write_reg(BB2P4GCFG11, read_reg(BB2P4GCFG11) & (~BIT22));//<crc_error not exit
    ///增强模式静态包长的发送需要配置长度信息
    write_reg(BB2P4GCFG9, ((read_reg(BB2P4GCFG9)&0xFFFF0000) | pkt_len));//<TX payload_length
    ///增强模式静态包长的接收需要配置长度信息
    write_reg(BB2P4GCFG10, ((read_reg(BB2P4GCFG10)&0xFFFF0000) | pkt_len));//<Rx Payload_length
    ///增强模式配置静态RX_ACK长度
    //write_reg(BB2P4GCFG7, ((read_reg(BB2P4GCFG7)&0x0000FFFF)));//<Rx ACK Payload length == 0bytes
    write_reg(BB2P4GCFG7, ((read_reg(BB2P4GCFG7)&0x0000FFFF) | (0x05<<16)));//<Rx ACK Payload length != 0bytes
    
    ///增强模式配置静态TX_ACK长度
    //write_reg(BB2P4GCFG9, ((read_reg(BB2P4GCFG9)&0x0000FFFF)));//<TX ack_payload_length == 0bytes
    write_reg(BB2P4GCFG9, ((read_reg(BB2P4GCFG9)&0x0000FFFF) | (0x05<<16)));//<TX ack_payload_length != 0bytes
    
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG25, 0x01E307F4);//<自动重传时间根据ack_payload_length设置;  hw_spi 切换时间30us;
#else//<32M
    write_reg(BB2P4GCFG25, 0x03C60DF4);//<0x00000644
#endif
}


void _2p4g_dynamic_enhanced_mode_set(uint16_t pkt_len)
{
    write_reg(BB2P4GCFG1, read_reg(BB2P4GCFG1) | (BIT16+BIT17+BIT18+BIT19));//<enhanced
    write_reg(BB2P4GCFG1,read_reg(BB2P4GCFG1) & (~BIT12));//< enable ACK respnse mode
    write_reg(BB2P4GCFG1, read_reg(BB2P4GCFG1) | (BIT14+BIT15));//<enhanced  dynamic pktlen mode
    write_reg(BB2P4GCFG10, read_reg(BB2P4GCFG10) & (~(0xF<<16)) | (0x09<<16));//<pktlen field 10bits
    write_reg(BB2P4GCFG11, read_reg(BB2P4GCFG11) & (~BIT22));//<crc_error not exit
    //<动态包长的发射需要配置长度,接收不需要配置
    write_reg(BB2P4GCFG9, ((read_reg(BB2P4GCFG9)&0xFFFF0000) | pkt_len));//<TX payload_length
    ///增强模式配置静态RX_ACK长度
    //write_reg(BB2P4GCFG7, ((read_reg(BB2P4GCFG7)&0x0000FFFF)));//<Rx ACK Payload length == 0bytes
    write_reg(BB2P4GCFG7, ((read_reg(BB2P4GCFG7)&0x0000FFFF) | (0x05<<16)));//<Rx ACK Payload length != 0bytes
    
    ///增强模式配置静态TX_ACK长度
    //write_reg(BB2P4GCFG9, ((read_reg(BB2P4GCFG9)&0x0000FFFF)));//<TX ack_payload_length == 0bytes
    write_reg(BB2P4GCFG9, ((read_reg(BB2P4GCFG9)&0x0000FFFF) | (0x05<<16)));//<TX ack_payload_length != 0bytes
    
#if(__SYSTEM_CLOCK == 16000000UL)
    write_reg(BB2P4GCFG25, 0x01E307F4);//<hw_spi 切换时间30us
#else//<32M
    write_reg(BB2P4GCFG25, 0x03C60DF4);//<0x00000644
#endif
}








void _2p4g_rate_set(uint16_t rate)
{
    if(rate == 100)
    {
        write_reg(BB2P4GCFG1, (read_reg(BB2P4GCFG1)&0xFFFFFC03) | RB_100K);//<TX_rate
        write_reg(BB2P4GCFG22,0x00000007);//<RX_rate and FD ; 0->1M,5->500K,6->250K,7->100K;
		_2p4g_fd_160k();
    }
    else if (rate == 250)
    {
        write_reg(BB2P4GCFG1, (read_reg(BB2P4GCFG1)&0xFFFFFC03) | RB_250K);//<TX_rate
        write_reg(BB2P4GCFG22,0x00000006);//<RX_rate and FD ; 0->1M,5->500K,6->250K,7->100K;
		_2p4g_fd_160k();
    }
    else if (rate == 500)
    {
        write_reg(BB2P4GCFG1, (read_reg(BB2P4GCFG1)&0xFFFFFC03) | RB_500K);//<TX_rate
        write_reg(BB2P4GCFG22,0x00000005);//<RX_rate and FD ; 0->1M,5->500K,6->250K,7->100K;
		_2p4g_fd_160k();
    }
    else if (rate == 1000)
    {
        write_reg(BB2P4GCFG1, (read_reg(BB2P4GCFG1)&0xFFFFFC03) | RB_1M);//<TX_rate
        write_reg(BB2P4GCFG22,0x00000000);//<RX_rate and FD ; 0->1M,5->500K,6->250K,7->100K;
		_2p4g_fd_250k();
    }
}


void _2p4g_rx_idcode_set(uint32_t rxid)
{
    write_reg(BB2P4GCFG6, rxid);    
}

void _2p4g_tx_idcode_set(uint32_t txid)
{
    write_reg(BB2P4GCFG8, txid);
}





void _2p4g_static_normal_txpld_len_set(uint16_t txpld_len)
{
    write_reg(BB2P4GCFG9, (read_reg(BB2P4GCFG9) & 0xFFFF0000) | txpld_len);
}

void _2p4g_dynamic_normal_txpld_len_set(uint16_t txpld_len)
{
    write_reg(BB2P4GCFG9, (read_reg(BB2P4GCFG9) & 0xFFFF0000) | txpld_len);
}

void _2p4g_static_enhanced_txpld_len_set(uint16_t txpld_len)
{
    write_reg(BB2P4GCFG9, (read_reg(BB2P4GCFG9) & 0xFFFF0000) | txpld_len);
}

void _2p4g_static_enhanced_txackpld_len_set(uint16_t txackpld_len)
{
    write_reg(BB2P4GCFG9, (read_reg(BB2P4GCFG9) & 0x0000FFFF) | (txackpld_len<<16));
}

void _2p4g_dynamic_enhanced_txpld_len_set(uint16_t txpld_len)
{
    write_reg(BB2P4GCFG9, (read_reg(BB2P4GCFG9) & 0xFFFF0000) | txpld_len);
}

void _2p4g_dynamic_enhanced_txackpld_len_set(uint16_t txackpld_len)
{
    write_reg(BB2P4GCFG9, (read_reg(BB2P4GCFG9) & 0x0000FFFF) | (txackpld_len<<16));
}



void _2p4g_static_normal_rxpld_len_set(uint16_t rxpld_len)
{
    write_reg(BB2P4GCFG10, (read_reg(BB2P4GCFG10) & 0xFFFF0000) | rxpld_len);
}

uint16_t _2p4g_dynamic_normal_rxpld_len_get(void)
{
    return read_reg(BB2P4GCFG10) & 0x0000FFFF;
}

void _2p4g_static_enhanced_rxpld_len_set(uint16_t rxpld_len)
{
    write_reg(BB2P4GCFG10, (read_reg(BB2P4GCFG10) & 0xFFFF0000) | rxpld_len);
}

void _2p4g_static_enhanced_rxackpld_len_set(uint16_t rxackpld_len)
{
    write_reg(BB2P4GCFG7, (read_reg(BB2P4GCFG7) & 0x0000FFFF) | (rxackpld_len<<16));
}

uint16_t _2p4g_dynamic_enhanced_rxpld_len_get(void)
{
    return read_reg(BB2P4GCFG10) & 0x0000FFFF;
}

uint16_t _2p4g_dynamic_enhanced_rxackpld_len_get(void)
{
    return read_reg(BB2P4GCFG10) & 0x0000FFFF;
}





void _2p4g_txfifo_write(uint8_t *data, uint16_t len)
{
    uint16_t i;
    
    for(i=0; i<len; i++)
    {
        write_reg(BB2P4GCFG18, data[i]);
    }
}

void _2p4g_rxfifo_read(uint8_t *data, uint16_t len)
{
    uint16_t i;
    
    for(i=0; i<len; i++)
    {
        data[i] = read_reg(BB2P4GCFG21) & 0xFF;//<read rxfifo
    }
}

void _2p4g_txfifo_clear(void)
{
    write_reg(BB2P4GCFG17, BIT1);
}

void _2p4g_rxfifo_clear(void)
{
    write_reg(BB2P4GCFG17, BIT0);
}






void _2p4g_idle_set(void)
{
    _2p4g_idle_efficiency_effect();
    write_reg(BB2P4GCFG1, read_reg(BB2P4GCFG1) & 0xFFFFFFFC);
}

void _2p4g_tx_set(void)
{
    _2p4g_trx_efficiency_effect();
    write_reg(BB2P4GCFG1, ((read_reg(BB2P4GCFG1) & 0xFFFFFFFC) | BIT1));//<open tx
}

void _2p4g_rx_set(void)
{
    _2p4g_trx_efficiency_effect();
    write_reg(BB2P4GCFG1, ((read_reg(BB2P4GCFG1) & 0xFFFFFFFC) | BIT0));//<open rx
}






uint32_t _2p4g_normal_tx_int_dispose(void)
{
    uint32_t status_int;

    status_int = read_reg(BB2P4GCFG15);
    if (status_int & BIT2)//<txdone
    {
        _2p4g_idle_efficiency_effect();
        write_reg(BB2P4GCFG14, read_reg(BB2P4GCFG14) | BIT4);//< clean tx_done_int_clear
    }

    return status_int;
}

uint32_t _2p4g_normal_rx_int_dispose(void)
{
    uint32_t int_status;

    int_status = read_reg(BB2P4GCFG15);
    if (int_status & BIT3)//<rxdone
    {
        _2p4g_idle_efficiency_effect();
        write_reg(BB2P4GCFG14, read_reg(BB2P4GCFG14) | BIT6 | BIT0);//< clean rx_done_int_clear
    }
    
    if (int_status & BIT1)//< crc_err
    {
        _2p4g_idle_efficiency_effect();
        write_reg(BB2P4GCFG14, read_reg(BB2P4GCFG14) | (BIT2 | BIT0));//<crc_err_int_clear, id_match_int_clear

        if (!(read_reg(BB2P4GCFG1) & BIT17))
        {//<crc err fifo no back
#if (1)//<rxfifo clear
            _2p4g_rxfifo_clear();
#else//<read rxfifo discard
            uint8_t i;
            uint16_t rxpld_len;
            
            rxpld_len = (read_reg(BB2P4GCFG10) & 0xFFFF);                
            for (i=0; i<rxpld_len; i++)
            {
                read_reg(BB2P4GCFG21);
            }
#endif
        }
    }

    return int_status;
}


uint32_t _2p4g_enhanced_tx_int_dispose(void)
{
    uint32_t int_status;

    int_status = read_reg(BB2P4GCFG15);
    if (int_status & BIT2)//<txdone
    {
        _2p4g_idle_efficiency_effect();
        write_reg(BB2P4GCFG14, read_reg(BB2P4GCFG14) | BIT4);//< clean tx_done_int_clear
    }
    if (int_status & BIT1)//<ack_crc error
    {//<crc err fifo back
        write_reg(BB2P4GCFG14, read_reg(BB2P4GCFG14) | BIT2);
    }
    if(int_status & BIT10)//<max_rt_int
    {//<多次重传均为收到预期ack的长度
        _2p4g_idle_efficiency_effect();
        write_reg(BB2P4GCFG14, read_reg(BB2P4GCFG14) | BIT20);//<clear int
    }

    return int_status;
}


uint32_t _2p4g_enhanced_rx_int_dispose(void)
{
    uint32_t int_status;

    int_status = read_reg(BB2P4GCFG15);

    if (int_status & BIT3)//<rxdone
    {
        _2p4g_idle_efficiency_effect();
        write_reg(BB2P4GCFG14, read_reg(BB2P4GCFG14) | (BIT6 | BIT0));//< write clean rx_done_int,id_match_int
    }
    if (int_status & BIT1)//<crc_err_int
    {//<crc err fifo back
        write_reg(BB2P4GCFG14, read_reg(BB2P4GCFG14) | (BIT2 | BIT0));//<write clean crc_error_int,id_match_int
    }
    
    return int_status;
}






boolean _2p4g_staitc_normal_tx(uint8_t ch, uint16_t rate, uint32_t tx_id, uint8_t *txpld_p, uint16_t txpld_len)
{
    _2p4g_channel_set(ch);
    _2p4g_rate_set(rate);
    _2p4g_tx_idcode_set(tx_id);
    _2p4g_static_normal_txpld_len_set(txpld_len);

    _2p4g_txfifo_write(txpld_p, txpld_len);

    _2p4g_tx_set();

    while(1)
    {
        if (_2p4g_normal_tx_int_dispose() & BIT2)//txdone
        {
            return TRUE;
        }
    }
}


boolean _2p4g_static_normal_rx(uint8_t ch, uint16_t rate, uint32_t rx_id, uint8_t *rxpld_p, uint16_t rxpld_len)
{
    uint32_t status_int;
    
    _2p4g_channel_set(ch);
    _2p4g_rate_set(rate);
    _2p4g_rx_idcode_set(rx_id);
    _2p4g_static_normal_rxpld_len_set(rxpld_len);

    _2p4g_rx_set();

    while(1)
    {
        status_int = _2p4g_normal_rx_int_dispose();
        
        if (status_int & BIT3)//<rxdone
        {
            _2p4g_rxfifo_read(rxpld_p, rxpld_len);
            
            return TRUE;
        }
        else if (status_int & BIT1)//<crc error
        {
            return FALSE;
        }
    }
}



boolean _2p4g_dynamic_normal_tx(uint8_t ch, uint16_t rate, uint32_t tx_id, uint8_t *txpld_p, uint16_t txpld_len)
{
    _2p4g_channel_set(ch);
    _2p4g_rate_set(rate);
    _2p4g_tx_idcode_set(tx_id);
    _2p4g_dynamic_normal_txpld_len_set(txpld_len);

    _2p4g_txfifo_write(txpld_p, txpld_len);
    
    _2p4g_tx_set();

    while(1)
    {
        if (_2p4g_normal_tx_int_dispose() & BIT2)//txdone
        {
            return TRUE;
        }
    }
}


boolean _2p4g_dynamic_normal_rx(uint8_t ch, uint16_t rate, uint32_t rx_id, uint8_t *rxpld_p, uint16_t *rxpld_len_p)
{
    uint32_t status_int;
    
    _2p4g_channel_set(ch);
    _2p4g_rate_set(rate);
    _2p4g_rx_idcode_set(rx_id);

    _2p4g_rx_set();

    while(1)
    {
        status_int = _2p4g_normal_rx_int_dispose();
        
        if (status_int & BIT3)//<rxdone
        {
            *rxpld_len_p = _2p4g_dynamic_normal_rxpld_len_get();
            _2p4g_rxfifo_read(rxpld_p, *rxpld_len_p);
            
            return TRUE;
        }
        else if (status_int & BIT1)//<crc error
        {
            return FALSE;
        }
    }
}


boolean _2p4g_static_enhanced_tx(uint8_t ch, uint16_t rate, uint32_t tx_id, uint32_t rx_id, uint8_t *txpld_p, uint16_t txpld_len, uint8_t *rxackpld_p, uint16_t rxackpld_len)
{
    uint32_t int_status;
    
    _2p4g_channel_set(ch);
    _2p4g_rate_set(rate);
    _2p4g_tx_idcode_set(tx_id);
    _2p4g_rx_idcode_set(rx_id);
    _2p4g_static_enhanced_txpld_len_set(txpld_len);
    _2p4g_static_enhanced_rxackpld_len_set(rxackpld_len);

    _2p4g_txfifo_write(txpld_p, txpld_len);
    
    _2p4g_tx_set();

    while(1)
    {
        int_status = _2p4g_enhanced_tx_int_dispose();
        
        if (int_status & BIT2)//<txdone
        {
            _2p4g_rxfifo_read(rxackpld_p, rxackpld_len);
            
            return TRUE;
        }
        if(int_status & BIT10)//<max_rt_int
        {
            return FALSE;
        }
    }
}

boolean _2p4g_static_enhanced_rx(uint8_t ch, uint16_t rate, uint32_t tx_id, uint32_t rx_id, uint8_t *txackpld_p, uint16_t txackpld_len, uint8_t *rxpld_p, uint16_t rxpld_len)
{
    uint32_t int_status;
    
    _2p4g_channel_set(ch);
    _2p4g_rate_set(rate);
    _2p4g_tx_idcode_set(tx_id);
    _2p4g_rx_idcode_set(rx_id);
    _2p4g_static_enhanced_rxpld_len_set(rxpld_len);
    _2p4g_static_enhanced_txackpld_len_set(txackpld_len);

    _2p4g_txfifo_write(txackpld_p, txackpld_len);
    _2p4g_rx_set();

    while(1)
    {
        int_status = _2p4g_enhanced_rx_int_dispose();
        
        if (int_status & BIT3)
        {
            _2p4g_rxfifo_read(rxpld_p, rxpld_len);
            
            return TRUE;
        }
    }
}


boolean _2p4g_dynamic_enhanced_tx(uint8_t ch, uint16_t rate, uint32_t tx_id, uint32_t rx_id, uint8_t *txpld_p, uint16_t txpld_len, uint8_t *rxackpld_p, uint16_t *rxackpld_len_p)
{
    uint32_t int_status;
    
    _2p4g_channel_set(ch);
    _2p4g_rate_set(rate);
    _2p4g_tx_idcode_set(tx_id);
    _2p4g_rx_idcode_set(rx_id);
    _2p4g_dynamic_enhanced_txpld_len_set(txpld_len);

    _2p4g_txfifo_write(txpld_p, txpld_len);
    
    _2p4g_tx_set();

    while(1)
    {
        int_status = _2p4g_enhanced_tx_int_dispose();
        
        if (int_status & BIT2)//<txdone
        {
            *rxackpld_len_p = _2p4g_dynamic_enhanced_rxackpld_len_get();
            _2p4g_rxfifo_read(rxackpld_p, *rxackpld_len_p);
            
            return TRUE;
        }
        if(int_status & BIT10)//<max_rt_int
        {
            return FALSE;
        }
    }
}


boolean _2p4g_dynamic_enhanced_rx(uint8_t ch, uint16_t rate, uint32_t tx_id, uint32_t rx_id, uint8_t *txackpld_p, uint16_t txackpld_len, uint8_t *rxpld_p, uint16_t *rxpld_len_p)
{
    uint32_t int_status;
    
    _2p4g_channel_set(ch);
    _2p4g_rate_set(rate);
    _2p4g_tx_idcode_set(tx_id);
    _2p4g_rx_idcode_set(rx_id);
    _2p4g_dynamic_enhanced_txackpld_len_set(txackpld_len);

    _2p4g_txfifo_write(txackpld_p, txackpld_len);
    _2p4g_rx_set();

    while(1)
    {
        int_status = _2p4g_enhanced_rx_int_dispose();
        if (int_status & BIT3)
        {
            *rxpld_len_p = _2p4g_dynamic_enhanced_rxpld_len_get();
            _2p4g_rxfifo_read(rxpld_p, *rxpld_len_p);

            return TRUE;
        }
    }
}


