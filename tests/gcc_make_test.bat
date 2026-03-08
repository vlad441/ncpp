@echo off
set currpath=%CD%
set gccpath=
::set gccpath="D:\Progs\IDE\MinGW\gcc-4.9.2-tdm\bin"
::set gccpath="D:\Progs\IDE\MinGW\gcc-10.3.0-tdm\bin"
::set gccpath="D:\Progs\IDE\MinGW\i686-13.1.0-posix-dwarf-msvcrt\bin"
::set gccpath="D:\Progs\IDE\MinGW\llvm-mingw-20250910-msvcrt-x86_64\bin"
::set gccpath="D:\Progs\IDE\MinGW\llvm-mingw-20190920-9.0.0-x86_64\bin"

set WARN_FLAGS=-Wall -Wno-misleading-indentation
set OPT_FLAGS=-O2 -std=c++98 -fno-exceptions -fno-rtti
set D_LNK_FLAGS=-s -m32 -lws2_32 -lpsapi -liphlpapi
set LNK_FLAGS=-static %D_LNK_FLAGS%

set mode=
if "%~1"=="LIB" (set "mode=%~1")
if "%~1"=="LIB_DLL" (set "mode=%~1")
if "%~1"=="LIB_SO" (set "mode=LIB_DLL")
if "%~1"=="FAST" (set "mode=%~1")
if "%~1"=="SAN" (set "mode=%~1")
if "%~1"=="GUI" (set "mode=%~1")
if "%~1"=="SGUI" (set "mode=GUI")
if "%~1"=="GL" (set "mode=%~1")
if "%1"=="DEF" (shift)
if not "%mode%"=="" (shift)

if "%~1"=="" (set fname=tests) else (set fname=%1)
set cppfile=%CD%\%fname%.cpp
set exefile=%CD%\%fname%.exe
set start_time=%TIME%

if "%gccpath%"=="" (set "gccpath=g++") else (cd /d %gccpath%)

if "%mode%"=="LIB" echo Make via ncpp.a... & g++ -D NCPP_LIB_USE %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% -L"%currpath%\..\lib" -lncpp %LNK_FLAGS%
if "%mode%"=="LIB_DLL" echo Make via ncpp.dll... & g++ -D NCPP_LIB_USE %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile%  -L"%currpath%\..\lib" -lncpp %D_LNK_FLAGS%
if "%mode%"=="FAST" echo Make with -O3 -march=native... & g++ -O3 -std=c++98 -march=native -fno-exceptions -fno-rtti %cppfile% -o %exefile% -flto -funroll-loops -fipa-cp-clone -ffast-math -fomit-frame-pointer %LNK_FLAGS%
if "%mode%"=="SAN" echo Make using sanitizers... & g++ %WARN_FLAGS% -g -Og -std=c++98 -fno-exceptions -fno-rtti %cppfile% -o %exefile% %D_LNK_FLAGS% -fsanitize=address,undefined
if "%mode%"=="GUI" echo Make for GUI... & g++ -DUSE_GUI -DNOUSE_GL %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %LNK_FLAGS% -lgdi32
if "%mode%"=="GL" echo Make for GL... & g++ -DUSE_GUI %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %LNK_FLAGS% -lopengl32 -lgdi32
if "%mode%"=="" echo Make (default)... & g++ %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %LNK_FLAGS%

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