/****************************************Copyright (c)**************************************************                         
** File name:			gb905_photo.c
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
#include	"misc/endian.h"

#include	"libs/sqlite3/sqlite3.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/av/gb905_audio.h"
#include	"module/gb905/av/gb905_photo.h"

#include	"middleware/info/av.h"
#include	"middleware/event/export/fleety_export.h"

#include	"middleware/db/file/db_file_bin.h"
#include	"middleware/db/file/db_file_path.h"
#include	"middleware/db/sqlite3/photo_sqlite3.h"

#include	"middleware/socket/fleety_socket.h"


#include	"app/av/fleety_photo.h"

#define		DEBUG_Y
#include	"libs/debug.h"



typedef struct
{
	unsigned short reason;		// 上传原因 
	unsigned int id;			// 图像id
	unsigned char camera_id;	// 摄像头id
	unsigned int size;			// 图像数据大小
	unsigned int offset;		// 起始地址
	unsigned int content;
}__packed gb905_upload_pic_body_t;


typedef struct{
	unsigned char camera_id;	// 摄像头id
	unsigned short cmd;			// 拍摄命令：0 表示停止拍摄；0xFFFF 表示录像；其它表示拍照张数
	unsigned short interval;	// 拍照间隔/录像时间  ：单位秒，0 表示按最小间隔拍照或一直录像
	unsigned char flag;			// 标志：=1, 保存；=0, 实时上传
	unsigned char resolution;	// 分辨率
	unsigned char quality;		// 图像/视频质量
	unsigned char brightness;	// 亮度
	unsigned char contrast;		// 对比度
	unsigned char saturation;	// 饱和度
	unsigned char chroma;		// 色度
}__packed gb905_take_pic_t;


typedef struct{
	unsigned short serial_number;		// 应答流水号
	unsigned int total_size;			// 检索总项数据包大小(检索出的项* 4)
	unsigned int offset;				// 本包数据在整个检索应答中的偏移量
	int content;						// 检索项
}__packed gb905_search_pic_ack_t;


typedef struct{
	unsigned char type;			// 类型
	unsigned int  file_id;		// 文件id
	int offset;					// 起始位置
}__packed gb905_upload_av_request_t;


/**
* @brief			国标拍照上传
* @param photo_id  照片id,   获得照片的名字和路径
* @param offset       此次上传开始的偏移量
* @param reason	上传拍照的原因(查询上传(0x00) or  立即拍照上传(序列号))
*
*/
void gb905_photo_upload_start(
	int photo_id,
	int offset,
	unsigned short reason)
{
	struct stat file_state;
	char path[MAX_PATH_CHARS_SIZE];
	char name[MAX_PATH_CHARS_SIZE];	

	photo_progress_info_t progress_info;

	DbgFuncEntry();
	
	progress_info.photo_id = photo_id;
	progress_info.reason =  reason;
	progress_info.offset = offset;
	
	build_photo_name(name,photo_id);
	build_photo_path(path,name);
	
	if(stat(path,&file_state) < 0)
	{
		DbgError("get photo jpeg file size failed!! \r\n");
		return;
	}

	progress_info.total = (int)file_state.st_size;

	set_photo_progress_info((unsigned char *)&progress_info);

	DbgFuncExit();
}


