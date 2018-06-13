

#ifndef __LL_DEFINES_H__
#define __LL_DEFINES_H__

#include "ll_headers.h"

//输入设备及输入通道
#define VI_DEV_MAX		4
#define VI_CHN_MAX		8

//输出设备及输出通道
#define VO_DEV_MAX		2
#define VO_CHN_PER_DEV_MAX	8

//输出设备名, 3515不支持AD
#define VO_DEV_HD  			0
#define VO_DEV_SD  			2

//制式对应的帧率
#define PAL_VIDEO_FRAME_RATE 25
#define NTSC_VIDEO_FRAME_RATE 30

// 背景色 RGB format is 1888
#define VO_BKGRD_RED      0xFF0000
#define VO_BKGRD_GREEN    0x00FF00
#define VO_BKGRD_BLUE     0x0000FF
#define VO_BKGRD_BLACK    0x000000


typedef enum VENC_MODE_E{
	VENC_1D1 = 0,
	VENC_2D1,
	VENC_2HD1,
	VENC_4HD1,
	VENC_2D12CIF,
	VENC_4CIF,
	VENC_4D1,
	VENC_MAX,
};


#define PLAYBACK_VO_MODE	1 //回放画面
#define PREVIEW_VO_MODE		2 //预览画面

typedef enum
{
	VO_VIDEO_LAYOUT_1X1,
	VO_VIDEO_LAYOUT_2X2,
	VO_VIDEO_LAYOUT_3X3,
	VO_VIDEO_LAYOUT_4X4,
	VO_VIDEO_LAYOUT_MAX,
}VO_VIDEO_LAYOUT_E;

//音频输入设备及输入通道
//#define AI_DEV_MAX		1
//#define AI_CHN_MAX		8

//音频输出设备及输出通道
//#define AO_DEV_MAX		1
//#define AO_CHN_MAX		2

//语音对讲输入通道
#define AI_IPPHONE_CHN	4
#define AO_IPPHONE_CHN	0


//typedef int (*AVEncodeDataProc)(int frame_type, void *data_node);
#define AENC_DATA_PROC_MAX	4
#define VENC_DATA_PROC_MAX	4


#define FRAME_TYPE_VIDEO		        0
#define FRAME_TYPE_AUDIO		        1
#define FRAME_TYPE_BIN_GPS		    2
#define FRAME_TYPE_BIN_ALARM		    3
#define FRAME_TYPE_BIN_IOSTATE		4
#define FRAME_TYPE_BIN_GSENSOR		5
#define FRAME_TYPE_BIN_GYROSENSOR	6
#define FRAME_TYPE_BIN_STATUS		    7

#define FRAME_TYPE_H264I		        0
#define FRAME_TYPE_H264P		        1
#define FRAME_TYPE_MEDIA_G726		   2
#define FRAME_TYPE_MEDIA_BIN	       3
#define FRAME_TYPE_MEDIA_PCM		4

typedef enum
{
	STREAM_TYPE_MAIN,	    //主码流
	STREAM_TYPE_SUBJECT,	//次码流
	STREAM_TYPE_MIRROR,	//镜像码流
	STREAM_TYPE_MAX,
}STREAM_TYPE_E;


#define MAX_ENCODE_CHANNEL 8


//typedef int (*MediaDataCallback)(int chn, const void *media_data, int media_type, int id, void *arg);
typedef int (*MediaDataCallback)(int chn, int stream_type, int frame_type, int frame_len, s64 frame_pts, char *frame_buf, void *args);

typedef struct
{
	MediaDataCallback cbFunc;
	void *cbArgs;
	int ireserved;
	void *preserved;
}MEDIA_DATA_CALLBACK_S;


#define HZK_FONT_PATH		"./res/font/CH_16.dat"
#define ASC_FONT_PATH		"./res/font/E_16.dat"



#define SAFE_DELETE_OBJECT(p)	if ((p) != NULL) {delete (p); (p) = NULL;}
#define SAFE_DELETE_ARRAY(p)	if ((p) != NULL) {delete [] (p); (p) = NULL;}
#define SAFE_CLOSE_HANDLE(p)	if ((p) != NULL) {CloseHandle((p)); (p) = NULL;}
#define SAFE_CLOSE_SOCKET(p)	if ((p) != -1) {net_close((p)); (p) = -1;}
#define SAFE_FREE(p)		if ((p) != NULL) {free (p); (p) = NULL;}

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)		if ( (p) != NULL ) { (p)->Release(); (p) = NULL;}
#endif

#endif //__LL_DEFINES_H__






