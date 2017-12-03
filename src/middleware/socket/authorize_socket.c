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
// 接收到服务器网络输入的处理回调函数
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


// 接收到服务器网络出错的处理回调函数
static void authorize_server_event_cb(struct bufferevent *bev, short event, void *arg)  
{
	unsigned char state;
	
	DbgFuncEntry();

	// 成功连接 状态变更
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

		// 自动close 套接字和free  读写缓冲区  
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
				
		// 创建一个bufferevent
		// BEV_OPT_CLOSE_ON_FREE : 当 bufferevent 被释放同时关闭底层
		bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);	
					
		// 设置读取方法和error 时候的方法  
		bufferevent_setcb(bev, authorize_server_read_cb, NULL, authorize_server_event_cb, NULL); 
		// 设置类型  
		//bufferevent_enable(bev, EV_READ | EV_PERSIST);
			
		struct sockaddr_in tSockAddr;
		memset(&tSockAddr, 0, sizeof(tSockAddr));
		tSockAddr.sin_family = AF_INET;
		tSockAddr.sin_addr.s_addr = inet_addr(authorize_server_ip_addr);
		tSockAddr.sin_port = htons(authorize_server_port);
			
		// 连接服务器
		if( bufferevent_socket_connect(bev, (struct sockaddr*)&tSockAddr, sizeof(tSockAddr)) < 0)
		{
			DbgError("connect to authorize server failed!\r\n"); 
		}
	}
	else	// 判断是否认证通过
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

	// 创建并初始化一个event_base 
	struct event_base* base = event_base_new();
	DbgPrintf("base = 0x%x\r\n",base);

	// 创建一个bufferevent
	// BEV_OPT_CLOSE_ON_FREE : 当 bufferevent 被释放同时关闭底层
	bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
	// 设置读取方法和error 时候的方法  
	bufferevent_setcb(bev, authorize_server_read_cb, NULL, authorize_server_event_cb,base); 
	// 设置类型  
	//bufferevent_enable(bev, EV_READ | EV_PERSIST);
	
	struct sockaddr_in tSockAddr;
	memset(&tSockAddr, 0, sizeof(tSockAddr));
	tSockAddr.sin_family = AF_INET;
	tSockAddr.sin_addr.s_addr = inet_addr(authorize_server_ip_addr);
	tSockAddr.sin_port = htons(authorize_server_port);
	
	// 连接认证服务器
	ret = bufferevent_socket_connect(bev, (struct sockaddr*)&tSockAddr, sizeof(tSockAddr));
	if(ret < 0)
	{
		DbgError("connect to authorize server failed!\r\n");
	}

	authorize_socket_mngr.bev = bev;
	authorize_socket_mngr.state = 0;


	struct event *timeout = NULL;
	struct timeval tv = {MAX_AUTHORIZE_SERVER_TIMEOUT, 0};

	// 增加定时器事件
	timeout = event_new(base, -1, EV_PERSIST, authorize_timeout_cb, base);
	// 激活定时器
	evtimer_add(timeout, &tv);

	// 事件循环
	event_base_dispatch(base);
	
	//这将自动close  套接字和free  读写缓冲区  
	bufferevent_free(bev);
	
	//	销毁event_base
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

