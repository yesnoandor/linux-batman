#ifndef _COMMON_H
#define	_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif



#include	<sys/types.h>
#include	<sys/socket.h>  
#include	<sys/time.h>

#include	<netinet/in.h>  
#include	<arpa/inet.h>  
#include	<errno.h>  
#include	<unistd.h>  			// unix 系统服务函数定义
#include	<termios.h>				// 终端控制定义
#include	<fcntl.h>				// 文件控制定义

  
#include	<stdio.h>  
#include	<string.h>  
#include	<stdlib.h>				// 标准C  库		
#include	<stdbool.h>

#include 	<pthread.h>

#if defined (PLATFORM_UBUNTU)
#include	"platform/ubuntu.h"
#elif defined (PLATFORM_QCOM)
#include	"platform/msm8909.h"
#elif defined (PLATFORM_HW)
#include	"platform/hi3520d.h"
#else
#error "No Support This Platform"
#endif

#define		MAX_IMEI_CHARS_SIZE		16
#define		MAX_IMSI_CHARS_SIZE		16
#define		MAX_HW_VERSION_SIZE		14
#define		MAX_SW_VERSION_SIZE		14


typedef struct{
	unsigned char * buf;					
	int len;									
}__packed buff_mgr_t;

typedef struct{
	buff_mgr_t raw;												
	buff_mgr_t now;
}__packed double_buff_mgr_t;


#ifdef __cplusplus
}
#endif


#endif

