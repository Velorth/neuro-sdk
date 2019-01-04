rmdir /Q /S "%NEUROSDK_64%"
rmdir /Q /S "%NEUROSDK_86%"
call generate-all.cmd nopause
call build-all.cmd nopause
if NOT "%1"=="nopause" pause