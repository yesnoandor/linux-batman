
/************************************************************************
tcp�������Ϳͻ���ʵ��.

server
һ�������߳� + ����ͻ����߳�

client
************************************************************************/

#ifndef __LL_THREAD_TCP_H__
#define __LL_THREAD_TCP_H__

typedef struct __thread_tcp_server TTServer;	// ����������
typedef struct __thread_tcp_client TTClient;	// �ͻ��˶���

// �ͻ���״̬
typedef enum
{
	CLI_STATE_ESTABLISHED = 0,	// ��������
	CLI_STATE_CLOSE,		// �ر�����
	CLI_STATE_RECV_DATA,		// ���յ�����
	CLI_STATE_SEND_DATA,		// �������ݺ�
	CLI_STATE_BEFORE_SELECT,	// ÿ��select֮ǰ
}CLI_STATE_E;

// �ص�����
typedef int (*ThreadTcpCallback)(CLI_STATE_E state,
				const char *recvBuf,
				int recvBufLen,
				TTServer *server,
				TTClient *client,
				void *args);

/************************************************************************
server
************************************************************************/
// ���������ٶ���
TTServer *TTServerNew(void);
void TTServerDestroy(TTServer *server);

// ��ʼ/ֹͣ/��ȡ״̬. 1=��������
int TTServerStartListen(TTServer *server, int port, int timeout, int connMax, ThreadTcpCallback cbfunc, void *cbargs);
int TTServerStopListen(TTServer *server);
int TTServerGetStatus(TTServer *server);

// ��������/��ȡ�ͻ�������
int TTServerSendAll(TTServer *server, const char *bufptr, int buflen);
int TTServerClientNum(TTServer *server);

// ��ӡ
void TTServerPrintInfo(TTServer *server);


/************************************************************************
client
************************************************************************/
// ����������
TTClient *TTClientNew(void);
void TTClientDestroy(TTClient *client);

// ��ʼ/ֹͣ/��ȡ״̬. 1=��������
int TTClientStart(TTClient *client, TTServer *server, int sock, int timeout, ThreadTcpCallback cbfunc, void *cbargs);
int TTClientStop(TTClient *client);
int TTClientGetStatus(TTClient *client);

// �߳��ڲ�����
int TTClientServerStop(TTClient *client);

// ��������/��ȡ��ǰ�ķ���/���ջ����С
int TTClientSend(TTClient *client, const char *bufptr, int buflen);
int TTClientSendEx(TTClient *client, char *bufptr_arr[], int buflen_arr[], int arrlen);
int TTClientGetSendBufSize(TTClient *client);
int TTClientGetRecvBufSize(TTClient *client);

// ����/��ȡ�û�����
void TTClientSetUserData(TTClient *client, int idx, void *userdata);
void *TTClientGetUserData(TTClient *client, int idx);

// ��ӡ
void TTClientPrintInfo(TTClient *client);

#endif // __LL_THREAD_TCP_H__




