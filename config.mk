##############################################################################################
# $ gcc -c func.c -o func.o : 编译生成obj文件
# $ gcc -c exe -o func.o   :  编译生成可执行文件
# $ ar rc libfunc.a func.o :  编译生成静态库
# $ gcc -shared -o libxxx.so aaa.o bbb.o
#
# -I : 指定搜索目录
# -L : 指定库的路径
# -l : 指定静态库的名称
#
# -g : 生成调试信息
# −Wall : 打开警告开关
# -O : 默认优化
#
# $< : 依赖目标中的第一个目标名字，如果依赖目标是以模式（即“%”）定义的，则表示符合模式的一系列文件集，其是一个一个去出来的
# $@ : 规则中的目标文件集
# $^ : 规则中所有的依赖目标的集合，用空格分隔
# $? : 所有比目标新的依赖目标的集合
#
# wildcard : 扩展通配符
# dir : 取目录函数			$(dir src/foo.c hacks)	: 返回值是“src/ ./”
# notdir :   取文件函数		$(notdir src/foo.c hack)	: 返回值是“foo.c hack”
# patsubst : 替换通配符 		$(patsubst <pattern>,<replacement>,<text>)
# subst ： 字符串替换符		$(subst <from>;,<to>;,<text>;) : 把字串<text>;中的<from>;字符串替换成<to>;
# addprefix : 加前缀 		$(addprefix src/,foo bar) ： 返回值是“src/foo src/bar”
# addsuffix : 加后缀			$(addsuffix .c,foo bar)	： 返回值是“foo.c bar.c”
# if :	条件语句				$(if <condition>;,<then-part>;,<else-part>;) 
# filter : 过滤函数			$(filter-out $(in),$(objects))	: 以模式过滤字符串中的单词，保留符合模式的单词
# filter-out : 反过滤函数 	$(filter-out $(ex),$(objects)) ： 以模式过滤字符串中的单词，去除符合模式的单词
# 
# include : 包含文件，
# -include : 忽略由于包含文件不存在或者无法创建时的错误提示 （同sinclude）
# 
#  = 是最基本的赋值 (将整个makefile展开后，再决定变量的值)
# := 是覆盖之前的值 (变量的值决定于它在makefile中的位置，而不是整个makefile展开后的最终值)
# ?= 是如果没有被赋值过就赋予等号后面的值
# += 是添加等号后面的值
# 
#  
# 
#  
##############################################################################################

##############################################################################################
# 配置文件config.mk
# 		1. 指定编译器及编译选项，可以选择交叉编译器
#		2. 指定了源代码目录及目标文件目录，这些目录由主Makefile给出，或者在make时给出
#		3. 编译c或c++源代码的规则
##############################################################################################



ifeq ($(platform), qcom)
	# cross compile... 
	CROSS_COMPILE = arm-linux-androideabi-
	CFLAGS += --sysroot=$(SYSDIR) -pie -fPIE -D PLATFORM_QCOM
	LDFLAGS += --sysroot=$(SYSDIR) -static
endif

ifeq ($(platform), mtk)
	CROSS_COMPILE = arm-linux-androideabi-
	CFLAGS += --sysroot=$(SYSDIR) -pie -fPIE -D PLATFORM_MTK
	LDFLAGS += --sysroot=$(SYSDIR) -static
endif

ifeq ($(platform), hw)
	CROSS_COMPILE = arm-hisiv100nptl-linux-
	CFLAGS += -D PLATFORM_HW
	LDFLAGS += -lpthread
endif

ifeq ($(platform), ubuntu)
	CROSS_COMPILE = 
	CFLAGS += -D PLATFORM_UBUNTU
	LDFLAGS += -lpthread
endif


CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar

ARFLAGS = cr
CFLAGS +=  -I$(INCDIR) -Wall 

LDFLAGS +=  -L$(LIBDIR) -levent_core -levent_pthreads -lmxml -lnmea -ldebug -lfifo  #-lpthread -lcb -llist  #-ljpeg -lspeex -lsqlite3 -static -lpthread

DEFS = 
CFLAGS += $(DEFS)


ifeq ($(debug), y)
CFLAGS += -g
endif

#ifeq ($(gb2014), y)
#CFLAGS += -D GB2014_PROTOCOL
#endif

#ifeq ($(thirdlibs),y)
#CFLAGS += -D JPEG_LIB_SUPPORT
#LDFLAGS += -ljpeg  

#CFLAGS += -D SQLITE3_LIB_SUPPORT
#LDFLAGS += -lsqlite3

#CFLAGS += -D SPEEX_LIB_SUPPORT
#LDFLAGS += -lspeex
#endif

#ifeq ($(coverage),y)
#CFLAGS += -g3 -O0 -fprofile-arcs -ftest-coverage
#LDFLAGS += -lgcov 
#endif

#ifeq ($(cmockery),y)
#CFLAGS += -D UNIT_TESTING -I /usr/local/include/google
#LDFLAGS += -L /usr/local/lib -lcmockery
#endif

# export to other Makefile 
export CC
export CXX
export AR

export CFLAGS
export ARFLAGS
export LDFLAGS


# make all .c or .cpp 
%.o: %.c
	@echo "Compling: " $(addsuffix .c, $(basename $(notdir $@)))
	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$@

%.o: %.cpp
	@echo "Compling: " $(addsuffix .cpp, $(basename $(notdir $@)))
	@$(CXX) $(CFLAGS) -c $< -o $(OBJDIR)/$@

