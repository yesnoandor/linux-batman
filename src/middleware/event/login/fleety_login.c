/****************************************Copyright (c)**************************************************                         
** File name:			fleety_login.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-15
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

#include	"module/gb905_peri/gb905_peri_common.h"
#include	"module/gb905_peri/meter/gb905_meter.h"
#include	"module/gb905_peri/toplight/gb905_toplight.h"


#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"
#include	"module/gb905/login/gb905_login.h"
#include	"module/gb905_ex/ui/ui_login.h"

#include	"middleware/info/toplight.h"
#include	"middleware/info/status.h"


#define		DEBUG_Y
#include	"libs/debug.h"


void fleety_login_treat(int login)
{
	if(login)
	{
		// ���ö���״̬
		set_toplight_operation_status(TOPLIGHT_STATUS_EMPTY);

		// ���ö����Ǽ�
		gb905_toplight_set_star_status(0x01);

		// ������������ϰ�ǩ����Ϣ
		gb905_login();

        //���õ�¼״̬
        set_login_mode();
        
		//��UI���͵�¼��Ϣ
		ui_login_info_send_treat();
	}
	else
	{
		
		// ���ö���״̬
		set_toplight_operation_status(TOPLIGHT_STATUS_STOP);
		
		// ������������°�ǩ����Ϣ
		gb905_logout();

        //����δ��¼״̬
        set_logout_mode();
	}
}


