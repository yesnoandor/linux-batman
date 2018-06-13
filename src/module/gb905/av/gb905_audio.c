/****************************************Copyright (c)**************************************************                         
** File name:			gb905_audio.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-02-03
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

#include	"misc/misc.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/av/gb905_audio.h"

#include	"middleware/db/file/db_file_bin.h"
#include	"middleware/db/file/db_file_path.h"
#include	"middleware/socket/fleety_socket.h"


#define		DEBUG_Y
#include	"libs/debug.h"


typedef struct{
	unsigned char reason;			// =0,����¼��; =1, �˿�Ͷ��;  =2, ����¼��
	gb905_timestamp_t start_timestamp;		//  ��ʼʱ��
	gb905_timestamp_t end_timestamp;		// ����ʱ��
}__packed gb905_search_record_t;

typedef struct{
	unsigned short serial_number;		// Ӧ����ˮ��
	unsigned int total_size;			// �����������ݰ���С(����������* 4)
	unsigned int offset;				// ������������������Ӧ���е�ƫ����
	int content;						// ������
}__packed gb905_search_record_ack_t;


typedef struct
{
	unsigned short reason;		// �ϴ���ˮ��
	unsigned int id;			// ��Ƶid
	unsigned int size;			// ��Ƶ���ݴ�С
	unsigned int offset;		// ��ʼ��ַ
	unsigned int content;
}__packed gb905_upload_record_body_t;


/**
* @brief			������Ƶ�ϴ�
* @param reason	�ϴ����յ�ԭ��(��ѯ�ϴ�(0x00) or  ���������ϴ�(���к�))
*
*/
void gb905_audio_upload_record_treat(
	int audio_id,
	int offset,
	unsigned short reason)
{
	struct stat file_state;
	int len;
	char path[MAX_PATH_CHARS_SIZE];
	char name[MAX_PATH_CHARS_SIZE];
	unsigned char * buf;
	
	DbgFuncEntry();

	sprintf(name,"%d.wav",audio_id);
	build_photo_path(path,name);
	
	if(stat(path,&file_state) < 0)
	{
		DbgError("get audio wav file size failed!! \r\n");
		return;
	}
	
	len = (int)file_state.st_size;
	DbgPrintf("len = %d\r\n",len);

	// 3 : magic_start xor magic_end 
	len = len + sizeof(gb905_header_t) + 3 + offsetof(gb905_upload_record_body_t,content);
	DbgPrintf("msg len = %d\r\n",len);
	
	buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("photo upload memory malloc failed!\r\n");
		return;
	}
	memset((void*)buf, 0, len);

	// ����header
	gb905_header_t * header;
	
	header = (gb905_header_t *)(buf + 1);
		
	gb905_build_header(header,MESSAGE_AUDIO_UPLOAD,len - sizeof(gb905_header_t) - 3);
#ifdef DEBUG_Y
	gb905_debug_header(header);
#endif

	// ����body
	gb905_upload_record_body_t * body;

	body = (gb905_upload_record_body_t *)(buf + 1 + sizeof(gb905_header_t));
		
	body->id = audio_id;
	body->size = (unsigned int)file_state.st_size - offset;
	body->offset = offset;
	body->reason = reason;
	file2buf(path,offset,(unsigned char *)&body->content,body->size);

	// ����
	gb905_send_data(MAIN_SOCKET,buf,len);
	
	free(buf);

	DbgFuncExit();
}

/**
* @brief			������Ƶ����
* @param buf	  ��Ƶ�����������ݵ�ַ
* @param len	  ��Ƶ������������ݳ���
*
*/
void gb905_audio_search_record_treat(unsigned char* buf,int len)
{
	gb905_search_record_t * search_record;
	gb905_header_t * header;
	int records = 0;
	
	DbgFuncEntry();

	header = (gb905_header_t *)(buf + 1);
		
	search_record = (gb905_search_record_t *)(buf + 1 + sizeof(gb905_header_t));

	DbgPrintf("reason = %d\r\n",search_record->reason);
	DbgPrintf("start timestamp is %02x-%02x-%02x %02x:%02x:%02x\r\n",
		search_record->start_timestamp.bcd[0],
		search_record->start_timestamp.bcd[1],
		search_record->start_timestamp.bcd[2],
		search_record->start_timestamp.bcd[3],
		search_record->start_timestamp.bcd[4],
		search_record->start_timestamp.bcd[5]);
	DbgPrintf("stop timestamp is %02x-%02x-%02x %02x:%02x:%02x\r\n",
		search_record->end_timestamp.bcd[0],
		search_record->end_timestamp.bcd[1],
		search_record->end_timestamp.bcd[2],
		search_record->end_timestamp.bcd[3],
		search_record->end_timestamp.bcd[4],
		search_record->end_timestamp.bcd[5]);
	
	// records = audio_db_get_record_count(search_pic);
	
	// magic_start xor magic_end 
	unsigned char *ack_buf;
	len = sizeof(gb905_header_t) + 3 + offsetof(gb905_search_record_ack_t,content) + records * sizeof(int);
	DbgPrintf("len = %d\r\n",len);
			
	ack_buf = (unsigned char *)malloc(len);
	if(!buf)
	{
		DbgError("photo search ack memory malloc failed!\r\n");
		return;
	}
	memset((void*)buf, 0, len);
	
	// ����header
	gb905_header_t * ack_header;
	ack_header = (gb905_header_t *)(ack_buf + 1);
			
	gb905_build_header(ack_header,MESSAGE_RECORD_SEARCH_ACK,len - sizeof(gb905_header_t) - 3);
#ifdef DEBUG_Y
	gb905_debug_header(ack_header);
#endif
	
	// ����body
	gb905_search_record_ack_t * body;
	
	body = (gb905_search_record_ack_t *)(ack_buf + 1 + sizeof(gb905_header_t));
	body->serial_number = header->msg_serial_number;
	body->offset = 0;
	body->total_size = records * sizeof(int);
	
	// photo_db_get_record(&body->content);
		
	// ����
	gb905_send_data(MAIN_SOCKET,buf,len);
		
	free(buf);
	
	DbgFuncExit();
}


