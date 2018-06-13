
#ifndef __LL_SOCKLIB_H__
#define __LL_SOCKLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
udp
************************************************************************/
int net_udp_new(int port, OUT struct sockaddr_in *addr);		// udp单播
int net_bcast_new(int port, OUT struct sockaddr_in *addr);		// udp广播
int net_mcast_new(char *mip, int mport, OUT struct sockaddr_in *addr);	// udp多播(组播)
int net_udp_read(int sock, char *bufptr, int buflen, OUT struct sockaddr_in *addr);
int net_udp_write(int sock, char *bufptr, int buflen, struct sockaddr_in *addr);


/************************************************************************
tcp
************************************************************************/
// 连接远程主机. timeout-超时时间(单位ms), -1为阻塞版本
int net_tcp_connect(char *host, int port, OUT struct sockaddr_in *addr, int timeout);
int net_tcp_listen(int port, OUT struct sockaddr_in *addr);
int net_accept_new(int listen_sock, OUT struct sockaddr_in *addr);

/************************************************************************
一些帮助函数
************************************************************************/
// 设置套接字为非阻塞方式
// 参数 : sock - 处于监听中或已经连接的套接字, flag - 1 非阻塞, 0 阻塞
int net_make_socket_nonblocking(int sock, int flag);
// 将域名或ip存入sockaddr结构中(如有必要将进行dns解析)
int net_make_addr(char *host, int port, OUT struct sockaddr_in *addr);
// 返回套接字的字符串信息
char *net_sock_info(int sock);
// 检查套接字的错误
int net_check_socket(int sock);
// 设置opt, 包含keepalive/linger
int net_test_set_opt(int sock);
// 获取所有的opt属性
int net_test_print_opt(int sock);

// 检查描述符的可读及可写性
// 注意win32下仅适用于网络套接字
int net_select_check_read(int fd, int timeout);
int net_select_check_write(int fd, int timeout);
int net_select_check_rw(int fd, int timeout);


// 获取IP地址列表, 返回值:IP地址的个数, ppint:存放IP地址的列表, 需要手动free
int net_get_ip_list(int **pp_int);


#ifdef __cplusplus
}
#endif 

#endif //__LL_SOCKLIB_H__

