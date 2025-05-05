::set gccpath="D:\Progs\IDE\MinGW\gcc-4.9.2-tdm\bin"
set currCD=%CD%
set cppfile=%CD%\..\test.cpp
set exefile=%CD%\..\test.exe

set gcc_flags=-O2 -std=c++98
set lnk_flags=-static -s -m32 -lws2_32 -lpsapi -liphlpapi
set dll_lnk_flags=-s -m32 -lws2_32 -lpsapi -liphlpapi
::set lgui_flags="-lopengl32"