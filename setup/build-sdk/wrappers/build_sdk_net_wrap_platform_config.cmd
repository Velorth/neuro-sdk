msbuild "%SDK_SOURCE%\wrappers\windows\neurosdk\neurosdk.sln" /t:Rebuild /p:Configuration=%2 /p:Platform=%1 /v:m
if NOT "%3"=="nopause" pause
