/****************************************Copyright (c)**************************************************                         
** File name:			fleety_export.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-27
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

#include	"libs/sqlite3/sqlite3.h"

#include	"misc/endian.h"
#include	"misc/check.h"
#include	"misc/misc.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/av/gb905_photo.h"
#include	"module/gb905/params/gb905_params.h"

#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_notice.h"


#include	"middleware/info/setting.h"
#include	"middleware/info/device.h"
#include	"middleware/db/file/db_file_bin.h"
#include	"middleware/db/file/db_file_path.h"

#include	"middleware/event/export/fleety_export.h"


#include	"middleware/db/sqlite3/db_sqlite3_path.h"
#include	"middleware/db/sqlite3/db_sqlite3.h"
#include	"middleware/db/sqlite3/photo_sqlite3.h"




#define		DEBUG_Y
#include	"libs/debug.h"






#define	UPDATE_FILE						"fleety_batman"

#define	PARAMS_EXPORT_SUCCESS_INFO	    "U盘导出参数成功"
#define	PARAMS_EXPORT_FAIL_INFO	        "请插入U盘再试"

#define	PARAMS_SETTING_SUCCESS_INFO	    "U盘设置参数成功"
#define	PARAMS_SETTING_FAIL_INFO	    "U盘设置参数失败"


/** 
* @brief 	 参数设置的处理(解析参数设置文件，更新参数)
* 
*/
static void update_setting_file(char *file_name,int file_len)
{
	//struct stat buf;
//	unsigned int file_len = 0;
	FILE *fp = NULL;
	int nread;
	char *buff = NULL;
	char path[MAX_PATH_CHARS_SIZE];
	
	DbgFuncEntry();

	build_export_path(path,file_name);
	DbgPrintf("path :%s\r\n",path);

	buff = (char *)malloc(file_len);
	
	fp = fopen(path,"rb");
	if(fp == NULL) 
	{
		DbgError("setting file open error!\r\n");

		free(buff);
		return;
	}

	fseek(fp, 0, SEEK_SET);
	nread = fread(buff,1,file_len,fp);
	
	if(nread == file_len)
	{
		gb905_set_params_treat((unsigned char *)buff,nread);
	}

	fclose(fp);
	free(buff);
	
	DbgFuncExit();
}



/** 
* @brief 	 参数设置处理(解析参数设置文件，更新参数)
* 
*/
static void setting_ctrl_file_parse(char *buf,int len)
{
	setting_ctrl_t * setting_ctrl;

	unsigned short crc;
	unsigned short temp_crc;

	char *name;
	int file_len;
	//int ret;

	DbgFuncEntry();
	
	setting_ctrl = (setting_ctrl_t *)buf;

	name = (char *)&setting_ctrl->file_name;
	
	//crc = (unsigned short)(buf + sizeof(params_setting_crl_t) - 4 + set_file_addr_len + 1); //4:size of  params_setting->set_file_addr  1:/0
	crc = *(unsigned short*)(buf + len - 2);
	crc = EndianReverse16(crc);
	
	temp_crc = crc16_compute((unsigned char *)buf,strlen(name) - 2,NULL);
	if(crc != temp_crc)
	{
		DbgError("crc verify failed!(%d:%d)\r\n",crc,temp_crc);

		ui_send_terminal_remind((unsigned char *)PARAMS_SETTING_FAIL_INFO,sizeof(PARAMS_SETTING_FAIL_INFO));
		return ;
	}
	else
	{
		file_len = EndianReverse32(setting_ctrl->file_len);
		//ret = get_params_file_info(set_file_addr,file_len);

		update_setting_file(name,file_len);
	}
	
	ui_send_terminal_remind((unsigned char *)PARAMS_SETTING_SUCCESS_INFO,sizeof(PARAMS_SETTING_SUCCESS_INFO));
	
	DbgFuncExit();	
}


