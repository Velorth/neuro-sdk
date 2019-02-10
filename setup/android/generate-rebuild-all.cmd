rmdir /Q /S "%NEUROSDK%\android"
call generate_sdk_projects.cmd nopause
call build_sdk.cmd nopause
if NOT "%1"=="nopause" pause