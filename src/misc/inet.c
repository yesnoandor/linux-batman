/****************************************Copyright (c)**************************************************                         
** File name:			endian.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-04
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


#define		DEBUG_Y
#include	"libs/debug.h"

void get_ip_addr(char *host_name, char *ip_addr)
{
	struct hostent *hp;
	struct in_addr in;
	struct sockaddr_in local_addr;

	DbgFuncEntry();

	DbgPrintf("host = %s\r\n",host_name);
	
	hp = gethostbyname(host_name);

	DbgPrintf("host2 = %s\r\n",host_name);
	if (!hp)
	{
		DbgError("can't solve host!\r\n");
		ip_addr = NULL;
		return;
	}

	memcpy(&local_addr.sin_addr.s_addr,hp->h_addr,4);
	in.s_addr = local_addr.sin_addr.s_addr;
	strcpy(ip_addr,inet_ntoa(in));
	
	DbgPrintf("host = %s\r\n",host_name);
	DbgPrintf("ip = %s\r\n",ip_addr);

	DbgFuncExit();
}

