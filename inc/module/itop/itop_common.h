/****************************************Copyright (c)**************************************************                         
** File name:			itop_common.h
** Descriptions:		 
**
**------------------------------------------------------------------------------------------------------
** Created by:		wenyu_xu
** Created date:		2017-11-03
** Version:			1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef _ITOP_COMMON_H
#define	_ITOP_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/*- Up message -*/
#define TYPE_UP_VEHICLE_LOCATION            	1		// 
#define TYPE_UP_ORDER_ACCEPT                	2
#define TYPE_UP_ORDER_REJECT                	3
#define TYPE_UP_DRIVER_LOGIN                	4
#define TYPE_UP_DRIVER_LOGOUT               	5
#define TYPE_UP_ALARM                       	6
#define TYPE_UP_RECORD_UPLOAD               	7
#define TYPE_UP_CANCEL_ORDER_CONFIRM        	8
#define TYPE_UP_CANCEL_TAKEN_ORDER          	9
#define TYPE_UP_REPORT                      	10
#define TYPE_UP_AUTH                        	11
#define TYPE_UP_PARAM_REQ                   	12
#define TYPE_UP_SPECIAL_ORDER_STATUS        	13
#define TYPE_UP_EXTEND_MSG                  	15

/*- Extend up message -*/
#define EXT_UP_INSURANCE_ORDER_CONFIRM      	0x01
#define EXT_UP_PHONE_LIST_CONFIRM               0x02
#define EXT_UP_MDT_PARAMETER_UP             	0x03
#define EXT_UP_ORDER_MODIFY_CONFM           	0x04
#define EXT_UP_OVER_ALARM                   	0x05
#define EXT_UP_REPORT_GPS_CELLID                0x06
#define EXT_UP_REPORT_FAREMETER               	0x0A
#define EXT_UP_CMD_RESPONSE                   	0x0C
#define EXT_UP_TCAR_STATE                     	0x0D
#define EXT_UP_TBUSINESS_DATA                 	0x0E
#define EXT_UP_TSC_BROKEN                     	0x0F
#define EXT_UP_TSC_LOGIN                      	0x10
#define EXT_UP_TSC_LOGOUT                     	0x11
#define EXT_UP_TSC_PARAM                      	0x12
#define EXT_UP_POS_DEALDATA                   	0x13
#define EXT_UP_POS_BROKEN                     	0x14
#define EXT_UP_POS_PACKDATA                   	0x15
#define EXT_UP_TSC_STATUS                     	0x16
#define EXT_UP_BLACKBOX_DATA                  	0x17
#define EXT_UP_MSGCONFIRM                   	0x19
#define EXT_UP_NODEALAEM_DATA                 	0x21
#define EXT_UP_CALL_RECORD                    	0x22
#define EXT_UP_MSGCONFIRM2                      0x23
#define EXT_UP_GENERAL_ALARM                    0x24
#define EXT_UP_SPEED_STATE                      0x25 
#define EXT_UP_TPRT_RPT              		    0x27
#define EXT_UP_LEFT_CALLTIME                    0x28
#define EXT_UP_LEFT_CALLRECORD                  0x29
#define EXT_UP_LOG_UPLOAD                       0x30
#define EXT_UP_CONTEND_ORDER_OCCUPIED           0x32
#define EXT_UP_BLACKLIST_ACK_35                 0x35
#define EXT_UP_BLACKLIST_REQUEST                0x36
#define EXT_UP_HZH_PRICEDATA                    0x37
#define EXT_UP_VTDR_SAVE_DATA                   0x38
#define EXT_UP_VTDR_BACKUP_DRIVER_LOGIN         0x39
#define EXT_UP_LOCATION                         0x3F
#define EXT_UP_PRE_CRUSH_INDICATOR              0x41
#define EXT_UP_OVER_SPEED_UNITS                 0x42
#define EXT_UP_OIL_CAPACITY                     0x43
#define EXT_UP_GENEAL_MSG                       0x49
#define EXT_UP_NEW_EVENT_REPORT                 0x52
#define EXT_UP_RFID_TAG_STATUS                  0x57
#define EXT_UP_WX_PRICEDATA                     0x58
#define EXT_UP_CAMERA_HOT_PLUG                  0x59
#define EXT_UP_FLOWNO_ALARM                     0x61
#define EXT_UP_SCH_BUS_MSG                      0x71
#define EXT_UP_SEARCHED_PIC_INFO                0x72
#define EXT_UP_WX_LED_MSG                       0x73
#define EXT_UP_MONITOR_CONFIRM_MSG              0x74
#define EXT_UP_REQ_DISPT_CALL_MSG               0x75
#define EXT_UP_PACKET_DATA                      0x76
#define EXT_UP_EXT_LOC_0x78                     0x78
#define EXT_UP_COMMON_ACK_0x79                  0x79
#define EXT_UP_DEBUG_STR_0x80                   0x80
#define EXT_UP_LED_STR_0x81                     0x81
#define EXT_UP_LOGINOUT_0x91                    0x91 

/*- Down message -*/
#define TYPE_DN_LOCATION_REQUEST            			1
#define TYPE_DN_ORDER_INFO                  			2
#define TYPE_DN_ORDER_DETAIL_INFO           			3
#define TYPE_DN_ORDER_TAKEN_NOTICE          			4
#define TYPE_DN_CANCEL_TAKEN_ORDER          			5
#define TYPE_DN_TEXT                        			6
#define TYPE_DN_MONITOR_REQUEST             			7
#define TYPE_DN_LOGIN_CONFIRM               			8
#define TYPE_DN_LOGOUT_CONFIRM              			9
#define TYPE_DN_CODE_DOWNLOAD               			10
#define TYPE_DN_SYS_PARAM								11
#define TYPE_DN_AUTH									12
#define TYPE_DN_RECORD_REQUEST							13
#define TYPE_DN_EXTEND_MSG								15

