/****************************************Copyright (c)**************************************************                         
** File name:			itop_authorize.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-10-30
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#include		"common.h"

#include		"middleware/socket/authorize_socket.h"
#include		"middleware/info/imei.h"
#include		"middleware/info/version.h"
#include		"middleware/info/device.h"


#include		"module/itop/itop_common.h"
#include		"module/itop/authorize/itop_authorize.h"

#define			DEBUG_Y
#include		"libs/debug.h"

#define 		AUTH_STATE				0x40


// ������֤Э������ݸ�ʽ
typedef struct{
	itop_msg_header_t header;
	char imei[MAX_IMEI_CHARS_SIZE];
	char hw_version[MAX_HW_VERSION_SIZE];
	unsigned char auth_state;		// =1, �˴�ΪΪ������֤��ԭ���Ѿ���֤����;	=0,�˴���֤Ϊ��һ����֤
	char imei_old[MAX_IMEI_CHARS_SIZE];
}__packed itop_authorize_t,* p_itop_authorize_t;

typedef struct{
	itop_msg_header_t header;
	unsigned int esn;				// Equiment Serial Number
}__packed itop_authorize_server_t,* p_itop_authorize_server_t;


/*
 * @brief	�Ƿ��Ѿ���֤�ɹ�
 *
 * @return 
 *	=true, ��֤�ɹ�
 * 	=false, ��֤ʧ��
 */
bool is_authorized(void)
{
	bool ret = false;
	device_info_t info;
	
	get_device_info(&info);
		
	if(info.mtd_id)
	{
		DbgGood("authorize mtd OK!(mtd_id = 0x%x)\r\n",info.mtd_id);
		ret = true;
	}

	return ret;
}




/*
* @brief	�õ���ǰ��Ҫ����֤����
*
* @return 
*	=0, ��Ҫ��һ����֤
* 	=1, ��Ҫ������֤��ԭ���Ѿ���֤����
*	=2, ����Ҫ��֤
*/
unsigned char get_authorize_status(void)
{
	unsigned char state;
	device_info_t info;
	char imei[16];

	DbgFuncEntry();

	//init_device_info();
	get_device_info(&info);
	get_imei(imei);

	DbgPrintf("imei = %s\r\n",imei);
	DbgPrintf("info.mdt_id = 0x%x\r\n",info.mtd_id);
	DbgPrintf("info.imei_id = %s\r\n",info.imei_id);
	if( (info.mtd_id != 0 && info.mtd_id != 0xFFFFFFFF)&& strlen(info.imei_id) == MAX_IMEI_CHARS_SIZE-1 )
	{
		if(strcmp(imei, info.imei_id) )
			state = AUTHORIZE_AGAIN;
		else
			state = AUTHORIZE_NO;
	}
	else
	{
		state = AUTHORIZE_FIRST;
	}

	DbgPrintf("state = %d\r\n",state);

	DbgFuncExit();

	return state;
}

/** 
   * @brief 	��֤Э�������
   * @param auth_state		��ǰ����֤״̬
   * 	=1, �˴�ΪΪ������֤��ԭ���Ѿ���֤����
   *	=0, �˴���֤Ϊ��һ����֤
   */
void itop_authorize_request(unsigned char auth_state)
{
    itop_authorize_t * authorize_msg;
    int len;
	unsigned char *buf=0;

	device_info_t info;

	DbgFuncEntry();
	
	len = sizeof(itop_ext_header_t) + sizeof(itop_authorize_t);

	DbgPrintf("len = %d\r\n",len);

	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("authorize request memory failed!\r\n");
		return;
	}
	memset((void*)buf, 0, len); 

	authorize_msg = (itop_authorize_t *)(buf + sizeof(itop_ext_header_t));
    authorize_msg->header.msg_type.whole = TYPE_UP_AUTH | AUTH_STATE;
    authorize_msg->header.mtd_l = 0;
    authorize_msg->header.msg_len = sizeof(itop_authorize_t) - 3;

    get_imei(authorize_msg->imei);
    get_hw_version(authorize_msg->hw_version);
    
    //strncpy((void*)msg->imei, (const void*)imei_id, MAX_IMEI_CHARS_SIZE);//(const void*)ModuleIMEI, IMEI_LEN);
    //strncpy((void*)msg->hw_version,(void*) VERSION_HARDWARE, strlen(VERSION_HARDWARE));
    authorize_msg->auth_state = auth_state;
    //strncpy((void*)msg->OldIMEI, (const void*)DeviceInfo.IMEI, IMEI_LEN);
	get_device_info(&info);
	strncpy((void*)authorize_msg->imei_old, (const void*)info.imei_id, MAX_IMEI_CHARS_SIZE);

	itop_protocol_request(buf,len);
	
	free(buf);

	DbgFuncExit();
}



/** 
   * @brief 	��֤Э��Ľ���
   * @param mtd_id		��������������Ϣ�е�mtd_id
   * @param msg_buf  	���������Ϣ�ĵ�ַ
   * 
   * @return	�����Ƿ���ȷ
   *	=0,	������ȷ
   *	=-1, ��������
   */
int itop_authorize_analyze(unsigned int mtd_id,unsigned char *msg_buf)
{  
    device_info_t info;
    char imei[MAX_IMEI_CHARS_SIZE];

    itop_authorize_server_t * auth_msg;

	DbgFuncEntry();

    auth_msg = (itop_authorize_server_t *)msg_buf;

    DbgPrintf("mtd_id = 0x%x\r\n",mtd_id);
    DbgPrintf("esn = 0x%x\r\n",auth_msg->esn);
    
    if (0 == mtd_id || 0xFFFF == mtd_id || 0xFFFFFFFF == mtd_id)
    {
    	DbgError("mtd is invaild!\r\n");
		return -1;
	}

	if(0 == auth_msg->esn ||  0xFFFFFFFF == auth_msg->esn)
	{
		DbgError("esn is invaild!\r\n");
		return -1;
	}

	get_device_info(&info);
	info.mtd_id = mtd_id;
	get_imei(imei);
	strncpy((void *)info.imei_id,(const void*)imei, MAX_IMEI_CHARS_SIZE);
	set_device_info(&info);

    DbgFuncExit();
    
    return 0;
}

/** 
   * @brief 	��֤���ܵ�ʵ��
   *		��֤�ɹ����˳�ѭ��
   *		��֤ʧ�ܣ�������������֤
   * 
   */
void itop_authorize_loop(void)
{
	unsigned char auth_state;
	
	DbgFuncEntry();

	auth_state = get_authorize_status();
	if(auth_state != AUTHORIZE_NO)
	{
		authorize_socket_init();
	}
	else
	{
		DbgGood("system is authorized!\r\n");
	}
	
	DbgFuncExit();
}


