/****************************************Copyright (c)**************************************************                         
** File name:			gprs_at_cmd.c
** Descriptions:		4G    ģ���AT   ָ�
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-04
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#include	"common.h"

#include	"misc/endian.h"
#include	"misc/misc.h"

#include	"module/gb905/phone_book/gb905_phone_book.h"

#include	"middleware/info/imei.h"

#include	"middleware/uart/fleety_uart.h"
#include	"middleware/info/call.h"

#include	"app/call/fleety_call.h"

#define		DEBUG_Y
#include	"libs/debug.h"

#define		AT_STR_MAX_SIZE				64

#define		GPRS_AT_MAGIC_NUM			0x0D0A

enum {
	GPRS_IDLE = 0,
	GPRS_WAIT,
};

enum {
	AT_DIALOUT=0,
	AT_CLCC,
	AT_SETCLIP,
	AT_ACCECT,
	AT_HANGUP,
	AT_CGSN,
	AT_CLVL,
	AT_CSPK,
	AT_ATE,
	AT_CFUN,
	AT_PSRAT,
	AT_COPS,
	AT_CREG,
	AT_CPIN,
	AT_CIMI,
	AT_CSQ,
};

enum {
	AT_RINGS=0,
	AT_GETCLIP,
};


typedef int (*at_cmd_callback_func)(unsigned char *buf, int len);


typedef struct{
	char* at_cmd;
	at_cmd_callback_func callback_func;
}at_cmd_pair_t;

typedef struct{
	at_cmd_pair_t current_at_cmd;

	pthread_mutex_t mutex;
	unsigned char state;
}gprs_descriptor_t;

static gprs_descriptor_t gprs;

static int at_cmd_common_ack_treat(unsigned char *buf,int len);
static int at_cmd_get_imei_ack_treat(unsigned char *buf,int len);
static int at_cmd_clcc_treat(unsigned char *buf,int len);
static int at_cmd_cops_treat(unsigned char *buf,int len);
static int at_cmd_cimi_treat(unsigned char *buf,int len);
static int at_cmd_csq_treat(unsigned char *buf,int len);
static int at_cmd_clip_treat(unsigned char *buf,int len);
static int at_cmd_orig_treat(unsigned char * buf,int len);
static int at_cmd_ring_treat(unsigned char *buf,int len);



static at_cmd_pair_t gprs_at_send_cmds[] = {
	{"ATD",at_cmd_common_ack_treat},			// ���ű�־ָ�Ӧ���־
	{"AT+CLCC",at_cmd_clcc_treat},				// ͨ��״̬��ѯ��־ָ�Ӧ���־			
	{"AT+CLIP",at_cmd_common_ack_treat},		// ����������ʾ����ʾ�绰���룬�����֧�֣��޷���ȡ������룩			
	{"ATA",at_cmd_common_ack_treat},			// �������
	{"AT+CHUP",at_cmd_common_ack_treat},		// �Ҷ�ͨ��������ܾ���ȥ��Ҷϣ�����Ҷϣ�
	{"AT+CGSN",at_cmd_get_imei_ack_treat},		// ��ȡIMEI   ��
	
	{"ATE1",at_cmd_common_ack_treat},			// �ر�AT  ָ����Է�ʽ��
	{"AT+CFUN=1",at_cmd_common_ack_treat},		// �����ֻ�ȫ���ܷ�ʽ
	{"AT+PSRAT",at_cmd_common_ack_treat},		// ��ѯ�����������
	{"AT+COPS?",at_cmd_cops_treat},				// ��ѯ��Ӫ��
	{"AT+CREG?",at_cmd_common_ack_treat},		// ע��������Ϣ
	{"AT+CPIN?",at_cmd_common_ack_treat},		// ��ѯ����PIN
	{"AT+CIMI",at_cmd_cimi_treat},				// ��ѯSIM��״̬
	{"AT+CSQ",at_cmd_csq_treat}					// ��ѯ�ź�����
};

static at_cmd_pair_t gprs_at_recv_cmds[]={
	{"^ORIG:",at_cmd_orig_treat},				// 
	{"RING",at_cmd_ring_treat},					// ������Ϣ��־
	{"+CLIP:",at_cmd_clip_treat},				// ������ʾ��Ϣ����RING  ����յ�
	{"+CLCC",at_cmd_clcc_treat}					// 
};



//-----
static void at_cmd_send(unsigned char *buff,int len)
{	
	DbgFuncEntry();

	fleety_uart_send(GPRS_UART,buff,len);
	
	DbgFuncExit();
}


/** 
* @brief	  AT   ����ͺ��ͨ��Ӧ��
* @param buf		����GPRS   AT   ���ݵ����ݻ����ַ
* @param len		����GPRS   AT   ���ݵ����ݻ��峤��
*
* @return    
*	= 0, ����
*	= -1, ����
*/
static int at_cmd_common_ack_treat(unsigned char *buf,int len)
{
	int ret = 0;
	
	DbgFuncEntry();
	
	//DbgPrintf("ack:%s\r\n",buf);

	gprs.state = GPRS_IDLE;

	// --- ???
	if(strcmp((const char *)buf,"OK\r\n") && strcmp((const char *)buf,"ERROR\r\n"))
	{
		ret = -1;
	}

	DbgFuncExit();

	return ret;
}

