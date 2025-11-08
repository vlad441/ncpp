@echo off
set currpath=%CD%
set gccpath="D:\Progs\IDE\MinGW\gcc-4.9.2-tdm\bin"
::set gccpath="D:\Progs\IDE\MinGW\gcc-10.3.0-tdm\bin"
::set gccpath="D:\Progs\IDE\MinGW\i686-13.1.0-posix-dwarf-msvcrt\bin"
::set gccpath="D:\Progs\IDE\MinGW\llvm-mingw-20250910-msvcrt-x86_64\bin"
::set gccpath="D:\Progs\IDE\MinGW\llvm-mingw-20190920-9.0.0-x86_64\bin"

set gcc_flags=-O2 -std=c++98
set lnk_libs=-lws2_32 -lpsapi -liphlpapi
::set lnk_libs=-lws2_32 -lpsapi -liphlpapi -lopengl32
set lnk_flags=-static -s -m32 %lnk_libs%
set dll_lnk_flags=-s -m32 %lnk_libs%
set ext_flags=-fno-exceptions -fno-rtti -Wno-misleading-indentation

set mode=
if "%~1"=="LIB" (set "mode=%~1")
if "%~1"=="LIB_DLL" (set "mode=%~1")
if "%~1"=="LIB_SO" (set "mode=LIB_DLL")
if "%~1"=="FAST" (set "mode=%~1")
if "%~1"=="SAN" (set "mode=%~1")
if "%1"=="DEF" (shift)
if not "%mode%"=="" (shift)

if "%~1"=="" (set fname=tests) else (set fname=%1)
set cppfile=%CD%\%fname%.cpp
set exefile=%CD%\%fname%.exe
set start_time=%TIME%

if "%gccpath%"=="" (set "gccpath=g++") else (cd /d %gccpath%)

if "%mode%"=="LIB" echo Make via ncpp.a... & g++ -D NCPP_LIB_USE -Wall %cppfile% -o %exefile% %gcc_flags% -L"%currpath%\..\lib" -lncpp %lnk_flags% %ext_flags%
if "%mode%"=="LIB_DLL" echo Make via ncpp.dll... & g++ -D NCPP_LIB_USE -Wall %cppfile% -o %exefile% %gcc_flags% -L"%currpath%\..\lib" -lncpp %dll_lnk_flags% %ext_flags%
if "%mode%"=="FAST" echo Make with -O3 -march=native... & g++ -O3 -std=c++98 -march=native %cppfile% -o %exefile% -flto -funroll-loops -fipa-cp-clone -ffast-math -fomit-frame-pointer %lnk_flags% %ext_flags%
if "%mode%"=="SAN" echo Make using sanitizers... & g++ -Wall %cppfile% -o %exefile% -g -Og -std=c++98 %dll_lnk_flags% -fsanitize=address,undefined %ext_flags%
if "%mode%"=="" echo Make (default)... & g++ -Wall %cppfile% -o %exefile% %gcc_flags% %lnk_flags% %ext_flags%
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