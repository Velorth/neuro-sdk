rmdir /Q /S "%SDK_SOURCE%\core\build\windows"
cmake -G "Visual Studio 15 2017 Win64" -H"%SDK_SOURCE%\core" -B"%SDK_SOURCE%\core\build\windows\x64"
cmake -G "Visual Studio 15 2017" -H"%SDK_SOURCE%\core" -B"%SDK_SOURCE%\core\build\windows\x86"
if NOT "%1"=="nopause" pause
