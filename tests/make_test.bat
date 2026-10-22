@echo off
set currCD=%CD%
set gccpath=
::set clangpath=
::set msvcpath=

::set gccpath="D:\Progs\IDE\MinGW\gcc-4.9.2-tdm\bin"
::set gccpath="D:\Progs\IDE\MinGW\gcc-10.3.0-tdm\bin"
::set gccpath="D:\Progs\IDE\MinGW\i686-13.1.0-posix-dwarf-msvcrt\bin"
::set gccpath="D:\Progs\IDE\MinGW\i686-15.2.0-posix-dwarf-msvcrt\bin"
::set clangpath="D:\Progs\IDE\MinGW\llvm-mingw-20250910-msvcrt-x86_64\bin"
::set clangpath="D:\Progs\IDE\MinGW\llvm-mingw-20190920-9.0.0-x86_64\bin"

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
set FNO_FLAGS_WH=%FNO_FLAGS% -fwhole-program
if "%COMPILER%"=="CLANG" (set "FNO_FLAGS_WH=%FNO_FLAGS% -fvisibility=hidden")
set OPT_FLAGS=-O2 -std=%STDCXX% %FNO_FLAGS_WH%

set DBG_LNK_FLAGS=%ARCH_FLAG% -lws2_32 -lpsapi -liphlpapi
::if "%COMPILER%"=="GCC" set DBG_LNK_FLAGS=%ARCH_FLAG% -nodefaultlibs -lws2_32 -lpsapi -liphlpapi -lsupc++ -lpthread -lmingw32 -lmingwex -lmsvcrt -lgcc -ladvapi32 -lkernel32
::if "%COMPILER%"=="CLANG" set DBG_LNK_FLAGS=%ARCH_FLAG% -nodefaultlibs -lws2_32 -lpsapi -liphlpapi -lpthread -lc++ -lclang_rt.builtins-i386 -lmingw32 -lmingwex -lmsvcrt -ladvapi32 -luser32 -lkernel32
if "%COMPILER%"=="CLANG" (set "DBG_LNK_FLAGS=-target i686-w64-mingw32 %DBG_LNK_FLAGS% -pthread")

set GCC_LIBS=-static-libgcc -static-libstdc++
set D_LNK_FLAGS=-s %GCC_LIBS% %DBG_LNK_FLAGS%
set LNK_FLAGS=-static %D_LNK_FLAGS%

set AUDIO_LIBS=-lwinmm
set IMAGE_LIBS=-lgdiplus -lgdi32 -lole32

set mode=
if "%~1"=="LIB" (set "mode=%~1")
if "%~1"=="LIB_DLL" (set "mode=%~1")
if "%~1"=="LIB_SO" (set "mode=LIB_DLL")
if "%~1"=="FAST" (set "mode=%~1")
if "%~1"=="SAN" (set "mode=%~1")
if "%~1"=="DBG" (set "mode=%~1")
if "%~1"=="SDBG" (set "mode=DBG")
if "%~1"=="GUI" (set "mode=%~1")
if "%~1"=="SGUI" (set "mode=GUI")
if "%~1"=="GL" (set "mode=%~1")
if "%~1"=="EXP" (set "mode=%~1")
if "%~1"=="NOCRT" (set "mode=%~1")
if "%~1"=="DLNK" (set "mode=%~1")
if "%1"=="DEF" (shift)
if not "%mode%"=="" (shift)

if "%~1"=="" (set fname=tests) else (set fname=%1)
set cppfile=%CD%\%fname%.cpp
set exefile=%CD%\%fname%.exe
set start_time=%TIME%

if not "%gccpath%"=="" (cd /d "%gccpath%")

