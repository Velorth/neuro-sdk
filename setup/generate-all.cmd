cd build-sdk
	call generate_sdk_projects.cmd nopause
	cd wrappers
		call generate_sdk_c_wrap_projects.cmd nopause
		cd ..
	cd ..
if NOT "%1"=="nopause" pause