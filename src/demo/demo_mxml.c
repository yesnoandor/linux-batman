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

// ����һ���µ�xml   �ļ�(new.xml)
int create_mxml(void)
{
	FILE *fp = fopen("new.xml","w");
		
	DbgFuncEntry();
	
	// ����һ���µ�XML	�ĵ���������xml   �ļ���ͷ
	mxml_node_t *xml = mxmlNewXML("1.0");
	
	// ���һ���µ�XML	Ԫ�ؽڵ�root
	mxml_node_t *root = mxmlNewElement(xml,"root");
	
	// ��root	�ڵ��£����һ���µ��ӽڵ�	data
	mxml_node_t *data = mxmlNewElement(root,"data");
	// �� data �ӽڵ��������id = 1
	mxmlElementSetAttr(data,"id","1");
	// �� data �ӽڵ��������type = STRING
	mxmlElementSetAttr(data,"type","STRING");
	// ����data  �ӽڵ��ı�ֵ= "Hello"
	mxmlNewText(data,0,"Hello");
	
	// ��root	�ڵ��£����һ���µ��ӽڵ�	data
	data = mxmlNewElement(root,"data");
	// �� data �ӽڵ��������id = 2
	mxmlElementSetAttr(data,"id","2");
	// �� data	�ӽڵ��������type = DWORD
	mxmlElementSetAttr(data,"type","DWORD");
	// ����data  �ӽڵ�����ֵ= 123
	mxmlNewText(data,0,"123");

	
	// ����ڵ����ݵ��ļ�
	mxmlSaveFile(xml,fp,MXML_NO_CALLBACK);
	fflush(fp);
	fclose(fp);
	
	DbgFuncExit();
	
	return 0;
}

// ����һ��xml   �ļ�(new.xml)
int traverse_mxml(void)
{
	FILE *fp;
	const char * temp;
	unsigned int val;
	
	DbgFuncEntry(); 

	// ����һ���ļ���һ��XML   �ڵ���
	fp = fopen("new.xml","r");
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
		temp = mxmlElementGetAttr(data,"id");
		DbgPrintf("id = %s\r\n",temp);
		// ��ȡ�ڵ��type   ����
		temp = mxmlElementGetAttr(data,"type");
		DbgPrintf("type = %s\r\n",temp);
		
		if(!strcmp(temp,"STRING"))
		{
			// ��ȡ�ڵ���ı�ֵ
			temp = mxmlGetText(data,NULL);
			DbgPrintf("str = %s\r\n",temp);
		}
		else if(!strcmp(temp,"DWORD"))
		{
			// ��ȡ�ڵ������ֵ
			temp = mxmlGetText(data,NULL);
			val = atoi(temp);
			DbgPrintf("val = %d\r\n",val);
		}
		else
		{
			DbgError("don't support this data type!\r\n");
		}

		// ����˳�����һ��data   �ڵ�
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

	// ����һ���ļ���һ��XML   �ڵ���
	fp = fopen("new.xml","r+");
	mxml_node_t *xml = mxmlLoadFile(NULL,fp,MXML_NO_CALLBACK);
	fseek(fp,0,SEEK_SET);

	mxml_node_t *root = NULL;
	mxml_node_t *data = NULL;
	
	// ����root    �ڵ�
	root = mxmlFindElement(xml,xml,"root",NULL,NULL,MXML_DESCEND);
	// ����˳��ĵ�һ��data   �ڵ�
	data = mxmlFindElement(root,xml,"data","id",NULL,MXML_DESCEND);
	mxmlSetText(data,0,"hello");

	// ����˳�����һ��data   �ڵ�
	val = 832;
	sprintf(temp,"%d",val);
	data = mxmlFindElement(data,xml,"data","id",NULL,MXML_DESCEND);
	mxmlSetText(data,0,temp);
	
	// ����ڵ����ݵ��ļ�
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

	// ����һ���ļ���һ��XML   �ڵ���
	fp = fopen("new.xml","r");
	mxml_node_t *xml = mxmlLoadFile(NULL,fp,MXML_NO_CALLBACK);
	fclose(fp);

	mxml_node_t *root = NULL;
	mxml_node_t *data = NULL;
	
	// ����root    �ڵ�
	root = mxmlFindElement(xml,xml,"root",NULL,NULL,MXML_DESCEND);
	// ����˳��ĵ�һ��data   �ڵ�
	data = mxmlFindElement(root,xml,"data",NULL,NULL,MXML_DESCEND);
	
	// ��ȡ�ڵ��id    ����
	temp = mxmlElementGetAttr(data,"id");
	DbgPrintf("id = %s\r\n",temp);
	// ��ȡ�ڵ��type   ����
	temp = mxmlElementGetAttr(data,"type");
	DbgPrintf("type = %s\r\n",temp);
		
	// ��ȡ�ڵ���ı�ֵ
	temp = mxmlGetText(data,NULL);
	DbgPrintf("str = %s\r\n",temp);

	// ��ȡ�ڵ������ֵ
	//val = mxmlGetText(data);
	//DbgPrintf("val = %d\r\n",val);
	
	DbgFuncExit();
	#endif
	
	return 0;
}




