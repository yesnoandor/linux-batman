/****************************************Copyright (c)**************************************************                         
** File name:			ui_heart_beat.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-12-20
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/




#ifndef _UI_HEART_BEAT_H
#define	_UI_HEART_BEAT_H

#ifdef __cplusplus
extern "C" {
#endif

#define		UI_HEART_BEAT_TIMEOUT		5

void ui_heart_beat_reset(void);

void ui_heart_beat_send(void);
void ui_heart_beat_treat(void);



#ifdef __cplusplus
}
#endif

#endif

