/* Copyright (c) 2015 Fleety Limited Co. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
#ifndef _CB_H
#define	_CB_H

#include	<pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char		cb_u8;
typedef unsigned short int	cb_u16;
typedef unsigned int		cb_u32;
typedef signed char			cb_s8;
typedef signed short int	cb_s16;
typedef signed int			cb_s32;
typedef char				cb_char;
typedef bool				cb_bool;


typedef struct {
	cb_u8	*bufptr;
	cb_u32	buflen;
	cb_u32	datalen;
	cb_u32	readpos;
	cb_u32	writepos;

	pthread_mutex_t mutex; 
}CircleBufferMngr;

bool cb_init(CircleBufferMngr **ppmngr, cb_u32 buflen);
bool cb_deinit(CircleBufferMngr **ppmngr);

cb_bool cb_full(CircleBufferMngr *pmngr);
cb_bool cb_empty(CircleBufferMngr *pmngr);
cb_u32 cb_datalen(CircleBufferMngr *pmngr);
cb_u32 cb_freelen(CircleBufferMngr *pmngr);

cb_u32 cb_read(CircleBufferMngr *pmngr,cb_u8 *outbuf,cb_s32 buflen);
cb_u32 cb_write(CircleBufferMngr *pmngr, cb_u8 *datptr, cb_s32 datlen);

cb_u32 cb_read_no_offset(CircleBufferMngr *pmngr,cb_u8 *outbuf,cb_s32 buflen);
void cb_read_move_offset(CircleBufferMngr *pmngr,cb_s32 offset);

#ifdef __cplusplus
}
#endif


#endif
