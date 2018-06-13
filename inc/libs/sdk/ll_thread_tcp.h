
/************************************************************************
tcp服务器和客户端实现.

server
一个监听线程 + 多个客户端线程

client
************************************************************************/

#ifndef __LL_THREAD_TCP_H__
#define __LL_THREAD_TCP_H__

typedef struct __thread_tcp_server TTServer;	// 服务器对象
typedef struct __thread_tcp_client TTClient;	// 客户端对象

// 客户端状态
typedef enum
{
	CLI_STATE_ESTABLISHED = 0,	// 建立连接
	CLI_STATE_CLOSE,		// 关闭连接
	CLI_STATE_RECV_DATA,		// 接收到数据
	CLI_STATE_SEND_DATA,		// 发送数据后
	CLI_STATE_BEFORE_SELECT,	// 每次select之前
}CLI_STATE_E;

// 回调函数
typedef int (*ThreadTcpCallback)(CLI_STATE_E state,
				const char *recvBuf,
				int recvBufLen,
				TTServer *server,
				TTClient *client,
				void *args);

/************************************************************************
server
************************************************************************/
// 创建及销毁对象
TTServer *TTServerNew(void);
void TTServerDestroy(TTServer *server);

// 开始/停止/获取状态. 1=运行正常
int TTServerStartListen(TTServer *server, int port, int timeout, int connMax, ThreadTcpCallback cbfunc, void *cbargs);
int TTServerStopListen(TTServer *server);
int TTServerGetStatus(TTServer *server);

// 发送数据/获取客户端数量
int TTServerSendAll(TTServer *server, const char *bufptr, int buflen);
int TTServerClientNum(TTServer *server);

// 打印
void TTServerPrintInfo(TTServer *server);


/************************************************************************
client
************************************************************************/
// 创建及销毁
TTClient *TTClientNew(void);
void TTClientDestroy(TTClient *client);

// 开始/停止/获取状态. 1=运行正常
int TTClientStart(TTClient *client, TTServer *server, int sock, int timeout, ThreadTcpCallback cbfunc, void *cbargs);
int TTClientStop(TTClient *client);
int TTClientGetStatus(TTClient *client);

// 线程内部调用
int TTClientServerStop(TTClient *client);

// 发送数据/获取当前的发送/接收缓存大小
int TTClientSend(TTClient *client, const char *bufptr, int buflen);
int TTClientSendEx(TTClient *client, char *bufptr_arr[], int buflen_arr[], int arrlen);
int TTClientGetSendBufSize(TTClient *client);
int TTClientGetRecvBufSize(TTClient *client);

// 设置/获取用户参数
void TTClientSetUserData(TTClient *client, int idx, void *userdata);
void *TTClientGetUserData(TTClient *client, int idx);

// 打印
void TTClientPrintInfo(TTClient *client);

#endif // __LL_THREAD_TCP_H__




