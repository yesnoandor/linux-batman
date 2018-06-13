
#ifndef __LL_SOCKLIB_H__
#define __LL_SOCKLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
udp
************************************************************************/
int net_udp_new(int port, OUT struct sockaddr_in *addr);		// udp����
int net_bcast_new(int port, OUT struct sockaddr_in *addr);		// udp�㲥
int net_mcast_new(char *mip, int mport, OUT struct sockaddr_in *addr);	// udp�ಥ(�鲥)
int net_udp_read(int sock, char *bufptr, int buflen, OUT struct sockaddr_in *addr);
int net_udp_write(int sock, char *bufptr, int buflen, struct sockaddr_in *addr);


/************************************************************************
tcp
************************************************************************/
// ����Զ������. timeout-��ʱʱ��(��λms), -1Ϊ�����汾
int net_tcp_connect(char *host, int port, OUT struct sockaddr_in *addr, int timeout);
int net_tcp_listen(int port, OUT struct sockaddr_in *addr);
int net_accept_new(int listen_sock, OUT struct sockaddr_in *addr);

/************************************************************************
һЩ��������
************************************************************************/
// �����׽���Ϊ��������ʽ
// ���� : sock - ���ڼ����л��Ѿ����ӵ��׽���, flag - 1 ������, 0 ����
int net_make_socket_nonblocking(int sock, int flag);
// ��������ip����sockaddr�ṹ��(���б�Ҫ������dns����)
int net_make_addr(char *host, int port, OUT struct sockaddr_in *addr);
// �����׽��ֵ��ַ�����Ϣ
char *net_sock_info(int sock);
// ����׽��ֵĴ���
int net_check_socket(int sock);
// ����opt, ����keepalive/linger
int net_test_set_opt(int sock);
// ��ȡ���е�opt����
int net_test_print_opt(int sock);

// ����������Ŀɶ�����д��
// ע��win32�½������������׽���
int net_select_check_read(int fd, int timeout);
int net_select_check_write(int fd, int timeout);
int net_select_check_rw(int fd, int timeout);


// ��ȡIP��ַ�б�, ����ֵ:IP��ַ�ĸ���, ppint:���IP��ַ���б�, ��Ҫ�ֶ�free
int net_get_ip_list(int **pp_int);


#ifdef __cplusplus
}
#endif 

#endif //__LL_SOCKLIB_H__

