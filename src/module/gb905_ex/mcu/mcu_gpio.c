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

//#include	"middleware/uart/fleety_uart.h"

#define		DEBUG_Y
#include	"libs/debug.h"


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

	DbgFuncEntry();

	gpio_in = *(unsigned int *)(&buf[1 + sizeof(gb905_ex_header_t)]);

	DbgPrintf("gpio_in = 0x%x\r\n",gpio_in);

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
