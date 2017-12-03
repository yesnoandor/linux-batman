#include	"common.h"

#include	"module/info/info2file.h"

#define		DEBUG_Y
#include	"libs/debug.h"



void build_file_path(char * file_path,const char * file_name)
{
	DbgFuncEntry();

	sprintf(file_path,"%s%s",COMMON_PATH,file_name);
	
	DbgFuncExit();
}


int file2info(const char * file_name,unsigned char *buff,int len)
{
	FILE *fp;
	size_t	size;
	int ret = 1;
	char file_path[MAX_PATH_CHARS_SIZE];
	DbgFuncEntry();

	build_file_path(file_path,file_name);
	DbgPrintf("file_path = %s\r\n",file_path);

	fp = fopen(file_path,"a+b");
	if(NULL==fp)
    {
    	DbgError("fopen failed!(%s)\r\n",file_path);
        return -1;
    }
	size = fread(buff,sizeof(char),len,fp);
	if(size != len)
	{
		memset(buff,0x00,len);
		ret = 0;
	}

	fflush(fp);
	fclose(fp);

	DbgFuncExit();
	
	return ret;
}

int info2file(const char * file_name,unsigned char *buff,int len)
{
	FILE *fp;
	size_t	size;
	int ret = 1;
	char file_path[MAX_PATH_CHARS_SIZE];
	
	DbgFuncEntry();

	build_file_path(file_path,file_name);
	
	DbgPrintf("file_path = %s\r\n",file_path);

	
	fp = fopen(file_path,"wb");
	if(NULL==fp)
    {
    	DbgError("fopen failed!(%s)\r\n",file_path);
        return -1;
    }
    
	size = fwrite(buff,sizeof(char),len,fp);

	DbgPrintf("size = 0x%x\r\n",size);

	if(size != len)
	{
		DbgError("write info error!\r\n");
		ret = 0;
	}

	fflush(fp);
	fclose(fp);

	DbgFuncExit();
	
	return ret;
}


