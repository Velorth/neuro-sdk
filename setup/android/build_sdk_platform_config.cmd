cmake --clean "%SDK_SOURCE%\core\build\android\%1\%2"
cmake --build "%SDK_SOURCE%\core\build\android\%1\%2" --target install
if NOT "%3"=="nopause" pause
