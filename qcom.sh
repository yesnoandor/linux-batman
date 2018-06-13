export NDK="/home/wenyu/Android/Sdk/ndk-bundle"
export SYSROOT="$NDK/platforms/android-19/arch-arm"
export QCOM_PATH="$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/"
#export LIB_PATH="$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/lib/"
export LIB_PATH="$SYSROOT/usr/lib/"

			
					 
#export ANDROID_GCC="$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc --sysroot=$SYSROOT -pie -fPIE"
export PATH=$PATH:$QCOM_PATH:$LIB_PATH

 
# ./configure --host=arm-hisiv100nptl-linux --prefix=/home/wenyu/apps/linux-batman/libs/src/libevent/arm_libs --disable-openssl
# ./configure --host=arm-linux-androideabi --prefix=./arm_libs --target=arm-linux CFLAGS="--sysroot=$SYSROOT -pie -fPIE" --disable-openssl  --disable-libevent-regress --disable-samples
# make
# make install

# --disable-openssl \
#	--disable-libevent-regress \	--> undefined reference to '__builtin_bswap16'  undefined reference to 'arc4random_addrandom'
#	--disable-samples \		--> undefined reference to '__builtin_bswap16'  undefined reference to 'arc4random_addrandom'

#	--disable-shared \
#	--disable-malloc-replacement \
#	--disable-thread-support \




#./configure --host=arm-linux-androideabi --prefix=/usr --target=arm-linux CFLAGS="--sysroot=$SYSROOT -pie -fPIE" LFLAGS="--sysroot=$SYSROOT" 
