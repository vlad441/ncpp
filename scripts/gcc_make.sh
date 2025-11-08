#!/bin/bash
source ./_params.sh
if [ $1 ]; then cppfile="$currCD/../$1.cpp"; exefile="$currCD/../$1"; fi
start_time=$(date +%s)

if [ -z "$gccpath" ]; then gccbin=g++; else cd "$gccpath" && gccbin="./g++"; fi
$gccbin -Wall "$cppfile" -o "$exefile" $gcc_flags $lnk_flags -fno-exceptions -fno-rtti -Wno-misleading-indentation
#$gccbin -O3 -std=c++98 -march=native -flto -funroll-loops -fipa-cp-clone -ffast-math -fomit-frame-pointer "$cppfile" -o "$exefile" -fno-exceptions -fno-rtti -Wno-misleading-indentation
#g++4.9 -Wall "$cppfile" -o "$exefile" $gcc_flags $lnk_flags
[ -z "$gccpath" ] && gccbin=g++ || cd "$currCD"

# ======= Time calc =======
echo "Build time: $(($(date +%s) - start_time)) sec."