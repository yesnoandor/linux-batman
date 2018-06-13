/****************************************Copyright (c)**************************************************                         
** File name:			ui_control.c
** Descriptions:		UI控制功能处理
**
**------------------------------------------------------------------------------------------------------
** Created by:		yang_wang
** Created date:		2018-02-09
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

#include	"module/gb905_ex/ui/ui_common.h"
#include	"module/gb905_ex/ui/ui_control.h"

#include	"middleware/event/acc/fleety_acc.h"

#define		DEBUG_Y
#include	"libs/debug.h"

enum{
    UI_CONTROL_PARAMS_EXPORT = 0,              //触发参数导出
    UI_CONTROL_MONITOR_CAR                     //开启非法点火监听
};

typedef struct{
    unsigned char control_type;
    unsigned char control_params;

}__packed ui_control_t;

void ui_control_termianl_treat(unsigned char *buf,int len)
{
    ui_control_t *ui_control;
    
    DbgFuncEntry();

    ui_control = (ui_control_t *)buf;

    DbgPrintf("ui control type is 0x%02x\r\n",ui_control->control_type);
    
    switch(ui_control->control_type)
    {
        case UI_CONTROL_PARAMS_EXPORT:
            break;

        case UI_CONTROL_MONITOR_CAR:
            fleety_acc_monitor_func(ui_control->control_params);
            break;

        default:
            DbgWarn(" control type is not support(0x%02x)!!!\r\n",ui_control->control_type);
            break;
    }

    DbgFuncExit();

}