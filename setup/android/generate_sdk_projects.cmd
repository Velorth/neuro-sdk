rmdir /Q /S "%SDK_SOURCE%\core\build\android"
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="%SDK_SOURCE%\setup\android\toolchain\android-armeabi-v7a-toolchain.cmake" -DCMAKE_BUILD_TYPE=Debug -H"%SDK_SOURCE%\core" -B"%SDK_SOURCE%\core\build\android\armeabi-v7a\Debug"
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="%SDK_SOURCE%\setup\android\toolchain\android-arm64-v8a-toolchain.cmake" -DCMAKE_BUILD_TYPE=Debug -H"%SDK_SOURCE%\core" -B"%SDK_SOURCE%\core\build\android\arm64-v8a\Debug"
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="%SDK_SOURCE%\setup\android\toolchain\android-x86-toolchain.cmake" -DCMAKE_BUILD_TYPE=Debug -H"%SDK_SOURCE%\core" -B"%SDK_SOURCE%\core\build\android\x86\Debug"
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="%SDK_SOURCE%\setup\android\toolchain\android-x86_64-toolchain.cmake" -DCMAKE_BUILD_TYPE=Debug -H"%SDK_SOURCE%\core" -B"%SDK_SOURCE%\core\build\android\x86_64\Debug"

cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="%SDK_SOURCE%\setup\android\toolchain\android-armeabi-v7a-toolchain.cmake" -DCMAKE_BUILD_TYPE=Release -H"%SDK_SOURCE%\core" -B"%SDK_SOURCE%\core\build\android\armeabi-v7a\Release"
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="%SDK_SOURCE%\setup\android\toolchain\android-arm64-v8a-toolchain.cmake" -DCMAKE_BUILD_TYPE=Release -H"%SDK_SOURCE%\core" -B"%SDK_SOURCE%\core\build\android\arm64-v8a\Release"
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="%SDK_SOURCE%\setup\android\toolchain\android-x86-toolchain.cmake" -DCMAKE_BUILD_TYPE=Release -H"%SDK_SOURCE%\core" -B"%SDK_SOURCE%\core\build\android\x86\Release"
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="%SDK_SOURCE%\setup\android\toolchain\android-x86_64-toolchain.cmake" -DCMAKE_BUILD_TYPE=Release -H"%SDK_SOURCE%\core" -B"%SDK_SOURCE%\core\build\android\x86_64\Release"
if NOT "%1"=="nopause" pause