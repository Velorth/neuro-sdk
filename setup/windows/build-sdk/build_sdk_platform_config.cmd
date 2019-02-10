cmake --clean "%SDK_SOURCE%\core\build\windows\%1"
cmake --build "%SDK_SOURCE%\core\build\windows\%1" --target INSTALL --config %2 -- /nologo /verbosity:minimal
if NOT "%3"=="nopause" pause
