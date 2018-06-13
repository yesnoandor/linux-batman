/****************************************Copyright (c)**************************************************                         
** File name:			update_socket.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-15
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
#include	"event2/thread.h"


#include	"module/gb905/gb905_common.h"
#include	"module/gb905/ctrl/gb905_control.h"

#include	"module/gb905_update/gb905_update_common.h"

#include	"middleware/info/update.h"

#define		DEBUG_Y
#include	"libs/debug.h"

typedef struct {
	struct bufferevent* bev;
	int state;		// =0,disconnected; =1,connected
	int timeout;	// 
	int threhold;
}socket_mgr_t;


static	pthread_t		update_socket_thread_id;
static	pthread_cond_t	update_socket_cond;
static	pthread_mutex_t update_socket_lock;

static socket_mgr_t	update_socket;

// 接收到服务器网络输入的处理回调函数
static void update_server_read_cb(struct bufferevent* bev, void* arg)  
{
	unsigned char msg[1024];
	int len =0;
	int offset=0;
	struct evbuffer *input;

	DbgFuncEntry();

	//len = bufferevent_read(bev, msg, sizeof(msg));

	input = bufferevent_get_input(bev);
	len = evbuffer_copyout(input, msg, sizeof(msg));

	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("msg[%d] = 0x%x\r\n",i,msg[i]);
		}
	}
	
	offset = gb905_update_protocol_ayalyze(msg,len);
	evbuffer_drain(input,offset);

	update_socket.timeout = 0;

	DbgPrintf("offset = %d\r\n",offset);

	DbgFuncExit(); 
}


// 接收到服务器网络出错的处理回调函数
static void update_server_event_cb(struct bufferevent *bev, short event, void *arg)  
{
	long index;
	
	DbgFuncEntry();

	index = (long)arg;

	DbgPrintf("index = %d\r\n",index);
	
	// 成功连接 状态变更
	if( BEV_EVENT_CONNECTED == event )
	{
        bufferevent_enable( bev, EV_READ | EV_PERSIST);
		
		update_socket.bev = bev;
		update_socket.timeout = 0;
		update_socket.threhold = 10;
		
        update_socket.state = 1;

		DbgGood("update server connect success!\r\n");
    }
	else
	{
		if (event & BEV_EVENT_EOF)
			DbgPrintf("update server connection closed!\r\n");
		else if (event & BEV_EVENT_ERROR)
			DbgPrintf("update server some other error!\n");

		// 自动close 套接字和free  读写缓冲区  
		bufferevent_free(bev);

		update_socket.bev = NULL;
		update_socket.state = 0;
	}
	
	DbgFuncExit();
}  

#if 0
static void * update_socket_loop_func(void *arg)
{
	gb905_update_param_t update_param;

	struct bufferevent* bev;
	//struct event_base* base;

	DbgFuncEntry();

	get_update_param((unsigned char *)&update_param);

	// 创建并初始化一个event_base  (  支持多线程)
	//evthread_use_pthreads();
	//base = event_base_new();	
	//evthread_make_base_notifiable(base);
	//DbgPrintf("base = 0x%x\r\n",base);
	
	if((strcmp(update_param.update_server_ipaddr,"") == 0) || (update_param.update_server_tcp_port == 0))
	{
		DbgError("server error(ip = %s,port = %d)\r\n",update_param.update_server_ipaddr,update_param.update_server_tcp_port);
		return (void *)-1;
	}

	
	// 创建并初始化一个event_base 
	struct event_base* base = event_base_new();
	DbgPrintf("base = 0x%x\r\n",base);
			
	// 创建一个bufferevent
	// BEV_OPT_CLOSE_ON_FREE : 当 bufferevent 被释放同时关闭底层
	bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
	// 设置读取方法和error 时候的方法  
	bufferevent_setcb(bev, update_server_read_cb, NULL, update_server_event_cb,(void *)0); 
	// 设置类型  
	//bufferevent_enable(bev, EV_READ | EV_PERSIST);
	
	struct sockaddr_in tSockAddr;
	memset(&tSockAddr, 0, sizeof(tSockAddr));
	tSockAddr.sin_family = AF_INET;
	tSockAddr.sin_addr.s_addr = inet_addr(update_param.update_server_ipaddr);
	tSockAddr.sin_port = htons(update_param.update_server_tcp_port);
	
	// 连接服务器
	if(bufferevent_socket_connect(bev, (struct sockaddr*)&tSockAddr, sizeof(tSockAddr)) < 0)
	{
		DbgError("connect to server error(ip = %s,port = %d) failed!\r\n",update_param.update_server_ipaddr,update_param.update_server_tcp_port); 
		return (void *)-1;
	}
	
	// 事件循环
	event_base_dispatch(base);
	
	// 这将自动close   套接字和free  读写缓冲区  
	bufferevent_free(bev);
	
	// 销毁event_base
	event_base_free(base);
	
	DbgFuncExit();

	return ((void *)0);
}
#endif

static int tcp_connect_server(const char* server_ip, int port)  
{
	int sockfd, status, save_errno;
	struct sockaddr_in server_addr;
	  
	memset(&server_addr, 0, sizeof(server_addr) );
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	status = inet_aton(server_ip, &server_addr.sin_addr);  
	  
	if( status == 0 ) //the server_ip is not valid value  
	{  
		errno = EINVAL;  
		return -1;	
	}
	
	sockfd = socket(PF_INET, SOCK_DGRAM, 0);  
	if( sockfd == -1 )
	{
		DbgError("create socket failed!(err = %s)\r\n",strerror(errno));
		return sockfd;	
	}
	  
	status = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr) );
	if( status == -1 )
	{		
		DbgError("connect socket failed!(err = %s)\r\n",strerror(errno));

		save_errno = errno;
		close(sockfd);  
		errno = save_errno; //the close may be error  

		return -1;	
	}  

	// 设置sockfd  为非阻塞的socket 
	evutil_make_socket_nonblocking(sockfd);  
	  
	return sockfd;	
}  

static void * update_socket_loop_func(void *arg)
{
	gb905_update_param_t update_param;
	struct bufferevent* bev;
	int sockfd;
	
	DbgFuncEntry();
	
	get_update_param((unsigned char *)&update_param);

	if((strcmp(update_param.update_server_ipaddr,"") == 0) || (update_param.update_server_tcp_port == 0))
	{
		DbgError("server error(ip = %s,port = %d)\r\n",update_param.update_server_ipaddr,update_param.update_server_tcp_port);
		return (void *)-1;
	}

	// 创建并初始化一个event_base 
	struct event_base* base = event_base_new();
	DbgPrintf("base = 0x%x\r\n",base);

	sockfd = tcp_connect_server(update_param.update_server_ipaddr, update_param.update_server_tcp_port);  
    if(sockfd == -1)  
    {  
        DbgError("tcp connect error!\r\n");  
        return (void *)-1;  
    }
    DbgGood("connect to server successful\r\n");



	// 创建一个bufferevent
	// BEV_OPT_CLOSE_ON_FREE : 当 bufferevent 被释放同时关闭底层
	bev = bufferevent_socket_new(base,sockfd,BEV_OPT_CLOSE_ON_FREE);

	

	// 当socket  关闭时会用到回调参数  
	// 设置读取方法和error 时候的方法  
	bufferevent_setcb(bev, update_server_read_cb, NULL, update_server_event_cb,(void *)0); 
	
	// 设置类型  
	bufferevent_enable(bev, EV_READ | EV_PERSIST);

	// 事件循环
	event_base_dispatch(base);

	DbgFuncExit();

	return ((void *)0); 
}  

// ----------
void update_socket_init(void)
{
	int ret;

	DbgFuncEntry();

	memset(&update_socket,0x00,sizeof(socket_mgr_t));
	
	pthread_mutex_init(&update_socket_lock, NULL);
	pthread_cond_init(&update_socket_cond, NULL);

	ret = pthread_create(&update_socket_thread_id, NULL, update_socket_loop_func, NULL);
	if(ret != 0)
	{
		DbgError("can't create update socket thread(error = %s)\r\n",strerror(ret));
	}

	DbgFuncExit();
}

void update_socket_send(unsigned char *buff,int len)
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

	if(update_socket.state)
	{
		bufferevent_write(update_socket.bev,buff,len);
	}
	else
	{
		DbgPrintf("update socket is not ready!\r\n");
	}
	
	DbgFuncExit();
}