static void record_photo_ctrl_file_parse(unsigned char *buf,int len)
{
	record_photo_ctrl_t * record_photo_ctrl;
	unsigned short crc;
	char str[80];
	
	DbgFuncEntry();
	
	record_photo_ctrl = (record_photo_ctrl_t *)buf;

	crc = crc16_compute((unsigned char *)record_photo_ctrl,offsetof(record_photo_ctrl_t,crc),NULL);
	
	if(crc == EndianReverse16(record_photo_ctrl->crc))
	{
		// 采集录音
		if(record_photo_ctrl->record_type == 0x01)
		{
			// 发给显示设备显示
			sprintf(str,"record export start!!!");
			// ui_

			// 根据采集类型，调用数据库导出到文件
			#if 0
			// 
			audio_search.reason = rp_export->r_export_type;
			memcpy(audio_search.start_time,rp_export->start_time,sizeof(rp_export->start_time));
			memcpy(audio_search.end_time,rp_export->end_time,sizeof(rp_export->end_time));
			
			if((rp_export->start_time[0]==0 && rp_export->start_time[1]==0)&&(rp_export->end_time[0]==0 && rp_export->end_time[1]==0))
			{
				audio_db_export_record_by_reason(audio_search.reason);
			}

			else
			{
				
				audio_db_export_record_by_time(audio_search.start_time,audio_search.end_time,audio_search.reason);
			}
			#endif

			memset(str,0x00,sizeof(str));
			sprintf(str,"record export finish!!!");			
			//ui_
		}	

		// 采集照片
		if(record_photo_ctrl->photo_en == 0x01)
		{
			// 发给显示设备显示拍照开始导出
			sprintf(str,"photo export start!!!");
            ui_send_terminal_remind((unsigned char*)str,strlen(str)+1);

			// 根据采集类型，调用数据库导出到文件
			photo_sqlite3_get_record_id(record_photo_ctrl);
			
			memset(str,0x00,sizeof(str));
            
            // 发给显示设备显示拍照导出完成
			sprintf(str,"photo export finish!!!");			
			ui_send_terminal_remind((unsigned char*)str,strlen(str)+1);
		}
	}
	

	DbgFuncExit();
}


static void operation_ctrl_file_parse(operation_ctrl_t * operation_ctrl)
{
	char str[80];
	unsigned short crc;
	
	DbgFuncEntry();

	crc = crc16_compute((unsigned char *)operation_ctrl,sizeof(operation_ctrl_t) - 2,NULL);
		
	// 根据采集类型，调用数据库导出到文件
	if(crc == EndianReverse16(operation_ctrl->crc))
	{
		// 发给显示设备显示
		sprintf(str,"operation export start!!!");
		// ui_

		#if 0
		if((operate_export->start_time[0]==0 && operate_export->start_time[1]==0)&&(operate_export->end_time[0]==0 && operate_export->end_time[1]==0))
		{
			operation_db_export_by_state(operate_export->export_class);
			logoutinfo_db_export_by_state(operate_export->export_class);
		}

		else
		{
			bcdtime_to_char(operate_export->start_time,start_time);
			bcdtime_to_char(operate_export->end_time,end_time);
			
			DbgPrintf("\r\n start_time :: %s \r\n",start_time);
			DbgPrintf("\r\n end_time :: %s \r\n",end_time);
			operation_db_export_by_time(start_time,end_time,operate_export->export_class);
			logoutinfo_db_export_by_time(start_time,end_time,operate_export->export_class);
		}
		#endif


		memset(str,0x00,sizeof(str));
		sprintf(str,"operation export finish!!!");
		// ui_
	}
	
	DbgFuncExit();
}


static void fleety_export_update_parse(void)
{
	
}


static void fleety_export_operation_parse(void)
{
	struct stat file_state;
	char path[MAX_PATH_CHARS_SIZE];
	int len = 0;
	operation_ctrl_t operation_ctrl;
	bool ret;
	
	DbgFuncEntry();

	build_export_path(path,EXPORT_OPERATION_CRL_FILE);

	// 得到控制文件的大小
	if(stat(path,&file_state) < 0)
	{
		DbgError("get operation ctrl file size failed!! \r\n");
		return;
	}
	
	len = (int)file_state.st_size;
	DbgPrintf("len = %d\r\n",len);
	
	if(len != sizeof(operation_ctrl_t))
	{
		DbgError("operation ctrl file size mismatch!! \r\n");
		return;
	}

	ret = file2buf(path,0,(unsigned char *)&operation_ctrl,sizeof(operation_ctrl_t));
	if(ret)
	{
		operation_ctrl_file_parse(&operation_ctrl);
	}
	
	DbgFuncExit();
}


