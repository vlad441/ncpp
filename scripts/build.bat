@echo off
call _params.bat
::call _gcc_obj_make.bat
if "%gccpath%"=="" (set gccar="ar") else (set gccar=%gccpath%\ar)
set cppfile=%currCD%\..\src\ncpp.cpp
set INCL_DIR=%currCD%\..\include
set LIB_DIR=%currCD%\..\lib
if "%start_time%"=="" (set start_time=%TIME%)
if not exist %LIB_DIR% mkdir %LIB_DIR%

::cd ../lib/obj
::echo Packing obj files to archive libncpp.lib...
::%gccar% rcs ../libncpp.a base_print.o base_utils0.o base_Array.o base_String.o base_dtos.o base_Buffer.o base_HashMap.o base_utils.o base_Date.o
::cd %currCD%

echo Build static-bundle lib...
if "%gccpath%"=="" (set gccpath="g++") else (cd /d %gccpath%)
g++ -D LIB_DIR_BUILD %WARN_FLAGS% %OPT_FLAGS% -c %cppfile% -o %LIB_DIR%\ncpp.o -I "%INCL_DIR%"
ar rcs %LIB_DIR%\libncpp-bundle.a %LIB_DIR%\ncpp.o
del %LIB_DIR%\ncpp.o

echo Build dynamic .dll lib...
g++ -D LIB_DIR_BUILD -shared %WARN_FLAGS% %OPT_FLAGS% %cppfile% -o %LIB_DIR%\ncpp.dll %D_LNK_FLAGS% -Wl,--out-implib,"%LIB_DIR%\ncpp.dll.lib"
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