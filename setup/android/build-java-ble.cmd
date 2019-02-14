rmdir /S /Q %SDK_SOURCE%\core\build\android\classes
mkdir %SDK_SOURCE%\core\build\android\classes
javac -d %SDK_SOURCE%\core\build\android\classes -classpath D:\android_sdk\platforms\android-28\android.jar;%SDK_SOURCE%\core\build\android\classes %SDK_SOURCE%\core\ble\android\java\com\neuromd\bleconnection\device\*.java
if errorlevel 1 (
    pause
    exit
)
javac -d %SDK_SOURCE%\core\build\android\classes -classpath D:\android_sdk\platforms\android-28\android.jar;%SDK_SOURCE%\core\build\android\classes %SDK_SOURCE%\core\ble\android\java\com\neuromd\bleconnection\exceptions\*.java
if errorlevel 1 (
    pause
    exit
)
javac -d %SDK_SOURCE%\core\build\android\classes -classpath D:\android_sdk\platforms\android-28\android.jar;%SDK_SOURCE%\core\build\android\classes %SDK_SOURCE%\core\ble\android\java\com\neuromd\bleconnection\scanner\*.java
if errorlevel 1 (
    pause
    exit
)
rmdir /S /Q %SDK_SOURCE%\core\build\android\lib
mkdir %SDK_SOURCE%\core\build\android\lib
jar cf %SDK_SOURCE%\core\build\android\lib\ble-neurosdk.jar %SDK_SOURCE%\core\build\android\classes\com\neuromd\bleconnection\device\* %SDK_SOURCE%\core\build\android\classes\com\neuromd\bleconnection\exceptions\* %SDK_SOURCE%\core\build\android\classes\com\neuromd\bleconnection\scanner\*
if errorlevel 1 (
    pause
    exit
)
if NOT "%3"=="nopause" pause