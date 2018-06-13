

#ifndef __COMM_FUN_H__
#define __COMM_FUN_H__

#include "ll_headers.h"

int select_check_read(int fd, int timeout);
int WaitVerIsTrue(int *ver);
long Mscount(unsigned long Count);

u64 usec_count(u64 usec);
u64 msec_count(u64 msec);
void trim_str(char *str);
int memtoint(void *bufptr, int buflen);
double memtodouble(void *bufptr, int buflen);

// ���ڴ��в����ڴ洮
char *mem_find(char *bufptr, int buflen, const char *what, int len);

typedef int (*command_callback)(int argc, char **argv);
int command_fgets_and_parse(command_callback cmdcb);
int command_parse(char *cmd, command_callback cmdcb);

int file2buf(char *bufptr, int buflen, int *actual_sz, char *filename);	// ����һ���ļ���buf
int buf2file(char *bufptr, int buflen, char *filename);					// д��buf��һ���ļ�
char *filetobuf(char *filename, int *buflen); 							// ע��Է���ֵfree
int bufadd2file(char *bufptr, int buflen, char *filename);				// ����buf��һ���ļ�
int file_exist(const char *path);


time_t mktime_ex(int year, int month, int day, int hour, int minute, int second);

//"2009-4-12 12:00:00"
time_t str2time(char *strtime);
char *time2str_ex(time_t tm, OUT char *bufptr, int buflen);
//"H20120727-103504P1A1P0.avi"
time_t filename_createtime(char *filename);
//��ȡstat��mtime, ������
time_t filename_endtime(char *fullpath);
//�����ļ��ĳߴ�
int filename_size(char *fullpath);
//����Ŀ¼���ļ��ĸ���, ���� '.' �� '..'
int file_num_of_dir(char *dir);
//�жϺ�׺, str - �ļ���, endsWidth - ��׺,  ignoreCase - ���Դ�Сд
int filename_endwith(const char* str, const char* endsWith, int ignoreCase);
//��ӡʱ��
void print_time(time_t tm);

//ʱ�����
#define TIME_START {			\
	struct timeval __stime, __etime;\
	gettimeofday(&__stime, NULL);

#define TIME_END(__stattime)		\
	gettimeofday(&__etime, NULL);	\
	__stattime += (__etime.tv_sec - __stime.tv_sec) * 1000 +  (__etime.tv_usec - __stime.tv_usec) / 1000; }

int DebugToFile(const char *fmt, ...);
int DebugToFileBin(u8 *bufptr, int buflen);
#ifdef WIN32
#define wprint	DebugToFile
#define DEBUG_FULL_PATH "./dbg.txt"
#else
#define wprint(fmt, args...)	DebugToFile("[%s/%d]" fmt, __FILE__, __LINE__,  ## args)
#define DEBUG_FULL_PATH "/tmp/dbg.txt"
#endif


#ifdef WIN32
int gettimeofday(struct timeval *tst, void *);

struct dirent {
	char		d_name[FILENAME_MAX];
	struct stat	d_st;
};
typedef struct {
	char			fullpath[FILENAME_MAX];
	HANDLE			handle;
	WIN32_FIND_DATA		filedata;
	struct dirent		result;
}DIR;
DIR *opendir(const char *fullpath);
int closedir(DIR *dir);
struct dirent *readdir(DIR *dir);

int ffs(int a);
#endif
int ffz(int a);

struct comma_str_t
{
	char	*val;
	int	val_len;
};
// �ַ����ķָ�
// ���ַ����������Զ��ŷָ��Ĵ�
// �ָ�����delimiterȷ��
int separate_str(char *bufptr, int buflen, const char *delimiter, OUT struct comma_str_t comma_arr[], int arrlen);


//BCD��ת��
u16 bin2bcd2(u16 bin);
u16 bcd2bin2(u16 bcd);

u32 bin2bcd4(u32 bin);
u32 bcd2bin4(u32 bcd);

// ʱ���ת��
int bb_time_to_bcd(time_t tm, OUT u8 bcd[6]);
time_t bb_time_from_bcd(u8 bcd[6]);
int bb_time_to_cur_day_minute(time_t tim);

//�����ַ������ֵ
unsigned char get_data_xor(char *pdata, int len);

#endif //__COMM_FUN_H__