/** 
* @brief	  AT   �����ȡIMEI    �ŷ��ͺ��յ���Ӧ��
* @param buf		����GPRS   AT   ���ݵ����ݻ����ַ
* @param len		����GPRS   AT   ���ݵ����ݻ��峤��
*
* @return    
*	= 0, ����
*	= -1, ����
*/
static int at_cmd_get_imei_ack_treat(unsigned char *buf,int len)
{
	int ret = 0;
	
	DbgFuncEntry();
	
	DbgPrintf("get imei ack:%s\r\n",buf);
	DbgPrintf("get imei len:%d\r\n",len);

	set_imei((char*)buf,len);
	
	gprs.state = GPRS_IDLE;

	// ----???? 
	if(strstr((const char *)buf,"OK\r\n") && strstr((const char *)buf,"ERROR\r\n"))
	{
		ret = -1;
	}

	DbgFuncExit();

	return ret;
}


static int at_cmd_clcc_treat(unsigned char *buf,int len)
{
	char * offset;
	char * tail;
	clcc_info_t clcc_info;
	static int uncall_count = 0;
	DbgFuncEntry();

	gprs.state = GPRS_IDLE;
	
	offset = strstr((const char *)buf,"+CLCC:");

	if(offset > 0)
	{
		offset += strlen("+CLCC:");

		offset = strchr(offset,',');		// idx
		clcc_info.idx = atoi(offset - 1);
		offset ++;
		
		offset = strchr(offset,',');		// dir
		clcc_info.dir = atoi(offset - 1);
		offset ++;
		
		offset = strchr(offset,',');		// stat		
		clcc_info.stat = atoi(offset - 1);
		offset ++;

		offset = strchr(offset,',');		// mode		
		clcc_info.mode = atoi(offset - 1);
		offset ++;
		
		offset = strchr(offset,',');		// mpty		
		clcc_info.mpty = atoi(offset - 1);
		offset =offset+2;
			
		tail = strchr(offset,'"');			// phone number
		memcpy(clcc_info.phone,offset,tail-offset);
		memcpy(&clcc_info.phone[tail-offset],"\0",1);

		//������ͨ���ű���ͽ���
		if(clcc_info.mode==0)
		{
			uncall_count = 0;
			set_clcc_info(&clcc_info);
			fleety_clcc_state_treat();
		}
        else//������ͨ��ģʽ
        {
           uncall_count++; 
        }
		
	}
	else if((offset = strstr((const char *)buf,"OK"))>0)
	{
		DbgPrintf(" passive hang up2!!!!\r\n");
		fleety_hand_up(PASSIVE_HANG_UP);

	}
	else
	{
		DbgPrintf(" at_clcc data error!!! \r\n");
	}

    if(uncall_count > 3)//3�β�ѯclcc��û������ͨ��״̬��Ҷϵ绰
    {
        DbgPrintf(" passive hang up1!!!!\r\n");
		fleety_hand_up(PASSIVE_HANG_UP);
        uncall_count = 0;
    }
	DbgFuncExit();
	
	return 0;
}

