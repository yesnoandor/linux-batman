
/*
#include	<stdio.h>  
#include	<string.h>  
#include	<errno.h>  
  
#include	<unistd.h>  
#include 	"event2/event.h"

#define		DEBUG_Y
#include	"libs/debug.h"

// ----------
// ���յ��ͻ������ӵĴ���ص�����
static int tcp_server_init(int port, int listen_num)  
{
	int errno_save;
	evutil_socket_t listener;
	struct sockaddr_in sin;
	
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if( listener == -1 )
		return -1;

	//  ���ö˿�����
    //  �����ΰ�ͬһ����ַ��Ҫ����socket  ��bind֮��  
    evutil_make_listen_socket_reuseable(listener);  
  
    sin.sin_family = AF_INET;  
    sin.sin_addr.s_addr = 0;  
    sin.sin_port = htons(port);  
  
    if(bind(listener, (struct sockaddr *)&sin, sizeof(sin)) < 0 )  
        goto error;  
  
    if(listen(listener, listen_num) < 0)  
        goto error;  
  
  
    // ��ƽ̨ͳһ�ӿڣ����׽�������Ϊ������״̬  
    evutil_make_socket_nonblocking(listener);  
  
    return listener;
	
error:
	errno_save = errno;
	evutil_closesocket(listener);
	errno = errno_save;

	return -1;  
}  

// ���յ��ͻ�����������Ĵ���ص�����
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
  
// ���յ��������յ�һ�����ӵĴ���ص�����
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

	// ��ȡ���ڵ�event_base
	base = (struct event_base*)arg;

	// ����һ���¼�event  :  ��������ͻ����������¼�  	
    // ������Ϊ�˶�̬����һ��event  �ṹ��  
    struct event *ev = event_new(NULL, -1, 0, NULL, NULL);
	//����̬�����Ľṹ����Ϊevent   �Ļص�����  
    event_assign(ev,
    			 base,
    			 sockfd, 
    			 EV_READ | EV_PERSIST,  
                 socket_read_cb,
                 (void*)ev);
	// ��event_base  ע��event
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
	
	// ��������ʼ��һ��event_base 
	struct event_base* base = event_base_new();  
  
	// ��Ӽ����ͻ������������¼�  
	struct event* ev_listen = event_new(base,
    									listener,
    									EV_READ | EV_PERSIST,  
                                        accept_cb,
                                        base);
	// ��event_base  ע��event
	event_add(ev_listen, NULL);

	// �¼�ѭ��
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

	// ���ö˿�����  ( Ҫ����socket  ��bind֮��)
	evutil_make_listen_socket_reuseable(listener);

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(port);
	
    if(bind(listener, (struct sockaddr *)&sin, sizeof(sin)) < 0 )  
        goto error;
	
	if(listen(listener, listen_num) < 0)
		goto error;
	
    // ��ƽ̨ͳһ�ӿڣ����׽�������Ϊ������״̬  
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

	// ��ȡ������������
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

	// �⽫�Զ�close  �׽��ֺ�free��д������  
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

	// ����sockfd  Ϊ��������socket 
	evutil_make_socket_nonblocking(sockfd);  
  
    DbgGood("accept a client %d\r\n", sockfd);

	// ��ȡ���ڵ�event_base
	base = (struct event_base*)arg;

	// ����һ��bufferevent : ��������ͻ����������¼�  
    struct bufferevent* bev = bufferevent_socket_new(base, sockfd, BEV_OPT_CLOSE_ON_FREE);
	// ���ö�ȡ�����ͳ�������  
	bufferevent_setcb(bev, socket_read_cb, NULL, event_cb, arg);
	// ��������  
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

	// ��������ʼ��һ��event_base 
	struct event_base* base = event_base_new();
	
    // ��Ӽ����ͻ������������¼�  
    struct event* ev_listen = event_new(base,
    									listener,
    									EV_READ | EV_PERSIST,
    									accept_cb,
    									base);
	// ��event_base  ע��event
    event_add(ev_listen, NULL);

	// �¼�ѭ��
    event_base_dispatch(base);

	//  ����event_base
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

	// ��ȡ������������
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
    
	//�⽫�Զ�close  �׽��ֺ�free  ��д������    
	bufferevent_free(bev);

	DbgFuncExit();	
}    


//һ���¿ͻ��������Ϸ�������    
//���˺���������ʱ��libevent�Ѿ�������accept������ͻ��ˡ��ÿͻ��˵�  
//�ļ�������Ϊfd  
static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,    
                 struct sockaddr *sock, int socklen, void *arg)    
{
	DbgFuncEntry();
	
	DbgGood("accept a client %d\n", fd);

	struct event_base *base = (struct event_base*)arg;    

	// Ϊ����ͻ��˷���һ��bufferevent    
	struct bufferevent *bev =  bufferevent_socket_new(base, fd,    
                                               BEV_OPT_CLOSE_ON_FREE);    

	// ���ö�ȡ�����ͳ�������  
    bufferevent_setcb(bev, socket_read_cb, NULL, socket_event_cb, NULL);

	// �������� 
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

	// ��������ʼ��һ��event_base 
	struct event_base *base = event_base_new();

	// �������Ӽ�����
	struct evconnlistener *listener 
		= evconnlistener_new_bind(base,
								listener_cb, base,
								LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,    
                                      10, (struct sockaddr*)&sin,    
                                      sizeof(struct sockaddr_in));
	// �¼�ѭ��
	event_base_dispatch(base);

	// �ͷ����Ӽ�����
	evconnlistener_free(listener);

	//  ����event_base
	event_base_free(base);

	DbgFuncExit();

	return 0;    
}    

 

