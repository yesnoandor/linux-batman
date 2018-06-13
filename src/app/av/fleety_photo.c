/****************************************Copyright (c)**************************************************                         
** File name:			fleety_photo.c
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

#include	"libs/sqlite3/sqlite3.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/av/gb905_photo.h"

#include	"middleware/info/av.h"
#include	"middleware/event/export/fleety_export.h"
#include	"middleware/db/file/db_file_path.h"
#include	"middleware/db/sqlite3/photo_sqlite3.h"

#include	"middleware/info/status.h"

#include	"platform/platform_av.h"

#include    "app/av/fleety_photo.h"

#define		DEBUG_Y
#include	"libs/debug.h"


static	pthread_t	fleety_photo_thread_id;
static	pthread_mutex_t fleety_photo_lock;
static	pthread_cond_t fleety_photo_cond;

/*
* @brief 	�������տ��Ʋ�����ʵ������
* @param photo_ctrl_info  ���տ��Ʋ���
*
*/
static void photo_period_treat(photo_ctrl_info_t * photo_ctrl_info)
{
	char path[MAX_PATH_CHARS_SIZE];
	char name[MAX_PATH_CHARS_SIZE];
	
	gb905_photo_info_t photo_info;
	
	DbgFuncEntry();
	
	if(photo_ctrl_info->status)
	{
		switch (photo_ctrl_info->flag)
		{
			case 0: 	// ʵʱ�ϴ�
				photo_info.reason = photo_ctrl_info->reason;
				photo_info.camera_id = photo_ctrl_info->camear_id;
				photo_info.photo_id.id = gb905_build_timestamp_id();

				build_photo_name(name,photo_info.photo_id.id);
				build_photo_path(path,name);
				platform_phoho_take_picture(path,photo_info.camera_id,&photo_ctrl_info->params);

				// ������Ϣ�������ݿ�
				photo_sqlite3_insert_record(&photo_info);

				// �����ϴ�
				gb905_photo_upload_start(photo_info.photo_id.id,0,photo_ctrl_info->serial_number);
				gb905_photo_upload_picture_treat();
				break;

			case 1: 	// ����
				photo_info.reason = photo_ctrl_info->reason;
				photo_info.camera_id = photo_ctrl_info->camear_id;
				photo_info.photo_id.id = gb905_build_timestamp_id();

				build_photo_name(name,photo_info.photo_id.id);
				build_photo_path(path,name);
				platform_phoho_take_picture(path,photo_info.camera_id,&photo_ctrl_info->params);
				
				// ������Ϣ�������ݿ�
				photo_sqlite3_insert_record(&photo_info);
				break;
		
			default:
				DbgWarn("don't support this save flag!\r\n");
				break;
		}
	}

	DbgFuncExit();
}

/*
* @brief 	����ѭ���߳�
* @param arg	  �����̵߳Ĳ������˴�ΪNULL
*
*/
static void * fleety_photo_loop_func(void *arg)  
{
	int ret;
	bool exit = false;

	unsigned int timeout_threhold;
	photo_ctrl_info_t photo_ctrl_info;
		
	struct timeval now;
	struct timespec outtime;

	DbgFuncEntry();

	get_photo_ctrl_info((unsigned char *)&photo_ctrl_info);

	timeout_threhold = photo_ctrl_info.interval;

	while(!exit)
	{
		pthread_mutex_lock(&fleety_photo_lock);

		photo_period_treat(&photo_ctrl_info);
		photo_ctrl_info.index++;
		if(photo_ctrl_info.index >= photo_ctrl_info.total)
		{
			DbgPrintf("photo finished,exit!\r\n");

			exit = true;
			continue;
		}

		gettimeofday(&now, NULL);
		outtime.tv_sec = now.tv_sec + timeout_threhold;
		outtime.tv_nsec = now.tv_usec * 1000;
    
		ret = pthread_cond_timedwait(&fleety_photo_cond, &fleety_photo_lock, &outtime);
        
		pthread_mutex_unlock(&fleety_photo_lock);

		if(ret == 0)	// ---> condition ok
		{
			DbgPrintf("photo cond ok,exit!\r\n");

			exit = true;
			continue;
		}
		else	// ---> timeout
		{
			DbgPrintf("photo timeout!\r\n");
		}
	}
	
	DbgFuncExit();
	
    return ((void *)0);
} 


/** 
* @brief 	���������߳�
*
*/
void fleety_photo_start(void)
{
	int err;

	DbgFuncEntry();
	
	pthread_mutex_init(&fleety_photo_lock, NULL);
	pthread_cond_init(&fleety_photo_cond, NULL);

	err = pthread_create(&fleety_photo_thread_id, NULL, fleety_photo_loop_func, NULL);
    if(err != 0)
    {
    	DbgError("create photo thread error: %s\r\n",strerror(err));  
		return;
	}

	DbgFuncExit();
}


/*
* @brief 	ֹͣ�����߳�
* 
*/
void fleety_photo_stop(void)
{
	DbgFuncEntry();
	
	pthread_cond_signal(&fleety_photo_cond);

	DbgFuncExit();
}


void fleety_photo_take_picture_treat(unsigned char reason)
{
    photo_ctrl_info_t photo_ctrl_info;

    DbgFuncEntry();
    
	photo_ctrl_info.status = 0x01;
    photo_ctrl_info.reason = reason;
    photo_ctrl_info.camear_id = 0x00;//Ĭ���õ�һ������ͷ
    
    if(reason == PHOTO_REASON_ALARM)
    {
        photo_ctrl_info.total = 3;//������3��
    }
    else
    {
        photo_ctrl_info.total = 1;//������1��
    }
	
	photo_ctrl_info.index = 0;
	photo_ctrl_info.interval = MIN_TAKE_PICTURE_INTERVAL;
	photo_ctrl_info.flag = 0x00;//ʵʱ�ϴ�
	//photo_ctrl_info.params.resolution = take_pic->resolution;
	//photo_ctrl_info.params.brightness = take_pic->brightness;
	//photo_ctrl_info.params.chroma = take_pic->chroma;
	//photo_ctrl_info.params.contrast = take_pic->contrast;
	//photo_ctrl_info.params.saturation = take_pic->saturation;
	//photo_ctrl_info.params.chroma = take_pic->chroma;

	photo_ctrl_info.serial_number = 0x00;

	set_photo_ctrl_info((unsigned char *)&photo_ctrl_info);
	debug_photo_ctrl_info();

	fleety_photo_start();


    DbgFuncExit();
}
void fleety_camera_fault_treat(void)
{
    taxi_status_t status;
    unsigned char camera_status;

    DbgFuncEntry();
    
    taxi_get_status(&status);
    
	camera_status = platform_get_camera_status();
    
    DbgPrintf("camera status = 0x%02x \r\n",camera_status);
    
    if(camera_status==0xF)
    {
        //��������ͷ����״̬
        if(status.alarm.flag.camera_fault)
        {
            clr_camera_alarm_status();
        }
    }
    else
    {
        //����������ͷ����״̬
        if(!status.alarm.flag.camera_fault)
        {
            set_camera_alarm_status();
        }   
    }
    
    DbgFuncExit();
}
