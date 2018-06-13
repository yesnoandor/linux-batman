/****************************************Copyright (c)**************************************************                         
** File name:			gb905_update_common.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-16
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

#include	"misc/bcd.h"
#include	"misc/check.h"
#include	"misc/endian.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/ctrl/gb905_control.h"
#include	"module/gb905_update/gb905_update_common.h"
#include	"module/gb905_update/isu/gb905_update_isu.h"
#include	"module/gb905_update/meter/gb905_update_meter.h"


#include	"middleware/info/device.h"
#include	"middleware/info/update.h"
#include	"middleware/db/file/db_file_bin.h"
#include	"middleware/db/file/db_file_path.h"
#include	"middleware/socket/update_socket.h"
#include	"middleware/event/fleety_event.h"

#define		DEBUG_Y
#include	"libs/debug.h"


#define		MAGIC_NUMBER						0x2424		// '$$'

#define		MESSAGE_GET_INFO_REQ				0x0001
#define		MESSAGE_GET_CODE_REQ				0x0002
#define		MESSAGE_FINISH_REQ					0x0003
#define		MESSAGE_CANCEL_REP					0x0004

#define		MESSAGE_GET_INFO_REP				0xA001
#define		MESSAGE_GET_CODE_REP				0xA002
#define		MESSAGE_FINISH_REP					0xA003
#define		MESSAGE_CANCEL_REQ					0xA004


static int	gb905_update_download_size;

/** 
* @brief 	����������������(�����ļ��Ѿ�׼����)
* @param  device_type	�������豸����
*
*/
static void gb905_update_peripheral_device(unsigned char device_type)
{
	DbgFuncEntry();

	switch (device_type)
	{
		case GB905_DEVICE_MCU:
			//gb905_update_isu_start();
			break;

		case GB905_DEVICE_METER:
			gb905_update_meter_start();
			break;

		case GB905_DEVICE_TOPLIGHT:
			//gb905_update_toplight_start();
			break;

		default:
			DbgWarn("don't support this device type(%d)\r\n",device_type);
			break;
	}

	DbgFuncExit();
}

/** 
* @brief 	���յ��Ĵ�����뵽bin   �ļ�
* @param  device_type	�������豸����
* @param  offset	�˴δ���ε�ƫ��
* @param  buf  	�˴δ���εĻ���
* @param  size  	�˴δ���εĳ���
*
*/
static bool gb905_update_bin(unsigned char device_type,int offset,unsigned char *buf,int size)
{
	bool ret = false;
	char path[MAX_PATH_CHARS_SIZE];
	
	DbgFuncEntry();

	switch (device_type)
	{
		case GB905_DEVICE_MCU:
			build_db_path(path,UPDATE_ISU_FILE);
			ret = buf2file(path,offset,buf,size);
			break;

		case GB905_DEVICE_METER:
			build_db_path(path,UPDATE_METER_FILE);
			ret = buf2file(path,offset,buf,size);
			break;

		case GB905_DEVICE_TOPLIGHT:
			build_db_path(path,UPDATE_TOPLIGHT_FILE);
			ret = buf2file(path,offset,buf,size);
			break;

		default:
			DbgWarn("don't support this device type(%d)\r\n",device_type);
			ret = false;
			break;
	}

	DbgFuncExit();

	return ret;
}


/** 
* @brief 	���汾�Ƿ���Ҫ����
* @param device_type	�������豸����
*
* @return	  = true,�汾�����ȷ����������
*/
static bool gb905_update_version_check(unsigned char device_type)
{
	bool ret;
	
	DbgFuncEntry();

	switch (device_type)
	{
		case GB905_DEVICE_MCU:
			ret = gb905_update_isu_version_check();
			break;

		case GB905_DEVICE_METER:
			ret = gb905_update_meter_version_check();
			break;

		case GB905_DEVICE_TOPLIGHT:
			//ret = gb905_update_toplight_version_check();
			break;

		default:
			DbgWarn("don't support this device type(%d)\r\n",device_type);
			ret = false;
			break;
	}

	DbgFuncExit();

	return ret;
}



