/****************************************Copyright (c)**************************************************                         
** File name:			authorize_socket.h
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

#ifndef _AUTHORIZE_SOCKET_H
#define	_AUTHORIZE_SOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

void authorize_socket_init(void);
void authorize_socket_send(unsigned char *buff,int len);

#ifdef __cplusplus
}
#endif

#endif




