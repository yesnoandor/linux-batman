/****************************************Copyright (c)**************************************************                         
** File name:			update_socket.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-01-15
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef __UPDATE_SOCKET_H
#define __UPDATE_SOCKET_H

#ifdef __cplusplus
extern "C" {
#endif



void update_socket_init(void);
void update_socket_send(unsigned char *buff,int len);


#ifdef __cplusplus
}
#endif



#endif




