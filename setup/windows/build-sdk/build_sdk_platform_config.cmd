cmake --clean "%SDK_SOURCE%\core\build\%1"
cmake --build "%SDK_SOURCE%\core\build\%1" --target INSTALL --config %2
if NOT "%3"=="nopause" pause
