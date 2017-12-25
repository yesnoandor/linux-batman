/****************************************Copyright (c)**************************************************                         
** File name:			fleety_socket.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-10-30
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
#include	"module/gb905/heart_beat/gb905_heart_beat.h"
#include	"module/gb905_ex/ui/ui_common.h"

#include	"middleware/event/fleety_event.h"
#include	"middleware/socket/fleety_socket.h"


#define		DEBUG_Y
#include	"libs/debug.h"

//----------



//#define		MAX_SOCKET_NUM			3




typedef	void(*PTR_SERVER_MSG_CB)(struct bufferevent*,void*);
typedef	void(*PTR_SERVER_EVENT_CB)(struct bufferevent *, short, void *);

typedef struct {
	struct bufferevent* bev;
	int state;		// =0,disconnected; =1,connected
	int timeout;	// 
	int threhold;
}FleetySocketMngr;


static	pthread_t		fleety_socket_thread_id;
static	pthread_cond_t	fleety_socket_cond;
static	pthread_mutex_t fleety_socket_lock;

static char * server_ip[] = 
{
	main_server_ip_addr,
	aux_server_ip_addr,
	ui_server_ip_addr
};


static short server_port[] =
{
	main_server_port,
	aux_server_port,
	ui_server_port
};
	

static FleetySocketMngr	socket_mngr_list[MAX_SOCKET_NUM];
static int pipe_fd[2] = {-1,-1};


static void main_server_read_cb(struct bufferevent* bev, void* arg);
static void main_server_event_cb(struct bufferevent *bev, short event, void *arg);  
static void aux_server_read_cb(struct bufferevent* bev, void* arg);
static void aux_server_event_cb(struct bufferevent *bev, short event, void *arg);  
static void ui_server_read_cb(struct bufferevent* bev, void* arg);
static void ui_server_event_cb(struct bufferevent *bev, short event, void *arg);  


static PTR_SERVER_MSG_CB server_read_cb[] = 
{
	main_server_read_cb,
	aux_server_read_cb,
	ui_server_read_cb
};

static PTR_SERVER_EVENT_CB server_event_cb[] = 
{
	main_server_event_cb,
	aux_server_event_cb,
	ui_server_event_cb
};


/*
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
	  
	sockfd = socket(PF_INET, SOCK_STREAM, 0);  
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
*/

/*
// 接收到终端输入的处理回调函数
static void cmd_msg_cb(int fd, short events, void* arg)  
{
	char msg[1024];

	DbgFuncEntry();
	
	int ret = read(fd, msg, sizeof(msg));
	
	if( ret <= 0 )
	{
		DbgError("read fail ");
		exit(1);
	}

	// 获取网络输入的bufferevent
	struct bufferevent* bev = (struct bufferevent*)arg;  
	
	//把终端的消息发送给服务器端  
	bufferevent_write(bev, msg, ret);

	DbgFuncExit();
}
*/

// 接收到服务器网络输入的处理回调函数
static void main_server_read_cb(struct bufferevent* bev, void* arg)  
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
	
	offset = gb905_protocol_ayalyze(msg,len);
	evbuffer_drain(input,offset);

	socket_mngr_list[MAIN_SOCKET].timeout = 0;

	DbgPrintf("offset = %d\r\n",offset);

	DbgFuncExit(); 
}


// 接收到服务器网络出错的处理回调函数
static void main_server_event_cb(struct bufferevent *bev, short event, void *arg)  
{
	long index;
	
	DbgFuncEntry();

	index = (long)arg;

	DbgPrintf("index = %d\r\n",index);
	
	// 成功连接 状态变更
	if( BEV_EVENT_CONNECTED == event )
	{
        bufferevent_enable( bev, EV_READ | EV_PERSIST);
        socket_mngr_list[index].state = 1;
    }
	else
	{
		if (event & BEV_EVENT_EOF)
			DbgPrintf("connection closed\r\n");
		else if (event & BEV_EVENT_ERROR)
			DbgPrintf("some other error\n");

		// 自动close 套接字和free  读写缓冲区  
		bufferevent_free(bev);

		socket_mngr_list[index].bev = NULL;
		socket_mngr_list[index].state = 0;
	}
	
	DbgFuncExit();
}  

static void aux_server_read_cb(struct bufferevent* bev, void* arg)  
{
	DbgFuncEntry();

	socket_mngr_list[AUX_SOCKET].timeout = 0;
	
	DbgFuncExit();
}

