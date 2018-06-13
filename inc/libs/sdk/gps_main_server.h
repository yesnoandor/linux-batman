#ifndef _GPS_MAIN_SERVER_H
#define _GPS_MAIN_SERVER_H


#ifdef __cplusplus
extern "C" {
#endif

#define 		__packed 		__attribute__((packed))


/****************************************************************************************************
*按键值整理:不同输入方式在一个指定表里映射按键值
*upd by cgd on 20140930
****************************************************************************************************/
typedef enum {

	//数字按键
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
	KEY_VIRTUAL_BACKSPACE 	= 0x0012,//退格键(CANCEL / Del)
	
	//加减键
	KEY_VIRTUAL_ADD 		= 0x0015,//加+
	KEY_VIRTUAL_DEC 		= 0x0016,//减-
	
	//方向按键
	KEY_VIRTUAL_UP			= 0x0020,//上
	KEY_VIRTUAL_DOWN		= 0x0021,//下
	KEY_VIRTUAL_LEFT		= 0x0022,//左
	KEY_VIRTUAL_RIGHT		= 0x0023,//右
	KEY_VIRTUAL_ENTER		= 0x0024,//确认
	KEY_VIRTUAL_ESC 		= 0x0025,//退出
	
	//辅助功能键
	KEY_VIRTUAL_POWER		= 0x0026,//电源键
	KEY_VIRTUAL_MENU		= 0x0027,//登录键
	KEY_VIRTUAL_INFO		= 0x0028,//info
	KEY_VIRTUAL_GOTO		= 0x0029,//goto
	KEY_VIRTUAL_OSD		    = 0x002a,//OSD
	KEY_VIRTUAL_F1			= 0x002d,//f1
	KEY_VIRTUAL_F2			= 0x002e,//f2
	KEY_VIRTUAL_F3			= 0x002f,//f3
	
	//播放按键
	KEY_VIRTUAL_RECORD		= 0x0030,//录像
	KEY_VIRTUAL_AUDIO		= 0x0031,//音频
	KEY_VIRTUAL_PLAY		= 0x0032,//播放
	KEY_VIRTUAL_PREV		= 0x0033,//上一个
	KEY_VIRTUAL_NEXT		= 0x0034,//下一个
	KEY_VIRTUAL_REW 		= 0x0035,//快退
	KEY_VIRTUAL_FF			= 0x0036,//快进
	KEY_VIRTUAL_STOP		= 0x0037,//停止
	KEY_VIRTUAL_PAUSE		= 0x0038,//暂停
	KEY_VIRTUAL_SF			= 0x0039,//慢放
	KEY_VIRTUAL_SR			= 0x003a,//慢退
	
	//云台按键
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
    
	//监控布局
	KEY_VIRTUAL_SINGLE		= 0x0050,//单画面(SD)
	KEY_VIRTUAL_QUAD		= 0x0051,//四宫格
	KEY_VIRTUAL_NINE		= 0x0052,//九宫格
	KEY_VIRTUAL_SIXTEEN 	= 0x0053,//十六宫格

	///对讲话咪按键
	KEY_VIRTUAL_MIC 		= 0x0060,//对讲话咪按键
} KEY_VIRTUAL_E;

#if 1

// 在预定义数组中的序号
typedef enum
{
	DISK_ID_UNKNOWN = 0,
	DISK_ID_DATA,	// 数据盘---不录像,用于记录日志，抓拍和运行数据记录等
	DISK_ID_REC1,	// 录像盘1 ---只作录像使用
	DISK_ID_REC2,	// 录像盘2
	DISK_ID_REC3,	// 录像盘3
	DISK_ID_REC4,	// 录像盘4
	DISK_ID_EXT1,	// 扩展盘 ---用途待定
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
录像回放控制
parameter:	ctrl_mode
	VIDEO_CTRL_PAUSE_RECORD		= 20, //暂停录像---进录像搜索页面时执行
	VIDEO_CTRL_RESUME_RECORD	, //继续录像---退出录像搜索页面时执行
	VIDEO_CTRL_OPEN_REC_FILE	, //打开录像文件
	VIDEO_CTRL_CLOSE_REC_FILE	, //关闭录像文件
	VIDEO_CTRL_SETLAYOUT		, //设置播放画面布局
	PIC_START_PLAY			, //开始图片播放	
	PIC_PLAY_ONE			, //播放一张图片
	PIC_END_PLAY			, //停止图片播放
	VIDEO_PLAY_STOP			= 0, //停止
	VIDEO_PLAY_NORMAL		, //播放
	VIDEO_PLAY_PAUSE		, //暂停
	VIDEO_PLAY_FORWARD1		, //快进1
	VIDEO_PLAY_FORWARD2		, //快进2
	VIDEO_PLAY_FORWARD3		, //快进3
	VIDEO_PLAY_FORWARD4		, //快进4
	VIDEO_PLAY_BACKWARD1		, //快退1
	VIDEO_PLAY_BACKWARD2		, //快退2
	VIDEO_PLAY_BACKWARD3		, //快退3
	VIDEO_PLAY_BACKWARD4		, //快退4
	VIDEO_PLAY_SLOWDOWN1		, //慢放1
	VIDEO_PLAY_SLOWDOWN2		, //慢放2
	VIDEO_PLAY_SLOWDOWN3		, //慢放3
	VIDEO_PLAY_SLOWDOWN4		, //慢放4
	VIDEO_PLAY_FORWARD_IFRAME	, //跳到下一I帧
	VIDEO_PLAY_BACKWARD_IFRAME	, //退到上一I帧
	VIDEO_PLAY_FORWARD_ALARM	, //跳到下一报警帧
	VIDEO_PLAY_BACKWARD_ALARM	, //退到上一报警帧
	VIDEO_PLAY_MAX_PLAY_STATUS	,

path :录像路径
offsettime; 回放偏移时间:指相对于录像开始时间的偏移时间,单位:s
layoutmode;  布局模式:Video_Layout_Mode
showpage;  显示页

*****************************************************************************************************/
void Video_Play_Ctrl(int ctrl_mode,char* path,int offsettime,int layoutmode,int showpage);


/***************************************************************************************************
视频预览布局控制 
parameter: ctrl_mode
	VIDEO_LAYOUT_1X1		= 0, //单画面
	VIDEO_LAYOUT_2X2		, //四画面
	VIDEO_LAYOUT_3X3		, //九画面
	VIDEO_LAYOUT_PARAM		, //参数设置布局
	VIDEO_LAYOUT_USER		, //用户自定义布局
	VIDEO_LAYOUT_MAX	
*****************************************************************************************************/
void Video_Layout_Mode_Ctrl(int ctrl_mode,int showpage);
unsigned int Get_Video_Layout_Mode(void);


/***************************************************************************************************
b.生成多媒体文件

filetype :	
	MULTIMEDIAFILE_AUDIO		= 1, //b.音频文件
	MULTIMEDIAFILE_VIDEO		= 2, //b.视频文件
	MULTIMEDIAFILE_PIC		= 3, //b.图片文件
*****************************************************************************************************/
void Control_CreateMultimediaFile(int filetype,char pic_size,char* filename,int channelno,int timelong);



/***************************************************************************************************
录像搜索
parameter :
rec_disk: 存储介质:0-所有磁盘;1-SD1;2-SD2;3-硬盘;4-备份硬盘
message_type:录像类型:0-所有录像;1-普通录像;2-报警录像;101-图片查询
starttime: 开始时间(格式:20141010-235959)
endtime:  结束时间(格式:20141010-235959)
*****************************************************************************************************/
int Control_Video_Search(int rec_disk,int message_type,char* starttime,char* endtime,char ren_data[2048*8]);




/***************************************************************************************************
录像文件导出

parameter :
rec_path: 录像文件路径,如:/mnt/sd1/rec_dir/a.264
export_path :  导出文件路径,如:/mnt/udisk/b.264
starttime: 导出开始时间:(格式:20160826-000101)
endtime:  导出结束时间(格式:20160826-235959)

注意：当指定录像文件路径时，可以正确返回导出进度，和导出结果。
当不指定录像文件路径时，不能正确知道导出进度和导出结果。
*****************************************************************************************************/
int Control_Video_File_OutPut(char* rec_path,char* export_path,char* starttime,char* endtime);




/***************************************************************************************************
网络参数,共有8组ip地址
parame :network_type   中心IP序号:0~7;0-视频服务器;1-gps服务器;2-备gps服务器;3~6-预留;7-升级ftp服务器(用户名和密码以空格间隔填到域名位置)

*****************************************************************************************************/
Param_CenterNetWork Get_CentterNetWork_Param(s32 network_type);

void Set_CentterNetWork_Param(s32 network_type,s8 * centerip,s32 contrlport);



/***************************************************************************************************
b.设备信息

*****************************************************************************************************/
Param_Terminal_Info Get_Param_Terminal_Info(void);

void Set_Param_Terminal_Info(Param_Terminal_Info info);




/***************************************************************************************************
本地网络参数

*****************************************************************************************************/	
Param_LocalNetWork Get_Param_LocalNetWork_Info(void);
void Set_Param_LocalNetWork_Info(Param_LocalNetWork info);




/***************************************************************************************************
录像设置

*****************************************************************************************************/
Param_RecordSet Get_Param_RecordSet_Info(void);
void Set_Param_RecordSet_Info(Param_RecordSet info);
void Set_Param_CameraType(s32 CameraType);


/***************************************************************************************************
录像编码设置

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

