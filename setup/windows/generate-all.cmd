cd build-sdk
	call generate_sdk_projects.cmd nopause
    cd ..
if NOT "%1"=="nopause" pause