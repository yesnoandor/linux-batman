/****************************************Copyright (c)**************************************************                         
** File name:			demo_libevent_client.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-10-28
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

/*
#include	<sys/types.h>  
#include	<sys/socket.h>  
#include	<netinet/in.h>  
#include	<arpa/inet.h>  
#include	<errno.h>  
#include	<unistd.h>  
  
#include	<stdio.h>  
#include	<string.h>  
#include	<stdlib.h>  
  
#include 	"event2/event.h"  
#include 	"event2/bufferevent.h" 
#include	"event2/buffer.h"
#include	"event2/util.h"

#define		DEBUG_Y
#include	"libs/debug.h"

// 接收到终端输入的处理回调函数
static void cmd_msg_cb(int fd, short events, void* arg)  
{
	char msg[1024];

	int ret = read(fd, msg, sizeof(msg));

	if( ret <= 0 )
	{
		DbgError("read fail ");
		exit(1);
	}

	int sockfd = *((int*)arg);

	//把终端的消息发送给服务器端  
	//为了简单起见，不考虑写一半数据的情况  
	write(sockfd, msg, ret);  
}

// 接收到服务器网络输入的处理回调函数
static void socket_read_cb(int fd, short events, void *arg)  
{
	char msg[1024];

	DbgFuncEntry();

	//为了简单起见，不考虑读一半数据的情况  
	int len = read(fd, msg, sizeof(msg)-1);
	if( len <= 0 )
	{
		DbgError("read fail (err = %s)\r\n",strerror(errno));
		exit(1);
	}

	msg[len] = '\0';
	
	DbgPrintf("recv %s from server\r\n", msg);  
}

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

//----------
int demo_libevent_client(int argc, char** argv)  
{	
	DbgFuncEntry();

	if( argc < 3 )
	{
		DbgError("please input 2 parameter!\r\n");
		return -1;
	}

	// 两个参数依次是服务器端的IP地址、端口号  
	int sockfd = tcp_connect_server(argv[1], atoi(argv[2]));
	if( sockfd == -1)
	{
		DbgError("tcp_connect error ");
		return -1;
	}
	DbgGood("connect to server successful\r\n");
	
	// 创建并初始化一个event_base 
	struct event_base* base = event_base_new();
	
	// 创建一个事件event : 监听网络服务器数据输入事件
	struct event *ev_sockfd = event_new(base, sockfd,
										EV_READ | EV_PERSIST,
										socket_read_cb,
										NULL);

	// 向event_base  注册event
	event_add(ev_sockfd, NULL);

	// 创建一个事件event  :  监听终端输入事件  
	struct event* ev_cmd = event_new(base, STDIN_FILENO,
									 EV_READ | EV_PERSIST,
									 cmd_msg_cb,
									 (void*)&sockfd);
	// 向event_base  注册event
	event_add(ev_cmd, NULL);

	// 事件循环
	event_base_dispatch(base);  
  
	DbgFuncExit();
	
	return 0;
}  
*/

/*
//-------------------------
#include	<sys/types.h>
#include	<sys/socket.h>  
#include	<netinet/in.h>  
#include	<arpa/inet.h>  
#include	<errno.h>  
#include	<unistd.h>  
  
#include	<stdio.h>  
#include	<string.h>  
#include	<stdlib.h>  
  
#include	"event2/event.h"
#include	"event2/bufferevent.h" 
#include	"event2/buffer.h" 
#include	"event2/util.h"

#define		DEBUG_Y
#include	"libs/debug.h"
  
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

// 接收到终端输入的处理回调函数
static void cmd_msg_cb(int fd, short events, void* arg)  
{
	char msg[1024];
	
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
}

// 接收到服务器网络输入的处理回调函数
static void server_msg_cb(struct bufferevent* bev, void* arg)  
{
	char msg[1024];

	DbgFuncEntry();

	size_t len = bufferevent_read(bev, msg, sizeof(msg));
	msg[len] = '\0';

	DbgPrintf("recv %s from server\r\n", msg); 

	DbgFuncExit();
}

// 接收到服务器网络出错的处理回调函数
static void event_cb(struct bufferevent *bev, short event, void *arg)  
{
	DbgFuncEntry();
	
	if (event & BEV_EVENT_EOF)
        DbgPrintf("connection closed\r\n");  
    else if (event & BEV_EVENT_ERROR)  
        DbgPrintf("some other error\n");  
  
    // 这将自动close 套接字和free读写缓冲区  
    bufferevent_free(bev);  
  
    struct event *ev = (struct event*)arg;  
    //因为socket已经没有，所以这个event也没有存在的必要了  
    event_free(ev);

	DbgFuncExit();
}  

// ----------
int demo_libevent_client(int argc, char** argv)  
{
	DbgFuncEntry();
	
    if( argc < 3 )
    {
    	DbgError("please input 2 parameter!\r\n");
		return -1;
	}
	
    //两个参数依次是服务器端的IP 地址、端口号  
    int sockfd = tcp_connect_server(argv[1], atoi(argv[2]));  
    if( sockfd == -1)  
    {  
        perror("tcp_connect error ");  
        return -1;  
    }
    DbgGood("connect to server successful\r\n");

	// 创建并初始化一个event_base 
	struct event_base* base = event_base_new();

	// 创建一个bufferevent
	// BEV_OPT_CLOSE_ON_FREE : 当 bufferevent 被释放同时关闭底层
    struct bufferevent* bev = bufferevent_socket_new(base, 
    												 sockfd,
    												 BEV_OPT_CLOSE_ON_FREE);

	// 监听终端输入事件  
    struct event* ev_cmd = event_new(base, 
    								 STDIN_FILENO,
    								 EV_READ | EV_PERSIST,
    								 cmd_msg_cb,
    								 (void*)bev);
	// 向event_base  注册event
	event_add(ev_cmd, NULL);

	// 当socket  关闭时会用到回调参数  
	// 设置读取方法和error 时候的方法  
	bufferevent_setcb(bev, server_msg_cb, NULL, event_cb, (void*)ev_cmd); 
	// 设置类型  
	bufferevent_enable(bev, EV_READ | EV_PERSIST);

	// 事件循环
	event_base_dispatch(base);

	DbgFuncExit();

	return 0;  
}  
*/



