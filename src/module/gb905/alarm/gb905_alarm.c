/****************************************Copyright (c)**************************************************                         
** File name:			gb905_alarm.c
** Descriptions:		λ�û㱨(0x0200)  ��λ����Ϣ��ѯ(0x8201/0x0201) ��Э�����
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-13
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

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"
#include	"module/gb905/av/gb905_photo.h"
#include	"module/gb905_peri/gb905_peri_common.h"
#include	"module/gb905_peri/toplight/gb905_toplight.h"

#include	"middleware/info/status.h"
#include	"middleware/info/toplight.h"

#include	"app/report/fleety_report.h"
#include    "app/av/fleety_photo.h"

#define		DEBUG_Y
#include	"libs/debug.h"

/** 
* @brief 	����ȷ�ϱ���
*
* @return  �����ն�ͨ��Ӧ���״̬
*/
unsigned char gb905_alarm_confirm_treat(void)
{
	DbgFuncEntry();

	reset_pre_alarm_status();
	set_emergency_alarm_status();
		
	DbgWarn("emergency alarm status!\r\n");

	fleety_report();

    //��������
	fleety_photo_take_picture_treat(PHOTO_REASON_ALARM);

	set_toplight_alarm_status();
	
	DbgFuncExit();

	return GB905_RESULT_OK;
}


/** 
* @brief 	���Ľ������
* 
* @return  �����ն�ͨ��Ӧ���״̬
*/
unsigned char gb905_alarm_cancel_treat(void)
{
	DbgFuncEntry();

	reset_emergency_alarm_status();
	
	fleety_report();
	
	reset_toplight_alarm_status();
	
	DbgFuncExit();

	return GB905_RESULT_OK;
}