/** 
* @brief 	����GB905  ����Э���header   ���ݽṹ
* @param header	GB905  ����Э���header  ��ַ
* @param msg_id	GB905  ����Э�����Ϣid 
* @param msg_len  	GB905  ����Э�����Ϣ�峤��
*
*/
static void gb905_update_build_header(gb905_update_header_t * header, unsigned short msg_id, unsigned short msg_len)
{
	device_info_t info;
	unsigned int mtd_id;
	static 	unsigned int msg_seq_number = 0;

	DbgFuncEntry();

	get_device_info(&info);
	
	memset(header,0x00,sizeof(gb905_update_header_t));
	
	header->magic_id = MAGIC_NUMBER;
	header->msg_id = EndianReverse16(msg_id);
	header->msg_len = EndianReverse16(msg_len);

	header->protocol_version = 0;
	header->uuid_device_id.vendor_id = GB905_FLEETY_VENDOR_ID;
	header->uuid_device_id.product_id = 0x00;

	mtd_id = info.mtd_id % 1000000;
	decimal2bcd(mtd_id,header->uuid_device_id.mtd_id,3);

	header->msg_serial_number = msg_seq_number;
	msg_seq_number++;

	DbgFuncExit();
}

/** 
* @brief 	����GB905  ����Э���������Ϣ������Ϣ��
* @param body	GB905  ����Э���������Ϣ������Ϣ���ַ
*
*/
static void gb905_update_build_info_req(gb905_update_info_req_body_t * body)
{
	gb905_update_param_t update_param;
	
	DbgFuncEntry();
	
	get_update_param((unsigned char *)&update_param);

	body->device_type = update_param.device_info.device_type;
	body->vendor_id = update_param.device_info.vendor_id;

	DbgFuncExit();
}

/** 
* @brief 	����GB905  ����Э�����������������Ϣ��
* @param body	GB905  ����Э�����������������Ϣ���ַ
* @param address	GB905  ����Э�����������������Ϣ�嵱ǰ���ĵ�ַ
*
*/
static void gb905_update_build_code_req(gb905_update_code_req_body_t * body,unsigned int address)
{
	gb905_update_param_t update_param;
	
	DbgFuncEntry();
	
	get_update_param((unsigned char *)&update_param);

	body->base_info.device_type = update_param.device_info.device_type;
	body->base_info.vendor_id = update_param.device_info.vendor_id;
	body->address = address;

	DbgFuncExit();
}

/** 
* @brief 	�Է��������ص�������Ϣ�����Ӧ��
* @param buf	Ӧ�����Ϣ���ַ
* @param len	Ӧ�����Ϣ�峤��
*
*/
static void gb905_update_info_ack(unsigned char * buf,int len)
{
	gb905_update_info_rep_body_t * update_info;
	
	DbgFuncEntry();
	
	//get_update_param((unsigned char *)&update_param);
	
	update_info = (gb905_update_info_rep_body_t *)buf;
	update_info->total_length = EndianReverse32(update_info->total_length);
	update_info->check_sum = EndianReverse32(update_info->check_sum);
	
	DbgPrintf("device_type = 0x%02x\r\n",update_info->base_info.device_type);
	DbgPrintf("vendor_id = 0x%02x\r\n",update_info->base_info.vendor_id);
	DbgPrintf("hw_version = 0x%02x\r\n",update_info->hw_version);
	DbgPrintf("main_sw_version = 0x%02x\r\n",update_info->sw_version[0]);
	DbgPrintf("vice_sw_version = 0x%02x\r\n",update_info->sw_version[1]);
	DbgPrintf("totol_length = %d\r\n",update_info->total_length);
	DbgPrintf("check_sum = %d\r\n",update_info->check_sum);
	
	set_update_info((unsigned char *)update_info);
	//update_info_save(info_rep);
	
	if(gb905_update_version_check(update_info->base_info.device_type))
	{
		gb905_update_download_size = 0;
		gb905_update_code_req(gb905_update_download_size);
	}
	else
	{
		unsigned char result;
		
		result = get_update_result();
		gb905_send_update_result(result);
	}

	DbgFuncExit();
}

/** 
* @brief 	�Է��������صĴ���εĴ���
* @param offset 	��ǰ����ε�ƫ�Ƶ�ַ
* @param buf		��ǰ����εĻ����ַ
* @param size		��ǰ����εĴ�С
*
*/
void gb905_update_get_code_treat(int offset,unsigned char*buf,unsigned short size)
{
	gb905_update_info_t update_info;
	
	DbgFuncEntry();

	get_update_info((unsigned char *)&update_info);
	
	gb905_update_bin(update_info.base_info.device_type,offset,buf,size);
	
	gb905_update_download_size += size;
	
	if(0 == update_info.total_length)
	{
		DbgError("update total length is zeor!!!\r\n");
	}
	else if(gb905_update_download_size < update_info.total_length)
	{
		gb905_update_code_req(gb905_update_download_size);
	}
	else
	{
		gb905_update_complete_req();
	}
	
	DbgFuncExit();
}


