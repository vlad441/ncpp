@echo off
set currCD=%CD%
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
set D_LNK_FLAGS=-s %ARCH_FLAG% -lws2_32 -lpsapi -liphlpapi
set LNK_FLAGS=-static %D_LNK_FLAGS%

set mode=
if "%~1"=="LIB" (set "mode=%~1")
if "%~1"=="LIB_DLL" (set "mode=%~1")
if "%~1"=="LIB_SO" (set "mode=LIB_DLL")
if "%~1"=="FAST" (set "mode=%~1")
if "%~1"=="SAN" (set "mode=%~1")
if "%~1"=="DBG" (set "mode=%~1")
if "%~1"=="GUI" (set "mode=%~1")
if "%~1"=="SGUI" (set "mode=GUI")
if "%~1"=="GL" (set "mode=%~1")
if "%~1"=="EXP" (set "mode=%~1")
if "%1"=="DEF" (shift)
if not "%mode%"=="" (shift)

if "%~1"=="" (set fname=example) else (set fname=%1)
set cppfile=%CD%\..\%fname%.cpp
set exefile=%CD%\..\%fname%.exe
set start_time=%TIME%

if "%gccpath%"=="" (set "gccpath=g++") else (cd /d %gccpath%)

if "%mode%"=="LIB" echo Make via ncpp.a... & g++ -D NCPP_LIB_USE %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% -L"%currCD%\..\lib" -lncpp %LNK_FLAGS%
if "%mode%"=="LIB_DLL" echo Make via ncpp.dll... & g++ -D NCPP_LIB_USE %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile%  -L"%currCD%\..\lib" -lncpp %D_LNK_FLAGS%
if "%mode%"=="FAST" echo Make with -O3 -march=native... & g++ -O3 -std=%STDCXX% -march=native -fno-exceptions -fno-rtti %cppfile% -o %exefile% -flto -funroll-loops -fipa-cp-clone -ffast-math -fomit-frame-pointer %LNK_FLAGS%
if "%mode%"=="SAN" echo Make using sanitizers... & g++ %WARN_FLAGS% -g -Og -std=%STDCXX% -fno-exceptions -fno-rtti %cppfile% -o %exefile% %D_LNK_FLAGS% -fsanitize=address,undefined
if "%mode%"=="DBG" echo Make debug ver -g -Og... & g++ %WARN_FLAGS% -g -Og -std=%STDCXX% -fno-exceptions -fno-rtti %cppfile% -o %exefile% %D_LNK_FLAGS%
if "%mode%"=="GUI" echo Make for GUI... & g++ -DUSE_GUI -DNOUSE_GL %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %LNK_FLAGS% -lgdi32
if "%mode%"=="GL" echo Make for GL... & g++ -DUSE_GUI %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %LNK_FLAGS% -lopengl32 -lgdi32
if "%mode%"=="EXP" echo Make for Experimental... & g++ -DUSE_GUI -DUSE_EXPERIMENTAL %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %LNK_FLAGS% -lopengl32 -lgdiplus -lgdi32 -lole32 -lwinmm
if "%mode%"=="" echo Make (default)... & g++ %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %LNK_FLAGS%

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