// ----------    
#include	<sys/types.h>
#include	<sys/socket.h>  
#include	<netinet/in.h>  
#include	<arpa/inet.h>  
#include	<errno.h>  
#include	<unistd.h>  
  
#include	<stdio.h>  
#include	<string.h>  
#include	<stdlib.h>  
  
#include	"event2/event.h"
#include	"event2/bufferevent.h" 
#include	"event2/buffer.h" 
#include	"event2/util.h"

#define		DEBUG_Y
#include	"libs/debug.h"


// 接收到终端输入的处理回调函数
static void cmd_msg_cb(int fd, short events, void* arg)  
{
	char msg[1024];

	int ret = read(fd, msg, sizeof(msg));
	if( ret < 0 )
	{
		DbgError("read fail \r\n");
		exit(1);
	}

	// 获取网络输入的bufferevent
	struct bufferevent* bev = (struct bufferevent*)arg;

	// 把终端的消息发送给服务器端  
	bufferevent_write(bev, msg, ret); 
} 

// 接收到服务器网络输入的处理回调函数
static void server_msg_cb(struct bufferevent* bev, void* arg)  
{
	char msg[1024];

	DbgFuncEntry();

	size_t len = bufferevent_read(bev, msg, sizeof(msg));
	msg[len] = '\0';

	DbgPrintf("recv %s from server\n", msg);

	DbgFuncEntry();
}  
  
  


// 接收到服务器网络出错的处理回调函数
static void event_cb(struct bufferevent *bev, short event, void *arg)  
{
	DbgFuncEntry();

	if (event & BEV_EVENT_EOF)
	{
		DbgPrintf("connection closed!\r\n");
	}
	else if (event & BEV_EVENT_ERROR)
	{
		DbgPrintf("some other error!\r\n");
	}
	else if( event & BEV_EVENT_CONNECTED)
	{
		DbgGood("the client has connected to server!\r\n");
		return ;
	}

	//这将自动close  套接字和free  读写缓冲区  
	bufferevent_free(bev);

	struct event *ev = (struct event*)arg;

	// 释放event 
	event_free(ev);

	DbgFuncExit();
	
}  

// ----------
int demo_libevent_client(int argc, char** argv)  
{
	DbgFuncEntry();

	if( argc < 3 )
	{
		//两个参数依次是服务器端的IP  地址、端口号 
		DbgError("please input 2 parameter!\r\n");
		return -1;
	}

	// 创建并初始化一个event_base 
	struct event_base *base = event_base_new();

	// 创建一个bufferevent
	// BEV_OPT_CLOSE_ON_FREE : 当 bufferevent 被释放同时关闭底层
	struct bufferevent* bev = bufferevent_socket_new(base,
    												 -1,
    												 BEV_OPT_CLOSE_ON_FREE);

	//监听终端输入事件  
	struct event* ev_cmd = event_new(base, STDIN_FILENO,  
                                     EV_READ | EV_PERSIST,  
                                     cmd_msg_cb, (void*)bev);  
  
	// 向event_base  注册event
	event_add(ev_cmd, NULL);

	struct sockaddr_in server_addr;

	memset(&server_addr, 0, sizeof(server_addr) );
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	inet_aton(argv[1], &server_addr.sin_addr);

	// 建立网络链接
	bufferevent_socket_connect(bev,
							   (struct sockaddr *)&server_addr,
							   sizeof(server_addr));

	// 设置读取方法和error 时候的方法	
	bufferevent_setcb(bev, server_msg_cb, NULL, event_cb, (void*)ev_cmd);
	// 设置类型  
	bufferevent_enable(bev, EV_READ | EV_PERSIST);

	// 事件循环
	event_base_dispatch(base);


	DbgFuncExit();

	return 0;
}  
  
  

