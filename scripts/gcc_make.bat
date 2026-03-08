@echo off
call _params.bat
if not "%1"=="" ( set cppfile=%currCD%\..\%1.cpp 
    set exefile=%currCD%\..\%1.exe )
set start_time=%TIME%

if "%gccpath%"=="" (set gccpath="g++") else (cd /d %gccpath%)
g++ %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %LNK_FLAGS%
cd /d "%currCD%"

:: ======= Time calc =======
set end_time=%TIME%
set /A startMS=((1%start_time:~0,2%-100)*3600000)+((1%start_time:~3,2%-100)*60000)+(1%start_time:~6,2%*1000)+(1%start_time:~9,2%)
set /A endMS=((1%end_time:~0,2%-100)*3600000)+((1%end_time:~3,2%-100)*60000)+(1%end_time:~6,2%*1000)+(1%end_time:~9,2%)
set /A durationMS=endMS-startMS
set /A durationS=durationMS/1000
set /A durationMS=durationMS%%1000
echo Build Time: %durationS%.%durationMS% sec.
pause