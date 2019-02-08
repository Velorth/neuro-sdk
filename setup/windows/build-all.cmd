cd build-sdk
	call build_sdk.cmd nopause
	cd wrappers
		call build_sdk_net_wrap.cmd nopause
		cd ..
	cd ..
if NOT "%1"=="nopause" pause