static int at_cmd_cops_treat(unsigned char *buf,int len)
{
#if 0
	char * offset;
	int m_net_type;				//�����������
	unsigned char save_net_type =0;
	char *tail;
	
	DbgFuncEntry();

	gprs.state = GPRS_IDLE;
	DbgPrintf("cops ack:%s\r\n",buf);
	
	if(strcmp((const char *)buf,"+COPS: 0")==0)
	{
		DbgPrintf(" cops data error!!! \r\n");
        save_sim_signal_status(0);
        set_sim_net_type(save_net_type);
        net_type_status =0;
		return 0;
	}
	
	offset = strstr((const char *)buf,"+COPS:");

	if(offset > 0)
	{
        net_type_status =1;
		offset += strlen("+COPS:");
		offset = strchr(offset,',');	
		offset++;

		offset = strchr(offset,',');	
		offset=offset +2;

		tail = strchr(offset,'"');	
		offset= offset+(tail-offset)+2;
		
		m_net_type = atoi(offset);
		DbgGood("rcv net type=%d\r\n",m_net_type);

		if(m_net_type==0)
		{
			save_net_type = MODULE_NET_TYPE_2G;
			DbgPrintf("net type:2G\r\n");
		}
		else if(m_net_type==2)
		{
			save_net_type = MODULE_NET_TYPE_3G;
			DbgPrintf("net type :3G\r\n");
		}
		else if(m_net_type==7)
		{
			save_net_type = MODULE_NET_TYPE_4G;
			DbgPrintf("net type:4G\r\n");
		}

		
	}

	else
	{
		DbgPrintf(" at_cops data error!!! \r\n");
	}

	set_sim_net_type(save_net_type);
#endif

	return 0;
	
}



static int at_cmd_cimi_treat(unsigned char *buf,int len)
{
#if 0

	gprs.state = GPRS_IDLE;
	unsigned int sim_card_status;
	
	DbgGood("CIMI ack:%s\r\n",buf);
	
	DbgFuncEntry();
	if(strcmp((const char *)buf,"ERROR")==0)
	{
		sim_card_status =0;
		DbgError("SIM card ERROR !!!\r\n");
		//send_text_display_query_to_wince((unsigned char*)SIM_CARD_ERROR_MSG,sizeof(SIM_CARD_ERROR_MSG),TTS_DISPLAY);
		
	}
	else
	{
		sim_card_status =1;
		DbgGood("SIM card OK !!!\r\n");
	}

	set_sim_status(sim_card_status);
#endif

	return 0;
}



static int at_cmd_csq_treat(unsigned char *buf,int len)
{
#if 0
	char * offset;
	char * offset1;
	int signal_value;
	unsigned char signal_quality =0;
	gprs.state = GPRS_IDLE;
	
	DbgGood("CSQ ack:%s\r\n",buf);

	DbgFuncEntry();

	offset = strstr((const char *)buf,"+CSQ:");

	if(offset > 0)
	{
		offset += strlen("+CSQ:");
		offset1 = offset;
		offset = strchr(offset,',');
		
		signal_value = atoi(offset - (offset - offset1));
		
		DbgWarn("signal value :%d\r\n",signal_value);
		
        if(net_type_status==0)
        {
            signal_quality = 0;
            save_sim_signal_status(signal_quality);
            return 0;
        }
		
		if(signal_value==0||signal_value==1)
		{
			signal_quality =1;
		}
		else if(signal_value>=2&&signal_value<=16)
		{
			signal_quality =2;
		}

		else if(signal_value>16&&signal_value<=30)
		{
			signal_quality =3;
		}
		else if(signal_value==31)
		{
			signal_quality =4;
		}

		else if(signal_value==99)
		{
			signal_quality =0;
		}
		else
		{
			DbgError(" invalid signal value!!! \r\n");
		}
		
		save_sim_signal_status(signal_quality);
	}

	else
	{
		DbgPrintf(" AT_CSQ data error!!! \r\n");
	}
	
	
	DbgFuncExit();
#endif

	return 0;
}




static int at_cmd_clip_treat(unsigned char *buf,int len)
{

	char * head;
	char * tail;
	char phone_num[PHONE_NUMBER_MAX_LEN];
	int ret = 0;

	DbgFuncEntry();

	/*
	** CLIPӦ���ʽ:
	** CLIP:
	** "12345678910",129,1,,,
	*/
	head = strchr((const char *)buf,'"');
	tail = strchr((const char *)head+1,'"');
	
	if(head && tail)
	{
		memset(phone_num,0x00,PHONE_NUMBER_MAX_LEN);
		memcpy(phone_num,head+1,tail-head-1);
		
		fleety_call_in(phone_num);
		DbgGood("call in phone number = %s\r\n",phone_num);
	}
	else
	{
		DbgError("don't support clip phone number!\r\n");
		ret = -1;
	}

	DbgFuncExit();

	return ret;

}

