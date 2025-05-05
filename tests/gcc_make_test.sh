#!/bin/bash
#gccpath="~/gcc-4.9.2/bin"
currpath=$(pwd)
[ -z "$1" ] && fname=tests || fname=$1
cppfile="$currpath/$fname.cpp"
exefile="$currpath/$fname"
start_time=$(date +%s)

gcc_flags="-O2 -std=c++98"
#lnk_flags="-static -s"
lnk_flags="-static -s -Wl,--whole-archive -lpthread -Wl,--no-whole-archive"

if [ -z "$gccpath" ]; then gccbin=g++; else cd "$gccpath" && gccbin="./g++"; fi
$gccbin -Wall "$cppfile" -o "$exefile" $gcc_flags $lnk_flags
#$gccbin -O3 -std=c++98 -march=native -flto -funroll-loops -ffast-math -funsafe-math-optimizations -fno-trapping-math -fno-math-errno -fomit-frame-pointer %cppfile% -o %exefile%
#g++4.9 -Wall "$cppfile" -o "$exefile" $gcc_flags $lnk_flags
[ -z "$gccpath" ] && gccbin=g++ || cd "$currpath"

# === Time ===
echo "Build time: $(($(date +%s) - start_time)) sec."

