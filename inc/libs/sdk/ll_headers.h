
#ifndef __LL_HEADERS_H__
#define __LL_HEADERS_H__

/************************************************************************
常用的头文件
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>
//#include <stdint.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <time.h>
#include <signal.h>
#include <malloc.h>
#include <limits.h>
#include <sys/types.h>
#include <ctype.h>
#include <math.h>


#ifdef WIN32
	#define _WIN32_WINNT 0x0501 //xp=0x501, 2000=0x500, 使用TryEnterCriticalSection函数, SRWLOCK需要vista以上版本支持
	#include <windows.h>
	#include <direct.h>
	#include <winioctl.h>
	#include <io.h>
	#pragma warning(disable:4786)
	#define sleeps(x) Sleep((x)*1000)
	#define sleepms Sleep
	#define strncasecmp(x,y,z) _strnicmp(x,y,z)
	#define strcasecmp(x,y) _stricmp(x,y)
	#define snprintf _snprintf
	#define vsnprintf _vsnprintf
	#define getcwd _getcwd
	#define strtoll(str, x, y) _atoi64(str)
	#define MKDIR(x)	mkdir(x)
	#define LLABS(x)	abs((int)(x))
#else //linux
	#include <unistd.h>
	#include <dirent.h>
	#include <sys/file.h> // flock函数
	#define sleepms(x) usleep((x)*1000)
	#define sleeps(x) sleep(x)
	#ifndef O_BINARY
	#define O_BINARY 0
	#endif
	#define MKDIR(x)	mkdir(x, 0755)
	#define LLABS(x)	llabs(x)
#endif

// 整型环境, stdint.h头文件
#if !defined __int8_t_defined && !defined __uint32_t_defined
#define __int8_t_defined
#define __uint32_t_defined
typedef unsigned char	uint8_t;
typedef signed char	int8_t;
typedef unsigned short	uint16_t;
typedef signed short	int16_t;
typedef unsigned int	uint32_t;
typedef signed int	int32_t;
#if defined WIN32
typedef __int64			int64_t;
typedef unsigned __int64	uint64_t;
#else
typedef long long		int64_t;
typedef unsigned long long	uint64_t;
#endif
#endif //!defined __int8_t_defined && !defined __uint32_t_defined

// 整型环境, 内核使用的类型
typedef char			s8;
typedef unsigned char		u8;
typedef short			s16;
typedef unsigned short		u16;
typedef int			s32;
typedef unsigned int		u32;
#if defined WIN32
typedef __int64			s64;
typedef unsigned __int64	u64;
#else
typedef long long		s64;
typedef unsigned long long	u64;
#endif


#ifdef WIN32
#define UINT64_C(x)	x ## UI64
#define INT64_C(x)	x ## I64
#define INT64STR	"%I64d"
#else
#define UINT64_C(x)	x ## ULL
#define INT64_C(x)	x ## LL
#define INT64STR	"%lld"
#endif


// 函数参数属性
#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
#ifndef INOUT
#define INOUT
#endif
// vc的c语言编译不支持inline关键字, gcc支持
#if !defined __cplusplus && !defined __unix__
#define inline
#endif

// max / min
#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b) ((a) > (b) ? (b) : (a))
#endif

// 位操作
#ifndef setbit
#ifndef NBBY			//the BSD family defines NBBY
#define	NBBY	8		//8 bits per byte
#endif
#define	setbit(a, i)	(((u8 *)a)[(i)/NBBY] |= 1<<((i)%NBBY))
#define	clrbit(a, i)	(((u8 *)a)[(i)/NBBY] &= ~(1<<((i)%NBBY)))
#define	isset(a, i)	(((const u8 *)a)[(i)/NBBY] & (1<<((i)%NBBY)))
#define	isclr(a, i)	((((const u8 *)a)[(i)/NBBY] & (1<<((i)%NBBY))) == 0)

#define getbit(a, i)	((((const u8 *)a)[(i)/NBBY] & (1<<((i)%NBBY))) ? 1 : 0)

// 获取或设置指定的位数
// a=指针, i=起始位, n=总位数
// 注意: 不能跨越字节, 即需要保证起始位i及总位数n在一个字节内
#define getbits(a, i, n)	((((const u8 *)a)[(i)/NBBY] >> ((i)%NBBY)) & ~(~0 << (n)))
#define setbits(a, i, n, val)	do {						\
	((u8 *)a)[(i)/NBBY]  &=  ~( ~(~0 << (n)) << ((i)%NBBY) );		\
	((u8 *)a)[(i)/NBBY]  |=  ((val) & ~(~0 << (n)) << ((i)%NBBY) );		\
}while(0)
#endif

// 整数<->缓存互转换. le=little endian, be=big endian. 注意指针p必须是char *类型的
#define put8(p, dat) ( (p)[0] = (u8)((dat)>>0)&0xff )
#define putle16(p, dat) ( (p)[0] = ((u16)(dat)>>0)&0xff, (p)[1] = ((u16)(dat)>>8)&0xff )
#define putle24(p, dat) ( (p)[0] = (u8)((u32)(dat)>>0)&0xff, (p)[1] = (u8)((u32)(dat)>>8)&0xff, (p)[2] = (u8)((u32)(dat)>>16)&0xff )
#define putle32(p, dat) ( (p)[0] = (u8)((u32)(dat)>>0)&0xff, (p)[1] = (u8)((u32)(dat)>>8)&0xff, (p)[2] = (u8)((u32)(dat)>>16)&0xff, (p)[3] = (u8)((u32)(dat)>>24)&0xff )
#define putbe16(p, dat) ( (p)[0] = ((u16)(dat)>>8)&0xff, (p)[1] = ((u16)(dat)>>0)&0xff )
#define putbe24(p, dat) ( (p)[0] = (u8)((u32)(dat)>>16)&0xff, (p)[1] = (u8)((u32)(dat)>>8)&0xff, (p)[2] = (u8)((u32)(dat)>>0)&0xff )
#define putbe32(p, dat) ( (p)[0] = (u8)((u32)(dat)>>24)&0xff, (p)[1] = (u8)((u32)(dat)>>16)&0xff, (p)[2] = (u8)((u32)(dat)>>8)&0xff, (p)[3] = (u8)((u32)(dat)>>0)&0xff )
#define putbe64(p, dat) ( (p)[0] = (u8)((u64)(dat)>>56)&0xff, (p)[1] = (u8)((u64)(dat)>>48)&0xff, (p)[2] = (u8)((u64)(dat)>>40)&0xff, (p)[3] = (u8)((u64)(dat)>>32)&0xff, (p)[4] = (u8)((u64)(dat)>>24)&0xff, (p)[5] = (u8)((u64)(dat)>>16)&0xff, (p)[6] = (u8)((u64)(dat)>>8)&0xff, (p)[7] = (u8)((u64)(dat)>>0)&0xff )

#define get8(p) ( (p)[0] )
#define getle16(p) ( ((u8)(p)[0] << 0) | ((u8)(p)[1] << 8) )
#define getle24(p) ( ((u8)(p)[0] << 0) | ((u8)(p)[1] << 8) | ((u8)(p)[2] << 16) )
#define getle32(p) ( ((u8)(p)[0] << 0) | ((u8)(p)[1] << 8) | ((u8)(p)[2] << 16) | ((u8)(p)[3] << 24) )
#define getbe16(p) ( ((u8)(p)[0] << 8) | ((u8)(p)[1] << 0) )
#define getbe24(p) ( ((u8)(p)[0] << 16) | ((u8)(p)[1] << 8) | ((u8)(p)[2] << 0) )
#define getbe32(p) ( ((u8)(p)[0] << 24) | ((u8)(p)[1] << 16) | ((u8)(p)[2] << 8) | ((u8)(p)[3] << 0) )

//字符转换
#define CHAR2BIN(ch)	( ((ch)>='A') ? ((ch)-'A'+10) : ((ch)-'0') )
#define BIN2CHAR(bin)	( ((bin)>=10) ? ((bin)-10+'A') : ((bin)+'0') )

#define getvalue4(p) 	(  CHAR2BIN((u8)(p)[0] << 0) )
#define getvalue8(p) 	(  CHAR2BIN((u8)(p)[0] << 4) | CHAR2BIN((u8)(p)[1] << 0) )
#define getvalue12(p) 	(  CHAR2BIN((u8)(p)[0] << 8) | CHAR2BIN((u8)(p)[1] << 4) | CHAR2BIN((u8)(p)[2] << 0) )
#define getvalue16(p) 	(  CHAR2BIN((u8)(p)[0] << 12) | CHAR2BIN((u8)(p)[1] << 8) \
						| CHAR2BIN((u8)(p)[2] << 4) | CHAR2BIN((u8)(p)[3] << 0) )
						
#define getvalue32(p) 	(  CHAR2BIN((u8)(p)[0] << 28) | CHAR2BIN((u8)(p)[1] << 24) \
						| CHAR2BIN((u8)(p)[2] << 20) | CHAR2BIN((u8)(p)[3] << 16) \
						| CHAR2BIN((u8)(p)[4] << 12) | CHAR2BIN((u8)(p)[5] << 8) \
						| CHAR2BIN((u8)(p)[6] << 4) | CHAR2BIN((u8)(p)[7] << 0) )

#define putvalue4(p, v)  ( (p)[0] = BIN2CHAR(((u8)(v)&0xf) >> 0) )
#define putvalue8(p, v)  ( (p)[0] = BIN2CHAR(((u8)(v)&0xf0) >> 4), (p)[1] = BIN2CHAR(((u8)(v)&0xf) >> 0) )
#define putvalue12(p, v) ( (p)[0] = BIN2CHAR(((u16)(v)&0xf00) >> 8), (p)[1] = BIN2CHAR(((u16)(v)&0xf0) >> 4), (p)[2] = BIN2CHAR(((u16)(v)&0xf) >> 0) )
#define putvalue16(p, v) ( (p)[0] = BIN2CHAR(((u16)(v)&0xf000) >> 12), (p)[1] = BIN2CHAR(((u16)(v)&0xf00) >> 8), 	\
							(p)[2] = BIN2CHAR(((u16)(v)&0xf0) >> 4), (p)[3] = BIN2CHAR(((u16)(v)&0xf) >> 0) )
#define putvalue32(p, v) ( (p)[0] = BIN2CHAR(((u32)(v)&0xf0000000) >> 28), (p)[1] = BIN2CHAR(((u32)(v)&0xf000000) >> 24),	\
							(p)[2] = BIN2CHAR(((u32)(v)&0xf00000) >> 20),	(p)[3] = BIN2CHAR(((u32)(v)&0xf0000) >> 16),	\
							(p)[4] = BIN2CHAR(((u32)(v)&0xf000) >> 12),	(p)[5] = BIN2CHAR(((u32)(v)&0xf00) >> 8),		\
							(p)[6] = BIN2CHAR(((u32)(v)&0xf0) >> 4),		(p)[7] = BIN2CHAR(((u32)(v)&0xf) >> 0) )

#define STRING2BIN(buf, len) do{									\
		for(int i = 0 ; i< len; i++){									\
			buf[i]=(CHAR2BIN(buf[i*2]) << 4) | (CHAR2BIN(buf[i*2+1]));	\
		}																\
	}while(0)
#define BIN2STRING(buf, len) do{									\
		buf[len*2]=0;													\
		for(int i = len-1 ; i>= 0; i++){								\
			buf[i*2]=BIN2CHAR((buf[i]&0xf0) >> 4);						\
			buf[i*2+1]=BIN2CHAR((buf[i]&0xf) >> 0); 					\
		}																\
	}while(0)
#define FUN_BIN2CHAR(buf,news,len) do{								\
			news[len*2]=0;												\
			for(int i = len-1 ; i>= 0; i++){							\
				news[i*2]=BIN2CHAR((buf[i]&0xf0) >> 4); 				\
				news[i*2+1]=BIN2CHAR((buf[i]&0xf) >> 0);				\
			}															\
		}while(0)
	
	
//BCD码转换
#define bin2bcd(bin)  ( (((bin)/10) << 4) | ((bin)%10) )
#define bcd2bin(bcd)  ( ((bcd)&0x0f) + ((((bcd)>>4)&0x0f)*10) )

// 字节对齐
//exp=0,align=1(0x1)
//exp=1,align=2(0x2)
//exp=2,align=4(0x4)
//exp=3,align=8(0x8)
//exp=4,align=16(0x10)
//exp=5,align=32(0x20)
//exp=17,align=131072(0x20000)
#define ROUND(x, exp)		(((x)>>(exp))<<(exp))
#define ALIGN(x, exp)		( (((x)+(1<<(exp))-1)>>(exp)) << (exp) )

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

/************************************************************************
网络相关
************************************************************************/
#ifdef WIN32
	#include <winsock.h>
	//#include <WS2TCPIP.H>
	#pragma comment(lib, "Wsock32.lib")

	#define net_read(x,y,z) recv(x,y,z,0)
	#define net_write(x,y,z) send(x,y,z,0)
	#define net_close closesocket
	#define neterr WSAGetLastError()
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/time.h>
	#include <netdb.h>
	#include <sys/ioctl.h>
	#include <net/if.h>
	#include <sys/utsname.h>
	#include <netinet/in.h>
	#include <netinet/ip.h>
	#include <netinet/tcp.h> // tcp keepalive
	#include <netinet/ip_icmp.h>
	#include <netpacket/packet.h>
	#include <net/ethernet.h>
	#include <net/if_arp.h>

	#define net_read read
	#define net_write write
	#define net_close close
	#define neterr errno	
