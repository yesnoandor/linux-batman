
/*
#include	<stdio.h>  
#include	<string.h>  
#include	<errno.h>  
  
#include	<unistd.h>  
#include 	"event2/event.h"

#define		DEBUG_Y
#include	"libs/debug.h"

// ----------
// 接收到客户端链接的处理回调函数
static int tcp_server_init(int port, int listen_num)  
{
	int errno_save;
	evutil_socket_t listener;
	struct sockaddr_in sin;
	
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if( listener == -1 )
		return -1;

	//  设置端口重用
    //  允许多次绑定同一个地址。要用在socket  和bind之间  
    evutil_make_listen_socket_reuseable(listener);  
  
    sin.sin_family = AF_INET;  
    sin.sin_addr.s_addr = 0;  
    sin.sin_port = htons(port);  
  
    if(bind(listener, (struct sockaddr *)&sin, sizeof(sin)) < 0 )  
        goto error;  
  
    if(listen(listener, listen_num) < 0)  
        goto error;  
  
  
    // 跨平台统一接口，将套接字设置为非阻塞状态  
    evutil_make_socket_nonblocking(listener);  
  
    return listener;
	
error:
	errno_save = errno;
	evutil_closesocket(listener);
	errno = errno_save;

	return -1;  
}  

// 接收到客户端网络输入的处理回调函数
static void socket_read_cb(int fd, short events, void *arg)  
{
	char msg[4096];
	struct event *ev = (struct event*)arg;
	int len;
	
	DbgFuncEntry();

	len = read(fd, msg, sizeof(msg) - 1);

	if( len <= 0 )
	{  
        printf("some error happen when read\n");  
        event_free(ev);  
        close(fd);  
        return ;
    }  
  
    msg[len] = '\0';  
    printf("recv the client msg: %s", msg);  
  
    char reply_msg[4096] = "I have recvieced the msg: ";  
    strcat(reply_msg + strlen(reply_msg), msg);  
  
    write(fd, reply_msg, strlen(reply_msg) );

	DbgFuncExit();
}
  
// 接收到服务器收到一个链接的处理回调函数
static void accept_cb(int fd, short events, void* arg)  
{
	evutil_socket_t sockfd;
	struct sockaddr_in client;
	struct event_base* base;

	DbgFuncEntry();

	socklen_t len = sizeof(client);

	sockfd = accept(fd, (struct sockaddr*)&client, &len );
	evutil_make_socket_nonblocking(sockfd);  
  
    DbgGood("accept a client %d\r\n", sockfd);

	// 获取存在的event_base
	base = (struct event_base*)arg;

	// 创建一个事件event  :  监听网络客户端有输入事件  	
    // 仅仅是为了动态创建一个event  结构体  
    struct event *ev = event_new(NULL, -1, 0, NULL, NULL);
	//将动态创建的结构体作为event   的回调参数  
    event_assign(ev,
    			 base,
    			 sockfd, 
    			 EV_READ | EV_PERSIST,  
                 socket_read_cb,
                 (void*)ev);
	// 向event_base  注册event
    event_add(ev, NULL);

	DbgFuncExit();
}  
  
  
  

//----------
int demo_libevent_server(void)  
{
	DbgFuncEntry();
	
	int listener = tcp_server_init(9999, 10);

	if( listener == -1 )
	{
		perror(" tcp_server_init error ");
		return -1;
	}
	
	// 创建并初始化一个event_base 
	struct event_base* base = event_base_new();  
  
	// 添加监听客户端请求连接事件  
	struct event* ev_listen = event_new(base,
    									listener,
    									EV_READ | EV_PERSIST,  
                                        accept_cb,
                                        base);
	// 向event_base  注册event
	event_add(ev_listen, NULL);

	// 事件循环
	event_base_dispatch(base);

	DbgFuncExit();

	return 0;  
}  
*/


