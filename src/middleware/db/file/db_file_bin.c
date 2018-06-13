/****************************************Copyright (c)**************************************************                         
** File name:			db_file_bin.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-12-09
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

#include	"misc/check.h"

#include	"middleware/db/file/db_file_path.h"

#define		DEBUG_Y
#include	"libs/debug.h"


/** 
* @brief 	将文件中的数据读取到缓冲
* @param name	读取的文件名
* @param offset	读取的偏移地址
* @param buff		缓冲区地址
* @param len		缓冲区长度
* 
* @return  返回保存是否成功
*/
bool file2buf(const char * path,int offset,unsigned char *buff,int len)
{
	FILE *fp;
	size_t	size;
	
	bool ret = false;
	//char path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	//build_file_path(path,name);
	DbgPrintf("path = %s\r\n",path);

	if(is_file_exist(path))
	{
		fp = fopen(path,"r+b");
		if(NULL==fp)
		{
			DbgError("fopen failed!(%s)\r\n",path);
			return ret;
		}

		fseek(fp, offset, SEEK_SET);
		
		size = fread(buff,sizeof(char),len,fp);
		if(size != len)
		{
			DbgError("fread file error!(size = %d,len = %d)\r\n",size,len);
		}
		else
		{
			ret = true;
		}
	}
	else
	{
		DbgPrintf("new a file(%s)\r\n",path);
		
		fp = fopen(path,"a+b");
		if(NULL==fp)
		{
			DbgError("fopen failed!(%s)\r\n",path);
			return ret;
		}
	
	}

	fflush(fp);
	fclose(fp);

	DbgFuncExit();
	
	return ret;
}


/** 
* @brief 	将缓冲区数据保存到文件
* @param name	保存的文件名
* @param buff		缓冲区地址
* @param len		缓冲区长度
* 
* @return  返回保存是否成功
*/
bool buf2file(const char * path,int offset,unsigned char *buff,int len)
{
	FILE *fp;
	size_t	size;
	
	bool ret = false;
	//char path[MAX_PATH_CHARS_SIZE];
	
	DbgFuncEntry();

	//build_file_path(path,name);
	
	DbgPrintf("path = %s\r\n",path);
	
	fp = fopen(path,"wb");
	if(NULL==fp)
	{
		DbgError("fopen failed!(%s)\r\n",path);
		return ret;
	}

	fseek(fp, offset, SEEK_SET);
	
	size = fwrite(buff,sizeof(char),len,fp);

	DbgPrintf("size = 0x%x\r\n",size);

	if(size != len)
	{
		DbgError("fwrite file error!(size = %d,len = %d)\r\n",size,len);
	}
	else
	{
		ret = true;
	}

	fflush(fp);
	fclose(fp);

	DbgFuncExit();
	
	return ret;
}


