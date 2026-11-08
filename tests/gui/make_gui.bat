@echo off
set currCD=%CD%
set gccpath=
::set clangpath=
::set msvcpath=

set COMPILER=GCC
set CBIN=g++
if "%~1"=="GCC"   (set "COMPILER=GCC"   & set "CBIN=g++" & shift)
if "%~1"=="CLANG" (set "COMPILER=CLANG" & set "CBIN=clang++" & set "gccpath=%clangpath%" & shift)
if "%~1"=="MSVC"  (set "COMPILER=MSVC"  & set "CBIN=cl" & set "gccpath=%msvcpath%" & shift)

set STDCXX=c++98
if "%~1"=="C++98" (set "STDCXX=c++98" & shift)
if "%~1"=="C++11" (set "STDCXX=c++11" & shift)
if "%~1"=="C++17" (set "STDCXX=c++17" & shift)
if "%~1"=="C++23" (set "STDCXX=c++23" & shift)

set ARCH_FLAG=-m32
set WL_ENTRY_FNAME=__start
if "%~1"=="x64" (set "ARCH_FLAG=-m64" & set "WL_ENTRY_FNAME=_start" & shift)
if "%~1"=="x32" (set "ARCH_FLAG=-m32" & set "WL_ENTRY_FNAME=__start" & shift)

set WARN_FLAGS=-Wall -Wno-misleading-indentation
set FNO_FLAGS=-fno-exceptions -fno-rtti
set FNO_FLAGS_WH=%FNO_FLAGS% -fwhole-program
::if "%COMPILER%"=="CLANG" (set "FNO_FLAGS_WH=%FNO_FLAGS% -flto=full -fvisibility=hidden -Wl,/OPT:REF -Wl,/OPT:ICF")
set OPT_FLAGS=-O2 -std=%STDCXX% %FNO_FLAGS_WH%

set DBG_LNK_FLAGS=%ARCH_FLAG% -lws2_32 -lpsapi -liphlpapi
if "%COMPILER%"=="CLANG" (set "DBG_LNK_FLAGS=-target i686-w64-mingw32 %DBG_LNK_FLAGS% -pthread")

set GCC_LIBS=-static-libgcc -static-libstdc++
set D_LNK_FLAGS=-s %GCC_LIBS% %DBG_LNK_FLAGS%
set LNK_FLAGS=-static %D_LNK_FLAGS%

set AUDIO_LIBS=-lwinmm
set IMAGE_LIBS=-lgdiplus -lgdi32 -lole32

if "%~1"=="" (set fname=gui-testapp) else (set fname=%1)
set cppfile="%CD%\%fname%.cpp"
set exefile="%CD%\%fname%.exe"
set start_time=%TIME%

if not "%gccpath%"=="" (cd /d "%gccpath%" && set CBIN=.\%CBIN%.exe)
echo Make for GUI (Experimental)... & %CBIN% -DUSE_GUI -DUSE_EXPERIMENTAL %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %LNK_FLAGS% -lopengl32 %IMAGE_LIBS% %AUDIO_LIBS%
if not "%gccpath%"=="" (cd /d "%currCD%")

:: === Time calc ===
set end_time=%TIME%
set /A startMS=((1%start_time:~0,2%-100)*3600000)+((1%start_time:~3,2%-100)*60000)+(1%start_time:~6,2%*1000)+(1%start_time:~9,2%)
set /A endMS=((1%end_time:~0,2%-100)*3600000)+((1%end_time:~3,2%-100)*60000)+(1%end_time:~6,2%*1000)+(1%end_time:~9,2%)
set /A durationMS=endMS-startMS
set /A durationS=durationMS/1000
set /A durationMS=durationMS%%1000
echo Build Time: %durationS%.%durationMS% sec.
pause