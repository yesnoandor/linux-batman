/****************************************Copyright (c)**************************************************                         
** File name:			mcu_gpio.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2016-11-29
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

#include	"module/gb905_ex/mcu/mcu_common.h"
#include	"module/gb905_ex/mcu/mcu_gpio.h"
#include	"module/gb905_ex/mcu/mcu_adc.h"


#include	"middleware/event/fleety_event.h"
#include	"middleware/info/status.h"

//#include	"middleware/uart/fleety_uart.h"

#define		DEBUG_Y
#include	"libs/debug.h"

#define		GPIO_ACC_ON			0X0001
#define		GPIO_ACC_OFF		0X10001

#define		GPIO_ALARM_PUSH		0X0002
#define		GPIO_ALARM_POP		0X20002

#define     GPIO_POWER_OFF      0x40004
#define     GPIO_POWER_ON       0x0004

typedef  struct
{
    unsigned char start_magic_id;
    mcu_protocol_header_t header;
	unsigned int  out;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed mcu_protocol_gpio_out_t;


typedef  struct
{
    unsigned char start_magic_id;
    gb905_ex_header_t header;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed mcu_protocol_gpio_map_t;

static unsigned int gpo_out = 0;


void mcu_gpo_output(int index,int level)
{
    gpo_out |= 0XFFFFFFF0;

	if(level)
		gpo_out |= 0x01 << index;
	else
		gpo_out &= ~(0x01 << index);

	mcu_send_gpio_out(gpo_out);
}


void mcu_send_gpio_out(unsigned int out)
{
	mcu_protocol_gpio_out_t gpio_out;

	DbgFuncEntry();
	
	mcu_protocol_build_header(&gpio_out.header,SYSTEM2MCU_GPIO_OUT,sizeof(out));

	gpio_out.out = out;

	mcu_protocol_send_data((unsigned char *)&gpio_out,sizeof(mcu_protocol_gpio_out_t));
		
	DbgFuncExit();
}

unsigned char mcu_gpio_in_treat(unsigned char *buf,int len)
{
	unsigned int gpio_in;
	fleety_event_t event;

	DbgFuncEntry();

	gpio_in = *(unsigned int *)(&buf[1 + sizeof(gb905_ex_header_t)]);

	DbgPrintf("gpio_in = 0x%x\r\n",gpio_in);

	switch(gpio_in)
	{
		case GPIO_ACC_ON:
			event.id = ACC_EVENT;
			event.param = ACC_ON;
			break;

		case GPIO_ACC_OFF:
			event.id = ACC_EVENT;
			event.param = ACC_OFF;
			break;

		case GPIO_ALARM_PUSH:
			event.id = ALARM_EVENT;
			event.param = ALARM_PUSH;
			break;

		case GPIO_ALARM_POP:
			event.id = ALARM_EVENT;
			event.param = ALARM_POP;
			break;

        case GPIO_POWER_OFF:
            gb905_power_off_alarm_treat();
            break;

            
		default:
			DbgWarn("this gpio in is not support!!!\r\n");
			break;
	}
	
	event.priority = DAFAULT_PRIORITY;			
	fleety_event_push(&event);
	
	DbgFuncExit();

	return GB905_EX_RESULT_OK;
	
}

void mcu_gpio_map_req(void)
{
	mcu_protocol_gpio_map_t protocol_gpio_map;

	DbgFuncEntry();
	
	mcu_protocol_build_header(&protocol_gpio_map.header,SYSTEM2MCU_GPIO_MAP_REQ,0);

	mcu_protocol_send_data((unsigned char *)&protocol_gpio_map,sizeof(mcu_protocol_gpio_map_t));
		
	DbgFuncExit();
}

void mcu_gpio_map_treat(unsigned char *buf,int len)
{
	unsigned short gpio_map;

	DbgFuncEntry();

	gpio_map = *(unsigned short *)(&buf[1 + sizeof(mcu_protocol_header_t)]);

	DbgPrintf("gpio_map = 0x%x\r\n",gpio_map);
	
	DbgFuncExit();
}
