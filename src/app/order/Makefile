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
# patsubst : 替换通配符 	$(patsubst <pattern>,<replacement>,<text>)
# subst ： 字符串替换符	$(subst <from>;,<to>;,<text>;) : 把字串<text>;中的<from>;字符串替换成<to>;
# addprefix : 加前缀 	$(addprefix src/,foo bar) ： 返回值是“src/foo src/bar”
#
# include : 包含文件，
# -include : 忽略由于包含文件不存在或者无法创建时的错误提示 （同sinclude）
# 
#  = 是最基本的赋值
# := 是覆盖之前的值
# ?= 是如果没有被赋值过就赋予等号后面的值
# += 是添加等号后面的值
###############################################

include $(TOPDIR)/config.mk

# 设置当前目录的特定的编译选项 
CFLAGS += 

CURDIR = $(shell pwd)

# 设定需要排除编译的目录
# EXDIRS += 

# 查找出当前目录下的所有子目录
SUBDIRS := $(addprefix $(shell pwd)/, $(filter-out $(EXDIRS),$(shell ls -l | grep ^d | awk '{print $$9}')))

# 得到当前目录下的所有C文件列表
SRC_C := $(wildcard *.c)
# 得到当前目录下所有C文件对应的OBJ文件列表
OBJ_C := $(SRC_C:.c=.o)

# 得到当前目录下的所有C++文件列表
SRC_CPP := $(wildcard *.cpp)
# 得到当前目录下所有C++文件对应的OBJ文件列表
OBJ_CPP := $(SRC_CPP:.cpp=.o)

#OBJ_C   := $(addprefix $(OBJDIR)/,$(patsubst %.c,%.o,$(SRC_C)))
#OBJ_CPP := $(addprefix $(OBJDIR)/,$(patsubst %.cpp,%.o,$(SRC_CPP)))



all:$(CURDIR)/.depend $(OBJ_C) $(OBJ_CPP)
	@echo $(SUBDIRS)	
	@for n in $(SUBDIRS); \
	do \
		if [ -f Makefile ]; then \
			$(MAKE) -C $$n || exit "$$?"; \
		fi; \
	done

clean:
	@echo $(SUBDIRS)
	@for n in $(SUBDIRS); do $(MAKE) -C $$n clean; done
	rm -f $(CURDIR)/.depend

.PHONY: all clean

#########################################################################

include $(TOPDIR)/rules.mk

sinclude $(CURDIR)/.depend

#########################################################################