static int at_cmd_orig_treat(unsigned char * buf,int len)
{
#if 0
	int call_id,call_type;
	char * head;
	char * tail;
	DbgFuncEntry();

	head = strchr((const char *)buf,' ');
	tail = strchr((const char *)buf,',');

	call_id = atoi(head);
	call_type = atoi(tail);
	
	DbgPrintf("call id = %d\r\n",call_id);
	DbgPrintf("call type = %d\r\n",call_type);
	DbgFuncExit();
#endif

	return 0;
}

static int at_cmd_ring_treat(unsigned char *buf,int len)
{
	int ret = 0;

#if 0
	DbgFuncEntry();

	DbgPrintf("ring:%s\r\n",buf);

	//sleep(3);
	//reject_call();

	//callin_accept();

	DbgFuncExit();
#endif

	return ret;
}


/** 
* @brief	  �ӽ��������б��в����Ƿ��ж�Ӧ��AT   ָ��
* @param buf		����GPRS   AT   ���ݵ����ݻ����ַ
* @param len		����GPRS   AT   ���ݵ����ݻ��峤��
*
* @return    ��Ӧ�������ţ�û��֧�ֵ�AT   ָ��,    �򷵻�-1
*/
static int gprs_recv_at_cmd_index(unsigned char * buf,int len)
{
	int i;
	char * head;

	for(i=0;i<ARRAY_SIZE(gprs_at_recv_cmds);i++)
	{
		//DbgPrintf("src = %s\r\n",at_recv_cmds[i].at_cmd);
		//DbgPrintf("dest = %s\r\n",buf);

		// strstr :  �ҳ�str2  �ַ�����str1�ַ����е�һ�γ��ֵ�λ��
		head = strstr((const char *)buf,gprs_at_recv_cmds[i].at_cmd);
		if(head == (char *)buf)
		{
			return i;
		}
	}

	return -1;
}




/** 
* @brief 	�����յ���AT   ָ��Ľ�βħ����("\r\n")
* @param buf	�յ����ݵĻ����ַ
* @param len	�յ����ݵĻ��峤��
* @param offset  	���ҵ���βħ���ֵ�ƫ�Ƶ�ַ
*
* @return	�Ƿ���ҵ�
*		=true, ���ҵ�
*		=false, û�в��ҵ�
*/
static bool gprs_find_magic_char(unsigned char *buf, int len,unsigned int * offset)
{
	unsigned short magic;
	int index = 0;
	bool ret = false;
	
	DbgFuncEntry();

	*offset = 0;
	
	while(index < len)
	{
		magic = EndianReverse16(*(unsigned short *)(buf + index));

		DbgPrintf("magic = 0x%x\r\n",magic);
		
		if(magic == GPRS_AT_MAGIC_NUM)
		{
			ret = true;
			*offset = index;
			break;
		}
		
		index++;
	}
	
	DbgFuncExit();

	return ret;
}




/** 
* @brief	   ��ȡ��һ��������GPRS   AT   ��Ϣ
* @param msg		����GPRS AT  ��Ϣ��buff  ����ָ��
*
* @return    ����Э���ƫ�Ƶ�ַ
*/
static int gprs_get_a_full_msg(buff_mgr_t *msg)
{
	bool ret;
	unsigned int head_offset = 0;
	int offset = 0;

	DbgFuncEntry();
	
	ret = gprs_find_magic_char(msg->buf,msg->len,&head_offset);

	DbgPrintf("ret = %d\r\n",ret);
	DbgPrintf("head_offset = %d\r\n",head_offset);
	
	if(ret && (msg->len - head_offset) > 2)	// least : \r\n
	{
		msg->len = head_offset + 2;
		offset = head_offset + 2;
	}

	DbgPrintf("offset = %d\r\n",offset);
	
	DbgFuncExit();
	
	return offset;
}


