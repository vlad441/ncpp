@echo off
set currpath=%CD%
set cppfile=%CD%\gui-testapp.cpp
set exefile=%CD%\gui-testapp.exe
set start_time=%TIME%

:: Переход к папке с компилятором C++
cd /d "D:\Progs\IDE\MinGW\gcc-4.9.2-tdm\bin"
@echo on
g++ -Wall %cppfile% -o %exefile% -static -s -O2 -std=c++98 -m32 -lws2_32 -lopengl32 -lgdi32
@echo off
cd /d "%currpath%"

:: === Time calc ===
set end_time=%TIME%
set /A startMS=((1%start_time:~0,2%-100)*3600000)+((1%start_time:~3,2%-100)*60000)+(1%start_time:~6,2%*1000)+(1%start_time:~9,2%)
set /A endMS=((1%end_time:~0,2%-100)*3600000)+((1%end_time:~3,2%-100)*60000)+(1%end_time:~6,2%*1000)+(1%end_time:~9,2%)
set /A durationMS=endMS-startMS
set /A durationS=durationMS/1000
set /A durationMS=durationMS%%1000
echo Build Time: %durationS%.%durationMS% sec.
pause