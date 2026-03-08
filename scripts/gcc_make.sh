#!/bin/bash
source ./_params.sh
if [ $1 ]; then cppfile="$currCD/../$1.cpp"; exefile="$currCD/../$1"; fi
start_time=$(date +%s)

if [ -z "$gccpath" ]; then gccbin=g++; else cd "$gccpath" && gccbin="./g++"; fi
$gccbin $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_FLAGS
[ -z "$gccpath" ] && gccbin=g++ || cd "$currCD"

# ======= Time calc =======
echo "Build time: $(($(date +%s) - start_time)) sec."