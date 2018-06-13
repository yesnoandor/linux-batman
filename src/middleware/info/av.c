/****************************************Copyright (c)**************************************************                         
** File name:			av.c
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

#include	"middleware/info/av.h"

#define		DEBUG_Y
#include	"libs/debug.h"

static photo_ctrl_info_t photo_ctrl_info;
static photo_progress_info_t photo_progress_info;


/** 
* @brief 	获取拍照控制参数
* @param buf 	  缓冲地址
*
*/
void get_photo_ctrl_info(unsigned char * buf)
{
	DbgFuncEntry();
	
	memcpy(buf,&photo_ctrl_info,sizeof(photo_ctrl_info_t));
	
	DbgFuncExit();
}

/** 
* @brief 	设置拍照控制参数
* @param buf 	 缓冲地址
* 
*/
void set_photo_ctrl_info(unsigned char *buf)
{
	DbgFuncEntry();

	memset(&photo_ctrl_info,0x00,sizeof(photo_ctrl_info_t));
	memcpy(&photo_ctrl_info,buf,sizeof(photo_ctrl_info_t));

	DbgFuncExit();
}

void debug_photo_ctrl_info(void)
{
	DbgFuncEntry();

	DbgPrintf("status = %d\r\n",photo_ctrl_info.status);	
	DbgPrintf("total = %d\r\n",photo_ctrl_info.total);
	DbgPrintf("index = %d\r\n",photo_ctrl_info.index);
	DbgPrintf("reason = %d\r\n",photo_ctrl_info.reason);
    DbgPrintf("camear_id = %d\r\n",photo_ctrl_info.camear_id);
	
	DbgPrintf("interval = %d\r\n",photo_ctrl_info.interval);
	DbgPrintf("flag = %d\r\n",photo_ctrl_info.flag);

	DbgPrintf("resolution = %d\r\n",photo_ctrl_info.params.resolution);
	DbgPrintf("quality = %d\r\n",photo_ctrl_info.params.quality);
	DbgPrintf("brightness = %d\r\n",photo_ctrl_info.params.brightness);
	DbgPrintf("contrast = %d\r\n",photo_ctrl_info.params.contrast);
	DbgPrintf("saturation = %d\r\n",photo_ctrl_info.params.saturation);
	DbgPrintf("chroma = %d\r\n",photo_ctrl_info.params.chroma);

	DbgPrintf("serial_number = %d\r\n",photo_ctrl_info.serial_number);
	
	DbgFuncExit();
}

/** 
* @brief 	获取当前AV  上传的进度
* @param buf 	  缓冲地址
*
*/
void get_photo_progress_info(unsigned char * buf)
{
	DbgFuncEntry();
	
	memcpy(buf,&photo_progress_info,sizeof(photo_progress_info_t));
	
	DbgFuncExit();
}

/** 
* @brief 	设置当前AV  上传的进度
* @param buf 	 缓冲地址
* 
*/
void set_photo_progress_info(unsigned char *buf)
{
	DbgFuncEntry();

	memset(&photo_progress_info,0x00,sizeof(photo_progress_info_t));
	memcpy(&photo_progress_info,buf,sizeof(photo_progress_info_t));

	DbgFuncExit();
}

void debug_photo_progress_info(void)
{
	DbgFuncEntry();

	DbgPrintf("photo_id = %d\r\n",photo_progress_info.photo_id);
	DbgPrintf("offset = %d\r\n",photo_progress_info.offset);
	DbgPrintf("total = %d\r\n",photo_progress_info.total);
	DbgPrintf("reason = %d\r\n",photo_progress_info.reason);

	DbgFuncExit();
}


