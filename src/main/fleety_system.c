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
#include    "module/gb905_ex/mcu/mcu_adc.h"

#include	"module/gb905_peri/gb905_peri_common.h"
#include	"module/gb905_peri/meter/gb905_meter.h"
#include	"module/gb905_peri/toplight/gb905_toplight.h"
#include	"module/gb905_peri/tsm/gb905_tsm.h"



#include	"middleware/db/fleety_db.h"
#include	"middleware/db/sqlite3/db_sqlite3.h"

#include	"middleware/uart/fleety_uart.h"
#include	"middleware/event/fleety_event.h"
#include	"middleware/socket/fleety_socket.h"

#include    "middleware/info/status.h"
#include	"middleware/info/setting.h"
#include	"middleware/info/device.h"
#include	"middleware/info/version.h"
#include	"middleware/info/av.h"

#include	"app/report/fleety_report.h"
#include	"app/record/fleety_record.h"

#include	"platform/platform_av.h"

#include    "app/av/fleety_photo.h"

#define		DEBUG_Y
#include	"libs/debug.h"

#define		MAX_MAIN_LOOP_TIMEOUT		1

static		pthread_cond_t		main_cond;
static		pthread_mutex_t		main_lock;

static void fleety_terminal_info_display(void)
{
    char hw_version[MAX_HW_VERSION_SIZE];
	char sw_version[MAX_SW_VERSION_SIZE];
	device_info_t info;
    
    DbgFuncEntry();

	get_hw_version(hw_version);
	get_sw_version(sw_version);
	setting_params_t *params;
	params = get_setting_params();

	get_device_info(&info);
	
	DbgGood("***********************************\n");
	DbgGood("**	terminal mtd id %d\n",info.mtd_id);
	DbgGood("**	terminal sw version %s\n",sw_version);
	DbgGood("**	terminal hw version %s\n",hw_version);
	DbgGood("**	main ip:%s main port:%d\n",params->main_server_ipaddr,params->main_server_tcp_port);
	DbgGood("**	vice ip:%s vice port:%d\n",params->vice_server_ipaddr,params->vice_server_tcp_port);
	DbgGood("***********************************\n");

    DbgFuncExit();
}

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
    fleety_terminal_info_display();
    fleety_taxi_status_init();
	platform_hardware_init();

	fleety_event_init();
	fleety_uart_init();
	fleety_socket_init();
	
	db_sqlite3_init();

	sleep(5);
	platform_hardware_post_init();

	sleep(5);
	itop_authorize_loop();
	
	fleety_record_init();
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

			// 进行补传
			if((count % SUPPLEMENT_TIMEOUT) == 0)
			{
				gb905_record_supplement(MAIN_SOCKET);
			}

			if((count % TOPLIGHT_TIMEOUT) == 0)
			{
				gb905_toplight_query_state();
			}

            if((count % TSM_TIMEOUT) == 0)
            {
                gb905_tsm_query_state();

            }

            if((count % ADC_REP_TIMEOUT) == 0)
            {
                mcu_send_adc_req(ADC_CHANNEL_ACC);
            }

            if((count % CAMERA_CHECK_TIMEOUT) == 0)
            {
                //摄像头故障处理
                fleety_camera_fault_treat();
            }
            
			#if 0
			// 拨号状态监测以及重新拨号机制
			if((count % PPPD_DAIL_TIMEOUT) == 0)
			{
				pppd_dail_check();
			}
			#endif

            //终端ADC值监测处理
			gb905_adc_value_monitor_treat();

			// 和国标服务器的心跳超时处理
			gb905_heart_beat_treat();
			
			// 计价器的心跳超时处理
			gb905_meter_heart_beat_treat();

			// 顶灯的心跳超时处理
			gb905_toplight_heart_beat_treat();

            // TSM模块的心跳超时处理
            gb905_tsm_heart_beat_treat();

            // 和MCU的心跳超时处理
			mcu_heart_beat_treat();
            
			// 和UI的心跳超时处理
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



