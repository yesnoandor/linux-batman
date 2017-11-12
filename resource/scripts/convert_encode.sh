#! /bin/bash

src_enc=utf-8
dst_enc=gb2312

src_dir=$1
dst_dir=$1"_"$dst_enc

function convert(){
    for file in `ls -1 $1`
    do
        if [ -d $1"/"$file ]
        then
            convert $1"/"$file
        else
            if [ "${file##*.}"x = "cpp"x ]||[ "${file##*.}"x = "h"x ];
            then
                orig_dir=$(pwd)
                cd $1
                #echo "cpp or h"
                filename=${file%.*}
                suffix=${file##*.}
                dst_file=$filename"_"$dst_enc"."$suffix
                iconv -c -f $src_enc -t $dst_enc $file > $dst_file
                rm $file
                cd $orig_dir
            else
                :
            fi
            #echo $1"/"$file
        fi
    done
}

if [ ! -n "$1" ] ;then
    echo "Error! Please input '$0 your_folder'!"
    exit 1
fi

cd $1
cd ..
if [ -d "${dst_dir}" ]; then
	if [ -d "${dst_dir}_BAK" ]; then
	  echo "`date "+%Y-%m-%d %H:%M:%S"` Remove bakup folder ${dst_dir}_BAK..."
	  rm -rf ${dst_dir}_BAK
	fi
  echo "`date "+%Y-%m-%d %H:%M:%S"` Bakup folder ${dst_dir} to ${dst_dir}_BAK..."
  cp -r ${dst_dir} ${dst_dir}_BAK
  rm -rf ${dst_dir}
fi
echo "`date "+%Y-%m-%d %H:%M:%S"` Create folder ${dst_dir}..."
cp -r $1 ${dst_dir}
echo "`date "+%Y-%m-%d %H:%M:%S"` Start conversion for '$1' from $src_enc to $dst_enc..."
convert ${dst_dir}
echo "`date "+%Y-%m-%d %H:%M:%S"` Done!"