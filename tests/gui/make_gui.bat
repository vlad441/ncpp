@echo off
set currpath=%CD%
set gccpath=

set STDCXX=c++98
if "%~1"=="C++98" (set "STDCXX=c++98" & shift)
if "%~1"=="C++11" (set "STDCXX=c++11" & shift)
if "%~1"=="C++17" (set "STDCXX=c++17" & shift)
if "%~1"=="C++23" (set "STDCXX=c++23" & shift)

set ARCH_FLAG=-m32
if "%~1"=="x64" (set "ARCH_FLAG=-m64" & shift)
if "%~1"=="x32" (set "ARCH_FLAG=-m32" & shift)

set WARN_FLAGS=-Wall -Wno-misleading-indentation
set OPT_FLAGS=-O2 -std=%STDCXX% -fno-exceptions -fno-rtti -fwhole-program
set D_LNK_FLAGS=-s %ARCH_FLAG% -lws2_32 -lpsapi -liphlpapi -lopengl32 -lgdiplus -lgdi32 -lole32
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