/*
#include	<stdio.h>
#include	<string.h>
#include	<errno.h>


#include	"event2/event.h"
#include	"event2/bufferevent.h" 


#define		DEBUG_Y
#include	"libs/debug.h"

static int tcp_server_init(int port, int listen_num)  
{  
    int errno_save;  
    evutil_socket_t listener;
	struct sockaddr_in sin;

	listener = socket(AF_INET, SOCK_STREAM, 0);  
    if( listener == -1 )  
        return -1;

	// 设置端口重用  ( 要用在socket  和bind之间)
	evutil_make_listen_socket_reuseable(listener);

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(port);
	
    if(bind(listener, (struct sockaddr *)&sin, sizeof(sin)) < 0 )  
        goto error;
	
	if(listen(listener, listen_num) < 0)
		goto error;
	
    // 跨平台统一接口，将套接字设置为非阻塞状态  
    evutil_make_socket_nonblocking(listener);  
  
    return listener;  
  
error:
	errno_save = errno;
	evutil_closesocket(listener);
	errno = errno_save;

	return -1;  
}  

static void socket_read_cb(struct bufferevent* bev, void* arg)  
{
	char msg[4096];

	DbgFuncEntry();

	// 获取网络输入数据
	size_t len = bufferevent_read(bev, msg, sizeof(msg));
    msg[len] = '\0';
	
    DbgPrintf("recv the client msg: %s", msg);

	char reply_msg[4096] = "I have recvieced the msg: ";
    strcat(reply_msg + strlen(reply_msg), msg);
    bufferevent_write(bev, reply_msg, strlen(reply_msg));

	DbgFuncExit();
}

static void event_cb(struct bufferevent *bev, short event, void *arg)  
{
	DbgFuncEntry();

	if (event & BEV_EVENT_EOF)
		DbgPrintf("connection closed\r\n");
	else if (event & BEV_EVENT_ERROR)
		DbgPrintf("some other error\r\n");

	// 这将自动close  套接字和free读写缓冲区  
	bufferevent_free(bev);

	DbgFuncExit();
}  

static void accept_cb(int fd, short events, void* arg)  
{
	evutil_socket_t sockfd;
	struct sockaddr_in client;
	struct event_base* base;

	DbgFuncEntry();
	
	socklen_t len = sizeof(client);

	sockfd = accept(fd, (struct sockaddr *)&client, &len );

	// 设置sockfd  为非阻塞的socket 
	evutil_make_socket_nonblocking(sockfd);  
  
    DbgGood("accept a client %d\r\n", sockfd);

	// 获取存在的event_base
	base = (struct event_base*)arg;

	// 创建一个bufferevent : 监听网络客户端有输入事件  
    struct bufferevent* bev = bufferevent_socket_new(base, sockfd, BEV_OPT_CLOSE_ON_FREE);
	// 设置读取方法和出错处理方法  
	bufferevent_setcb(bev, socket_read_cb, NULL, event_cb, arg);
	// 设置类型  
    bufferevent_enable(bev, EV_READ | EV_PERSIST);

	DbgFuncExit();
}

//----------
int demo_libevent_server(int argc, char** argv)  
{
	DbgFuncEntry();

	int listener = tcp_server_init(9999, 10);  
	if( listener == -1 )
	{
		perror(" tcp_server_init error ");
		return -1;
	}

	// 创建并初始化一个event_base 
	struct event_base* base = event_base_new();
	
    // 添加监听客户端请求连接事件  
    struct event* ev_listen = event_new(base,
    									listener,
    									EV_READ | EV_PERSIST,
    									accept_cb,
    									base);
	// 向event_base  注册event
    event_add(ev_listen, NULL);

	// 事件循环
    event_base_dispatch(base);

	//  销毁event_base
	event_base_free(base);

	DbgFuncExit();
	
	return 0;  
}  
*/


   
#include	<netinet/in.h>    
#include	<sys/socket.h>    
#include	<unistd.h>    
	   


#include	<stdio.h>
#include	<string.h>
#include	<errno.h>


#include	"event2/event.h"
#include	"event2/listener.h"
#include	"event2/bufferevent.h" 
#include	"event2/thread.h"  


#define		DEBUG_Y
#include	"libs/debug.h"

static void socket_read_cb(struct bufferevent *bev, void *arg)    
{
	char msg[4096];

	DbgFuncEntry();

	// 获取网络输入数据
	size_t len = bufferevent_read(bev, msg, sizeof(msg)-1 );

	msg[len] = '\0';
	DbgPrintf("server read the data %s\n", msg);

	char reply[4096] = "I has read your data: ";
	strcat(reply + strlen(reply), msg);
	bufferevent_write(bev, reply, strlen(reply) );
	
	DbgFuncExit();
}

static void socket_event_cb(struct bufferevent *bev, short events, void *arg)    
{
	DbgFuncEntry();

	if (events & BEV_EVENT_EOF)
		DbgPrintf("connection closed\r\n");
	else if (events & BEV_EVENT_ERROR)
        DbgPrintf("some other error\r\n");
    
	//这将自动close  套接字和free  读写缓冲区    
	bufferevent_free(bev);

	DbgFuncExit();	
}    


//一个新客户端连接上服务器了    
//当此函数被调用时，libevent已经帮我们accept了这个客户端。该客户端的  
//文件描述符为fd  
static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,    
                 struct sockaddr *sock, int socklen, void *arg)    
{
	DbgFuncEntry();
	
	DbgGood("accept a client %d\n", fd);

	struct event_base *base = (struct event_base*)arg;    

	// 为这个客户端分配一个bufferevent    
	struct bufferevent *bev =  bufferevent_socket_new(base, fd,    
                                               BEV_OPT_CLOSE_ON_FREE);    

	// 设置读取方法和出错处理方法  
    bufferevent_setcb(bev, socket_read_cb, NULL, socket_event_cb, NULL);

	// 设置类型 
	bufferevent_enable(bev, EV_READ | EV_PERSIST);

	DbgFuncExit();
}

int demo_libevent_server(void)    
{
	DbgFuncEntry();
	
	//evthread_use_pthreads();		//enable threads    

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(9999);

	// 创建并初始化一个event_base 
	struct event_base *base = event_base_new();

	// 创建连接监听器
	struct evconnlistener *listener 
		= evconnlistener_new_bind(base,
								listener_cb, base,
								LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,    
                                      10, (struct sockaddr*)&sin,    
                                      sizeof(struct sockaddr_in));
	// 事件循环
	event_base_dispatch(base);

	// 释放连接监听器
	evconnlistener_free(listener);

	//  销毁event_base
	event_base_free(base);

	DbgFuncExit();

	return 0;    
}    

 