static void aux_server_event_cb(struct bufferevent *bev, short event, void *arg)  
{
	DbgFuncEntry();
	DbgFuncExit();
}

static void ui_server_read_cb(struct bufferevent* bev, void* arg)  
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
		
	offset = ui_protocol_ayalyze(msg,len);
	evbuffer_drain(input,offset);
	
	socket_mngr_list[MAIN_SOCKET].timeout = 0;
	
	DbgPrintf("offset = %d\r\n",offset);
	
	DbgFuncExit(); 
}

static void ui_server_event_cb(struct bufferevent *bev, short event, void *arg)  
{
	long index;
	
	DbgFuncEntry();

	index = (long)arg;

	DbgPrintf("index = %d\r\n",index);
	
	// 成功连接 状态变更
	if( BEV_EVENT_CONNECTED == event )
	{
        bufferevent_enable( bev, EV_READ | EV_PERSIST);
        socket_mngr_list[index].state = 1;
    }
	else
	{
		if (event & BEV_EVENT_EOF)
			DbgPrintf("connection closed\r\n");
		else if (event & BEV_EVENT_ERROR)
			DbgPrintf("some other error\n");

		// 自动close 套接字和free  读写缓冲区  
		bufferevent_free(bev);

		socket_mngr_list[index].bev = NULL;
		socket_mngr_list[index].state = 0;
	}
	
	DbgFuncExit();
}


//  定时器事件回调函数
static void socket_timeout_cb(int fd, short event, void * pArg)
{
	DbgFuncEntry();

	long i;
	struct bufferevent* bev;
	struct event_base* base = (struct event_base*)pArg;

	DbgPrintf("base = 0x%x\r\n",base);

	for(i=0;i<MAX_SOCKET_NUM;i++)
	{
		if((strcmp(server_ip[i],"") == 0) || (server_port[i] == 0))
		{
			continue;
		}

		// 超时保护
		socket_mngr_list[i].timeout++;
		if(socket_mngr_list[i].timeout >= socket_mngr_list[i].threhold)
		{
			socket_mngr_list[i].bev = NULL;
			socket_mngr_list[i].state = 0;

			DbgWarn("socket transfer timeout\r\n");	
		}

		// 重连机制
		if(0 == socket_mngr_list[i].state)
		{
			DbgPrintf("re-connect socket ip = %s port = %d\r\n",server_ip[i],server_port[i]);
			
			// 创建一个bufferevent
			// BEV_OPT_CLOSE_ON_FREE : 当 bufferevent 被释放同时关闭底层
			bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);	
				
			// 设置读取方法和error 时候的方法  
			bufferevent_setcb(bev, server_read_cb[i], NULL, server_event_cb[i], (void *)i); 
			// 设置类型  
			//bufferevent_enable(bev, EV_READ | EV_PERSIST);
		
			struct sockaddr_in tSockAddr;
			memset(&tSockAddr, 0, sizeof(tSockAddr));
			tSockAddr.sin_family = AF_INET;
			tSockAddr.sin_addr.s_addr = inet_addr(server_ip[0]);
			tSockAddr.sin_port = htons(server_port[0]);
		
			// 连接服务器
			if( bufferevent_socket_connect(bev, (struct sockaddr*)&tSockAddr, sizeof(tSockAddr)) < 0)
			{
				DbgError("connect to server error(ip = %s,port = %d) failed!\r\n",server_ip[0],server_port[0]); 
			}
		}
		else
		{
			#if 0
			// 心跳机制
			switch (i)
			{
				case MAIN_SOCKET:
					gb905_heart_beat_send();
					break;

				default:
					break;
			}
			#endif
		}
	
	}
	
	DbgFuncExit();
}

static void pipe_read_cb(int fd, short event, void * pArg)
{
	int len;
	unsigned char msg[1024];
	
	DbgFuncEntry();

	len = read(fd, msg, sizeof(msg));
	
	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("msg[%d] = 0x%x\r\n",i,msg[i]);
		}
	}

	fleety_event_treat();
	
	DbgFuncExit();
}

