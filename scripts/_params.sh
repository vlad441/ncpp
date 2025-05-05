#!/bin/bash
#gccpath="~/gcc-4.9.2/bin"
#clangpath="~/clang-3.5.2/bin"
currCD=$(pwd)
cppfile="$currCD/../test.cpp"
exefile="$currCD/../test"
start_time=$(date +%s)

gcc_flags="-O2 -std=c++98"
#static_lnk_flags="-static -s -Wl,--whole-archive -lpthread -Wl,--no-whole-archive"
#partial_static_flags="-lGL -Wl,-static -s -static-libgcc -static-libstdc++ -lc -Wl,--whole-archive -lpthread -Wl,--no-whole-archive"
lnk_flags="-static -s -Wl,--whole-archive -lpthread -Wl,--no-whole-archive"
dll_lnk_flags="-s -pthread -lGL"
#lgui_flags="-Wl,-Bdynamic -lGL -Wl,-Bstatic"

#gui_lnk_flags="-lGL -pthread -Wl,-static -s -static-libgcc -static-libstdc++ -lc"
# Create the mixed static/shared library
#gcc -nostdlib -shared  -o libbnc2.so   -L.  c.o    -lb  -Wl,-static -la
