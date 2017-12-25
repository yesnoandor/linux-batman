/****************************************Copyright (c)**************************************************                         
** File name:			fleety_system.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-17
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


#include	"module/itop/authorize/itop_authorize.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/heart_beat/gb905_heart_beat.h"

#include	"module/gb905_ex/mcu/mcu_heart_beat.h"
#include	"module/gb905_ex/ui/ui_heart_beat.h"


#include	"middleware/db/fleety_db.h"
#include	"middleware/uart/fleety_uart.h"
#include	"middleware/event/fleety_event.h"
#include	"middleware/socket/fleety_socket.h"

#include	"middleware/info/setting.h"

#include	"main/fleety_report.h"

#define		DEBUG_Y
#include	"libs/debug.h"

#define		MAX_MAIN_LOOP_TIMEOUT		1

static		pthread_cond_t		main_cond;
static		pthread_mutex_t		main_lock;


void system_process(void)
{
	int ret;
	int exit = 0;
	int count = 0;
	
	struct timeval now;
	struct timespec outtime;

	setting_params_t * setting;

	DbgFuncEntry();

	fleety_db_init();
	platform_hardware_init();

	fleety_event_init();
	fleety_uart_init();
	fleety_socket_init();

	sleep(5);
	platform_hardware_post_init();

	sleep(5);
	itop_authorize_loop();

	fleety_report_init();
	
	pthread_mutex_init(&main_lock, NULL);
	pthread_cond_init(&main_cond, NULL);

	sleep(2);

	while(!exit)
	{
		pthread_mutex_lock(&main_lock);

		gettimeofday(&now, NULL);
		outtime.tv_sec = now.tv_sec + MAX_MAIN_LOOP_TIMEOUT;
		outtime.tv_nsec = now.tv_usec * 1000;

		ret = pthread_cond_timedwait(&main_cond, &main_lock, &outtime);
        pthread_mutex_unlock(&main_lock);

        if(ret == 0)	// ---> condition ok
		{
			DbgPrintf("main cond ok!\r\n");
			sleep(5);
			exit = 1;
			continue;
		}
		else	// ---> timeout
		{
			DbgPrintf("main process timeout!\r\n");

			count++;

			// 发送国标服务器的心跳
			setting = get_setting_params();

			if((count % setting->heartbeat_interval) == 0)
			{
				gb905_heart_beat_send();
			}


			// 发送MCU   的心跳
			if((count % MCU_HEART_BEAT_TIMEOUT) == 0)
			{
				mcu_heart_beat_send();
			}

			// 发送UI  的心跳
			if((count % UI_HEART_BEAT_TIMEOUT) == 0)
			{
				ui_heart_beat_send();
			}
			

			// 和国标服务器的心跳超时处理
			gb905_heart_beat_treat();
			

			// 和MCU   的心跳超时处理
			mcu_heart_beat_treat();


			// 和UI  的心跳超时处理
			ui_heart_beat_treat();
		}

	}

	DbgFuncExit();
}

void exit_system_process(void)
{
	DbgFuncEntry();
	
	pthread_cond_signal(&main_cond);

	DbgFuncExit();
}