#endif


#ifndef WIN32
	//linux netlink socket
	#include <sys/ioctl.h>
	#include <sys/un.h>
	#include <linux/types.h>
	#include <linux/netlink.h>

	//linux framebuffer
	#include <sys/ioctl.h>
	#include <sys/mman.h>
	#include <asm/types.h>
	#include <linux/fb.h>

	//linux input
	//#include <linux/input.h>

	//linux disk utils
	#include <sys/mount.h>
	#include <sys/ioctl.h>
	#include <sys/statfs.h>
	#include <mntent.h>
	#include <linux/hdreg.h>
	#include <linux/fs.h>
#endif

/************************************************************************
线程相关
************************************************************************/
#ifdef WIN32
	#include <process.h>

	#define sem_t HANDLE
	#define sem_init(x,pShared,InitValue) *(x)=CreateSemaphore(NULL,InitValue,1000000,NULL)
	#define sem_destroy(x) (CloseHandle(*(x))==0?1:0)
	#define sem_wait(x) WaitForSingleObject(*(x),INFINITE)
	//#define sem_timedwait(x, y) WaitForSingleObject(*(x), ((y)->tv_sec*1000)+((y)->tv_nsec/1000))
	#define sem_trywait(x) ((WaitForSingleObject(*(x),0)==WAIT_OBJECT_0)?0:1)
	#define sem_post(x) ReleaseSemaphore(*(x),1,NULL)
	#define sem_time_wait(x, y) ((WaitForSingleObject(*(x),y)==WAIT_OBJECT_0)?0:1)	// 信号量超时等待, y表示等待的毫秒数

	#define pthread_mutex_t CRITICAL_SECTION
	#define pthread_mutex_init(x, y) InitializeCriticalSection(x)
	#define pthread_mutex_destroy(x) DeleteCriticalSection(x)
	#define pthread_mutex_lock(x) EnterCriticalSection(x)
	#define pthread_mutex_trylock(x) (TryEnterCriticalSection(x)?0:-1)
	#define pthread_mutex_unlock(x) LeaveCriticalSection(x)