/*- Extend down message -*/
#define EXTEND_TYPE_DN_INSURANCE_ORDER          		0x01
#define EXTEND_TYPE_DN_DRIVER_PHONE         			0x02
#define EXTENT_TYPE_DN_MDT_QUERY            			0x03
#define EXTENT_TYPE_DN_SYS_COMMAND          			0x04
#define EXTENT_TYPE_DN_ORDER_MODIFY         			0x05
#define EXTENT_TYPE_DN_PHONE_QUERY          			0x06
#define EXTENT_TYPE_DN_ALRRM_CLR                		0x07
#define EXTENT_TYPE_DN_LOCK_SERV                		0x08
#define EXTENT_TYPE_DN_KMESG                    		0x09
#define EXTENT_TYPE_DN_EVT_LIST             			0x0A
#define EXTENT_TYPE_DN_PHONE_LST2           			0x0B
#define EXTENT_TYPE_DN_CTRL_CDM             			0x0D
#define EXTENT_TYPE_DN_TSC_PARAM                       	0x0E
#define EXTENT_TYPE_DN_TSC_LOCK_TIME_STATUS  			0x0F
#define EXTENT_TYPE_DN_QUERY_SC_DEALDATA      			0x10
#define EXTENT_TYPE_DN_QUERY_POS_DEALDATA    			0x11
#define EXTENT_TYPE_DN_QUERY_POS_PACKDATA    			0x12
#define EXTENT_TYPE_DN_POS_BLACKLIST                	0x13
#define EXTENT_TYPE_DN_SCPOS_UPDATE                		0x14
#define EXTENT_TYPE_DN_QUERY_SC_PARAM           		0x15
#define EXTENT_TYPE_DN_BLACKBOX_QUERY           		0x16
#define EXTENT_TYPE_DN_NODE_ALARM                   	0x18
#define EXTENT_TYPE_DN_SUPER_NOTICE                		0x19
#define EXTENT_TYPE_DN_LOCK2                           	0x1A
#define EXTENT_TYPE_DN_PUSH_CALL                		0x1B
#define EXTENT_TYPE_DN_CAMERA_CONTROL          			0x1d
#define EXTENT_TYPE_DN_QUERY_LEFT_CALLTIME  			0x1E
#define EXTENT_TYPE_DN_LEFT_CALLTIME                	0x1F
#define EXTENT_TYPE_DN_LOG_REQ                  		0x20
#define EXTENT_TYPE_DN_CLASSIFIED_TEXT              	0x23
#define EXTENT_TYPE_DN_HZH_BLACKLIST                	0x24
#define EXTENT_TYPE_DN_HZH_PDATA_ACK               		0x25
#define EXTENT_TYPE_DN_VTDR_SET_QUERY              		0x29
#define EXTENT_TYPE_DN_VTDR_BACKUP_DRIVER_INFO    		0x30
#define EXTENT_TYPE_DN_PIC_UPLOAD_ACK           		0x35
#define EXTENT_TYPE_DN_OIL_CAPACITY_QUERY    			0x36
#define EXTENT_TYPE_DN_GQY_MSG               			0x39
#define EXTENT_TYPE_DN_CAMERA_HOTPLUG_ACK    			0x40
#define EXTENT_TYPE_DN_WX_PDATA_ACK          			0x53
#define EXTENT_TYPE_DN_WX_PIC_UPLOAD         			0x54
#define EXTENT_TYPE_DN_WX_LOCK_FLAG          			0x55
#define EXTENT_TYPE_DN_WX_DATA               			0x56
#define EXTENT_TYPE_DN_ALARM_ACK             			0x57
#define EXTENT_TYPE_DN_SCH_BUS_MSG           			0x59
#define EXTENT_TYPE_DN_SAVED_PIC_SERACH      			0x61
#define EXTENT_TYPE_DN_SERACHED_PIC_UP       			0x62
#define EXTENT_TYPE_DN_WX_LED_MSG            			0x65
#define EXTENT_TYPE_DN_PIC_INDEX_ACK         			0x66
#define EXTENT_TYPE_DN_GRADE_LOG_REQ         			0x6C
#define EXTENT_TYPE_DN_LED_SCREEN            			0x71
#define EXTENT_TYPE_DN_LOGINOUT_0x91         			0x91



//#define __packed __attribute__((packed))

typedef union 
{
	unsigned char whole;
	
	struct
	{
		// low --> high
		unsigned char msg_id:4;				// =0, 消息号
		unsigned char reserved:2;			// 
		unsigned char state:2;				// 
		
    }msg_type;
}itop_msg_type_t;


// 描述消息的头格式
typedef struct{
	itop_msg_type_t msg_type;
	unsigned short msg_len;
	unsigned short mtd_l;
}__packed itop_msg_header_t ,*p_itop_msg_header_t;

// 描述扩展消息的头格式
typedef struct 
{
	itop_msg_type_t msg_type;
	unsigned char ext_msg_id;
	unsigned short msg_len;
	unsigned short mtd_l;
}__packed itop_ext_msg_header_t;


// 描述扩展头格式
typedef struct 
{
    unsigned short   head_F0F0;
    unsigned short   series_NO;
    unsigned short   mtd_h;
}__packed itop_ext_header_t;

int itop_protocol_analyze(unsigned char * buf,int len);
int itop_send_data(unsigned char socket_index,unsigned char * buf, int len);
void itop_protocol_request(unsigned char * buf, int len);

#ifdef __cplusplus
}
#endif

#endif

