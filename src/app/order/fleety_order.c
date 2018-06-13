/****************************************Copyright (c)**************************************************                         
** File name:			fleety_order.c
** Descriptions:		订单处理
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-02-07
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#include	"common.h"

#include	"module/gb905/gb905_common.h"
#include	"module/gb905/order/gb905_order.h"

#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_history_info.h"
#include	"module/gb905_ex/ui/ui_order.h"
#include	"module/gb905_ex/ui/ui_notice.h"

#include	"middleware/db/sqlite3/order_sqlite3.h"
#include	"middleware/info/toplight.h"
#include	"middleware/info/order.h"
#include	"middleware/info/status.h"


#include	"app/order/fleety_order.h"

#define		DEBUG_Y
#include	"libs/debug.h"

#define ORDER_GRAP_REMIND_INFO	    "本次抢标失败！！！"
#define ORDER_CANCEL_REMIND_INFO	"订单已取消！！！"
#define ORDER_CANCEL_FINISH_INFO	"订单已完成！！！"


//判断是否需要将简明信息发送到UI
void fleety_send_briefing_judge(void)
{
 	order_info_brief_t order_brief;

	DbgFuncEntry();

	get_order_brief_info((unsigned char *)&order_brief);

    //在登录并且是空车的状态下
	if(get_login_mode()&& !get_loading_status())
	{
        //在电召中
        if(get_order_state())
        {
            //电召中只能接受预约订单
            if(order_brief.type==ORDER_TYPE_RESERVED)
            {
                //设置订单状态
                set_order_status_info(ORDER_STATUS_BRIEF);
                
                //发送简明信息到UI
                ui_send_order_briefing();
            }
        }
        else
        {
            if(order_brief.type==ORDER_TYPE_APPOINT)
            {
                //发送简明信息到UI
                ui_send_order_briefing();

                //自动抢单
                gb905_order_grab_treat(); 
            }
            else
            {
                //设置订单状态
                set_order_status_info(ORDER_STATUS_BRIEF);
                
                //发送简明信息到UI
                ui_send_order_briefing();   
            }
          
        }
        
    }

    DbgFuncExit();
   
}

//判断是否需要将详情信息发送到UI
void fleety_send_detials_judge(void)
{
    order_info_detail_t detail_info;
    DbgFuncEntry();

    get_order_detail_info((unsigned char *)&detail_info);
    
    //订单状态为抢单成功
    if(get_order_status_info()==ORDER_STATUS_CRAB_ACK)
    {

        //发送详情信息到UI
    	ui_send_order_detials();

        //收到详情后将订单信息存入数据库
    	order_sqlite3_insert_record();

        if(detail_info.order_type==ORDER_TYPE_REAL_TIME||detail_info.order_type==ORDER_TYPE_APPOINT)
        {
            //设置顶灯状态
        	set_toplight_operation_status(TOPLIGHT_STATUS_ORDER);

            //设置电召状态位
            set_order_state(true);

            set_order_status_info(ORDER_STATUS_DETAIL);
        }

    }

    DbgFuncExit();
}

//订单抢标失败处理
void fleety_order_grap_fail_treat(void)
{
    DbgFuncEntry();

	set_order_status_info(ORDER_STATUS_IDLE);

    //发送抢标失败提示到UI
    ui_send_terminal_remind((unsigned char*)ORDER_GRAP_REMIND_INFO,sizeof(ORDER_GRAP_REMIND_INFO));
        
    DbgFuncExit();
    
}

//订单取消处理(平台下发取消)
void fleety_order_cancel_treat(void)
{
	DbgFuncEntry();

    set_order_status_info(ORDER_STATUS_SERVER_CANCEL);

    //平台已取消该订单，更新数据库状态
    order_sqlite3_update_state();

	//语音提示驾驶员订单已被取消
    ui_send_terminal_remind((unsigned char*)ORDER_CANCEL_REMIND_INFO,sizeof(ORDER_CANCEL_REMIND_INFO));

    //设置电召状态位
    set_order_state(false);

    //顶灯恢复空车状态
    set_toplight_operation_status(TOPLIGHT_STATUS_EMPTY);

    DbgFuncExit();

}

//订单完成处理
void fleety_order_finish_treat(void)
{
    DbgFuncEntry();

	set_order_status_info(ORDER_STATUS_COMPLETE_ACK);
    
    //收到完成订单的应答，更新数据库状态
    order_sqlite3_update_state();

	//语音提示驾驶员订单已完成
    ui_send_terminal_remind((unsigned char*)ORDER_CANCEL_FINISH_INFO,sizeof(ORDER_CANCEL_FINISH_INFO));

    //设置电召状态位
    set_order_state(false);
    
    DbgFuncExit();
}

void fleety_order_start_treat(unsigned int order_id)
{
    
    DbgFuncEntry();

    //设置顶灯状态
	set_toplight_operation_status(TOPLIGHT_STATUS_ORDER);

    //设置电召状态位
    set_order_state(true);

    set_order_status_info(ORDER_STATUS_DETAIL);
    
    DbgFuncExit();
    
}
