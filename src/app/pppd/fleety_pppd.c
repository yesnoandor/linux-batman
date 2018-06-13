/****************************************Copyright (c)**************************************************                         
** File name:			fleety_pppd.c
** Descriptions:		终端pppd拨号功能
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-08
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

#include	"middleware/db/file/db_file_path.h"

#include	"app/pppd/fleety_pppd.h"


#define		DEBUG_Y
#include	"libs/debug.h"


#define		CMD_STR_MAX_LEN			256
#define		PPPD_MSG_MAX_LEN		2048


//终端拨号命令
void pppd_dail_first(void)
{
	static int dail_count = 0;
	char cmd[CMD_STR_MAX_LEN]={0};
	
	DbgFuncEntry();

	DbgPrintf(" terminal pppd dail %d\r\n",dail_count);
	snprintf(cmd,sizeof(cmd),"/etc/ppp/huawei-ppp-on card card 3gnet *99\\# WCDMA");

	system(cmd);
	
	dail_count++;
	
	DbgFuncExit();
}


//终端检测拨号状态以及重新拨号机制
void pppd_dail_check(void)
{
	FILE *fp;
	char cmd[CMD_STR_MAX_LEN]={0};
	char buf[PPPD_MSG_MAX_LEN];
	
	DbgFuncEntry();

	//执行ifconfig命令( 18P:第18行;$1:第1列)
	snprintf(cmd,sizeof(cmd),"ifconfig | sed -n '18p /^$/!h;$!d;g' | awk '{print $1}'");
	DbgPrintf("cmd = %s\r\n",cmd);
	
	if((fp = popen(cmd,"r")) == NULL) 
	{
		DbgError("Fail to popen \r\n");
		
		return ;
	}

	if(fgets(buf, 200, fp) == NULL) 
	{
		DbgError("Fail to fgets \r\n");
		pclose(fp);

		pppd_dail_first();
		return ;
	}

	pclose(fp);
		
	DbgPrintf("return : %s\r\n",buf);

	// ppp0不存在
	if(strcmp(buf,"ppp0\n") != 0)
	{
		pppd_dail_first();
	}	
	
	DbgFuncExit();
}
