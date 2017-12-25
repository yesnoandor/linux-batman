/****************************************Copyright (c)**************************************************                         
** File name:			common.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-04
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _COMMON_H
#define	_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif



#include	<sys/types.h>
#include	<sys/socket.h>  
#include	<sys/time.h>
#include	<sys/stat.h>			// 获取文件属性

#include	<netinet/in.h>  
#include	<arpa/inet.h>  
#include	<errno.h>  
#include	<unistd.h>  			// unix 系统服务函数定义
#include 	<dirent.h>				// unix类目录操作的头文件
#include	<termios.h>				// 终端控制定义
#include	<fcntl.h>				// 文件控制定义

  
#include	<stdio.h>  
#include	<string.h>  
#include	<stdlib.h>				// 标准C  库		
#include	<stdbool.h>

#include 	<pthread.h>

#include	<linux/input.h>

#include	"platform/platform.h"

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

