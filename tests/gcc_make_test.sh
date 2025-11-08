#!/bin/bash
currpath=$(pwd)
#gccpath="~/gcc-4.9.2/bin"

gcc_flags="-O2 -std=c++98"
#lnk_flags="-static -s"
lnk_flags="-static -s -Wl,--whole-archive -lpthread -Wl,--no-whole-archive"
dll_lnk_flags="-s -lpthread"
ext_flags="-fno-exceptions -fno-rtti -Wno-misleading-indentation"
#-Wl,--trace

mode=""
case "$1" in ("LIB" | "LIB_DLL" | "LIB_SO" | "FAST" | "SAN" | "MUSL" | "DEF")
		mode=$1
        shift 1 # Смещаем аргументы на 1: $1 теперь второй аргумент
        ;;
esac
[ -z "$1" ] && fname=tests || fname=$1
cppfile="$currpath/$fname.cpp"
exefile="$currpath/$fname"
start_time=$(date +%s)

if [ -z "$gccpath" ]; then gccbin=g++; else cd "$gccpath" && gccbin="./g++"; fi
#gccbin="g++4.9"

if [ "$mode" = "LIB" ]; then
    echo "Make via ncpp.a..." & $gccbin -D NCPP_LIB_USE -Wall "$cppfile" -o "$exefile" $gcc_flags -L"$currpath/../lib" -lncpp $lnk_flags $ext_flags
elif [ "$mode" = "LIB_DLL" ] || [ "$mode" = "LIB_SO" ]; then
    echo "Make via ncpp.so..." & $gccbin -D NCPP_LIB_USE -Wall "$cppfile" -o "$exefile" $gcc_flags -L"$currpath/../lib" -lncpp $dll_lnk_flags $ext_flags
elif [ "$mode" = "FAST" ]; then
    echo "Make with -O3 -march=native..." & $gccbin -O3 -std=c++98 -march=native $cppfile -o $exefile -flto -funroll-loops -fipa-cp-clone -ffast-math -fomit-frame-pointer $lnk_flags $ext_flags
elif [ "$mode" = "SAN" ]; then
	echo "Make using sanitizers..." & $gccbin -Wall "$cppfile" -o "$exefile" -g -Og -std=c++98 -lpthread -fsanitize=address,undefined $ext_flags
elif [ "$mode" = "MUSL" ]; then
    echo "Make with musl..." & musl-gcc -w "$cppfile" -o "$exefile" $gcc_flags $lnk_flags -lsupc++ $ext_flags
else
    echo "Make (default)..." & $gccbin -Wall "$cppfile" -o "$exefile" $gcc_flags $lnk_flags $ext_flags
fi
#addr2line -e /home/ncpp-site/ncpp-server 0x560c02476df2
[ -z "$gccpath" ] && gccbin=g++ || cd "$currpath"

# === Time ===
echo "Build time: $(($(date +%s) - start_time)) sec."