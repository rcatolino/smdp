targets : smdp android-smdp

smdp :
	cd src && make

android-smdp :
	cd android && ndk-build
