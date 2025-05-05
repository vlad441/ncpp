#!/bin/bash
source ./_params.sh
if [ $1 ]; then cppfile="$currCD/../$1.cpp"; exefile="$currCD/../$1"; fi
start_time=$(date +%s)

if [ -z "$gccpath" ]; then gccbin=g++; else cd "$gccpath" && gccbin="./g++"; fi
$gccbin -Wall "$cppfile" -o "$exefile" $gcc_flags $lnk_flags
#$gccbin -O3 -std=c++98 -march=native -flto -funroll-loops -ffast-math -funsafe-math-optimizations -fno-trapping-math -fno-math-errno -fomit-frame-pointer %cppfile% -o %exefile%
#g++4.9 -Wall "$cppfile" -o "$exefile" $gcc_flags $lnk_flags
[ -z "$gccpath" ] && gccbin=g++ || cd "$currCD"

# === Time ===
echo "Build time: $(($(date +%s) - start_time)) sec."