static void * fleety_socket_loop_func(void *arg)
{
	long i;
	struct bufferevent* bev;
	struct event_base* base;
		
	DbgFuncEntry();

	// 创建并初始化一个event_base  (  支持多线程)
	evthread_use_pthreads();
	base = event_base_new();	
	evthread_make_base_notifiable(base);
	//DbgPrintf("base = 0x%x\r\n",base);
	
	for(i=0;i<MAX_SOCKET_NUM;i++)
	{
		if((strcmp(server_ip[i],"") == 0) || (server_port[i] == 0))
		{
			continue;
		}
			
		// 创建一个bufferevent
		// BEV_OPT_CLOSE_ON_FREE : 当 bufferevent 被释放同时关闭底层
		bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
		// 设置读取方法和error 时候的方法  
		bufferevent_setcb(bev, server_read_cb[i], NULL, server_event_cb[i],(void *)i); 
		// 设置类型  
		//bufferevent_enable(bev, EV_READ | EV_PERSIST);
	
		struct sockaddr_in tSockAddr;
		memset(&tSockAddr, 0, sizeof(tSockAddr));
		tSockAddr.sin_family = AF_INET;
		tSockAddr.sin_addr.s_addr = inet_addr(server_ip[i]);
		tSockAddr.sin_port = htons(server_port[i]);
	
		// 连接服务器
		if(bufferevent_socket_connect(bev, (struct sockaddr*)&tSockAddr, sizeof(tSockAddr)) < 0)
		{
			DbgError("connect to server error(ip = %s,port = %d) failed!\r\n",server_ip[i],server_port[i]); 
			continue;
		}
	
		socket_mngr_list[i].bev = bev;
		socket_mngr_list[i].state = 0;
		socket_mngr_list[i].timeout = 0;
		socket_mngr_list[i].threhold = 10;
	}

	// 创建超时事件
	struct event *timeout = NULL;
	struct timeval tv = {10, 0};
	
	// 增加定时器事件
	timeout = event_new(base, -1, EV_PERSIST, socket_timeout_cb, base);
	// 激活定时器
	evtimer_add(timeout, &tv);
	
	// 创建fifo   匿名管道事件
	struct event *pipe_event = NULL;
	
	if(pipe(pipe_fd))
	{
		DbgError("create pipe failed!\r\n");
		return ((void *)-1);;
	}
	
	DbgGood("open pipe success!\r\n");

	pipe_event = event_new(base,
						   pipe_fd[0],
						   EV_READ | EV_PERSIST,
						   pipe_read_cb,
						   NULL);
	event_add(pipe_event, NULL);
	
	
	/*
	// 监听终端输入事件  
	struct event* ev_cmd = event_new(base, 
									 STDIN_FILENO,
									 EV_READ | EV_PERSIST,
									 cmd_msg_cb,
									 (void*)bev);
	// 向event_base  注册event
	event_add(ev_cmd, NULL);
	*/
		
	// 事件循环
	event_base_dispatch(base);
	
	// 取消定时器
	evtimer_del(timeout);
		
	// 这将自动close  套接字和free  读写缓冲区  
	bufferevent_free(bev);
	
	// 销毁event_base
	event_base_free(base);
	
	DbgFuncExit();

	return ((void *)0);
}

// ----------
void fleety_socket_init(void)
{
	int ret;

	DbgFuncEntry();

	memset(socket_mngr_list,0x00,sizeof(FleetySocketMngr));
	
	pthread_mutex_init(&fleety_socket_lock, NULL);
	pthread_cond_init(&fleety_socket_cond, NULL);

	ret = pthread_create(&fleety_socket_thread_id, NULL, fleety_socket_loop_func, NULL);
	if(ret != 0){
		DbgError("can't create fleety socket thread(error = %s)\r\n",strerror(ret));
	}

	DbgFuncExit();
}

void fleety_socket_send(int index,unsigned char *buff,int len)
{
	DbgFuncEntry();

	DbgPrintf("index = %d\r\n",index);
	
	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("buff[%d] = 0x%x\r\n",i,buff[i]);
		}
	}

	if(socket_mngr_list[index].state)
	{
		bufferevent_write(socket_mngr_list[index].bev,buff,len);
	}
	else
	{
		DbgPrintf("socket[%d] is not ready!\r\n",index);
	}
	
	DbgFuncExit();
}

void fleety_pipe_send(void)
{
	char c = 'w';
	
	DbgFuncEntry();

	write(pipe_fd[1],&c,sizeof(char));

	DbgFuncExit();
}