/** 
* @brief 	�Է��������ص�������������Ӧ��Ĵ���
* @param buf	Ӧ�����Ϣ���ַ
* @param len	Ӧ�����Ϣ�峤��
*
*/
static void gb905_update_code_ack(unsigned char * buf,int len)
{
	int offset;
	unsigned short size;
	
	gb905_update_code_rep_body_t *body;
	
	DbgFuncEntry();
	
	body = (gb905_update_code_rep_body_t *)buf;
	offset = EndianReverse32(body->address);
	size = EndianReverse16(body->size);

	DbgPrintf("offset = 0x%x\r\n",offset);
	DbgPrintf("size = 0x%x\r\n",size);
	
	gb905_update_get_code_treat(offset,body->content,size);
		
	DbgFuncExit();
}


/** 
* @brief 	�Է��������ص������������Ӧ��Ĵ���
* @param buf	Ӧ�����Ϣ���ַ
* @param len	Ӧ�����Ϣ�峤��
*
*/
static void gb905_update_complete_ack(unsigned char * buf,int len)
{
	fleety_event_t event;
	gb905_update_complete_req_ack_body_t * update_complete_rep;
	
	DbgFuncEntry();
	
	update_complete_rep = (gb905_update_complete_req_ack_body_t *)buf;
	
	DbgPrintf("finish_rep->device_type = 0x%x\r\n",update_complete_rep->device_type);
	DbgPrintf("finish_rep->vendor_id = 0x%x\r\n",update_complete_rep->vendor_id);

	gb905_update_peripheral_device(update_complete_rep->device_type);

	// exit update socket
	event.id = UPDATE_SOCKET_EXIT_EVENT;
	event.param = 0;
	event.priority = DAFAULT_PRIORITY;
	fleety_event_push(&event);
	
	DbgFuncExit();
}

/** 
* @brief 	�Է���������ȡ��������Ӧ��
* @param buf	Ӧ�����Ϣ���ַ
* @param len	Ӧ�����Ϣ�峤��
*
*/
static void gb905_update_cancel_ack(unsigned char device_type,unsigned char vendor_id)
{
	fleety_event_t event;

	gb905_update_info_req_t update_cancel_rep;
	
	DbgFuncEntry();
	
	gb905_update_build_header(&update_cancel_rep.header,MESSAGE_CANCEL_REP,sizeof(gb905_update_info_req_body_t));
	gb905_update_debug_header(&update_cancel_rep.header);

	update_cancel_rep.body.device_type = device_type;
	update_cancel_rep.body.vendor_id = vendor_id;
	
	gb905_update_send_data((unsigned char *)&update_cancel_rep,sizeof(update_cancel_rep));
	sleep(3);

	// exit update socket
	event.id = UPDATE_SOCKET_EXIT_EVENT;
	event.param = 0;
	event.priority = DAFAULT_PRIORITY;
	fleety_event_push(&event);

	//clear_update_info();
	//clear_update_socket_info();
	
	DbgFuncExit();
}

/** 
* @brief 	�Է�����ȡ�������Ĵ���
* @param buf	ȡ����������Ϣ���ַ
* @param len	ȡ��������Ϣ���峤��
*
*/
static void gb905_update_cancel_req_treat(unsigned char * buf,int len)
{
	gb905_update_cancel_req_body_t * body;
	
	DbgFuncEntry();

	body = (gb905_update_cancel_req_body_t *)buf;
	
	if(body->vendor_id == GB905_FLEETY_VENDOR_ID)
	{
		gb905_update_cancel_ack(body->device_type,body->vendor_id);
		set_update_result(GB905_UPDATE_SERVER_CANCEL);
	}
	else
	{
		DbgWarn("\r\n update cancel vendor id mismatch!!!\r\n");
	}
			
	DbgFuncExit();

}




