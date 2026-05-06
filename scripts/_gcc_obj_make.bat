@echo off
call _params.bat
if "%gccpath%"=="" (set gccbin=g++) else (set gccbin=%gccpath%\g++)
set cppdir=%currCD%\..
set INCL_DIR=%cppdir%\include
set SRC_DIR=%cppdir%\src
set OBJ_DIR=%cppdir%\lib\obj
set NCPP_H=%SRC_DIR%\ncppdef.hpp

if not exist "%cppdir%\lib" mkdir "%cppdir%\lib"
if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"

::== Base module ==
::call :compile ncpp-base
call :compile base/print
call :compile base/utils0
call :compile base/Array "-include base/utils0.hpp"
call :compile base/String "-include base/Array.hpp"
call :compile base/dtos "-include base/utils0.hpp -include base/String.hpp"
::call :compile base/Buffer.cpp "-include base/utils0.hpp -include base/String.hpp" //Depends: utils0, Array, String, dtos
::call :compile base/HashMap.cpp //Depends: Array
::== ==
::call :compile sys/utils
::call :compile sys/fs "-I""%INCL_DIR%"" -include ""%NCPP_Hbase%"""

goto :eof
:compile
set "fcpp=%~1"& set "fname=%~1"
set "fname=%fname:/=_%"
if not exist "%cppdir%\lib\obj\%fcpp%.o" (
    echo Compiling %fcpp%.cpp...
    %gccbin% -D NCPP_LIB_BUILD -include "%NCPP_H%" -I "%INCL_DIR%" %~2 %OPT_FLAGS% -c "%SRC_DIR%\%fcpp%.cpp" -o "%OBJ_DIR%\%fname%.o"
)
goto :eof