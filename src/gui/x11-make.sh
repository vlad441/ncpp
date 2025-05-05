#!/bin/bash
currpath=$(pwd)
cppfile="$currpath/gui-testapp.cpp"
exefile="$currpath/gui-testapp"
start_time=$(date +%s)

# Переход к папке с компилятором C++
#cd "~/gcc-4.9.2/bin"
#g++ -Wall "$cppfile" -o "$exefile" -static -s -O2 -std=c++98
g++4.9 -Wall "$cppfile" -o "$exefile" -s -Os -std=c++98 -lX11 -lxcb -lXau -lXdmcp -lpthread -lGL
#cd "$currpath"

# === Time ===
echo "Build time: $(($(date +%s) - start_time)) sec."