if "%mode%"=="LIB" (
    echo Make via ncpp.a...
    %CBIN% -D NCPP_LIB_USE %WARN_FLAGS% %OPT_FLAGS% "%cppfile%" -o "%exefile%" -L"%currCD%\..\lib" -lncpp %LNK_FLAGS%
    if errorlevel 1 exit /b 1
)
if "%mode%"=="LIB_DLL" (
    echo Make via ncpp.dll...
    %CBIN% -D NCPP_LIB_USE %WARN_FLAGS% %OPT_FLAGS% "%cppfile%" -o "%exefile%"  -L"%currCD%\..\lib" -lncpp %D_LNK_FLAGS%
    if errorlevel 1 exit /b 1
)
if "%mode%"=="FAST" (
    echo Make with -O3 -march=native...
    %CBIN% %WARN_FLAGS% -O3 -std=%STDCXX% %FNO_FLAGS_WH% "%cppfile%" -o "%exefile%" -march=native -mtune=native -flto -funroll-loops -fipa-cp-clone -ffast-math -fomit-frame-pointer %LNK_FLAGS%
    if errorlevel 1 exit /b 1
)
if "%mode%"=="DBG" (
    echo Make debug -g -Og...
    %CBIN% %WARN_FLAGS% -g -Og -std=%STDCXX% %FNO_FLAGS_WH% "%cppfile%" -o "%exefile%" %DBG_LNK_FLAGS%
    if errorlevel 1 exit /b 1
)
if "%mode%"=="SAN" (
    echo Make using sanitizers...
    %CBIN% %WARN_FLAGS% -g -Og -std=%STDCXX% %FNO_FLAGS_WH% "%cppfile%" -o "%exefile%" %D_LNK_FLAGS% -fsanitize=address,undefined
    if errorlevel 1 exit /b 1
)
if "%mode%"=="GUI" (
    echo Make for GUI...
    %CBIN% -DUSE_GUI -DNOUSE_GL %WARN_FLAGS% %OPT_FLAGS% "%cppfile%" -o "%exefile%" %LNK_FLAGS% -lgdi32
    if errorlevel 1 exit /b 1
)
if "%mode%"=="GL" (
    echo Make for GL...
    %CBIN% -DUSE_GUI %WARN_FLAGS% %OPT_FLAGS% "%cppfile%" -o "%exefile%" %LNK_FLAGS% -lopengl32 -lgdi32
    if errorlevel 1 exit /b 1
)
if "%mode%"=="EXP" (
    echo Make for Experimental...
    %CBIN% -DUSE_GUI -DUSE_EXPERIMENTAL %WARN_FLAGS% %OPT_FLAGS% "%cppfile%" -o "%exefile%" %LNK_FLAGS% -lopengl32 %IMAGE_LIBS% %AUDIO_LIBS%
    if errorlevel 1 exit /b 1
)
if "%mode%"=="NOCRT" (
    echo Make NOCRT...
    %CBIN% -DNCPP_NOCRT %WARN_FLAGS% -O2 -std=%STDCXX% %FNO_FLAGS% "%cppfile%" -o "%exefile%" %ARCH_FLAG% -Wl,--entry=%WL_ENTRY_FNAME% -s -static -nostdlib -lkernel32
    if errorlevel 1 exit /b 1
)
if "%mode%"=="DLNK" (
    echo Make DLNK ^(Dynamic Lnk^)...
    %CBIN% %WARN_FLAGS% %OPT_FLAGS% "%cppfile%" -o "%exefile%" %D_LNK_FLAGS%
    if errorlevel 1 exit /b 1
)
if "%mode%"=="" (
    echo Make ^(default^)...
    %CBIN% %WARN_FLAGS% %OPT_FLAGS% "%cppfile%" -o "%exefile%" %LNK_FLAGS%
    if errorlevel 1 exit /b 1
)

if not "%gccpath%"=="" (cd /d "%currCD%")

:: === Time calc ===
set end_time=%TIME%
set /A startMS=((1%start_time:~0,2%-100)*3600000)+((1%start_time:~3,2%-100)*60000)+(1%start_time:~6,2%*1000)+(1%start_time:~9,2%)
set /A endMS=((1%end_time:~0,2%-100)*3600000)+((1%end_time:~3,2%-100)*60000)+(1%end_time:~6,2%*1000)+(1%end_time:~9,2%)
set /A durationMS=endMS-startMS
set /A durationS=durationMS/1000
set /A durationMS=durationMS%%1000
echo Build Time: %durationS%.%durationMS% sec.
exit /b 0