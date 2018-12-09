cmake --clean "%SDK_SOURCE%\wrappers\cross-platform\c\build\%1"
cmake --build "%SDK_SOURCE%\wrappers\cross-platform\c\build\%1" --target INSTALL --config %2
if NOT "%3"=="nopause" pause
