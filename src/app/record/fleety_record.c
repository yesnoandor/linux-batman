/****************************************Copyright (c)**************************************************                         
** File name:			fleety_record.c
** Descriptions:		��ȡ�洢����Ϣ��¼�����в���
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-17
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

#include	"libs/list.h"


#include	"misc/misc.h"
#include	"misc/endian.h"

#include	"module/gb905/gb905_common.h"

#include	"middleware/socket/fleety_socket.h"
#include	"middleware/info/setting.h"

#include	"middleware/db/sqlite3/operation_sqlite3.h"
#include	"middleware/db/sqlite3/report_sqlite3.h"

#include	"app/record/fleety_record.h"

//#define		DEBUG_Y
#include	"libs/debug.h"


#define		MAX_SUPPLEMENT_COUNT	20
static list_t *	msg_record_list[MAX_SOCKET_NUM];

//���浽��������ݽṹ
typedef struct {
	unsigned short id;				//��Ϣid
	unsigned short seq;				//��Ϣ��ˮ��
	int try_count;					//���Է��ʹ���
	time_t last_time;				//�����ʱ��
	unsigned char * msg_buf;		//���浽��������ݵ�ַ
	int msg_len;					//���浽��������ݳ���
}msg_record_t;

/**
* @brief ��ӡ����������Ϣ
* 
* 
*/
static bool record_put_elem(msg_record_t *record)  
{
	int i;
	
	if(!record)
		return false;

	DbgFuncEntry();
	
	DbgPrintf("id = 0x%x\r\n",record->id);
	DbgPrintf("seq = 0x%x\r\n",record->seq);
	DbgPrintf("try_count = %d\r\n",record->try_count);
	DbgPrintf("msg_len = %d\r\n",record->msg_len);
	DbgPrintf("msg_buf = 0x%x\r\n",record->msg_buf);

	for(i=0;i<record->msg_len;i++)
	{
		DbgPrintf("msg[%d] = 0x%x\r\n",i,record->msg_buf[i]);
	}

	DbgFuncExit();
	
	return true;  
}


/**
* @brief �ָ���¼�����ݲ�������������
* 
* 
*/
void fleety_record_init(void)
{
	list_t * list;
	int index;

	DbgFuncEntry();

	for(index=0;index<MAX_SOCKET_NUM;index++)
	{
		list = list_create(sizeof(msg_record_t));
		if(!list)
		{
			DbgError("msg record list create failed!\r\n");
		}
		
		msg_record_list[index] = list;
	}

	//�����ݿ�ָ�����
	report_sqlite3_restore_record();
	operation_sqlite3_restore_record();
	
	DbgFuncExit();
}

/**
* @brief ������������
* 
* 
*/
void fleety_record_deinit(void)
{
	int index;

	for(index=0;index<MAX_SOCKET_NUM;index++)
	{
		list_destory(msg_record_list[index]);
	}
}

/**
* @brief ��������Ϣ���ӵ�����������
* @param socket_index			��������
* @param header				��Ϣͷ��Ϣָ��
* @param buf					������Ϣ�ĵ�ַ
* @param len					������Ϣ�ĳ���
*
*/
bool gb905_record_msg(unsigned char socket_index,gb905_header_t * header,unsigned char  * buf,int len)
{
	bool ret = true;
	list_t * list;
	msg_record_t msg_record;
	time_t cur_time;

	DbgFuncEntry();

	list = msg_record_list[socket_index];

	if(list->node_count >= MAX_NODE_COUNT)
	{
		DbgWarn("MAX_NODE_COUNT = %d\r\n",list->node_count);
		ret = false;
		return ret;
	}
	

	time(&cur_time);

	msg_record.id = EndianReverse16(header->msg_id);
	msg_record.seq = EndianReverse16(header->msg_serial_number);
	msg_record.try_count = 0;
	msg_record.last_time = cur_time;
	msg_record.msg_len = len;
	msg_record.msg_buf = (unsigned char *)malloc(len);
	if(!msg_record.msg_buf)
	{
		DbgError("malloc msg record failed!\r\n");
		ret = false;
		return ret;
	}
	memcpy(msg_record.msg_buf,buf,len);
	
	node_append(list,(void *)&msg_record);

	DbgFuncExit();

	return ret;
}



