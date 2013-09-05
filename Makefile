NDK_ROOT:=$(HOME)/workspace/android-ndk-r8e
NDK_SYSROOT:=--sysroot=$(NDK_ROOT)/platforms/android-14/arch-arm/
NDK_TCPATH:=$(NDK_ROOT)/toolchains/arm-linux-androideabi-4.7/prebuilt/linux-x86_64/bin/arm-linux-androideabi-
HOST_TARGET:=`uname`-`uname -m`
REMOTE_TATGET:=arm-androideabi-4.7

.PHONY: smdp host remote targets android-smdp

targets : smdp android-smdp

smdp : host remote

host :
	cd src && make -e OBJDIR=./$(HOST_TARGET)

remote :
	cd src && make -e OBJDIR=./$(REMOTE_TATGET) SYSROOT=$(NDK_SYSROOT) TCPATH=$(NDK_TCPATH)

clean :
	cd src && make -e OBJDIR=./$(HOST_TARGET) clean
	cd src && make -e OBJDIR=./$(REMOTE_TATGET) SYSROOT=$(NDK_SYSROOT) TCPATH=$(NDK_TCPATH) clean
	cd android && ndk-build clean

android-smdp :
	cd android && ndk-build
