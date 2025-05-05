@echo off
call _params.bat
if "%gccpath%"=="" (set gccbin=g++) else (set gccbin=%gccpath%\g++)
set cppdir=%currCD%\..
set ncpp_include=%cppdir%\include
set ncpp_src=%cppdir%\src
set ncpp_obj=%cppdir%\lib\obj
set NCPP_H=%ncpp_src%\ncpp.cpp.h
set NCPP_Hbase=%ncpp_include%\ncpp-base.h

if not exist "%cppdir%\lib" mkdir "%cppdir%\lib"
if not exist "%ncpp_obj%" mkdir "%ncpp_obj%"

call :compile ncpp-base
call :compile system/fs "-I""%ncpp_include%"" -include ""%NCPP_Hbase%"""
::call :compile ncpp-system
::call :compile net/socket
::call :compile net/http
::call :compile experimantal/ncpp-thread
::call :compile net/server
::call :compile ncpp-object
::call :compile ncpp-bigint
::call :compile ncpp-crypto
::call :compile gui/gui

goto :eof
:compile
set "fcpp=%~1"& set "fname=%~1"
set "fname=%fname:/=_%"
if not exist "%cppdir%\lib\%fcpp%.o" (
    echo Compiling %fcpp%.cpp...
    %gccbin% -include "%NCPP_H%" %~2 -c "%ncpp_src%\%fcpp%.cpp" -o "%ncpp_obj%\%fname%.o" %gcc_flags%
)
goto :eof