#else
	#include <pthread.h>
	#include <semaphore.h>
	//注意下面的函数需要链接 -lpthread -lrt
#define sem_time_wait(x, y)		\
	(__extension__({		\
		timespec ts;		\
		int rv;			\
		clock_gettime(CLOCK_REALTIME, &ts);	\
		ts.tv_sec += (y / 1000 );		\
		ts.tv_nsec += ( y % 1000 ) * 1000;	\
		rv = sem_timedwait(x, &ts);		\
		rv;					\
	}))
#endif

/************************************************************************
用于debug的调试信息
************************************************************************/
#ifdef VERBOSE
#define print printf
#else
#define print
#endif

#define tprint printf
#ifdef WIN32
#define dprint printf
#else
#define dprint(fmt, args...) fprintf(stderr, "[%s,%d]" fmt, __FILE__, __LINE__, ## args);fflush(stdout)
#endif

#ifndef NODEBUG

#undef _assert

#ifdef WIN32
#define __PRINT_INFO(strErr, strFile, iLine)			\
	fprintf(stderr,						\
		"  >%s, assert failed (%s:%d)\n"		\
		"  >errno : %d, %s\n"				\
		"  >network last error : %d, last error : %d\n\n",\
		strErr, strFile, iLine,				\
		errno, strerror(errno),				\
		WSAGetLastError(), GetLastError())
