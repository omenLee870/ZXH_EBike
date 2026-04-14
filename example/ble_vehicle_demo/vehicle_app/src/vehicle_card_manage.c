#include "bsp.h"

card_mode_e g_card_mode_e = CHECK_CARD_MODE;
card_type_e g_card_type_e = INVAILD_CARD;
card_operate_state_e  g_card_operate_state_e = CARD_PUT_UP;
card_param_t g_card_param = 
{
	.card_num  = 0,
};

void recover_check_mode(void)//�ָ��쿨ģʽ
{
	g_card_mode_e = CHECK_CARD_MODE;	
}
stack_timer_t card_timer;
void card_event_handler(stack_timer_t *ts)
{	
	if(g_card_mode_e == ADD_CARD_MODE){
		//���ӿ���ʱʧ��
	}
	else if(g_card_mode_e == DEL_CARD_MODE){
		//ɾ������ʱʧ��
	}                                                                 
	recover_check_mode();
}
void card_operation_time(uint16_t time)
{
	lib_stop_timer(&card_timer);
	lib_start_timer(&card_timer,card_event_handler,time);	
}
void card_mode_operation_time(card_mode_e mode,uint16_t time)//���ÿ�Ƭ������ʱʱ��-���ڿ�Ƭ���Ӻ�ɾ��
{
	set_card_mode(mode);
	card_operation_time(time);
}
static uint8_t compare_card_id(void)
{
	for(uint8_t i = 0;i < CARD_TOTAL_NUM;i++){
//		LogHex("g_card_param.card_id:",(uint8_t*)&g_card_param.card_id[i*10],10);
		if(memcmp(&g_card_param.card_id[i][0],IDA,10) == 0){
			LogInfo("compare_card_id::i = %d\r\n",i);
			return (i+1);
		}
	}
	return 0;
}
void check_card(void)//�쿨
{
	if(compare_card_id()){//�Ѿ����ӿ�Ƭ���ж�Ӧ�Ŀ���
		//�ͻ��Լ�����
		LogInfo("check_card_success\r\n");
	}	
}
static uint8_t find_add_card_place(void)
{
	uint8_t zero[10] = {0};
	for(uint8_t i = 0;i < CARD_TOTAL_NUM;i++){
		if(memcmp(&g_card_param.card_id[i][0],zero,10) == 0){
			return i;
		}		
	}
	return 0;
}
void add_card(void)//����
{
	uint8_t add_place;
LogInfo("add_card::g_card_param.card_num = %d\r\n",g_card_param.card_num);
LogHex("IDA",IDA,10);
	if((g_card_param.card_num < CARD_TOTAL_NUM)//����û������
	 &&(!compare_card_id())){//���Ų�һ��
		if(write_card(PassWdA,PassWdA,6,otp_addr,6)){//����д��ȥ
			g_card_param.card_num++;		
			add_place = find_add_card_place();
			memcpy(&g_card_param.card_id[add_place][0],IDA,10);
			write_card_flash();
			LogInfo("add_card_success::g_card_param.card_num = %d\r\n",g_card_param.card_num);
		}
		else{
			LogInfo("write_data_to_card_fail\r\n");
		}
	}
	else if(compare_card_id()&&(!compare_card_sn(otp_addr,6))){
		if(write_card(PassWdA,PassWdA,6,otp_addr,6)){//����д��ȥ	
		}
		else{
			LogInfo("write_data_to_card_fail\r\n");
		}
	}
	else{
		if(g_card_param.card_num >= CARD_TOTAL_NUM){//��������
		}
		else if(compare_card_id()&&compare_card_sn(otp_addr,6)){//���Ѿ����ӹ�
			//���ӿ�Ƭ�ɹ��ӿں���-�ͻ��Լ�����	
		}
		LogInfo("add_card_fail\r\n");	
	}			 
	recover_check_mode();
	lib_stop_timer(&card_timer);				 
}
void del_card(void)//ɾ��
{
	if(g_card_param.card_num != 0){
		uint8_t del_id = compare_card_id();
		if(del_id){
			memset(&g_card_param.card_id[del_id - 1][0],0,10);
			g_card_param.card_num--;
			write_card_flash();
			recover_check_mode();
			lib_stop_timer(&card_timer);	
			reset_card();
			LogInfo("del_card_success\r\n");
		}
	}	
}
void (*card_mode[])(void) =
{
	check_card,
	add_card,
	del_card,
};
void set_card_mode(card_mode_e mode)
{
	g_card_mode_e = mode;
}
uint8_t is_vaild_card(void)
{
	if(compare_card_secret_key(PassWdA,6)){
		if(g_card_mode_e == ADD_CARD_MODE){//��������ģʽ
			LogInfo("ADD_CARD_MODE:vaild_card\r\n");
			return VAILD_CARD;
		}
		if(compare_card_sn(otp_addr,6)){
			LogInfo("CHECK_CARD_MODE::vaild_card\r\n");
			return VAILD_CARD;
		}
	}
	return INVAILD_CARD;
}

stack_timer_t card_state_timer;
void card_state_handler(stack_timer_t *ts)
{	
	g_card_operate_state_e = CARD_PUT_UP;//1s����̧��
}
void card_state_switch_time(uint16_t time)
{
	lib_stop_timer(&card_state_timer);
	lib_start_timer(&card_state_timer,card_state_handler,time);	
}
/*��δ�뿪*/
void card_put_down(void)
{
	if(g_card_operate_state_e == CARD_PUT_DOWN){
		card_state_switch_time(CARD_STATE_SWITH_TIME);
	}	
}
void card_manage(void)
{
	g_card_type_e = (card_type_e)is_vaild_card();
	if((g_card_type_e != INVAILD_CARD)){//��Ч��
//	 &&(get_card_id())){//��ȡ����-����ˢ������	
		if(g_card_operate_state_e == CARD_PUT_UP){
			g_card_operate_state_e = CARD_PUT_DOWN;//�п�����
			card_mode[g_card_mode_e]();
		}
	}
}
void nfc_process(void)
{
	nfc_card_process();
}
/**************************************card flash**************************************************/
void read_card_flash(void)
{
	uint8_t len = sizeof(g_card_param);
	storage_read(CARD_FLASH_ID,(uint8_t*)&g_card_param,len,(uint32_t*)&len);
LogHex("read_card_flash::g_card_param",(uint8_t*)&g_card_param,sizeof(g_card_param));
	if(g_card_param.card_num == 0xff){
		memset(&g_card_param,0,len);
		LogInfo("first_read_card_flash_addr\r\n");		
	}
}
void write_card_flash(void)
{
	uint8_t len = sizeof(g_card_param);
LogHex("write_card_flash::g_card_param",(uint8_t*)&g_card_param,len);
	storage_save(CARD_FLASH_ID, (uint8_t*)&g_card_param, len);
}