/** 
* @brief 	 录像,     照片采集控制文件的解析
* 
*/
static void fleety_export_record_photo_parse(void)
{
	struct stat file_state;
	unsigned char *buf = NULL;
	int len = 0;
	
	char path[MAX_PATH_CHARS_SIZE];
	bool ret;
		
	DbgFuncEntry();

	build_export_path(path,EXPORT_RECORD_PHOTO_CRL_FILE);
	
	// 得到控制文件的大小
	if(stat(path,&file_state) < 0)
	{
		DbgError("get record photo ctrl file size failed!! \r\n");
		return;
	}
	
	len = (int)file_state.st_size;
	DbgPrintf("len = %d\r\n",len);
	
	buf = malloc(len);
	memset(buf,0,len);

	ret = file2buf(path,0,buf,len);
	if(ret)
	{
		record_photo_ctrl_file_parse(buf,len);
	}
	
	free(buf);

	DbgFuncExit();
}

/** 
* @brief 	图片或录音文件导出处理
* 
*/
void fleety_export_record_photo_treat(unsigned char export_type,int id)
{
    
	char c;
	char file_name[MAX_PATH_CHARS_SIZE];
	char save_path[MAX_PATH_CHARS_SIZE];
    char export_path[MAX_PATH_CHARS_SIZE];
	
	FILE *in_fp = NULL;
	FILE *out_fp = NULL;

	DbgFuncEntry();

    //导出图片
    if(export_type == export_type)
    {
        build_photo_name(file_name,(unsigned int)id);
        build_photo_path(save_path,file_name);
    }

    //导出录音
  	else
  	{
        build_audio_name(file_name,(unsigned int)id);
        build_photo_path(save_path,file_name);
    }

	/* build record or photo export file export path*/
	build_export_record_photo_path(export_path,file_name);

#if 1
	/* copy file*/
	in_fp = fopen(export_path,"wb");
	out_fp = fopen(save_path,"rb");
	
	while(!feof(out_fp))
	{
		if(fread(&c,1,1,out_fp))
		{
			fwrite(&c,1,1,in_fp);
		}
	}

	fclose(in_fp);
	fclose(out_fp);
#endif

	DbgFuncExit();
}

/** 
* @brief 	 参数导出处理(收到屏幕的操作，导出对应的参数)
* 
*/
static void fleety_export_params_treat(void)
{
	device_info_t info;
	char path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();
	if(access(UDISK_PATH,F_OK)==0)
	{
    	get_device_info(&info);
    	DbgPrintf("info mtd id = 0x%x\r\n",info.mtd_id);

    	sprintf(path,"%s%d00.CFG",EXPORT_PATH,info.mtd_id);

    	export_setting_params(path);

        ui_send_terminal_remind((unsigned char *)PARAMS_EXPORT_SUCCESS_INFO,sizeof(PARAMS_EXPORT_SUCCESS_INFO));
   
    }
    else
    {
        ui_send_terminal_remind((unsigned char *)PARAMS_EXPORT_FAIL_INFO,sizeof(PARAMS_EXPORT_FAIL_INFO));   
    }
	DbgFuncExit();
}



/** 
* @brief 	 参数设置处理(解析参数设置文件，更新系统的参数)
* 
*/
static void fleety_export_setting_treat(void)
{
	struct stat file_state;
	int len = 0;
	char *buf = NULL;
	char path[MAX_PATH_CHARS_SIZE];
	bool ret;
	
	DbgFuncEntry();

	build_export_path(path,EXPORT_PARAMS_CRL_FILE);
	
	// 得到控制文件的大小
	if(stat(path,&file_state) < 0)
	{
		DbgError("get update file size failed!! \r\n");
		return;
	}
	
	len = (int)file_state.st_size;
	DbgPrintf("len = %d\r\n",len);
	buf = malloc(len);
	memset(buf,0,len);

	ret = file2buf(path,0,(unsigned char *)buf,len);
	if(ret)
	{
		setting_ctrl_file_parse(buf,len);
	}
	
	free(buf);
	
	DbgFuncExit();
}


/** 
* @brief 	 导出事件的处理
* @param type		导出事件的类型
* 
*/
void fleety_export_treat(int type)
{
	DbgFuncEntry();

	DbgPrintf("export type = %d\r\n",type);
	
	switch(type)
	{
		case EXPORT_SETTING:
			fleety_export_setting_treat();
			break;
			
		case EXPORT_PARAMS:
			fleety_export_params_treat();
			break;
			
		case EXPORT_RECORD_PHOTO:
			fleety_export_record_photo_parse();
			break;

		case EXPORT_OPERATION:
			fleety_export_operation_parse();
			break;

		case EXPORT_UPDATE:
			fleety_export_update_parse();
			break;

		default:
			DbgWarn("don't support export type(%d)\r\n",type);
			break;
	}
	
	DbgFuncExit();
}
