###############################################
# $ gcc -c func.c -o func.o : 编译生成obj文件
# $ gcc -c exe -o func.o   :  编译生成可执行文件
# $ ar rc libfunc.a func.o :  编译生成静态库
#
#    
# -L : 指定库的路径
# -l : 指定静态库的名称
#
# -g : 生成调试信息
# −Wall : 打开警告开关
# -O : 默认优化
# -I : 
#
# $< : 依赖目标中的第一个目标名字，如果依赖目标是以模式（即“%”）定义的，则表示符合模式的一系列文件集，其是一个一个去出来的
# $@ : 规则中的目标文件集
# $^ : 规则中所有的依赖目标的集合，用空格分隔
# $? : 所有比目标新的依赖目标的集合
#
# wildcard : 扩展通配符
# notdir :   去除路径
# patsubst : 替换通配符
###############################################
ifeq ($(platform), qcom)
CROSS_COMPILE = arm-linux-androideabi-
endif

ifeq ($(platform), hw)
CROSS_COMPILE = arm-hisiv100nptl-linux-
endif

ifeq ($(platform), ubuntu)
CROSS_COMPILE =
endif

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar

ifeq ($(platform), qcom)
CFLAGS += --sysroot=/home/wenyu/Android/Sdk/ndk-bundle/platforms/android-19/arch-arm -pie -fPIE
endif

LIB = libcb.a

CFLAGS += -g -Wall

SRC = $(wildcard *.c)
OBJS := $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@


$(LIB):  $(OBJS)
	$(AR) rc $@ $< 

all: $(LIB)

clean:
	-rm -f $(LIB) *.elf *.o


