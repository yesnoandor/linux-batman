/****************************************Copyright (c)**************************************************                         
** File name:			fleety_call.c
** Descriptions:		通话功能处理
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-22
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

#include	"platform/platform.h"

#include	"module/gb905/phone_book/gb905_phone_book.h"
#include	"module/gb905_ex/ui/ui_call.h"
#include	"module/gprs/gprs_at_cmd.h"

#include	"middleware/db/sqlite3/phone_book_sqlite3.h"
#include	"middleware/info/setting.h"
#include	"middleware/info/status.h"
#include	"middleware/info/call.h"

#include	"app/call/fleety_call.h"

//#define		DEBUG_Y
#include	"libs/debug.h"

static	pthread_t	call_thread_id;


#define		CAN_CALL_IN				0x01		//仅呼入
#define		CAN_CALL_OUT			0x02		//仅呼出
#define		CAN_CALL_OUT_AND_IN		0x03		//能呼入和呼出

#define		CALLING					0x01		//通话中
#define		CALL_DROPPED			0x02		//不在通话中

static void close_sperker(void)
{
	DbgFuncEntry();
	
    system("echo 0 > /sys/class/fleety_leds_class/fleety_leds/amp_mute");

	DbgFuncExit();
}
 
static void open_sperker(void)
{
	DbgFuncEntry();
	
    system("echo 1 > /sys/class/fleety_leds_class/fleety_leds/amp_mute");
	
 	DbgFuncExit();
}
 
static void close_mic_mute(void)
{
	DbgFuncEntry();

	system("echo 1 > /sys/class/fleety_leds_class/fleety_leds/mix_mute");

 	DbgFuncExit();
}
 
static void open_mic_mute(void)
{
	DbgFuncEntry();

    system("echo 0 > /sys/class/fleety_leds_class/fleety_leds/mix_mute");

 	DbgFuncExit();
}



/** 
* @brief 			判断是否限制呼出
* @param phone_num		电话号码
*
* @return	是否限制呼出
*/
static bool judge_whether_limit_call_out(char *phone_num)
{
	bool ret = false;
	unsigned char flag; //flag 0x01:can call in 0x02:can call out 0x03:can call out and call in	

	DbgFuncEntry();

	flag = phone_book_sqlite3_get_falg_by_number((unsigned char*)phone_num);

	// 标志为仅呼入，则表示限制呼出
	if(flag == CAN_CALL_IN)
	{
		ret = true;
	}
	else
	{
		ret = false;
	}
	
	DbgFuncExit();

	return ret;
}

/** 
* @brief 			判断是否限制呼入
* @param phone_num		电话号码
*
* @return	是否限制呼入
*/
static bool judge_whether_limit_call_in(char *phone_num)
{
	bool ret = false;
	
	DbgFuncEntry();

	unsigned char flag;	//flag 0x01:can call in 0x02:can call out 0x03:can call out and call in
	
	DbgFuncEntry();

	flag = phone_book_sqlite3_get_falg_by_number((unsigned char *)phone_num);

	//标志位仅呼出，则表示限制呼入
	if(flag == CAN_CALL_OUT)
	{
		ret = true;
	}
	else
	{
		ret = false;
	}
		
	DbgFuncExit();

	return ret;
}


/** 
* @brief 			终端电话接听策略(自动接听或者手动接听)
* @param phone_num		电话号码
*
*/
static void fleety_answer_call_policy(char *phone_num)
{
	setting_params_t *param;
	
	DbgFuncEntry();

	param = get_setting_params();

	if(!param->telephone_answer_policy)//setting_params->telephone_answer_policy  =0:AUTO_ACCEPT  ,=1:(ACC ON :AUTO_ACCEPT   ;  ACC OFF :HAND_ACCEPT)
	{
		//AUTO_ANSWER
		fleety_answer_call();
	}
	else if(param->telephone_answer_policy==1)
	{
		if(get_acc_status())
		{
			//AUTO_ANSWER
			fleety_answer_call();

		}
		else
		{
			ui_send_call_in(phone_num);
			//HAND_ANSWER
		}

	}
	else
	{
		DbgWarn("telephone answer policy is not support!!!\r\n");
	}
	DbgFuncExit();

}


/** 
* @brief 			挂断电话处理
* 
*/
void fleety_hand_up(unsigned char hang_up_type)
{
	call_info_t * call_info;

	DbgFuncEntry();

	//关闭扬声器和麦克
	close_sperker();
	close_mic_mute();

	if(hang_up_type == ACTIVE_HANG_UP)
	{
		gprs_hand_up();
	}
	
	call_info = get_call_info();
	call_info->start_time = 0;
	call_info->status = CALL_DROPPED;
	set_call_info();
	ui_send_call_state((unsigned char)call_info->status);

	DbgFuncExit();
}


/** 
* @brief 			接听电话处理
* 
*/

void fleety_answer_call(void)
{
	call_info_t *call_info;
	
	DbgFuncEntry();
	
	call_info = get_call_info();

	//打开扬声器和麦克
	open_sperker();
	open_mic_mute();

	//发送接听命令
	gprs_answer_call();

	call_info->status = CALLING;
	set_call_info();
	
	DbgFuncExit();

}

