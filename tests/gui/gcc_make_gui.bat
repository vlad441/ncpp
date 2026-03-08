@echo off
set currpath=%CD%
set gccpath=

set WARN_FLAGS=-Wall -Wno-misleading-indentation
set OPT_FLAGS=-O2 -std=c++98 -fno-exceptions -fno-rtti
set D_LNK_FLAGS=-s -m32 -lws2_32 -lpsapi -liphlpapi -lopengl32 -lgdi32
set LNK_FLAGS=-static %D_LNK_FLAGS%

if "%~1"=="" (set fname=gui-testapp) else (set fname=%1)
set cppfile=%CD%\%fname%.cpp
set exefile=%CD%\%fname%.exe
set start_time=%TIME%

if "%gccpath%"=="" (set "gccpath=g++") else (cd /d %gccpath%)
g++ -DUSE_GUI -DNOUSE_OBJECT -DNOUSE_CRYPTO -DNOUSE_NET %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %LNK_FLAGS%
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