/** 
* @brief 	������������Э���ħ����
* @param buf 		ȡ����������Ϣ���ַ
* @param len 		ȡ��������Ϣ���峤��
* @param offset	���ҵ�ħ���ֵ�ƫ�Ƶ�ַ
*
* @return	�Ƿ���ҵ�
*		=true, ���ҵ�
*		=false, û�в��ҵ�
*/
static bool gb905_update_find_magic_char(unsigned char *buf, int len,unsigned int * offset)
{
	unsigned short * p_magic;
	unsigned short magic;
	
	int index=0;
	bool ret = false;
	
	DbgFuncEntry();
	
	*offset = 0;
	
	while(index < len)
	{
		p_magic = (unsigned short *)(buf+index);
		magic = EndianReverse16(*p_magic);
		
		if(magic == MAGIC_NUMBER)
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

static int gb905_update_get_a_full_msg(unsigned char * buf,int len,buff_mgr_t *msg)
{
	bool ret;
	unsigned int head_offset = 0;
	int offset;
	gb905_update_header_t *header;
	unsigned short msg_lens;
	
	DbgFuncEntry();

	ret = gb905_update_find_magic_char(buf,len,&head_offset);

	if(ret && (len - head_offset >= sizeof(gb905_update_header_t)))
	{
		header = (gb905_update_header_t *)(buf+head_offset);
		msg_lens = EndianReverse16(header->msg_len);
		if(len - head_offset >= sizeof(gb905_update_header_t) + msg_lens + 2)
		{
			DbgPrintf("head_offset = %d\r\n",head_offset);
			
			msg->buf = (buf + head_offset);
			msg->len = sizeof(gb905_update_header_t) + msg_lens + 2;
			offset = head_offset + sizeof(gb905_update_header_t) + msg_lens + 2;
		}

		else
		{
			msg->len =0;
			offset = head_offset;
		}
	}

	else
	{
		msg->len =0;
		offset = head_offset;
	}

	DbgFuncExit();
	
	return offset;
}

static bool gb905_update_parse_header(buff_mgr_t *msg)
{
	bool ret = true;
	gb905_update_header_t * header;

	DbgFuncEntry();

	header = (gb905_update_header_t *)(msg->buf);

	header->msg_id = EndianReverse16(header->msg_id);
	header->msg_len = EndianReverse16(header->msg_len);
	
	if( header->msg_len != msg->len - sizeof(gb905_update_header_t) - 2)
	{
		ret = false;

		DbgError("msg len is invaild!\r\n");
		return ret;
	}
	
	if( !is_bcd((unsigned char *)(&header->uuid_device_id), sizeof(uuid_device_id_t)) )
	{
		DbgError("update uuid is invaild!\r\n");
		
		ret = false;
		return ret;
	}

	DbgFuncExit();

	return ret;
}

static bool gb905_update_parse_protocol(buff_mgr_t *msg)
{
	bool ret = true;
	
	gb905_update_header_t * header;
	unsigned short msg_no;

	DbgFuncEntry();

	header = (gb905_update_header_t *)(msg->buf);
	msg_no = header->msg_id;
	
	DbgPrintf("msg_no = 0x%x\r\n",msg_no);
	
	switch(msg_no)
	{
		case MESSAGE_GET_INFO_REP:
			gb905_update_info_ack(msg->buf + sizeof(gb905_update_header_t),header->msg_len);
			break;
			
		case MESSAGE_GET_CODE_REP:
			gb905_update_code_ack(msg->buf + sizeof(gb905_update_header_t),header->msg_len);
			break;
			
		case MESSAGE_FINISH_REP:
			gb905_update_complete_ack(msg->buf + sizeof(gb905_update_header_t),header->msg_len);
			break;
			
		case MESSAGE_CANCEL_REQ:
			gb905_update_cancel_req_treat(msg->buf + sizeof(gb905_update_header_t),header->msg_len);
			break;
		
		default:
			DbgError("don't support this msg no(0x%x)\r\n",msg_no);
			ret = false;
			break;
	}
	
	DbgFuncExit();

	return ret;
}

static int gb905_update_parse_a_full_msg(buff_mgr_t *msg)
{
	int ret;

	DbgFuncEntry();

	ret = gb905_update_parse_header(msg);
	if( ret < 0)
	{
		return ret;
	}

	ret = gb905_update_parse_protocol(msg);

	DbgFuncExit();

	return ret;
}




/** 
   * @brief 	GB905  ��������Э��Ľ���
   * @param buf		��Ž����Ժ����Ϣ����
   * @param len  		��Ž�����ǰ����Ϣ����
   * 
   * @return			��Ž����Ժ����Ϣ���� �����Ȼ���٣�
   */
int gb905_update_protocol_ayalyze(unsigned char * buf,int len)
{
	buff_mgr_t msg;
	unsigned char * msg_buf;
	int msg_len;
	int offset;
	unsigned short crc;
	
	DbgFuncEntry();
	
	msg_buf = buf;
	msg_len = len;
	
	do{
		memset(&msg,0x00,sizeof(msg));
		offset = gb905_update_get_a_full_msg(msg_buf,msg_len,&msg);

		#if 1
		{
			int i;
				
			DbgPrintf("offset = %d\r\n",offset);
			DbgPrintf("len = %d\r\n",msg.len);
			
			for(i=0;i<msg.len;i++)
			{
				DbgPrintf("update_msg_buf[%d] = 0x%2x\r\n",i,msg.buf[i]);
			}
		}
		#endif

		if(offset && msg.len)
		{
			crc = crc_computer(msg.buf,msg.len-2);
			if(*(unsigned short*)&msg.buf[msg.len-2] != EndianReverse16(crc))
			{
				DbgError("\r\n update crc verify failed!\r\n");
			}

			else
			{
				gb905_update_parse_a_full_msg(&msg);
			}
			
		}
		
		msg_buf += offset;
		msg_len -= offset;
		
	}while(offset && msg.len> 0&& msg_len> 0);
	
	DbgFuncExit();
	
	return (len - msg_len);
}

/** 
* @brief 	��������������������
* @param buf	 	��ŷ������ݵ���Ϣ����
* @param len		��ŷ������ݵ���Ϣ����
*
*/
void gb905_update_send_data(unsigned char * buf, int len)
{
	unsigned short crc;

	DbgFuncEntry();
	
	crc = crc_computer((unsigned char *)&buf[0],len - 2);
	*(unsigned short *)(&buf[len-2]) = crc;

	DbgPrintf("crc = 0x%x\r\n",crc);
	DbgPrintf("len = 0x%x\r\n",len);

	#if 0
	int i;
	for(i=0;i<len;i++)
	{
		DbgPrintf("update_send_buf[%d] = 0x%x\r\n",i,buf[i]);
	}
	#endif
	
	update_socket_send(buf,len);

	DbgFuncExit();
}

/** 
* @brief 	�����������������Ϣ
*
*/
void gb905_update_info_req(void)
{
	gb905_update_info_req_t update_info_req;
	
	DbgFuncEntry();
	
	gb905_update_build_header(&update_info_req.header,MESSAGE_GET_INFO_REQ,sizeof(gb905_update_info_req_t));
	gb905_update_debug_header(&update_info_req.header);

	gb905_update_build_info_req(&update_info_req.body);

	gb905_update_send_data((unsigned char *)&update_info_req,sizeof(gb905_update_info_req_t));

	DbgFuncExit();
}

/** 
* @brief 	���������������
* @param offset	����ε�ƫ����	
*
*/
void gb905_update_code_req(int offset)
{
	gb905_update_code_req_t update_code_req;
	
	DbgFuncEntry();
	
	gb905_update_build_header(&update_code_req.header,MESSAGE_GET_CODE_REQ,sizeof(gb905_update_code_req_body_t));
	gb905_update_debug_header(&update_code_req.header);
	
	gb905_update_build_code_req(&update_code_req.body,EndianReverse32(offset));
	
	gb905_update_send_data((unsigned char *)&update_code_req,sizeof(gb905_update_code_req_t));

	DbgFuncExit();
}

/** 
* @brief 	������ɺ�����������͵�����
*
*/
void gb905_update_complete_req(void)
{
	gb905_update_complete_req_t update_complete_req;
	
	DbgFuncEntry();
	
	gb905_update_build_header(&update_complete_req.header,MESSAGE_FINISH_REQ,sizeof(gb905_update_info_req_body_t));
	gb905_update_debug_header(&update_complete_req.header);

	gb905_update_build_info_req(&update_complete_req.body);
	
	gb905_update_send_data((unsigned char *)&update_complete_req,sizeof(update_complete_req));

	DbgFuncExit();
}


void gb905_update_debug_header(gb905_update_header_t * header)
{
	DbgFuncEntry();

	DbgPrintf("msg_id = 0x%x\r\n",header->msg_id);
	DbgPrintf("msg_len = 0x%x\r\n",header->msg_len);
	
	DbgPrintf("vendor_id = 0x%02x\r\n",header->uuid_device_id.vendor_id);
	DbgPrintf("product_id = 0x%02x\r\n",header->uuid_device_id.product_id);
	DbgPrintf("mtd_id[0] = 0x%02x\r\n",header->uuid_device_id.mtd_id[0]);
	DbgPrintf("mtd_id[1] = 0x%02x\r\n",header->uuid_device_id.mtd_id[1]);
	DbgPrintf("mtd_id[2] = 0x%02x\r\n",header->uuid_device_id.mtd_id[2]);
	DbgPrintf("msg_serial_number = %d\r\n",header->msg_serial_number);
	
	DbgFuncExit();
}



