/****************************************Copyright (c)**************************************************                         
** File name:			demo_libevent_client.c
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-15
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

#define		DEBUG_Y
#include	"libs/debug.h"

// 创建一个新的xml   文件(new.xml)
int create_mxml(void)
{
	FILE *fp = fopen("new.xml","w");
		
	DbgFuncEntry();
	
	// 创建一个新的XML	文档树，生成xml   文件的头
	mxml_node_t *xml = mxmlNewXML("1.0");
	
	// 添加一个新的XML	元素节点root
	mxml_node_t *root = mxmlNewElement(xml,"root");
	
	// 在root	节点下，添加一个新的子节点	data
	mxml_node_t *data = mxmlNewElement(root,"data");
	// 往 data 子节点添加属性id = 1
	mxmlElementSetAttr(data,"id","1");
	// 往 data 子节点添加属性type = STRING
	mxmlElementSetAttr(data,"type","STRING");
	// 设置data  子节点文本值= "Hello"
	mxmlNewText(data,0,"Hello");
	
	// 在root	节点下，添加一个新的子节点	data
	data = mxmlNewElement(root,"data");
	// 往 data 子节点添加属性id = 2
	mxmlElementSetAttr(data,"id","2");
	// 往 data	子节点添加属性type = DWORD
	mxmlElementSetAttr(data,"type","DWORD");
	// 设置data  子节点整数值= 123
	mxmlNewText(data,0,"123");

	
	// 保存节点内容到文件
	mxmlSaveFile(xml,fp,MXML_NO_CALLBACK);
	fflush(fp);
	fclose(fp);
	
	DbgFuncExit();
	
	return 0;
}

// 遍历一个xml   文件(new.xml)
int traverse_mxml(void)
{
	FILE *fp;
	const char * temp;
	unsigned int val;
	
	DbgFuncEntry(); 

	// 载入一个文件到一个XML   节点树
	fp = fopen("new.xml","r");
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
		temp = mxmlElementGetAttr(data,"id");
		DbgPrintf("id = %s\r\n",temp);
		// 获取节点的type   属性
		temp = mxmlElementGetAttr(data,"type");
		DbgPrintf("type = %s\r\n",temp);
		
		if(!strcmp(temp,"STRING"))
		{
			// 获取节点的文本值
			temp = mxmlGetText(data,NULL);
			DbgPrintf("str = %s\r\n",temp);
		}
		else if(!strcmp(temp,"DWORD"))
		{
			// 获取节点的整数值
			temp = mxmlGetText(data,NULL);
			val = atoi(temp);
			DbgPrintf("val = %d\r\n",val);
		}
		else
		{
			DbgError("don't support this data type!\r\n");
		}

		// 查找顺序的下一个data   节点
		data = mxmlFindElement(data,xml,"data",NULL,NULL,MXML_DESCEND);
	}
	
	DbgFuncExit();
	
	return 0;
}

int replace_mxml(void)
{
	FILE *fp;
	char temp[32];
	int val;
	
	DbgFuncEntry(); 

	// 载入一个文件到一个XML   节点树
	fp = fopen("new.xml","r+");
	mxml_node_t *xml = mxmlLoadFile(NULL,fp,MXML_NO_CALLBACK);
	fseek(fp,0,SEEK_SET);

	mxml_node_t *root = NULL;
	mxml_node_t *data = NULL;
	
	// 查找root    节点
	root = mxmlFindElement(xml,xml,"root",NULL,NULL,MXML_DESCEND);
	// 查找顺序的第一个data   节点
	data = mxmlFindElement(root,xml,"data","id",NULL,MXML_DESCEND);
	mxmlSetText(data,0,"hello");

	// 查找顺序的下一个data   节点
	val = 832;
	sprintf(temp,"%d",val);
	data = mxmlFindElement(data,xml,"data","id",NULL,MXML_DESCEND);
	mxmlSetText(data,0,temp);
	
	// 保存节点内容到文件
	mxmlSaveFile(xml,fp,MXML_NO_CALLBACK);
	fflush(fp);
	fclose(fp);

	DbgFuncExit();
	
	return 0;
	
}

int demo_mxml(void)
{
	//create_mxml();
	//traverse_mxml();
	replace_mxml();

	#if 0
	FILE *fp;
	const char * temp;
	unsigned int val;
	
	DbgFuncEntry(); 

	// 载入一个文件到一个XML   节点树
	fp = fopen("new.xml","r");
	mxml_node_t *xml = mxmlLoadFile(NULL,fp,MXML_NO_CALLBACK);
	fclose(fp);

	mxml_node_t *root = NULL;
	mxml_node_t *data = NULL;
	
	// 查找root    节点
	root = mxmlFindElement(xml,xml,"root",NULL,NULL,MXML_DESCEND);
	// 查找顺序的第一个data   节点
	data = mxmlFindElement(root,xml,"data",NULL,NULL,MXML_DESCEND);
	
	// 获取节点的id    属性
	temp = mxmlElementGetAttr(data,"id");
	DbgPrintf("id = %s\r\n",temp);
	// 获取节点的type   属性
	temp = mxmlElementGetAttr(data,"type");
	DbgPrintf("type = %s\r\n",temp);
		
	// 获取节点的文本值
	temp = mxmlGetText(data,NULL);
	DbgPrintf("str = %s\r\n",temp);

	// 获取节点的整数值
	//val = mxmlGetText(data);
	//DbgPrintf("val = %d\r\n",val);
	
	DbgFuncExit();
	#endif
	
	return 0;
}




