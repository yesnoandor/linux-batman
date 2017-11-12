##############################################################################################
# $ gcc -c func.c -o func.o : 编译生成obj文件
# $ gcc -c exe -o func.o   :  编译生成可执行文件
# $ ar rc libfunc.a func.o :  编译生成静态库
#
# -I :     
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
#  = 是最基本的赋值
# := 是覆盖之前的值
# ?= 是如果没有被赋值过就赋予等号后面的值
# += 是添加等号后面的值
# 
#  Makefile 
# 
#  
##############################################################################################

###############################################
# rules.mk : 依赖规则文件，主要是产生depend文件，
#			 有了depend文件，当修改了头文件后，包含该头文件的源文件都会被重新编译。
###############################################

CURDIR = $(shell pwd)

#$(warning SRC_C:$(SRC_C))
#$(warning SRC_CPP:$(SRC_CPP))

_depend: $(CURDIR)/.depend

$(CURDIR)/.depend: $(TOPDIR)/config.mk $(SRC_C) $(SRC_CPP)
	@rm -f $@
	@for f in $(SRC_C); do \
	g=`echo $$f | sed -e 's/\(.*\)\.\w/\1.o/'`; \
	$(CC) -MM $(CFLAGS) -E -MQ $(CURDIR)/$$g $$f >> $@ ; \
	done
	@for f in $(SRC_CPP); do \
		g=`echo $$f | sed -e 's/\(.*\)\...\w/\1.o/'`; \
		$(CC) -MM $(CFLAGS) -E -MQ $(CURDIR)/$$g $$f >> $@ ; \
	done
