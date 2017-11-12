/****************************************Copyright (c)**************************************************                         
** File name:			itop_authorize.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-03
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef _ITOP_AUTHORIZE_H
#define	_ITOP_AUTHORIZE_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
    AUTHORIZE_FIRST = 0,
    AUTHORIZE_AGAIN,
    AUTHORIZE_NO,
};

bool is_authorized(void);
unsigned char get_authorize_status(void);
void itop_authorize_request(unsigned char authorize_state);
int itop_authorize_analyze(unsigned int mtd_id,unsigned char *msg_buf);
void itop_authorize_loop(void);



#ifdef __cplusplus
}
#endif

#endif

