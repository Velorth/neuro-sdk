rmdir /Q /S "%SDK_SOURCE%\wrappers\cross-platform\c\build"
cmake -G "Visual Studio 15 2017 Win64" -H"%SDK_SOURCE%\wrappers\cross-platform\c" -B"%SDK_SOURCE%\wrappers\cross-platform\c\build\x64"
cmake -G "Visual Studio 15 2017" -H"%SDK_SOURCE%\wrappers\cross-platform\c" -B"%SDK_SOURCE%\wrappers\cross-platform\c\build\x86"
if NOT "%1"=="nopause" pause