/** 
* @brief 			通话时长监听(防止超出预设通话时长)
* 
*/
static void fleety_check_call_time(void)
{
	struct timeval now;
	unsigned int cur_call_time = 0;
	setting_params_t * params;
	call_info_t *call_info;
	
	DbgFuncEntry();

	params = get_setting_params();
	call_info = get_call_info();
	
	if(call_info->start_time)
	{
		gettimeofday(&now,NULL);
		cur_call_time = now.tv_sec-call_info->start_time;

        call_info->total_time +=  cur_call_time;
        
		if(call_info->total_time > params->max_time_talk_month)
		{
			DbgWarn("over max time talk month %d>%d, refuse call out!!!!\r\n",call_info->total_time,params->max_time_talk_month);
			fleety_hand_up(ACTIVE_HANG_UP);//当月通话超时，挂断电话
		}
		
		if(cur_call_time > params->max_time_talk_once)
		{
			set_call_info();
			DbgWarn("over max time talk once %d>%d, refuse call out!!!!\r\n",cur_call_time,params->max_time_talk_once);
			fleety_hand_up(ACTIVE_HANG_UP);//单次通话超时，挂断电话
		}
	}

	DbgFuncExit();
	
}


/** 
* @brief 			CLCC应答数据处理(通话状态)
* 
*/
void fleety_clcc_state_treat(void)
{
	
	struct timeval cur_time;
	clcc_info_t clcc_info;
	call_info_t *call_info;
	
	DbgFuncEntry();

	get_clcc_info(&clcc_info);
	call_info = get_call_info();

	//开始语音通话
	if(clcc_info.stat==0&&call_info->start_time==0)
	{
		gettimeofday(&cur_time,NULL);
		call_info->start_time = cur_time.tv_sec;
        call_info->status = CALLING;
		ui_send_call_state((unsigned char)call_info->status);
	}
    
	set_call_info();
	
	DbgFuncExit();
	
}


/** 
* @brief 			电话呼出处理
* @param phone_num		电话号码
* @param type			通话类型(1:监听;0:普通通话)
*
* @return	是否呼出成功
*/
bool fleety_call_out(char *phone_num,unsigned char type)
{
	bool ret;
	setting_params_t *param;
	call_info_t * call_info;
	DbgFuncEntry();

	param = get_setting_params();
	call_info = get_call_info();


	//limit call out
	if(judge_whether_limit_call_out(phone_num))
	{
		DbgWarn("limit %s call out , refuse call out!!!!\r\n",phone_num);
		
		return false;
	}
	
	//over max talk time of month 
	if(call_info->total_time > param->max_time_talk_month)
	{
		DbgWarn("over max time talk month %d>%d, refuse call out!!!!\r\n",call_info->total_time,param->max_time_talk_month);
	
		return false;
	}

	//普通通话
	if(type == CALL_TYPE_COMMON)
	{
		open_sperker();
		open_mic_mute();
	}
	
	//监听
	else
	{
		close_sperker();
		close_mic_mute();
	}
	
	ret = gprs_dail_out(phone_num);

	if(ret)
	{
		call_info->status = CALLING;
		set_call_info();
	}

	DbgFuncExit();

	return ret;
}

/** 
* @brief 			电话呼入处理
* @param phone_num		电话号码
*
* 
*/
void fleety_call_in(char *phone_num)
{
	setting_params_t *param;
	call_info_t * call_info;
	
	DbgFuncEntry();

	param = get_setting_params();
	call_info = get_call_info();

	//如果为复位电话，则终端重启
	if(!strcmp((char *)param->phone_number_reset,phone_num))
	{
		DbgWarn("reset number %s , system reset!!!!\r\n",phone_num);

        //挂断电话
		fleety_hand_up(ACTIVE_HANG_UP);
        
        sleep(2);
		//终端重启
        platform_system_reset();
	}
	
	//限制呼入，终端主动挂断电话
	if(judge_whether_limit_call_in(phone_num))
	{
		DbgWarn("limit %s call in , hand up!!!!\r\n",phone_num);
		//挂断电话
		fleety_hand_up(ACTIVE_HANG_UP);

		return ;
	}

	//超出本月通话总时长，终端主动挂断电话
	if(call_info->total_time > param->max_time_talk_month)
	{
		DbgWarn("over max time talk month %d>%d, hand up!!!!\r\n",call_info->total_time,param->max_time_talk_month);
		//挂断电话
		fleety_hand_up(ACTIVE_HANG_UP);

		return ;		
	}

	//接听
	fleety_answer_call_policy(phone_num);

	DbgFuncExit();
}


//监听通话状态
static void *fleety_check_status_func(void *arg)
{
	time_t timep;
	struct tm *p;
	call_info_t	*call_info;
	
	DbgFuncEntry();
	
	while(1)
	{
		
		call_info = get_call_info();
		time(&timep);
		p = localtime(&timep);
		
		if(p->tm_mday==1&&p->tm_hour==0&&p->tm_min==0&&(p->tm_sec==0||p->tm_sec==1))//月初清空月通话总时长
		{
			call_info->total_time=0;
			set_call_info();
		}
		
		if(call_info->status==CALLING)//有呼入或呼出开始监听通话状态
		{
			gprs_send_clcc();

			fleety_check_call_time();
		}
		
		sleep(2);
		
	}

	
	DbgFuncExit();

	return ((void*)0);
}

//终端通话功能初始化(监听通话状态)
void fleety_call_function_init(void)
{
	int err = 0;
	
	DbgFuncEntry();
		
	err = pthread_create(&call_thread_id, NULL, fleety_check_status_func, NULL);
    if(err != 0)
	{
    	DbgError("create thread error: %s/n",strerror(err));  
		return;
	}
   	gprs_set_clip();

	DbgFuncExit();
}

