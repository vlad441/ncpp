@echo off
set currCD=%CD%
::set gccpath=
set clangpath="D:\Progs\IDE\MinGW\llvm-mingw-20190920-9.0.0-x86_64\bin"
::set msvcpath=

set COMPILER=GCC
set CBIN=g++
if "%~1"=="GCC"   (set "COMPILER=GCC"   & set "CBIN=g++" & shift)
if "%~1"=="CLANG" (set "COMPILER=CLANG" & set "CBIN=clang++" & set "gccpath=%clangpath%" & shift)
if "%~1"=="MSVC"  (set "COMPILER=MSVC"  & set "CBIN=cl" & set "gccpath=%msvcpath%" & shift)

set ARCH_FLAG=-m32
set WL_ENTRY_FNAME=__start
if "%~1"=="x64" (set "ARCH_FLAG=-m64" & set "WL_ENTRY_FNAME=_start" & shift)
if "%~1"=="x32" (set "ARCH_FLAG=-m32" & set "WL_ENTRY_FNAME=__start" & shift)

::Info: https://stackoverflow.com/questions/42022132/how-to-create-tiny-pe-win32-executables-using-mingw
::https://stackoverflow.com/questions/16266795/programming-language-that-makes-the-smallest-executables/16270204#16270204

set WARN_FLAGS=-Wall -Wno-misleading-indentation
set FNO_FLAGS=-fno-exceptions -fno-rtti
set OPT_FLAGS=-Os -std=c++98 %FNO_FLAGS%
::set OPT_FLAGS=-O2 -std=c++98 %FNO_FLAGS%
set LNK_FLAGS=-s -static -nostdlib -lkernel32
::-Wl,--section-alignment,16,--file-alignment,16 -Wl,-N -Wl,--omagic 
::-Wl,--merge-section -Wl,--merge-sec-contents=.rdata=.text -Wl,--merge-sec-contents=.data=.text
::strip --strip-all --remove-section=.comment --remove-section=.note hello.exe

set mode=
if "%~1"=="NOCRT" (set "mode=%~1")
if "%~1"=="EXTRA" (set "mode=%~1")
if "%~1"=="CLANG" (set "mode=%~1")
if "%1"=="DEF" (set "mode=NOCRT")
if not "%mode%"=="" (shift)

if "%~1"=="" (set fname=extra-small) else (set fname=%1)
set cppfile=%CD%\%fname%.cpp
set exefile=%CD%\%fname%.exe
set start_time=%TIME%

::GCC: 776 bytes | CLANG: 660 bytes

if not "%gccpath%"=="" (cd /d %gccpath%)

if "%mode%"=="NOCRT" echo Make NOCRT (DEF g++ size) %ARCH_FLAG%... & %CBIN% %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %ARCH_FLAG% -Wl,--entry=%WL_ENTRY_FNAME% %LNK_FLAGS%
if "%mode%"=="EXTRA" echo Make NOCRT (EXTRA_v2 g++ Small) %ARCH_FLAG%... & %CBIN% %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %ARCH_FLAG% -Wl,--entry=%WL_ENTRY_FNAME% %LNK_FLAGS% -Wl,--section-alignment,1,--file-alignment,1

if not "%mode%"=="CLANG" if not "%COMPILER%"=="CLANG" (goto :skip_clang)
echo Make NOCRT (CLANG lld Small) %ARCH_FLAG%... & %CBIN% %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %ARCH_FLAG% %LNK_FLAGS% -target i686-w64-mingw32 -Wl,-e,_start -Wl,/FILEALIGN:1 -Wl,/ALIGN:1 -Wl,/MERGE:.rdata=.text -Wl,/MERGE:.data=.text -Wl,/FIXED
:skip_clang

if "%mode%"=="" if "%COMPILER%"=="GCC" echo Make NOCRT (EXTRA Small) %ARCH_FLAG%... & %CBIN% %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %exefile% %ARCH_FLAG% -Wl,--entry=%WL_ENTRY_FNAME% %LNK_FLAGS% -Wl,--section-alignment,16,--file-alignment,16 

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