chcp 65001
set RMDIR=rd /s /q
set RM=del /f /q

%RMDIR% src\experimental
%RMDIR% src\stdcpp
%RMDIR% tests\gui
%RM% tests\alloctest.cpp
%RM% tests\alloctest2.cpp
%RM% tests\image_test.cpp
%RM% tests\base-test.cpp
%RM% tests\cpp-clean.cpp
%RM% tests\extra-small.cpp
%RM% tests\http-test.cpp
%RM% tests\make_extra.bat
%RM% tests\malloc-debug.h
%RM% tests\musl-gcc.sh
%RM% tests\pcm-test.cpp
%RM% tests\perf-tests.cpp
%RM% tests\stdcpp_test.cpp
%RM% tests\ws_test.txt
%RM% tests\make_test1.bat
%RM% tests\my-asm.js
%RM% tests\*.txt
%RM% tests\*.exe
%RM% tests\*.log
%RM% tests\tests
%RM% index
%RM% "nani deska.txt"
%RM% Roadmap.txt
%RM% "Изменения.txt"
pause