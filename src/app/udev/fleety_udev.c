/****************************************Copyright (c)**************************************************                         
** File name:			fleety_udev.c
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
#include	"misc/check.h"

#include	"module/gb905/gb905_common.h"

#include	"middleware/db/file/db_file_path.h"

#include	"middleware/event/fleety_event.h"
#include	"middleware/event/export/fleety_export.h"


#define		DEBUG_Y
#include	"libs/debug.h"


#define		UDEV_BUFFER_SIZE		2048

#define		UDEV_UDISK_UNKNOWN		0
#define		UDEV_UDISK_PLUG_IN		1
#define		UDEV_UDISK_PLUG_OUT		2


static int udev_netlink_sock = -1;
static volatile int udev_exit;

static	pthread_t	udev_thread_id;
static	pthread_mutex_t udev_lock;

static int udisk_state = UDEV_UDISK_UNKNOWN;

static bool fleety_udev_check_udisk(void)
{
	bool ret = false;
	FILE *fp;
	char cmd[MAX_CMD_CHARS_SIZE];
	char buf[MAX_CMD_CHARS_SIZE] = {0};

	DbgFuncEntry();

	snprintf(cmd,80,"df | sed '/^$/!h;$!d;g' | awk '{print $6}'");
	DbgPrintf("cmd = %s\r\n",cmd);

	DbgFuncExit();

	if((fp = popen(cmd,"r")) == NULL) 
	{
		DbgError("Fail to popen \r\n");
		
		return ret;
	}

	if(fgets(buf, MAX_CMD_CHARS_SIZE, fp) == NULL) 
	{
		DbgError("Fail to fgets \r\n");

		pclose(fp);
		return ret;
	}

	pclose(fp);
		
	DbgPrintf("return : %s\r\n",buf);
	DbgPrintf("return len : %d\r\n",strlen(buf));
	DbgPrintf("len : %d\r\n",strlen(UDISK_PATH));
	DbgPrintf("str : %s\r\n",UDISK_PATH);

	if(strstr(buf,UDISK_PATH))
	{
		DbgPrintf("fleety udisk is exist\r\n");
		ret = true;
	}
	else
	{
		DbgPrintf("fleety udisk is not exist\r\n");
	}

	DbgFuncExit();

	return ret;
}


static int fleety_udev_init_netlink_socket(void)
{
	struct sockaddr_nl snl;
	int ret;

	DbgFuncEntry();

	memset(&snl, 0x00, sizeof(struct sockaddr_nl));
	snl.nl_family = AF_NETLINK;
	snl.nl_pid = getpid();
	snl.nl_groups = 1;

	udev_netlink_sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if (udev_netlink_sock == -1) {
		DbgError("error getting socket: %s\n", strerror(errno));
		return -1;
	}

	ret = bind(udev_netlink_sock, (struct sockaddr *) &snl,sizeof(struct sockaddr_nl));
	if (ret < 0) {
		DbgError("bind failed: %s\n", strerror(errno));
		
		close(udev_netlink_sock);
		udev_netlink_sock = -1;
		return -1;
	}

	DbgFuncExit();
	
	return 0;
}

static void fleety_udev_parse(char *buf,int len)
{
	fleety_event_t event;
	
	DbgFuncEntry();

	if(udisk_state != UDEV_UDISK_PLUG_IN)
	{
		if(strstr(buf,"add"))
		{
			sleep(10);

			if(fleety_udev_check_udisk())
			{
				udisk_state = UDEV_UDISK_PLUG_IN;
					
				event.id = UDEV_UDISK_EVENT;
				event.param = UDEV_UDISK_PLUG_IN;
				event.priority = DAFAULT_PRIORITY;
						
				fleety_event_push(&event);
			}
			
		}
	}

	if(udisk_state != UDEV_UDISK_PLUG_OUT)
	{
		if(strstr(buf,"remove"))
		{
			sleep(10);

			if(!fleety_udev_check_udisk())
			{
				udisk_state = UDEV_UDISK_PLUG_IN;
					
				event.id = UDEV_UDISK_EVENT;
				event.param = UDEV_UDISK_PLUG_IN;
				event.priority = DAFAULT_PRIORITY;
						
				fleety_event_push(&event);
			}	
		}
	}

	DbgFuncExit();
}


static void * fleety_udev_loop_func(void *arg)  
{
	fd_set readfds;
	bool udev_exit = false;

	int ret;
	ssize_t size;
	
	char buf[UDEV_BUFFER_SIZE];
	
	DbgFuncEntry();
	
	while (!udev_exit)
	{
		size = 0;
		
		FD_ZERO(&readfds);
		
		if (udev_netlink_sock >= 0)
			FD_SET(udev_netlink_sock, &readfds);

		ret = select(udev_netlink_sock + 1, &readfds, NULL, NULL, NULL);
		switch(ret)
		{
			case -1:
				//exit = 1;
				DbgError("socket select failed!(err = %s)\r\n",strerror(errno));
				break;
				
			case 0:
				DbgWarn("socket timeout!\r\n");
				break;
		
			default:			
				if (FD_ISSET(udev_netlink_sock, &readfds))
				{
					size = recv(udev_netlink_sock, buf, sizeof(buf), 0);
					if(size < 0)
					{	
						DbgError("netlink socket recv failed!(size = %ld,err = %s)\r\n",size,strerror(errno));
						break;
					}
					else if(size == 0)
					{
						DbgWarn("netlink socket disconnect\r\n");
						continue;
					}
					else	
					{
						ssize_t len = 0;

						DbgPrintf("size = %ld\r\n",size);
							
						while(len < size)
						{
							DbgPrintf("str = %s\r\n",buf + len);
							DbgPrintf("str len = d\r\n",strlen(buf + len));

							len += strlen(buf) + 1;

							DbgPrintf("len = %ld\r\n",len);
						}

					
						fleety_udev_parse(buf,strlen(buf) + 1);
						
					}		
				}
			
				break;
		}
	}


	DbgFuncExit();

	return 0;
}

void fleety_udev_init(void)
{
	int ret;

	DbgFuncEntry();

	fleety_udev_init_netlink_socket();
	
	// thread init	
	pthread_mutex_init(&udev_lock, NULL);
	
	ret = pthread_create(&udev_thread_id, NULL, fleety_udev_loop_func, NULL);
    if(ret != 0)
    {
    	DbgError("create udev thread error: %s\r\n",strerror(errno));  
		return;
	}
}

void fleety_udisk_treat(int udev_id)
{
	fleety_event_t event;    
	char path[MAX_PATH_CHARS_SIZE];
	
	DbgFuncEntry();

    
    if(udev_id == UDEV_UDISK_PLUG_IN)
    {
        memset(path,0x00,sizeof(path));
    	build_export_path(path,EXPORT_PARAMS_CRL_FILE);
        if(is_file_exist(path))
        {
            DbgPrintf("CONFIG.CRL is exit!!!\r\n");
        	event.id = EXPORT_EVENT;
			event.param = EXPORT_SETTING;
			event.priority = DAFAULT_PRIORITY;
			fleety_event_push(&event);
        }
        else
        {
            DbgPrintf("CONFIG.CRL is not exit!!!\r\n"); 
        }

        memset(path,0x00,sizeof(path));
    	build_export_path(path,EXPORT_OPERATION_CRL_FILE);
        if(is_file_exist(path))
        {
            DbgPrintf("YYEXPORT.CRL is exit!!!\r\n");
        	event.id = EXPORT_EVENT;
			event.param = EXPORT_OPERATION;
			event.priority = DAFAULT_PRIORITY;
			fleety_event_push(&event);            
        }
        else
        {
            DbgPrintf("YYEXPORT.CRL is not exit!!!\r\n");
        }

        memset(path,0x00,sizeof(path));
    	build_export_path(path,EXPORT_RECORD_PHOTO_CRL_FILE);
        if(is_file_exist(path))
        {
            DbgPrintf("RPEXPORT.CRL is exit!!!\r\n");
        	event.id = EXPORT_EVENT;
			event.param = EXPORT_RECORD_PHOTO;
			event.priority = DAFAULT_PRIORITY;
			fleety_event_push(&event);            
        }
        else
        {
            DbgPrintf("RPEXPORT.CRL is not exit!!!\r\n");
        }
    }



    DbgFuncExit();
}
