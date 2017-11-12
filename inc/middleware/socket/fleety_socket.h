/****************************************Copyright (c)**************************************************                         
** File name:			fleety_socket.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-01-19
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef __FLEETY_SOCKET_H
#define __FLEETY_SOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

enum{
	MAIN_SOCKET,
	AUX_SOCKET,
	UI_SOCKET
};


void fleety_socket_init(void);
void fleety_socket_send(int index,unsigned char *buff,int len);
void fleety_pipe_send(void);


#ifdef __cplusplus
}
#endif



#endif