/**
* @brief ɾ��ƽ̨��Ӧ�����Ϣ��������
* @param socket_index			��������
* @param ack					Ӧ����Ϣ�����Ϣ�ĵ�ַ
*
*/
bool gb905_record_cancel(unsigned char socket_index,ack_body_t *ack)
{
	int i;
	msg_record_t msg_record;
	bool ret = false;
	list_t * list;

	DbgFuncEntry();

	list = msg_record_list[socket_index];

	for(i=0;i<list->node_count;i++)
	{
		if(elem_get(list,i,(void *)&msg_record))
		{
			DbgPrintf("id = 0x%x\r\n",msg_record.id);
			DbgPrintf("seq = 0x%x\r\n",msg_record.seq);
			DbgPrintf("try_count = %d\r\n",msg_record.try_count);

			if(ack->id == MESSAGE_LOCATION_SUPPLEMENT_REPORT || ack->id == MESSAGE_LOCATION_REPORT)
			{
				if(ack->seq == msg_record.seq)
				{
					free(msg_record.msg_buf);
					node_delete(list,i);
					ret = true;
					break;
				}
			}
			else if((ack->id == msg_record.id) && (ack->seq == msg_record.seq))
			{
				free(msg_record.msg_buf);
				node_delete(list,i);
				ret = true;
				break;
			}

			else
			{
				DbgPrintf("ack->id = 0x%02x\r\n",ack->id);
			}
		}
		else
		{
			DbgError("can't get elem data!\r\n");
		}
	}


	if(list->node_count == 0)
	{
		//��������Ϊ�գ����´����ݿ�ָ�����		
		report_sqlite3_restore_record();
		operation_sqlite3_restore_record();
	}

	DbgFuncExit();

	return ret;
}

/**
* @brief ������ָ�����ʱ���в���(�����������л�ȡ����������Ϣ)
* @param socket_index			��������
*
*/
int gb905_record_supplement(unsigned char socket_index)
{
	int i,j,count;
	msg_record_t msg_record;
	gb905_header_t * header;
	setting_params_t * setting_params;
	time_t cur_time;
	double delta;
	list_t * list;
	
	DbgFuncEntry();

	list = msg_record_list[socket_index];
	//gb905_debug_list(list);

	setting_params = get_setting_params();

	count = MIN(list->node_count,MAX_SUPPLEMENT_COUNT);

	DbgPrintf("count = %d\r\n",count);
	DbgPrintf("tcp_msg_retry_count = %d\r\n",setting_params->tcp_msg_retry_count);

	for(i=0;i<count;i++)
	{
		if(elem_get(list,i,(void *)&msg_record))
		{
			
			//DbgPrintf("try_count = %d\r\n",msg_record.try_count);
			
			if(msg_record.try_count > setting_params->tcp_msg_retry_count)
			{
				free(msg_record.msg_buf);
				node_delete(list,i);
				//������ˮ��ɾ����Ӧ���ݿ�����(����ϴ�δ�ɹ�)
			}
			else
			{
				// To Do...
				time(&cur_time);
				delta = difftime(cur_time,msg_record.last_time);

				DbgPrintf("seconds = %d\r\n",(int)cur_time);
				DbgPrintf("last seconds = %d\r\n",(int)msg_record.last_time);
				DbgPrintf("delta = %f\r\n",delta);
				DbgPrintf("try_count = %d\r\n",msg_record.try_count);
				
				if((msg_record.try_count + 1) * setting_params->tcp_msg_ack_timeout < delta)
				{
					//����ʱΪ0x0200,����ʱӦ��Ϊ0x0203(����δ�����ݿ��ȡ�����)
					if(msg_record.msg_buf[1]==0x02&&msg_record.msg_buf[2]==0x00)
					{
						msg_record.msg_buf[1] = 0x02;
						msg_record.msg_buf[2] = 0x03;
					}
					
					header = (gb905_header_t * )&msg_record.msg_buf[1];
										
					gb905_debug_header(header);

					gb905_send_data(socket_index,msg_record.msg_buf,msg_record.msg_len);
					
					DbgPrintf("supplement data::");
					for(j=0;j<msg_record.msg_len;j++)
					{
						DbgPrintf("0x%02x ",msg_record.msg_buf[j]);
					}
					DbgPrintf("\r\n");
					
					msg_record.try_count++;
					msg_record.last_time = cur_time;

					elem_set(list,i,(void *)&msg_record);
				}
			}
		}
		else
		{
			DbgError("can't get elem data!\r\n");
			break;
		}
	}

	DbgFuncExit();

	return i;
}
		
/**
* @brief ��ӡ����������Ϣ
* 
* 
*/
void gb905_debug_list(list_t * list)
{
	if(!list)
		return;
		
	list_lamda(list,(bool(*)(void *))record_put_elem);
}
		

