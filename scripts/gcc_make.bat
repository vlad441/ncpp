@echo off
call _params.bat
if not "%1"=="" ( set cppfile=%currCD%\..\%1.cpp 
    set exefile=%currCD%\..\%1.exe )
set start_time=%TIME%

if "%gccpath%"=="" (set gccpath="g++") else (cd /d %gccpath%)
g++ -Wall %cppfile% -o %exefile% %gcc_flags% %lnk_flags% %lgui_flags%
::g++ -Wall %cppfile% -o %exefile% %gcc_flags% -static -s -m32 -L"%currpath%\..\lib" -lncpp
::g++ -O3 -std=c++98 -march=native -flto -funroll-loops -ffast-math -funsafe-math-optimizations -fno-trapping-math -fno-math-errno -fomit-frame-pointer %cppfile% -o %exefile%
cd /d "%currCD%"

:: === Time calc ===
set end_time=%TIME%
set /A startMS=((1%start_time:~0,2%-100)*3600000)+((1%start_time:~3,2%-100)*60000)+(1%start_time:~6,2%*1000)+(1%start_time:~9,2%)
set /A endMS=((1%end_time:~0,2%-100)*3600000)+((1%end_time:~3,2%-100)*60000)+(1%end_time:~6,2%*1000)+(1%end_time:~9,2%)
set /A durationMS=endMS-startMS
set /A durationS=durationMS/1000
set /A durationMS=durationMS%%1000
echo Build Time: %durationS%.%durationMS% sec.
pause