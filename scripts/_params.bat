::set gccpath="D:\Progs\IDE\MinGW\gcc-4.9.2-tdm\bin"

set currCD=%CD%
set cppfile=%CD%\..\example.cpp
set exefile=%CD%\..\example.exe

set ext_flags=-fno-exceptions -fno-rtti -Wno-misleading-indentation

set WARN_FLAGS=-Wall -Wno-misleading-indentation
set OPT_FLAGS=-O2 -std=c++98 -fno-exceptions -fno-rtti
set LIBS=-lws2_32 -lpsapi -liphlpapi
set D_LNK_FLAGS=-s -m32 %LIBS%
set LNK_FLAGS=-static %D_LNK_FLAGS%