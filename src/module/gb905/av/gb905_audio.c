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
	unsigned char reason;			// =0,正常录音; =1, 乘客投诉;  =2, 报警录音
	gb905_timestamp_t start_timestamp;		//  起始时间
	gb905_timestamp_t end_timestamp;		// 结束时间
}__packed gb905_search_record_t;

typedef struct{
	unsigned short serial_number;		// 应答流水号
	unsigned int total_size;			// 检索总项数据包大小(检索出的项* 4)
	unsigned int offset;				// 本包数据在整个检索应答中的偏移量
	int content;						// 检索项
}__packed gb905_search_record_ack_t;


typedef struct
{
	unsigned short reason;		// 上传流水号
	unsigned int id;			// 音频id
	unsigned int size;			// 音频数据大小
	unsigned int offset;		// 起始地址
	unsigned int content;
}__packed gb905_upload_record_body_t;


/**
* @brief			国标音频上传
* @param reason	上传拍照的原因(查询上传(0x00) or  立即拍照上传(序列号))
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

	// 构造header
	gb905_header_t * header;
	
	header = (gb905_header_t *)(buf + 1);
		
	gb905_build_header(header,MESSAGE_AUDIO_UPLOAD,len - sizeof(gb905_header_t) - 3);
#ifdef DEBUG_Y
	gb905_debug_header(header);
#endif

	// 构造body
	gb905_upload_record_body_t * body;

	body = (gb905_upload_record_body_t *)(buf + 1 + sizeof(gb905_header_t));
		
	body->id = audio_id;
	body->size = (unsigned int)file_state.st_size - offset;
	body->offset = offset;
	body->reason = reason;
	file2buf(path,offset,(unsigned char *)&body->content,body->size);

	// 发送
	gb905_send_data(MAIN_SOCKET,buf,len);
	
	free(buf);

	DbgFuncExit();
}

/**
* @brief			国标音频检索
* @param buf	  音频检索命令数据地址
* @param len	  音频检索命令的数据长度
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
	
	// 构造header
	gb905_header_t * ack_header;
	ack_header = (gb905_header_t *)(ack_buf + 1);
			
	gb905_build_header(ack_header,MESSAGE_RECORD_SEARCH_ACK,len - sizeof(gb905_header_t) - 3);
#ifdef DEBUG_Y
	gb905_debug_header(ack_header);
#endif
	
	// 构造body
	gb905_search_record_ack_t * body;
	
	body = (gb905_search_record_ack_t *)(ack_buf + 1 + sizeof(gb905_header_t));
	body->serial_number = header->msg_serial_number;
	body->offset = 0;
	body->total_size = records * sizeof(int);
	
	// photo_db_get_record(&body->content);
		
	// 发送
	gb905_send_data(MAIN_SOCKET,buf,len);
		
	free(buf);
	
	DbgFuncExit();
}


