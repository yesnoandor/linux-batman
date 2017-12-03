/****************************************Copyright (c)**************************************************                         
** File name:			authorize_socket.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-02
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
  
#include	"event2/event.h"
#include	"event2/bufferevent.h" 
#include	"event2/buffer.h" 
#include	"event2/util.h"

#include	"module/itop/itop_common.h"
#include	"module/itop/authorize/itop_authorize.h"
#include	"module/info/device_info.h"

//#define		DEBUG_Y
#include	"libs/debug.h"

//----------
#define		authorize_server_ip_addr		"218.90.157.214"
#define		authorize_server_port			8688

#define		MAX_AUTHORIZE_SERVER_TIMEOUT	10

typedef struct {
	struct bufferevent* bev;
	int state;		// =0,disconnected; =1,connected
}authorize_socket_mngr_t;

static authorize_socket_mngr_t	authorize_socket_mngr;

//----------
// ���յ���������������Ĵ���ص�����
static void authorize_server_read_cb(struct bufferevent* bev, void* arg)  
{
	unsigned char msg[1024];
	size_t len,offset;
	struct event_base* base = (struct event_base*)arg;
	struct evbuffer *input;

	DbgFuncEntry();

	//sleep(3);
	//len = bufferevent_read(bev, msg, sizeof(msg));

	input=bufferevent_get_input(bev);
	len = evbuffer_copyout(input, msg, sizeof(msg));
	
	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("msg[%d] = 0x%x\r\n",i,msg[i]);
		}
	}
	
	offset = itop_protocol_ayalyze(msg,len);
	evbuffer_drain(input,offset);

	if(is_authorized())
	{
		DbgGood("authorize mtd OK,exit authorize loop!\r\n");
		event_base_loopbreak(base);
	}
	
	DbgFuncExit();
}


// ���յ��������������Ĵ���ص�����
static void authorize_server_event_cb(struct bufferevent *bev, short event, void *arg)  
{
	unsigned char state;
	
	DbgFuncEntry();

	// �ɹ����� ״̬���
	if( BEV_EVENT_CONNECTED == event )
	{
		DbgGood("connect to authorize server success!\r\n");

		bufferevent_enable( bev, EV_READ | EV_PERSIST);
		authorize_socket_mngr.state = 1;
		authorize_socket_mngr.bev = bev;

		state = get_authorize_status();
		itop_authorize_request(state);
	}
	else
	{
		if (event & BEV_EVENT_EOF)
			DbgPrintf("authorize connection closed\r\n");
		else if (event & BEV_EVENT_ERROR)
			DbgPrintf("authorize happen some other errors\r\n");

		// �Զ�close �׽��ֺ�free  ��д������  
		bufferevent_free(bev);

		authorize_socket_mngr.state = 0;
	}
	
	DbgFuncExit();
}  

static void authorize_timeout_cb(int fd, short event, void * pArg)
{
	struct bufferevent* bev;
	struct event_base* base = (struct event_base*)pArg;

	DbgFuncEntry();

	DbgPrintf("base = 0x%x\r\n",base);

	if(0 == authorize_socket_mngr.state)
	{
		DbgPrintf("re-connect authorize socket\r\n");
				
		// ����һ��bufferevent
		// BEV_OPT_CLOSE_ON_FREE : �� bufferevent ���ͷ�ͬʱ�رյײ�
		bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);	
					
		// ���ö�ȡ������error ʱ��ķ���  
		bufferevent_setcb(bev, authorize_server_read_cb, NULL, authorize_server_event_cb, NULL); 
		// ��������  
		//bufferevent_enable(bev, EV_READ | EV_PERSIST);
			
		struct sockaddr_in tSockAddr;
		memset(&tSockAddr, 0, sizeof(tSockAddr));
		tSockAddr.sin_family = AF_INET;
		tSockAddr.sin_addr.s_addr = inet_addr(authorize_server_ip_addr);
		tSockAddr.sin_port = htons(authorize_server_port);
			
		// ���ӷ�����
		if( bufferevent_socket_connect(bev, (struct sockaddr*)&tSockAddr, sizeof(tSockAddr)) < 0)
		{
			DbgError("connect to authorize server failed!\r\n"); 
		}
	}
	else	// �ж��Ƿ���֤ͨ��
	{	
		if(is_authorized())
		{
			DbgGood("authorize mtd OK,exit authorize loop!\r\n");
			event_base_loopbreak(base);
		}
		else
		{
			unsigned char state;
			
			DbgError("authorize mtd fail\r\n");

			state = get_authorize_status();
			itop_authorize_request(state);
		}
	}
	
	DbgFuncExit();
}


//----------
void authorize_socket_init(void)  
{
	int ret;
	struct bufferevent* bev;
	
	DbgFuncEntry();

	// ��������ʼ��һ��event_base 
	struct event_base* base = event_base_new();
	DbgPrintf("base = 0x%x\r\n",base);

	// ����һ��bufferevent
	// BEV_OPT_CLOSE_ON_FREE : �� bufferevent ���ͷ�ͬʱ�رյײ�
	bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
	// ���ö�ȡ������error ʱ��ķ���  
	bufferevent_setcb(bev, authorize_server_read_cb, NULL, authorize_server_event_cb,base); 
	// ��������  
	//bufferevent_enable(bev, EV_READ | EV_PERSIST);
	
	struct sockaddr_in tSockAddr;
	memset(&tSockAddr, 0, sizeof(tSockAddr));
	tSockAddr.sin_family = AF_INET;
	tSockAddr.sin_addr.s_addr = inet_addr(authorize_server_ip_addr);
	tSockAddr.sin_port = htons(authorize_server_port);
	
	// ������֤������
	ret = bufferevent_socket_connect(bev, (struct sockaddr*)&tSockAddr, sizeof(tSockAddr));
	if(ret < 0)
	{
		DbgError("connect to authorize server failed!\r\n");
	}

	authorize_socket_mngr.bev = bev;
	authorize_socket_mngr.state = 0;


	struct event *timeout = NULL;
	struct timeval tv = {MAX_AUTHORIZE_SERVER_TIMEOUT, 0};

	// ���Ӷ�ʱ���¼�
	timeout = event_new(base, -1, EV_PERSIST, authorize_timeout_cb, base);
	// ���ʱ��
	evtimer_add(timeout, &tv);

	// �¼�ѭ��
	event_base_dispatch(base);
	
	//�⽫�Զ�close  �׽��ֺ�free  ��д������  
	bufferevent_free(bev);
	
	//	����event_base
	event_base_free(base);

	DbgFuncExit();
} 


void authorize_socket_send(unsigned char *buff,int len)
{
	DbgFuncEntry();

	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("buff[%d] = 0x%x\r\n",i,buff[i]);
		}
	}

	bufferevent_write(authorize_socket_mngr.bev,buff,len);

	DbgFuncExit();
}

