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
* @brief	����XML    �ļ�
* @param name	��ȡ���ļ���
* @param addr_list		����������ֶε�ַ���ַ�ָ������
* @param type_list		����������ֶ����͵��ַ�ָ������
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
	
	// ����һ���µ�XML	�ĵ���������xml   �ļ���ͷ
	mxml_node_t *xml = mxmlNewXML("1.0");
		
	// ���һ���µ�XML	Ԫ�ؽڵ�root
	mxml_node_t *root = mxmlNewElement(xml,"root");

	for(i=0;i<size;i++)
	{
		// ��root	�ڵ��£����һ���µ��ӽڵ�	data
		mxml_node_t *data = mxmlNewElement(root,"data");
		// �� data �ӽڵ��������id = i
		sprintf(temp,"%d",i);
		mxmlElementSetAttr(data,"id",temp);
	
		DbgPrintf("id = %d\r\n",i);
			
		if(!strcmp(type_list[i],"STRING"))
		{
			// ��data   �ӽڵ��������type = STRING
			mxmlElementSetAttr(data,"type","STRING");
			// ����data  �ӽڵ��ı�ֵ
			mxmlNewText(data,0,(char *)addr_list[i]);

			DbgPrintf("str = %s\r\n",addr_list[i]);
		}
		else if(!strcmp(type_list[i],"DWORD"))
		{
			// �� data	�ӽڵ��������type = DWORD
			mxmlElementSetAttr(data,"type","DWORD");
			// ����data  �ӽڵ�����ֵ= 123
			sprintf(temp,"%d",*(unsigned int *)addr_list[i]);
			mxmlNewText(data,0,temp);

			DbgPrintf("val = %d\r\n",*(unsigned int *)addr_list[i]);
		}
		else
		{
			DbgError("don't support this data type!\r\n");
		}
	}
	
	// ����ڵ����ݵ��ļ�
	mxmlSaveFile(xml,fp,MXML_NO_CALLBACK);
	fflush(fp);
	fclose(fp);
	
	DbgFuncExit();
	
}



/** 
* @brief	��XML   �ļ���ȡ���ò���
* @param name	��ȡ���ļ���
* @param addr_list		����������ֶε�ַ���ַ�ָ������
* @param type_list		����������ֶ����͵��ַ�ָ������
* 
* @return  ���ر����Ƿ�ɹ�
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

	// ����һ���ļ���һ��XML   �ڵ���
	fp = fopen(path,"r");
	mxml_node_t *xml = mxmlLoadFile(NULL,fp,MXML_NO_CALLBACK);
	fclose(fp);

	mxml_node_t *root = NULL;
	mxml_node_t *data = NULL;
			
	// ����root    �ڵ�
	root = mxmlFindElement(xml,xml,"root",NULL,NULL,MXML_DESCEND);
	// ����˳��ĵ�һ��data   �ڵ�
	data = mxmlFindElement(root,xml,"data",NULL,NULL,MXML_DESCEND);

	while(data){
		// ��ȡ�ڵ��id    ����
		str = mxmlElementGetAttr(data,"id");
		id = atoi(str);
		DbgPrintf("id = %d\r\n",id);
			
		// ��ȡ�ڵ��type	����
		type = mxmlElementGetAttr(data,"type");
		DbgPrintf("type = %s\r\n",type);
				
		if(!strcmp(type,"STRING"))
		{
			// ��ȡ�ڵ���ı�ֵ
			str = mxmlGetText(data,NULL);
			DbgPrintf("str = %s\r\n",str);

			if(str != NULL)
			{
				strcpy((char *)addr_list[id],str);
			}
		}
		else if(!strcmp(type,"DWORD"))
		{
			// ��ȡ�ڵ������ֵ
			str = mxmlGetText(data,NULL);
			val = atoi(str);
			DbgPrintf("val = %d\r\n",val);

			*(unsigned int *)addr_list[id] = val;
		}
		else
		{
			DbgError("don't support this data type!\r\n");
		}
		
		// ����˳�����һ��data   �ڵ�
		data = mxmlFindElement(data,xml,"data",NULL,NULL,MXML_DESCEND);
	}
			
	DbgFuncExit();
		
	return true;
}



/** 
* @brief	���ò�����xml    �ļ�
* @param name	��ȡ���ļ���
* @param addr_list		����������ֶε�ַ���ַ�ָ������
* @param type_list		����������ֶ����͵��ַ�ָ������
* 
* @return  ���ر����Ƿ�ɹ�
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

	// ����һ���ļ���һ��XML   �ڵ���
	fp = fopen(path,"r");
	mxml_node_t *xml = mxmlLoadFile(NULL,fp,MXML_NO_CALLBACK);
	fclose(fp);

	
	//fseek(fp,0,SEEK_SET);

	mxml_node_t *root = NULL;
	mxml_node_t *data = NULL;

	// ����root    �ڵ�
	root = mxmlFindElement(xml,xml,"root",NULL,NULL,MXML_DESCEND);
	
	// ����˳��ĵ�һ��data   �ڵ�
	data = mxmlFindElement(root,xml,"data",NULL,NULL,MXML_DESCEND);

	while(data){
		// ��ȡ�ڵ��id    ����
		str = mxmlElementGetAttr(data,"id");
		id = atoi(str);
		DbgPrintf("id = %d\r\n",id);
			
		// ��ȡ�ڵ��type	����
		type = mxmlElementGetAttr(data,"type");
		DbgPrintf("type = %s\r\n",type);
			
		if(!strcmp(type,"STRING"))
		{			
			// ���ýڵ���ı�ֵ
			str = (const char *)addr_list[id];
			mxmlSetText(data,0,str);

			DbgPrintf("str = %s\r\n",str);	
		}
		else if(!strcmp(type,"DWORD"))
		{
			// ���ýڵ������ֵ
			val = *(unsigned int *)addr_list[id];
			sprintf(temp,"%d",val);
			mxmlSetText(data,0,temp);

			DbgPrintf("val = %d\r\n",val);
		}
		else
		{
			DbgError("don't support this data type!\r\n");
		}
		
		
		// ����˳�����һ��data   �ڵ�
		data = mxmlFindElement(data,xml,"data",NULL,NULL,MXML_DESCEND);

	}

	// ����ڵ����ݵ��ļ�
	fp = fopen(path,"w");
	mxmlSaveFile(xml,fp,MXML_NO_CALLBACK);
	fflush(fp);
	fclose(fp);

	DbgFuncExit();

	return true;
}

