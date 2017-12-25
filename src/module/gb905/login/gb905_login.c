/****************************************Copyright (c)**************************************************                         
** File name:			gb905_login.c
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

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/report/gb905_report.h"
#include	"module/gb905/login/gb905_login.h"

#include	"middleware/info/meter.h"
#include	"middleware/socket/fleety_socket.h"

#define		DEBUG_Y
#include	"libs/debug.h"

/** 
* @brief 	构造登陆签到的数据结构
* @param login_body 		登陆签到数据结构地址
*
*/
static void gb905_build_login_body(gb905_login_body_t * login_body)
{
	gb905_meter_open_success_body_t meter_open;
		
	DbgFuncEntry();

	get_meter_open_success_info((char *)&meter_open);
	
	gb905_build_report_body(&login_body->report);

	memcpy(login_body->company_license_number,&meter_open,sizeof(gb905_login_body_t) - sizeof(report_body_t));
	
	DbgFuncExit();
}

/** 
* @brief 	构造整个登陆签到的数据结构
* @param login 		整个登陆签到数据结构地址
*
*/
static void gb905_build_login(gb905_login_t * login)
{
	DbgFuncEntry();
	
	gb905_build_header(&login->header,MESSAGE_LOGIN_MSG,sizeof(gb905_login_body_t));

	gb905_build_login_body(&login->login_body);
		
	DbgFuncExit();
}

/** 
* @brief 	构造登陆签到的数据结构
* @param login_body 		登陆签到数据结构地址
*
*/
static void gb905_build_logout_body(gb905_logout_body_t * logout_body)
{
	gb905_meter_close_success_body_t meter_close;
		
	DbgFuncEntry();

	get_meter_close_success_info((char *)&meter_close);
	
	gb905_build_report_body(&logout_body->report);

	memcpy(&logout_body->meter_close_info,&meter_close,sizeof(gb905_meter_close_success_body_t));

	logout_body->logout_mode = 0;
	
	DbgFuncExit();
}

/** 
* @brief 	构造整个登陆签到的数据结构
* @param login 		整个登陆签到数据结构地址
*
*/
void gb905_build_logout(gb905_logout_t * logout)
{
	DbgFuncEntry();
	
	gb905_build_header(&logout->header,MESSAGE_LOGOUT_MSG,sizeof(gb905_logout_body_t));

	gb905_build_logout_body(&logout->logout_body);
	
	DbgFuncExit();
}


/** 
* @brief 	发送上班签到数据到服务器
*
*/
void gb905_login(void)
{
	gb905_login_t login;
	
	DbgFuncEntry();
	
	gb905_build_login(&login);
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&login,sizeof(gb905_login_t));
	
	DbgFuncExit();
}


/** 
* @brief 	发送下班签退数据到服务器
*
*/
void gb905_logout(void)
{
	gb905_logout_t logout;

	DbgFuncEntry();

	gb905_build_logout(&logout);
	gb905_send_data(MAIN_SOCKET,(unsigned char *)&logout,sizeof(gb905_logout_t));
	
	DbgFuncExit();	
}

