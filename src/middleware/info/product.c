/****************************************Copyright (c)**************************************************                         
** File name:			product.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2018-03-10
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

#include	"libs/mxml.h"

#include	"misc/misc.h"

#include	"middleware/db/file/db_file_path.h"
#include	"middleware/db/file/db_file_xml.h"

#include	"middleware/info/product.h"

#define		DEBUG_Y
#include	"libs/debug.h"


static product_params_t product_params;

static unsigned char *product_params_point[] =
{
	(unsigned char *)&product_params.meter_vendor_id,
	(unsigned char *)&product_params.toplight_vendor_id,
	(unsigned char *)&product_params.tsm_vendor_id,

	(unsigned char *)product_params.hardware_version,			
};

static char *product_params_type[] =
{
	"DWORD",
	"DWORD",
	"DWORD",

	"STRING",
};




/** 
* @brief	从xml    文件中获取产品参数
*
*/
void init_product_params(void)
{
	bool ret;
	char path[MAX_PATH_CHARS_SIZE];
	char product_file_name[MAX_PATH_CHARS_SIZE];
	
	DbgFuncEntry();

	sprintf(product_file_name,"%s.xml",PRODUCT);
	
	build_db_path(path,product_file_name);
	ret = xml2data(path,product_params_point);
	if(!ret)
	{
		DbgWarn("product info xml is not exist!\r\n");
		
		memset(&product_params,0x00,sizeof(product_params_t));

		product_params.meter_vendor_id = METER_VENDOR_ID;
		product_params.toplight_vendor_id = TOPLIGHT_VENDOR_ID;
		product_params.tsm_vendor_id = TSM_VENDOR_ID;

		strcpy((char *)product_params.hardware_version,DEFAULT_HW_VERSION);
		
		DbgPrintf("list size = %d\r\n",ARRAY_SIZE(product_params_point));
		
		init_xml(product_file_name,product_params_point,product_params_type,ARRAY_SIZE(product_params_point));
	}
	
	debug_product_params();
	
	DbgFuncExit();

}


void get_product_params(unsigned char *buf)
{
	memcpy(buf,&product_params,sizeof(product_params_t));
}





void debug_product_params(void)
{
	DbgFuncEntry();

	DbgPrintf("meter_vendor_id = 0x%x\r\n",product_params.meter_vendor_id);
	DbgPrintf("toplight_vendor_id = 0x%x\r\n",product_params.toplight_vendor_id);
	DbgPrintf("tsm_vendor_id = 0x%x\r\n",product_params.tsm_vendor_id);

	DbgPrintf("hardware_version = %s\r\n",product_params.hardware_version);

	DbgFuncExit();
}