/** 
* @brief	   ����һ��������GPRS   AT  ��Ϣ
* @param msg		����GPRS   AT   ���ݵ�buff  ����ָ��
*
* @return    �����Ƿ�ɹ�
*/
static int gprs_parse_a_full_msg(buff_mgr_t * msg)
{
	int ret = 0;
	int index;

	DbgFuncEntry();

	DbgPrintf("state = %d\r\n",gprs.state);

	if(gprs.state == GPRS_WAIT)
	{
		// 2 : 0x0D0A �س����в�����
		ret = gprs.current_at_cmd.callback_func(msg->buf,msg->len - 2);
	}
	else
	{
		index = gprs_recv_at_cmd_index(msg->buf,msg->len);

		DbgPrintf("index = %d\r\n",index);
		
		if(index >= 0  &&  index < ARRAY_SIZE(gprs_at_recv_cmds) )
		{
			// 2 : 0x0D0A �س����в�����
			ret = gprs_at_recv_cmds[index].callback_func(msg->buf,msg->len - 2);
		}
		else
		{
			//DbgWarn("received a unknown at cmd(%s)\r\n",msg->buf);
		}
	}

	DbgFuncExit();
	
	return ret;
}

//-----
/** 
* @brief 		��4G    ģ�鷢�ͻ�ȡIMEI   ����
*
* @return 		���������Ƿ�ɹ�
*/
bool gprs_get_imei(void)
{
	int ret = false;
	char str[AT_STR_MAX_SIZE];

	DbgFuncEntry();
		
	pthread_mutex_lock(&gprs.mutex);
	
	if(gprs.state == GPRS_IDLE)
	{
		gprs.state = GPRS_WAIT;

		gprs.current_at_cmd = gprs_at_send_cmds[AT_CGSN];

		snprintf(str,AT_STR_MAX_SIZE,"%s\r\n",gprs_at_send_cmds[AT_CGSN].at_cmd);
		DbgPrintf("get imei: %s\r\n",str);
		at_cmd_send((unsigned char *)str,strlen(str));

		ret = true;
	}

	pthread_mutex_unlock(&gprs.mutex);

	DbgFuncExit();
	
	return ret;
}


/** 
* @brief 		��4G    ģ�鷢�Ͳ�ѯ����״̬����
*
* @return 		���������Ƿ�ɹ�
*/
bool gprs_get_call_state(void)
{
	bool ret = false;	
	char str[AT_STR_MAX_SIZE];

	DbgFuncEntry();
	
	pthread_mutex_lock(&gprs.mutex);

	if(gprs.state == GPRS_IDLE)
	{
		gprs.state = GPRS_WAIT;

		gprs.current_at_cmd = gprs_at_send_cmds[AT_CLCC];
		
		snprintf(str,AT_STR_MAX_SIZE,"%s\r\n",gprs_at_send_cmds[AT_CLCC].at_cmd);	
		DbgPrintf("get call state: %s\r\n",str);
		at_cmd_send((unsigned char *)str,strlen(str));

		ret = true;
	}

	pthread_mutex_unlock(&gprs.mutex);

	DbgFuncExit();
	
	return ret;
}


/** 
* @brief 		��4G    ģ�鷢�ͺ�������
*
* @return 		���������Ƿ�ɹ�
*/
bool gprs_dail_out(char *phone_num)
{
	bool ret = false;	
	char str[AT_STR_MAX_SIZE];

	DbgFuncEntry();
	
	pthread_mutex_lock(&gprs.mutex);

	if(gprs.state == GPRS_IDLE)
	{
		gprs.state = GPRS_WAIT;

		gprs.current_at_cmd = gprs_at_send_cmds[AT_DIALOUT];
		
		snprintf(str,AT_STR_MAX_SIZE,"%s%s;\r\n",gprs_at_send_cmds[AT_DIALOUT].at_cmd,phone_num);	
		DbgPrintf("dail out: %s\r\n",str);
		at_cmd_send((unsigned char *)str,strlen(str));

		ret = true;
	}

	pthread_mutex_unlock(&gprs.mutex);

	DbgFuncExit();
	
	return ret;
}

/** 
* @brief 		��4G    ģ�鷢������CLIP����
*
* @return 		���������Ƿ�ɹ�
*/
bool gprs_set_clip(void)
{
	bool ret = false;	
	char str[AT_STR_MAX_SIZE];

	DbgFuncEntry();
	
	pthread_mutex_lock(&gprs.mutex);

	if(gprs.state == GPRS_IDLE)
	{
		gprs.state = GPRS_WAIT;

		gprs.current_at_cmd = gprs_at_send_cmds[AT_SETCLIP];
		
		snprintf(str,AT_STR_MAX_SIZE,"%s=1\r\n",gprs_at_send_cmds[AT_SETCLIP].at_cmd);	
		DbgPrintf("dail out: %s\r\n",str);
		at_cmd_send((unsigned char *)str,strlen(str));

		ret = true;
	}

	pthread_mutex_unlock(&gprs.mutex);

	DbgFuncExit();
	
	return ret;
}


