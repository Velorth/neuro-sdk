call build_sdk_platform_config.cmd armeabi-v7a Debug nopause
call build_sdk_platform_config.cmd armeabi-v7a Release nopause
call build_sdk_platform_config.cmd arm64-v8a Debug nopause
call build_sdk_platform_config.cmd arm64-v8a Release nopause
call build_sdk_platform_config.cmd x86 Debug nopause
call build_sdk_platform_config.cmd x86 Release nopause
call build_sdk_platform_config.cmd x86_64 Debug nopause
call build_sdk_platform_config.cmd x86_64 Release nopause
if NOT "%1"=="nopause" pause
