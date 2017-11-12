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

// ���յ��ն�����Ĵ���ص�����
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

	//���ն˵���Ϣ���͸���������  
	//Ϊ�˼������������дһ�����ݵ����  
	write(sockfd, msg, ret);  
}

// ���յ���������������Ĵ���ص�����
static void socket_read_cb(int fd, short events, void *arg)  
{
	char msg[1024];

	DbgFuncEntry();

	//Ϊ�˼�����������Ƕ�һ�����ݵ����  
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
	
	// ����sockfd  Ϊ��������socket 
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

	// �������������Ƿ������˵�IP��ַ���˿ں�  
	int sockfd = tcp_connect_server(argv[1], atoi(argv[2]));
	if( sockfd == -1)
	{
		DbgError("tcp_connect error ");
		return -1;
	}
	DbgGood("connect to server successful\r\n");
	
	// ��������ʼ��һ��event_base 
	struct event_base* base = event_base_new();
	
	// ����һ���¼�event : ����������������������¼�
	struct event *ev_sockfd = event_new(base, sockfd,
										EV_READ | EV_PERSIST,
										socket_read_cb,
										NULL);

	// ��event_base  ע��event
	event_add(ev_sockfd, NULL);

	// ����һ���¼�event  :  �����ն������¼�  
	struct event* ev_cmd = event_new(base, STDIN_FILENO,
									 EV_READ | EV_PERSIST,
									 cmd_msg_cb,
									 (void*)&sockfd);
	// ��event_base  ע��event
	event_add(ev_cmd, NULL);

	// �¼�ѭ��
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

	// ����sockfd  Ϊ��������socket 
	evutil_make_socket_nonblocking(sockfd);  
	  
	return sockfd;	
}  

// ���յ��ն�����Ĵ���ص�����
static void cmd_msg_cb(int fd, short events, void* arg)  
{
	char msg[1024];
	
	int ret = read(fd, msg, sizeof(msg));
	
	if( ret <= 0 )
	{
		DbgError("read fail ");
		exit(1);
	}

	// ��ȡ���������bufferevent
	struct bufferevent* bev = (struct bufferevent*)arg;  
	
	//���ն˵���Ϣ���͸���������  
	bufferevent_write(bev, msg, ret);  
}

// ���յ���������������Ĵ���ص�����
static void server_msg_cb(struct bufferevent* bev, void* arg)  
{
	char msg[1024];

	DbgFuncEntry();

	size_t len = bufferevent_read(bev, msg, sizeof(msg));
	msg[len] = '\0';

	DbgPrintf("recv %s from server\r\n", msg); 

	DbgFuncExit();
}

// ���յ��������������Ĵ���ص�����
static void event_cb(struct bufferevent *bev, short event, void *arg)  
{
	DbgFuncEntry();
	
	if (event & BEV_EVENT_EOF)
        DbgPrintf("connection closed\r\n");  
    else if (event & BEV_EVENT_ERROR)  
        DbgPrintf("some other error\n");  
  
    // �⽫�Զ�close �׽��ֺ�free��д������  
    bufferevent_free(bev);  
  
    struct event *ev = (struct event*)arg;  
    //��Ϊsocket�Ѿ�û�У��������eventҲû�д��ڵı�Ҫ��  
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
	
    //�������������Ƿ������˵�IP ��ַ���˿ں�  
    int sockfd = tcp_connect_server(argv[1], atoi(argv[2]));  
    if( sockfd == -1)  
    {  
        perror("tcp_connect error ");  
        return -1;  
    }
    DbgGood("connect to server successful\r\n");

	// ��������ʼ��һ��event_base 
	struct event_base* base = event_base_new();

	// ����һ��bufferevent
	// BEV_OPT_CLOSE_ON_FREE : �� bufferevent ���ͷ�ͬʱ�رյײ�
    struct bufferevent* bev = bufferevent_socket_new(base, 
    												 sockfd,
    												 BEV_OPT_CLOSE_ON_FREE);

	// �����ն������¼�  
    struct event* ev_cmd = event_new(base, 
    								 STDIN_FILENO,
    								 EV_READ | EV_PERSIST,
    								 cmd_msg_cb,
    								 (void*)bev);
	// ��event_base  ע��event
	event_add(ev_cmd, NULL);

	// ��socket  �ر�ʱ���õ��ص�����  
	// ���ö�ȡ������error ʱ��ķ���  
	bufferevent_setcb(bev, server_msg_cb, NULL, event_cb, (void*)ev_cmd); 
	// ��������  
	bufferevent_enable(bev, EV_READ | EV_PERSIST);

	// �¼�ѭ��
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


// ���յ��ն�����Ĵ���ص�����
static void cmd_msg_cb(int fd, short events, void* arg)  
{
	char msg[1024];

	int ret = read(fd, msg, sizeof(msg));
	if( ret < 0 )
	{
		DbgError("read fail \r\n");
		exit(1);
	}

	// ��ȡ���������bufferevent
	struct bufferevent* bev = (struct bufferevent*)arg;

	// ���ն˵���Ϣ���͸���������  
	bufferevent_write(bev, msg, ret); 
} 

// ���յ���������������Ĵ���ص�����
static void server_msg_cb(struct bufferevent* bev, void* arg)  
{
	char msg[1024];

	DbgFuncEntry();

	size_t len = bufferevent_read(bev, msg, sizeof(msg));
	msg[len] = '\0';

	DbgPrintf("recv %s from server\n", msg);

	DbgFuncEntry();
}  
  
  


// ���յ��������������Ĵ���ص�����
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

	//�⽫�Զ�close  �׽��ֺ�free  ��д������  
	bufferevent_free(bev);

	struct event *ev = (struct event*)arg;

	// �ͷ�event 
	event_free(ev);

	DbgFuncExit();
	
}  

// ----------
int demo_libevent_client(int argc, char** argv)  
{
	DbgFuncEntry();

	if( argc < 3 )
	{
		//�������������Ƿ������˵�IP  ��ַ���˿ں� 
		DbgError("please input 2 parameter!\r\n");
		return -1;
	}

	// ��������ʼ��һ��event_base 
	struct event_base *base = event_base_new();

	// ����һ��bufferevent
	// BEV_OPT_CLOSE_ON_FREE : �� bufferevent ���ͷ�ͬʱ�رյײ�
	struct bufferevent* bev = bufferevent_socket_new(base,
    												 -1,
    												 BEV_OPT_CLOSE_ON_FREE);

	//�����ն������¼�  
	struct event* ev_cmd = event_new(base, STDIN_FILENO,  
                                     EV_READ | EV_PERSIST,  
                                     cmd_msg_cb, (void*)bev);  
  
	// ��event_base  ע��event
	event_add(ev_cmd, NULL);

	struct sockaddr_in server_addr;

	memset(&server_addr, 0, sizeof(server_addr) );
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	inet_aton(argv[1], &server_addr.sin_addr);

	// ������������
	bufferevent_socket_connect(bev,
							   (struct sockaddr *)&server_addr,
							   sizeof(server_addr));

	// ���ö�ȡ������error ʱ��ķ���	
	bufferevent_setcb(bev, server_msg_cb, NULL, event_cb, (void*)ev_cmd);
	// ��������  
	bufferevent_enable(bev, EV_READ | EV_PERSIST);

	// �¼�ѭ��
	event_base_dispatch(base);


	DbgFuncExit();

	return 0;
}  
  
  

