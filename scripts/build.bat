@echo off
set currCD=%CD%
set gccpath=
::set clangpath=
::set msvcpath=

set COMPILER=GCC
set CBIN=g++
if "%~1"=="GCC"   (set "COMPILER=GCC"   & set "CBIN=g++" & shift)
::if "%~1"=="CLANG" (set "COMPILER=CLANG" & set "CBIN=i686-w64-mingw32-clang++" & set "gccpath=%clangpath%" & shift)
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
::if "%ARCH_FLAG%"=="-m64" if "%COMPILER%"=="CLANG" (set "ARCH_FLAG=-target x86_64-w64-mingw32 %ARCH_FLAG%")
::if "%ARCH_FLAG%"=="-m32" if "%COMPILER%"=="CLANG" (set "ARCH_FLAG=-target i686-w64-mingw32 %ARCH_FLAG%")

set WARN_FLAGS=-Wall -Wno-misleading-indentation
set FNO_FLAGS=-fno-exceptions -fno-rtti
set OPT_FLAGS=-O2 -std=%STDCXX% %FNO_FLAGS%
::-fno-default-inline -fno-visibility-inlines-hidden -fkeep-inline-functions -fno-inline

set DBG_LNK_FLAGS=%ARCH_FLAG% -lws2_32 -lpsapi -liphlpapi
if "%COMPILER%"=="CLANG" (set "DBG_LNK_FLAGS=-target i686-w64-mingw32 %DBG_LNK_FLAGS% -pthread")

set GCC_LIBS=-static-libgcc -static-libstdc++
set D_LNK_FLAGS=-s %GCC_LIBS% %DBG_LNK_FLAGS%
set LNK_FLAGS=-static %D_LNK_FLAGS%

set AUDIO_LIBS=-lwinmm
set IMAGE_LIBS=-lgdiplus -lgdi32 -lole32

set mode=
if "%~1"=="LIB" (set "mode=%~1")
if "%1"=="DEF" (shift)
if not "%mode%"=="" (shift)

::call _params.bat
::call _gcc_obj_make.bat
::if "%gccpath%"=="" (set ARBIN="ar") else (set ARBIN=%gccpath%\ar)

set cppfile=%currCD%\..\src\ncpp.cpp
set exefile=%currCD%\%fname%.exe
set INCL_DIR=%currCD%\..\include
set LIB_DIR=%currCD%\..\lib
if not exist %LIB_DIR% mkdir %LIB_DIR%

set start_time=%TIME%

if not "%gccpath%"=="" (cd /d "%gccpath%")

:: cd ../lib/obj
:: echo --- Packing obj files to archive libncpp.lib...
:: ar rcs ../libncpp.a base_print.o base_utils0.o base_Array.o base_String.o base_dtos.o base_Buffer.o base_HashMap.o base_utils.o base_Date.o
:: cd %currCD%

echo Build static-bundle lib...
if "%gccpath%"=="" (set gccpath="g++") else (cd /d %gccpath%)
%CBIN% -D LIB_DIR_BUILD %WARN_FLAGS% %OPT_FLAGS% -c %cppfile% -o %LIB_DIR%\ncpp.o -I "%INCL_DIR%" -Wl,--version-script=export.map
ar rcs %LIB_DIR%\libncpp-bundle.a %LIB_DIR%\ncpp.o
del %LIB_DIR%\ncpp.o

echo Build dynamic .dll lib...
%CBIN% -D LIB_DIR_BUILD -shared %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %LIB_DIR%\ncpp.dll %D_LNK_FLAGS% -Wl,--out-implib,"%LIB_DIR%\ncpp.dll.lib"
cd /d "%currCD%"

if not "%gccpath%"=="" (cd /d "%currCD%")

:: === Time calc ===
set end_time=%TIME%
set /A startMS=((1%start_time:~0,2%-100)*3600000)+((1%start_time:~3,2%-100)*60000)+(1%start_time:~6,2%*1000)+(1%start_time:~9,2%)
set /A endMS=((1%end_time:~0,2%-100)*3600000)+((1%end_time:~3,2%-100)*60000)+(1%end_time:~6,2%*1000)+(1%end_time:~9,2%)
set /A durationMS=endMS-startMS
set /A durationS=durationMS/1000
set /A durationMS=durationMS%%1000
echo Build Lib Time: %durationS%.%durationMS% sec.
pause