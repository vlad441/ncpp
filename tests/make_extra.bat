@echo off
set currCD=%CD%
set gccpath=

set ARCH_FLAG=-m32
if "%~1"=="x64" (set "ARCH_FLAG=-m64" & shift)
if "%~1"=="x32" (set "ARCH_FLAG=-m32" & shift)

set WARN_FLAGS=-Wall -Wno-misleading-indentation
set FNO_FLAGS=-fno-exceptions -fno-rtti
set OPT_FLAGS=-O2 -std=c++98 %FNO_FLAGS%
set LNK_FLAGS=-s -static -nostdlib -lkernel32

if "%~1"=="" (set fname=extra-small) else (set fname=%1)
set cppfile=%CD%\%fname%.cpp
set exefile=%CD%\%fname%.exe
set start_time=%TIME%

if "%gccpath%"=="" (set "gccpath=g++") else (cd /d %gccpath%)

if "%ARCH_FLAG%"=="-m64" echo Make NOCRT x64... & g++ %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %ARCH_FLAG% -Wl,--entry=_start %LNK_FLAGS%
if "%ARCH_FLAG%"=="-m32" echo Make NOCRT x32... & g++ %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %ARCH_FLAG% -Wl,--entry=__start %LNK_FLAGS%

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