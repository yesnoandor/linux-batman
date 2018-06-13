/****************************************Copyright (c)**************************************************                         
** File name:			ui_order.h
** Descriptions:		订单业务处理(与APK信息交互)
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-01-19
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _UI_ORDER_H
#define	_UI_ORDER_H


#ifdef __cplusplus
extern "C" {
#endif

void ui_send_order_briefing(void);
void ui_send_order_detials(void);
void ui_order_grab_treat(unsigned int order_id,unsigned char result);
void ui_order_control_treat(unsigned char *buf,int len);
void ui_send_history_order_list(unsigned char display_type);


#ifdef __cplusplus
}
#endif

#endif