/** 
* @brief 		��4G    ģ�鷢�ͽ����绰������
*
* @return 		���������Ƿ�ɹ�
*/

bool gprs_answer_call(void)
{
	bool ret = false;	
	char str[AT_STR_MAX_SIZE];
	
	DbgFuncEntry();
	
	pthread_mutex_lock(&gprs.mutex);

	if(gprs.state == GPRS_IDLE)
	{
		gprs.state = GPRS_WAIT;

		gprs.current_at_cmd = gprs_at_send_cmds[AT_ACCECT];

		snprintf(str,AT_STR_MAX_SIZE,"%s\r\n",gprs_at_send_cmds[AT_ACCECT].at_cmd);
		DbgPrintf("answer call: %s\r\n",str);
		at_cmd_send((unsigned char *)str,strlen(str));

		ret = true;
	}
	
	pthread_mutex_unlock(&gprs.mutex);

	DbgFuncExit();
	
	return ret;
}

/** 
* @brief 		��4G    ģ�鷢�͹Ҷϵ绰������
*
* @return 		���������Ƿ�ɹ�
*/
bool gprs_hand_up(void)
{
	bool ret = false;
	char str[AT_STR_MAX_SIZE];

	DbgFuncEntry();

	pthread_mutex_lock(&gprs.mutex);

	if(gprs.state == GPRS_IDLE)
	{
		gprs.state = GPRS_WAIT;

		gprs.current_at_cmd = gprs_at_send_cmds[AT_HANGUP];

		snprintf(str,AT_STR_MAX_SIZE,"%s\r\n",gprs_at_send_cmds[AT_HANGUP].at_cmd);
		DbgPrintf("hang up: %s\r\n",str);
		at_cmd_send((unsigned char *)str,strlen(str));

		ret = true;
	}

	pthread_mutex_unlock(&gprs.mutex);
	
	DbgFuncExit();

	return ret;
}


/** 
* @brief 		��4G    ģ�鷢��CLCC������(��ѯͨ��״̬)
*
* @return 		���������Ƿ�ɹ�
*/
bool gprs_send_clcc(void)
{
	bool ret = false;
	char str[AT_STR_MAX_SIZE];

	DbgFuncEntry();
	
	pthread_mutex_lock(&gprs.mutex);

	if(gprs.state == GPRS_IDLE)
	{
		gprs.state = GPRS_WAIT;

		gprs.current_at_cmd = gprs_at_send_cmds[AT_CLCC];

		snprintf(str,AT_STR_MAX_SIZE,"%s\r\n",gprs_at_send_cmds[AT_CLCC].at_cmd);
		DbgPrintf("send clcc: %s\r\n",str);
		at_cmd_send((unsigned char *)str,strlen(str));

		ret = true;
	}

	pthread_mutex_unlock(&gprs.mutex);

	DbgFuncExit();
	
	return ret;
}

//-----
/** 
* @brief 	4G ģ���յ�AT  ָ���Э�����
* @param buf	 	���AT   ��Ϣ����
* @param len		���AT   ��Ϣ����
*
* @return 		�����������ݳ���(   ���Դӻ��������Ƴ�)
*/
int gprs_at_protocol_ayalyze(unsigned char * buf,int len)
{
	unsigned char * msg_buf;
	int msg_len;
	
	int offset = 0;
	buff_mgr_t at_buf;

	DbgFuncEntry();

	msg_buf = buf;
	msg_len = len;

	do{
		memset(&at_buf,0,sizeof(buff_mgr_t));
		
		at_buf.buf = msg_buf;
		at_buf.len = msg_len;
		
		offset = gprs_get_a_full_msg(&at_buf);
		if(offset && at_buf.len > 2)
		{
			gprs_parse_a_full_msg(&at_buf);	
		}

		msg_buf += offset;
		msg_len -= offset;
	}while(offset && at_buf.len && msg_len > 0);

	//DbgPrintf("msg_len = %d\r\n",msg_len);
	//DbgPrintf("len = %d\r\n",len);

	DbgFuncExit();

	return (len - msg_len);
}

void gprs_at_protocol_init(void)
{
	DbgFuncEntry();

	memset(&gprs,0x00,sizeof(gprs_descriptor_t));
	gprs.state = GPRS_IDLE;
	pthread_mutex_init(&gprs.mutex, NULL);

	DbgFuncExit();
}

