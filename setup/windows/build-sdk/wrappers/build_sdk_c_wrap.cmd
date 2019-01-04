call build_sdk_c_wrap_platform_config.cmd x86 Debug nopause
call build_sdk_c_wrap_platform_config.cmd x86 Release nopause
call build_sdk_c_wrap_platform_config.cmd x64 Debug nopause
call build_sdk_c_wrap_platform_config.cmd x64 Release nopause
if NOT "%1"=="nopause" pause