/**
* @brief			国标拍照上传
* @param photo_id  照片id,   获得照片的名字和路径
* @param offset       此次上传开始的偏移量
* @param reason	上传拍照的原因(查询上传(0x00) or  立即拍照上传(序列号))
*
*/
void gb905_photo_upload_picture_treat(void)
{
	struct stat file_state;
	int offset,len;
	char path[MAX_PATH_CHARS_SIZE];
	char name[MAX_PATH_CHARS_SIZE];
	unsigned char * buf;
	
	photo_progress_info_t progress_info;
	
	DbgFuncEntry();

	get_photo_progress_info((unsigned char *)&progress_info);

	offset = progress_info.offset;
	len = progress_info.total - progress_info.offset;
	DbgPrintf("len = %d\r\n",len);

	// 3 : magic_start xor magic_end 
	len = len + sizeof(gb905_header_t) + 3 + offsetof(gb905_upload_pic_body_t,content);
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
		
	gb905_build_header(header,MESSAGE_PICTURE_UPLOAD,len - sizeof(gb905_header_t) - 3);
#ifdef DEBUG_Y
	gb905_debug_header(header);
#endif

	// 构造body
	gb905_upload_pic_body_t * body;

	body = (gb905_upload_pic_body_t *)(buf + 1 + sizeof(gb905_header_t));
		
	body->id = progress_info.photo_id;
	body->camera_id = 0;
	body->size = MIN((unsigned int)file_state.st_size - offset,512);
	body->offset = offset;
	body->reason = progress_info.reason;

	build_photo_name(name,progress_info.photo_id);
	build_photo_path(path,name);
	file2buf(path,offset,(unsigned char *)&body->content,body->size);

	// 发送
	gb905_send_data(MAIN_SOCKET,buf,len);
	
	free(buf);

	progress_info.offset += body->size;
	set_photo_progress_info((unsigned char *)&progress_info);

	DbgFuncExit();
}


/**
* @brief			国标摄像立即拍摄命令处理
* @param buf	  立即拍摄整个消息命令的数据地址
* @param len	  立即拍摄整个消息命令的数据长度
*
* @return  返回终端通用应答的状态
*/
unsigned char gb905_photo_take_picture_treat(unsigned char *buf,int len)
{
	gb905_header_t * header;
	gb905_take_pic_t * take_pic;
	photo_ctrl_info_t photo_ctrl_info;
	
	DbgFuncEntry();

	header = (gb905_header_t *)(buf + 1);
	take_pic = (gb905_take_pic_t *)(buf +  1 + sizeof(gb905_header_t));

	if(take_pic->cmd == 0x00)
	{
		DbgPrintf("stop video capture!\r\n");
	}
	else if(take_pic->cmd == 0xFFFF)
	{
		DbgPrintf("start video capture!\r\n");
	}
	else
	{
		photo_ctrl_info.status = 0x01;
        photo_ctrl_info.camear_id = take_pic->camera_id - 1;//平台下发ID是1~4；终端摄像头ID是0~3
		photo_ctrl_info.total = EndianReverse16(take_pic->cmd);
        photo_ctrl_info.reason = PHOTO_REASON_PLATFORM;
        photo_ctrl_info.index = 0;
		photo_ctrl_info.interval = MIN(take_pic->interval,MAX_TAKE_PICTURE_INTERVAL);
		photo_ctrl_info.flag = take_pic->flag;
		photo_ctrl_info.params.resolution = take_pic->resolution;
		photo_ctrl_info.params.brightness = take_pic->brightness;
		photo_ctrl_info.params.chroma = take_pic->chroma;
		photo_ctrl_info.params.contrast = take_pic->contrast;
		photo_ctrl_info.params.saturation = take_pic->saturation;
		photo_ctrl_info.params.chroma = take_pic->chroma;

		photo_ctrl_info.serial_number = header->msg_serial_number;

		set_photo_ctrl_info((unsigned char *)&photo_ctrl_info);
		debug_photo_ctrl_info();

		fleety_photo_start();
	}

	DbgFuncExit();
	
	return GB905_RESULT_OK;
}