#else
#define __PRINT_INFO(strErr, strFile, iLine)		\
	char bufptr[32];				\
	fprintf(stderr,					\
		"  >%s, assert failed (%s:%d)\n"	\
		"  >errno : %d, %s\n\n",		\
		strErr, strFile, iLine,			\
		errno, strerror_r(errno, bufptr, sizeof(bufptr)))
#endif

#define _assert(condition)	\
do {				\
	if ( !(condition) )	\
	{			\
		__PRINT_INFO("ERROR", __FILE__, __LINE__);\
		exit(-1);	\
	}			\
}while(0)

#define _warn(condition)	\
do {				\
	if ( !(condition) )	\
	{			\
		__PRINT_INFO("WARNING", __FILE__, __LINE__);\
		fflush(stderr);	\
	}			\
}while(0)


#define _assertex(condition, file, line)		\
do {							\
	if ( !(condition) )				\
	{						\
		__PRINT_INFO("ERROR", file, line);	\
		exit(-1);				\
	}						\
}while(0)

#define _warnex(condition, file, line)			\
do {							\
	if ( !(condition) )				\
	{						\
		__PRINT_INFO("WARNING", file, line);	\
		fflush(stderr);				\
	}						\
}while(0)


#else
#define _assert(condition)
#define _assertex(condition, file, line)
#define _warn(condition)
#define _warnex(condition, file, line)
#endif //NODEBUG

#define print_buf(p, len) do {				\
	int ii;						\
	for ( ii=0; ii<(len); ii++ ) {			\
		if ( (ii%16) == 0 ) { tprint("\n"); }	\
		if ( (ii%8) == 0 ) { tprint("  ");}	\
		tprint("%02x ", ((unsigned char *)(p))[ii]);\
	}						\
	tprint("\n");					\
} while(0)

#define print_str(p, len) do {				\
	int ii;						\
	for ( ii=0; ii<(len); ii++ ) {			\
		tprint("%c", ((char *)(p))[ii]  );	\
	}						\
	tprint("\n");					\
}while(0)

#define tprint_buf		print_buf
#endif //__LL_HEADERS_H__

