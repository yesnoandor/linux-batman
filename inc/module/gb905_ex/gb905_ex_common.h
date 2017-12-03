#ifndef _STM32_PROTOCOL_H
#define	_STM32_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif



enum{
	SOCKET_DOMAIN = 0,
	UART_DOMAIN,
};

enum{
	GB905_EX_RESULT_OK = 0,
	GB905_EX_RESULT_FAIL,
	GB905_EX_RESULT_UNKNOWN,
};


/*
* STM32 串口通讯协议的头信息
*/
typedef struct{
	unsigned short msg_id;							// 消息ID
	unsigned short msg_len;							// 消息体长度
	unsigned short msg_serial_number;				// 消息体流水号 （按发送顺序从0 开始循环累加）
	unsigned short reserved;
}__packed gb905_ex_header_t ,*p_gb905_ex_header_t;



/*
* 串口通讯协议的应答信息
*/
typedef struct{
	unsigned short seq;
	unsigned short id;
	unsigned char result;
}__packed gb905_ex_ack_body_t;

typedef  struct
{
    unsigned char start_magic_id;
    gb905_ex_header_t header;
    gb905_ex_ack_body_t ack_body;
    unsigned char xor;
    unsigned char end_magic_id;
} __packed gb905_ex_ack_t;

//typedef int (*p_analyze_func_t)(unsigned char *,int);
typedef void (*p_build_header_func_t)(gb905_ex_header_t * ,unsigned short,unsigned short);
typedef void (*p_build_ack_func_t)(gb905_ex_ack_t *,unsigned short ,gb905_ex_header_t *,unsigned char );
typedef	void (*p_send_data_func_t)(unsigned char ,unsigned char ,unsigned char * ,int);
typedef bool(*p_parse_protocol_t)(buff_mgr_t * msg);




struct gb905_ex_funcs{
	int (* p_analyze_func)(struct gb905_ex_funcs * funcs, unsigned char *,int);

	p_build_header_func_t p_build_header_func;
	p_build_ack_func_t p_build_ack_func;
	p_send_data_func_t p_send_data_func;
	
	p_parse_protocol_t p_parse_protocol;

	unsigned char domain;
	unsigned char index;
};

typedef struct gb905_ex_funcs gb905_ex_funcs_t;
//{
	// p_analyze_func_t p_analyze_func;

//};
typedef int (*p_analyze_func_t)(gb905_ex_funcs_t *,unsigned char *,int);

#if 0
void gb905_ex_build_header(gb905_ex_header_t * header, unsigned short msg_id, unsigned short msg_len);
void gb905_ex_build_ack(gb905_ex_ack_t * ack,gb905_ex_header_t * header,unsigned char result);

void gb905_ex_send_ack(gb905_ex_header_t * header,unsigned char result);
void gb905_ex_send_data(unsigned char index,unsigned char * buf, int len);

int gb905_protocol_ayalyze(unsigned char * buf,int len);
#endif

gb905_ex_funcs_t * gb905_ex_base_create(unsigned char domain,unsigned char index,p_parse_protocol_t callback);

void gb905_ex_base_release(gb905_ex_funcs_t *funcs);



void gb905_ex_debug_header(gb905_ex_header_t * header);
void gb905_ex_debug_ack(gb905_ex_ack_body_t * ack_body);



#ifdef __cplusplus
}
#endif

#endif

