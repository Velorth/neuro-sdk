rmdir /Q /S "%NEUROSDK%"
call generate-all.cmd nopause
call build-all.cmd nopause
if NOT "%1"=="nopause" pause