#ifndef _GPS_MAIN_SERVER_H
#define _GPS_MAIN_SERVER_H


#ifdef __cplusplus
extern "C" {
#endif

#define 		__packed 		__attribute__((packed))


/****************************************************************************************************
*����ֵ����:��ͬ���뷽ʽ��һ��ָ������ӳ�䰴��ֵ
*upd by cgd on 20140930
****************************************************************************************************/
typedef enum {

	//���ְ���
	KEY_VIRTUAL_0        	= 0x0000,
	KEY_VIRTUAL_1        	= 0x0001,
	KEY_VIRTUAL_2        	= 0x0002,
	KEY_VIRTUAL_3        	= 0x0003,
	KEY_VIRTUAL_4        	= 0x0004,
	KEY_VIRTUAL_5        	= 0x0005,
	KEY_VIRTUAL_6        	= 0x0006,
	KEY_VIRTUAL_7        	= 0x0007,
	KEY_VIRTUAL_8        	= 0x0008,
	KEY_VIRTUAL_9        	= 0x0009,
	KEY_VIRTUAL_10       	= 0x000a,
	KEY_VIRTUAL_11       	= 0x000b,
	KEY_VIRTUAL_12       	= 0x000c,
	KEY_VIRTUAL_13       	= 0x000d,
	KEY_VIRTUAL_14       	= 0x000e,
	KEY_VIRTUAL_15       	= 0x000f,
	KEY_VIRTUAL_16       	= 0x0010,
	KEY_VIRTUAL_10_A	 	= 0x0011,
	KEY_VIRTUAL_BACKSPACE 	= 0x0012,//�˸��(CANCEL / Del)
	
	//�Ӽ���
	KEY_VIRTUAL_ADD 		= 0x0015,//��+
	KEY_VIRTUAL_DEC 		= 0x0016,//��-
	
	//���򰴼�
	KEY_VIRTUAL_UP			= 0x0020,//��
	KEY_VIRTUAL_DOWN		= 0x0021,//��
	KEY_VIRTUAL_LEFT		= 0x0022,//��
	KEY_VIRTUAL_RIGHT		= 0x0023,//��
	KEY_VIRTUAL_ENTER		= 0x0024,//ȷ��
	KEY_VIRTUAL_ESC 		= 0x0025,//�˳�
	
	//�������ܼ�
	KEY_VIRTUAL_POWER		= 0x0026,//��Դ��
	KEY_VIRTUAL_MENU		= 0x0027,//��¼��
	KEY_VIRTUAL_INFO		= 0x0028,//info
	KEY_VIRTUAL_GOTO		= 0x0029,//goto
	KEY_VIRTUAL_OSD		    = 0x002a,//OSD
	KEY_VIRTUAL_F1			= 0x002d,//f1
	KEY_VIRTUAL_F2			= 0x002e,//f2
	KEY_VIRTUAL_F3			= 0x002f,//f3
	
	//���Ű���
	KEY_VIRTUAL_RECORD		= 0x0030,//¼��
	KEY_VIRTUAL_AUDIO		= 0x0031,//��Ƶ
	KEY_VIRTUAL_PLAY		= 0x0032,//����
	KEY_VIRTUAL_PREV		= 0x0033,//��һ��
	KEY_VIRTUAL_NEXT		= 0x0034,//��һ��
	KEY_VIRTUAL_REW 		= 0x0035,//����
	KEY_VIRTUAL_FF			= 0x0036,//���
	KEY_VIRTUAL_STOP		= 0x0037,//ֹͣ
	KEY_VIRTUAL_PAUSE		= 0x0038,//��ͣ
	KEY_VIRTUAL_SF			= 0x0039,//����
	KEY_VIRTUAL_SR			= 0x003a,//����
	
	//��̨����
	KEY_VIRTUAL_PTZ 		= 0x0040,
	KEY_VIRTUAL_FOCUS_ADD	= 0x0041,
	KEY_VIRTUAL_FOCUS_DEC	= 0x0042,
	KEY_VIRTUAL_ZOOM_ADD	= 0x0043,
	KEY_VIRTUAL_ZOOM_DEC	= 0x0044,
	KEY_VIRTUAL_IRIS_ADD	= 0x0045,
	KEY_VIRTUAL_IRIS_DEC	= 0x0046,
	KEY_VIRTUAL_SPEED_ADD	= 0x0047,
	KEY_VIRTUAL_SPEED_DEC	= 0x0048,
	KEY_VIRTUAL_WIPER		= 0x0049,//(WIPER)
	KEY_VIRTUAL_PRESET		= 0x004a,
	KEY_VIRTUAL_SNAP		= 0x004b,//(SNAP)
    KEY_VIRTUAL_AUTO		= 0x004c,//(auto)
    
	//��ز���
	KEY_VIRTUAL_SINGLE		= 0x0050,//������(SD)
	KEY_VIRTUAL_QUAD		= 0x0051,//�Ĺ���
	KEY_VIRTUAL_NINE		= 0x0052,//�Ź���
	KEY_VIRTUAL_SIXTEEN 	= 0x0053,//ʮ������

	///�Խ����䰴��
	KEY_VIRTUAL_MIC 		= 0x0060,//�Խ����䰴��
} KEY_VIRTUAL_E;

#if 1

// ��Ԥ���������е����
typedef enum
{
	DISK_ID_UNKNOWN = 0,
	DISK_ID_DATA,	// ������---��¼��,���ڼ�¼��־��ץ�ĺ��������ݼ�¼��
	DISK_ID_REC1,	// ¼����1 ---ֻ��¼��ʹ��
	DISK_ID_REC2,	// ¼����2
	DISK_ID_REC3,	// ¼����3
	DISK_ID_REC4,	// ¼����4
	DISK_ID_EXT1,	// ��չ�� ---��;����
	DISK_ID_MAX,
}DISK_ID_E;

typedef enum hiPIC_SIZE_E
{
    PIC_QCIF = 0,
    PIC_CIF,    
    PIC_2CIF,    
    PIC_HD1, 
    PIC_D1,
    PIC_960H,
    
    PIC_QVGA,    /* 320 * 240 */
    PIC_VGA,     /* 640 * 480 */    
    PIC_XGA,     /* 1024 * 768 */   
    PIC_SXGA,    /* 1400 * 1050 */    
    PIC_UXGA,    /* 1600 * 1200 */    
    PIC_QXGA,    /* 2048 * 1536 */

    PIC_WVGA,    /* 854 * 480 */
    PIC_WSXGA,   /* 1680 * 1050 */      
    PIC_WUXGA,   /* 1920 * 1200 */
    PIC_WQXGA,   /* 2560 * 1600 */
    
    PIC_HD720,   /* 1280 * 720 */
    PIC_HD1080,  /* 1920 * 1080 */
    
    PIC_BUTT
}PIC_SIZE_E;

#endif



void mysdk_init(void);
void myserver_init(void);


/***************************************************************************************************
¼��طſ���
parameter:	ctrl_mode
	VIDEO_CTRL_PAUSE_RECORD		= 20, //��ͣ¼��---��¼������ҳ��ʱִ��
	VIDEO_CTRL_RESUME_RECORD	, //����¼��---�˳�¼������ҳ��ʱִ��
	VIDEO_CTRL_OPEN_REC_FILE	, //��¼���ļ�
	VIDEO_CTRL_CLOSE_REC_FILE	, //�ر�¼���ļ�
	VIDEO_CTRL_SETLAYOUT		, //���ò��Ż��沼��
	PIC_START_PLAY			, //��ʼͼƬ����	
	PIC_PLAY_ONE			, //����һ��ͼƬ
	PIC_END_PLAY			, //ֹͣͼƬ����
	VIDEO_PLAY_STOP			= 0, //ֹͣ
	VIDEO_PLAY_NORMAL		, //����
	VIDEO_PLAY_PAUSE		, //��ͣ
	VIDEO_PLAY_FORWARD1		, //���1
	VIDEO_PLAY_FORWARD2		, //���2
	VIDEO_PLAY_FORWARD3		, //���3
	VIDEO_PLAY_FORWARD4		, //���4
	VIDEO_PLAY_BACKWARD1		, //����1
	VIDEO_PLAY_BACKWARD2		, //����2
	VIDEO_PLAY_BACKWARD3		, //����3
	VIDEO_PLAY_BACKWARD4		, //����4
	VIDEO_PLAY_SLOWDOWN1		, //����1
	VIDEO_PLAY_SLOWDOWN2		, //����2
	VIDEO_PLAY_SLOWDOWN3		, //����3
	VIDEO_PLAY_SLOWDOWN4		, //����4
	VIDEO_PLAY_FORWARD_IFRAME	, //������һI֡
	VIDEO_PLAY_BACKWARD_IFRAME	, //�˵���һI֡
	VIDEO_PLAY_FORWARD_ALARM	, //������һ����֡
	VIDEO_PLAY_BACKWARD_ALARM	, //�˵���һ����֡
	VIDEO_PLAY_MAX_PLAY_STATUS	,

path :¼��·��
offsettime; �ط�ƫ��ʱ��:ָ�����¼��ʼʱ���ƫ��ʱ��,��λ:s
layoutmode;  ����ģʽ:Video_Layout_Mode
showpage;  ��ʾҳ

*****************************************************************************************************/
void Video_Play_Ctrl(int ctrl_mode,char* path,int offsettime,int layoutmode,int showpage);


/***************************************************************************************************
��ƵԤ�����ֿ��� 
parameter: ctrl_mode
	VIDEO_LAYOUT_1X1		= 0, //������
	VIDEO_LAYOUT_2X2		, //�Ļ���
	VIDEO_LAYOUT_3X3		, //�Ż���
	VIDEO_LAYOUT_PARAM		, //�������ò���
	VIDEO_LAYOUT_USER		, //�û��Զ��岼��
	VIDEO_LAYOUT_MAX	
*****************************************************************************************************/
void Video_Layout_Mode_Ctrl(int ctrl_mode,int showpage);
unsigned int Get_Video_Layout_Mode(void);


/***************************************************************************************************
b.���ɶ�ý���ļ�

filetype :	
	MULTIMEDIAFILE_AUDIO		= 1, //b.��Ƶ�ļ�
	MULTIMEDIAFILE_VIDEO		= 2, //b.��Ƶ�ļ�
	MULTIMEDIAFILE_PIC		= 3, //b.ͼƬ�ļ�
*****************************************************************************************************/
void Control_CreateMultimediaFile(int filetype,char pic_size,char* filename,int channelno,int timelong);



/***************************************************************************************************
¼������
parameter :
rec_disk: �洢����:0-���д���;1-SD1;2-SD2;3-Ӳ��;4-����Ӳ��
message_type:¼������:0-����¼��;1-��ͨ¼��;2-����¼��;101-ͼƬ��ѯ
starttime: ��ʼʱ��(��ʽ:20141010-235959)
endtime:  ����ʱ��(��ʽ:20141010-235959)
*****************************************************************************************************/
int Control_Video_Search(int rec_disk,int message_type,char* starttime,char* endtime,char ren_data[2048*8]);




/***************************************************************************************************
¼���ļ�����

parameter :
rec_path: ¼���ļ�·��,��:/mnt/sd1/rec_dir/a.264
export_path :  �����ļ�·��,��:/mnt/udisk/b.264
starttime: ������ʼʱ��:(��ʽ:20160826-000101)
endtime:  ��������ʱ��(��ʽ:20160826-235959)

ע�⣺��ָ��¼���ļ�·��ʱ��������ȷ���ص������ȣ��͵��������
����ָ��¼���ļ�·��ʱ��������ȷ֪���������Ⱥ͵��������
*****************************************************************************************************/
int Control_Video_File_OutPut(char* rec_path,char* export_path,char* starttime,char* endtime);




/***************************************************************************************************
�������,����8��ip��ַ
parame :network_type   ����IP���:0~7;0-��Ƶ������;1-gps������;2-��gps������;3~6-Ԥ��;7-����ftp������(�û����������Կո��������λ��)

*****************************************************************************************************/
Param_CenterNetWork Get_CentterNetWork_Param(s32 network_type);

void Set_CentterNetWork_Param(s32 network_type,s8 * centerip,s32 contrlport);



/***************************************************************************************************
b.�豸��Ϣ

*****************************************************************************************************/
Param_Terminal_Info Get_Param_Terminal_Info(void);

void Set_Param_Terminal_Info(Param_Terminal_Info info);




/***************************************************************************************************
�����������

*****************************************************************************************************/	
Param_LocalNetWork Get_Param_LocalNetWork_Info(void);
void Set_Param_LocalNetWork_Info(Param_LocalNetWork info);




/***************************************************************************************************
¼������

*****************************************************************************************************/
Param_RecordSet Get_Param_RecordSet_Info(void);
void Set_Param_RecordSet_Info(Param_RecordSet info);
void Set_Param_CameraType(s32 CameraType);


/***************************************************************************************************
¼���������

*****************************************************************************************************/

Param_RecordCodeSet Get_Param_RecordCode_Info(int StreamType,unsigned char chn);
void Set_Param_RecordCode_Info(Param_RecordCodeSet info);



/*   get audio data control   */
void Send_Av_Ctl_Protocol(char * cmd,int len);

unsigned char get_camera_status(void);
void Set_Param_Record_OSD_GPS_Info(char gpsInfo[32]);		//chrade add need add to shenzheng 170716

unsigned int get_record_status(void);
unsigned int get_playback_status(void);
int set_remote_key(int type, int subtype, int State);
#ifdef __cplusplus
}
#endif


#endif