/**
* @brief			国标拍照检索
* @param buf	  拍照检索命令整个消息体的数据地址
* @param len	  拍照检索命令整个消息体的数据长度
*
*/
void gb905_photo_search_picture_treat(unsigned char* buf,int len)
{
	gb905_search_pic_t * search_pic;
	gb905_header_t * header;
	int records = 0;
	
	DbgFuncEntry();

	header = (gb905_header_t *)(buf + 1);
		
	search_pic = (gb905_search_pic_t *)(buf + 1 + sizeof(gb905_header_t));
    
    //camera_id=0表示查询所有摄像头
    if(search_pic->camera_id)
    {
        search_pic->camera_id = search_pic->camera_id - 1;//平台下发ID是1~4；终端摄像头ID是0~3
    }
    
    DbgPrintf("camera_id = %d\r\n",search_pic->camera_id);
	DbgPrintf("reason = %d\r\n",search_pic->reason);
	DbgPrintf("start timestamp is %02x-%02x-%02x %02x:%02x:%02x\r\n",
		search_pic->start_timestamp.bcd[0],
		search_pic->start_timestamp.bcd[1],
		search_pic->start_timestamp.bcd[2],
		search_pic->start_timestamp.bcd[3],
		search_pic->start_timestamp.bcd[4],
		search_pic->start_timestamp.bcd[5]);
	DbgPrintf("stop timestamp is %02x-%02x-%02x %02x:%02x:%02x\r\n",
		search_pic->end_timestamp.bcd[0],
		search_pic->end_timestamp.bcd[1],
		search_pic->end_timestamp.bcd[2],
		search_pic->end_timestamp.bcd[3],
		search_pic->end_timestamp.bcd[4],
		search_pic->end_timestamp.bcd[5]);
	

	// 根据检索条件(摄像头id, 拍照原因,  起始时间和结束时间)
	// 获得记录数
	 records = photo_sqlite3_get_record_count(search_pic);
	
		
	// magic_start xor magic_end 
	unsigned char *ack_buf;
    int ack_len;
	ack_len = sizeof(gb905_header_t) + 3 + offsetof(gb905_search_pic_ack_t,content) + records * sizeof(int);
	DbgPrintf("len = %d\r\n",ack_len);
			
	ack_buf = (unsigned char *)malloc(ack_len);
	if(!ack_buf)
	{
		DbgError("photo search ack memory malloc failed!\r\n");
		return;
	}
	memset((void*)ack_buf, 0, ack_len);
	
	// 构造header
	gb905_header_t * ack_header;
	ack_header = (gb905_header_t *)(ack_buf + 1);
			
	gb905_build_header(ack_header,MESSAGE_PICTURE_SEARCH_ACK,ack_len - sizeof(gb905_header_t) - 3);
#ifdef DEBUG_Y
	gb905_debug_header(ack_header);
#endif
	
	// 构造body
	gb905_search_pic_ack_t * body;
	
	body = (gb905_search_pic_ack_t *)(ack_buf + 1 + sizeof(gb905_header_t));
	body->serial_number = header->msg_serial_number;
	body->offset = 0;
	body->total_size = EndianReverse32(records * sizeof(int));

	// 根据检索条件(摄像头id, 拍照原因,  起始时间和结束时间)
	// 获得每个照片ID  
	photo_sqlite3_get_record(search_pic,&body->content);
		
	// 发送
	gb905_send_data(MAIN_SOCKET,ack_buf,ack_len);
		
	free(ack_buf);
	
	DbgFuncExit();
}



/**
* @brief			图像音频上传请求
* @param buf	  拍照检索命令消息体的数据地址
* @param len	  拍照检索令消息体的数据长度
*
*/
void gb905_photo_upload_av_request_treat(unsigned char* buf,int len)
{
	gb905_upload_av_request_t * upload_av_request;

	upload_av_request = (gb905_upload_av_request_t *)buf;

    upload_av_request->file_id = EndianReverse32(upload_av_request->file_id);
    
	switch (upload_av_request->type)
	{
		case 0x00:		// 照片
			gb905_photo_upload_start(upload_av_request->file_id,upload_av_request->offset,0x00);
			gb905_photo_upload_picture_treat();
			break;

		case 0x01:		// 音频
			gb905_photo_upload_start(upload_av_request->file_id,upload_av_request->offset,0x00);
			gb905_photo_upload_picture_treat();
			break;

		case 0x02:		// 视频
			break;

		default:
			DbgWarn("don't support this av type(%d)!\r\n",upload_av_request->type);
			break;
	}
}

