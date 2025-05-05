@echo off
call _params.bat
::call _gcc_obj_make.bat
if "%gccpath%"=="" (set gccar="ar") else (set gccar=%gccpath%\ar)
set cppfile=%currCD%\..\src\ncpp.cpp
set ncpp_include=%currCD%\..\include
set ncpp_lib=%currCD%\..\lib
if "%start_time%"=="" (set start_time=%TIME%)
if not exist %ncpp_lib% mkdir %ncpp_lib%

::cd ../lib/obj
::echo Packing in archive libncpp.lib...
::%gccar% rcs ../libncpp.lib ncpp-base.o system_fs.o ncpp-system.o net_socket.o net_http.o ncpp-thread.o net_server.o
::cd %currCD%

echo Build static-bundle lib...
if "%gccpath%"=="" (set gccpath="g++") else (cd /d %gccpath%)
g++ -Wall -c %cppfile% -o %ncpp_lib%\ncpp.o %gcc_flags% -I "%ncpp_include%"
ar rcs %ncpp_lib%\libncpp-bundle.lib %ncpp_lib%\ncpp.o
del %ncpp_lib%\ncpp.o

echo Build dynamic .dll lib...
g++ -shared %cppfile% -o %ncpp_lib%\ncpp.dll %gcc_flags% %dll_lnk_flags% -Wl,--out-implib,"%ncpp_lib%\ncpp.dll.lib"
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