/****************************************Copyright (c)**************************************************                         
** File name:			db_file_xml.c
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

#include	"libs/mxml.h"

#include	"misc/misc.h"
#include	"misc/check.h"

#include	"middleware/db/file/db_file_path.h"

#define		DEBUG_Y
#include	"libs/debug.h"


/** 
* @brief	创建XML    文件
* @param name	读取的文件名
* @param addr_list		保存参数各字段地址的字符指针数组
* @param type_list		保存参数各字段类型的字符指针数组
* 
*/
void init_xml(const char* name,unsigned char * addr_list[],char * type_list[],int size)
{
	int i;
	char temp[32];
	FILE *fp;
	char path[MAX_PATH_CHARS_SIZE];

	DbgFuncEntry();

	build_file_path(path,name);
	DbgPrintf("path = %s\r\n",path);

	fp = fopen(path,"w");
	
	// 创建一个新的XML	文档树，生成xml   文件的头
	mxml_node_t *xml = mxmlNewXML("1.0");
		
	// 添加一个新的XML	元素节点root
	mxml_node_t *root = mxmlNewElement(xml,"root");

	for(i=0;i<size;i++)
	{
		// 在root	节点下，添加一个新的子节点	data
		mxml_node_t *data = mxmlNewElement(root,"data");
		// 往 data 子节点添加属性id = i
		sprintf(temp,"%d",i);
		mxmlElementSetAttr(data,"id",temp);
	
		DbgPrintf("id = %d\r\n",i);
			
		if(!strcmp(type_list[i],"STRING"))
		{
			// 往data   子节点添加属性type = STRING
			mxmlElementSetAttr(data,"type","STRING");
			// 设置data  子节点文本值
			mxmlNewText(data,0,(char *)addr_list[i]);

			DbgPrintf("str = %s\r\n",addr_list[i]);
		}
		else if(!strcmp(type_list[i],"DWORD"))
		{
			// 往 data	子节点添加属性type = DWORD
			mxmlElementSetAttr(data,"type","DWORD");
			// 设置data  子节点整数值= 123
			sprintf(temp,"%d",*(unsigned int *)addr_list[i]);
			mxmlNewText(data,0,temp);

			DbgPrintf("val = %d\r\n",*(unsigned int *)addr_list[i]);
		}
		else
		{
			DbgError("don't support this data type!\r\n");
		}
	}
	
	// 保存节点内容到文件
	mxmlSaveFile(xml,fp,MXML_NO_CALLBACK);
	fflush(fp);
	fclose(fp);
	
	DbgFuncExit();
	
}



/** 
* @brief	从XML   文件获取设置参数
* @param name	读取的文件名
* @param addr_list		保存参数各字段地址的字符指针数组
* @param type_list		保存参数各字段类型的字符指针数组
* 
* @return  返回保存是否成功
*/
bool xml2data(const char* name,unsigned char * addr_list[])
{
	FILE *fp;
	char path[MAX_PATH_CHARS_SIZE];

	unsigned int id;
	const char * type;
	const char * str;
	unsigned int val;
		
	DbgFuncEntry();
		
	build_file_path(path,name);
	DbgPrintf("path = %s\r\n",path);

	if(!is_file_exist(path))
	{
		DbgError("path %s is not exist\r\n",path);
		return false;
	}

	// 载入一个文件到一个XML   节点树
	fp = fopen(path,"r");
	mxml_node_t *xml = mxmlLoadFile(NULL,fp,MXML_NO_CALLBACK);
	fclose(fp);

	mxml_node_t *root = NULL;
	mxml_node_t *data = NULL;
			
	// 查找root    节点
	root = mxmlFindElement(xml,xml,"root",NULL,NULL,MXML_DESCEND);
	// 查找顺序的第一个data   节点
	data = mxmlFindElement(root,xml,"data",NULL,NULL,MXML_DESCEND);

	while(data){
		// 获取节点的id    属性
		str = mxmlElementGetAttr(data,"id");
		id = atoi(str);
		DbgPrintf("id = %d\r\n",id);
			
		// 获取节点的type	属性
		type = mxmlElementGetAttr(data,"type");
		DbgPrintf("type = %s\r\n",type);
				
		if(!strcmp(type,"STRING"))
		{
			// 获取节点的文本值
			str = mxmlGetText(data,NULL);
			DbgPrintf("str = %s\r\n",str);

			if(str != NULL)
			{
				strcpy((char *)addr_list[id],str);
			}
		}
		else if(!strcmp(type,"DWORD"))
		{
			// 获取节点的整数值
			str = mxmlGetText(data,NULL);
			val = atoi(str);
			DbgPrintf("val = %d\r\n",val);

			*(unsigned int *)addr_list[id] = val;
		}
		else
		{
			DbgError("don't support this data type!\r\n");
		}
		
		// 查找顺序的下一个data   节点
		data = mxmlFindElement(data,xml,"data",NULL,NULL,MXML_DESCEND);
	}
			
	DbgFuncExit();
		
	return true;
}



/** 
* @brief	设置参数到xml    文件
* @param name	读取的文件名
* @param addr_list		保存参数各字段地址的字符指针数组
* @param type_list		保存参数各字段类型的字符指针数组
* 
* @return  返回保存是否成功
*/
bool data2xml(const char* name,unsigned char * addr_list[])
{
	FILE *fp;
	char path[MAX_PATH_CHARS_SIZE];

	unsigned int id;
	const char * type;
	const char * str;
	unsigned int val;
	char temp[32];
	
	DbgFuncEntry();

	build_file_path(path,name);
	DbgPrintf("path = %s\r\n",path);
	
	if(!is_file_exist(path))
	{
		DbgError("path %s is not exist\r\n",path);
		return false;
	}

	// 载入一个文件到一个XML   节点树
	fp = fopen(path,"r");
	mxml_node_t *xml = mxmlLoadFile(NULL,fp,MXML_NO_CALLBACK);
	fclose(fp);

	
	//fseek(fp,0,SEEK_SET);

	mxml_node_t *root = NULL;
	mxml_node_t *data = NULL;

	// 查找root    节点
	root = mxmlFindElement(xml,xml,"root",NULL,NULL,MXML_DESCEND);
	
	// 查找顺序的第一个data   节点
	data = mxmlFindElement(root,xml,"data",NULL,NULL,MXML_DESCEND);

	while(data){
		// 获取节点的id    属性
		str = mxmlElementGetAttr(data,"id");
		id = atoi(str);
		DbgPrintf("id = %d\r\n",id);
			
		// 获取节点的type	属性
		type = mxmlElementGetAttr(data,"type");
		DbgPrintf("type = %s\r\n",type);
			
		if(!strcmp(type,"STRING"))
		{			
			// 设置节点的文本值
			str = (const char *)addr_list[id];
			mxmlSetText(data,0,str);

			DbgPrintf("str = %s\r\n",str);	
		}
		else if(!strcmp(type,"DWORD"))
		{
			// 设置节点的整数值
			val = *(unsigned int *)addr_list[id];
			sprintf(temp,"%d",val);
			mxmlSetText(data,0,temp);

			DbgPrintf("val = %d\r\n",val);
		}
		else
		{
			DbgError("don't support this data type!\r\n");
		}
		
		
		// 查找顺序的下一个data   节点
		data = mxmlFindElement(data,xml,"data",NULL,NULL,MXML_DESCEND);

	}

	// 保存节点内容到文件
	fp = fopen(path,"w");
	mxmlSaveFile(xml,fp,MXML_NO_CALLBACK);
	fflush(fp);
	fclose(fp);

	DbgFuncExit();

	return true;
}

