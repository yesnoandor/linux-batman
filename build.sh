#!/bin/bash


readonly usage="\

Usage: 
	$0 [<platform>]
	$0

Examples:
	$0 ubuntu
	$0 qcom
	$0 mtk
	$0 hw
	$0 all
	$0 clean
"

# 显示帮助
function show_help()
{
	echo -e "$usage"
}

function build_ubuntu()
{
	echo "build ubuntu::++++++++++"
	
	make clean
	sleep 1
	
	core=$(grep 'processor' /proc/cpuinfo | sort -u | wc -l)
	make platform=ubuntu -j$core
	
	echo "build ubuntu::----------"
}

function build_qcom()
{
	echo "build qcom::++++++++++"
	
	make clean
	sleep 1

	NDK="/home/wenyu/Android/Sdk/ndk-bundle"
	QCOM_PATH="$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/"
	PATH=$PATH:$QCOM_PATH

	core=$(grep 'processor' /proc/cpuinfo | sort -u | wc -l)
	
	make platform=qcom -j$core
	
	echo "build qcom::----------"
}


function build_mtk()
{
	echo "build mtk::++++++++++"

	make clean
	sleep 1

	NDK="/home/wenyu/Android/Sdk/ndk-bundle"
	MTK_PATH="$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/"
	PATH=$PATH:$MTK_PATH

	core=$(grep 'processor' /proc/cpuinfo | sort -u | wc -l)
	
	make platform=mtk -j$core

	echo "build mtk::----------"
}

function build_hw()
{
	echo "build hw::++++++++++"

	make clean
	sleep 1
	
	core=$(grep 'processor' /proc/cpuinfo | sort -u | wc -l)
	make platform=hw -j$core
	
	echo "build hw::----------"
}

function build_all()
{
	echo "build all::++++++++++"

	build_ubuntu
	build_qcom
	
	echo "build all::----------"
}

# 解析编译命令
function build_parse()
{
	echo "build parse::++++++++++"
	echo "platform = $1"

	case $1 in
		ubuntu)
			build_ubuntu
			;;
		qcom)
			build_qcom
			;;
		mtk)
			build_mtk
			;;
		hw)
			build_hw
			;;
		all)
			build_all
			;;
		*)
			echo "wrong platform: $i"
			show_help
			exit 1
			;;
	esac
	
	echo "build parse::----------"
}


set -x




# 获取顶层目录
TOP_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo "TOP DIR = $TOP_DIR"

# 获取参数列表
echo "arg num = $#"
echo "args = $@"


if [ $# -eq 1 ]; then
	build_parse $1

	exit 0
fi

show_help
exit 1

