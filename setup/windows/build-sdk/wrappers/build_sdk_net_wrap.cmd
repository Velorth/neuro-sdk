call build_sdk_net_wrap_platform_config.cmd "Any CPU" Debug nopause
call build_sdk_net_wrap_platform_config.cmd "Any CPU" Release nopause
call build_sdk_net_wrap_platform_config.cmd x64 Debug nopause
call build_sdk_net_wrap_platform_config.cmd x64 Release nopause
if NOT "%1"=="nopause" pause
