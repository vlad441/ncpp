#!/bin/bash
currpath=$(pwd)
#gccpath="~/gcc-4.9.2/bin"

WARN_FLAGS="-Wall -Wno-misleading-indentation"
OPT_FLAGS="-O2 -std=c++98 -fno-exceptions -fno-rtti"
LNK_FLAGS="-static -s -Wl,--whole-archive -lpthread -Wl,--no-whole-archive"
LNK_GCC_LIBS="-s -static-libgcc -static-libstdc++"
D_LNK_FLAGS="$LNK_GCC_LIBS -lpthread"

mode=""
case "$1" in ("LIB" | "LIB_DLL" | "LIB_SO" | "FAST" | "SAN" | "MUSL" | "SGUI" | "GUI" | "GL" | "DEF")
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
    echo "Make via ncpp.a..." & $gccbin -D NCPP_LIB_USE $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" -L"$currpath/../lib" -lncpp $LNK_FLAGS
elif [ "$mode" = "LIB_DLL" ] || [ "$mode" = "LIB_SO" ]; then
    echo "Make via ncpp.so..." & $gccbin -D NCPP_LIB_USE $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" -L"$currpath/../lib" -lncpp $D_LNK_FLAGS
elif [ "$mode" = "FAST" ]; then
    echo "Make with -O3 -march=native..." & $gccbin -O3 -std=c++98 -march=native -fno-exceptions -fno-rtti "$cppfile" -o "$exefile" -flto -funroll-loops -fipa-cp-clone -ffast-math -fomit-frame-pointer $LNK_FLAGS
elif [ "$mode" = "SAN" ]; then
	echo "Make using sanitizers..." & $gccbin $WARN_FLAGS -g -Og -std=c++98 -fno-exceptions -fno-rtti "$cppfile" -o "$exefile" -lpthread -fsanitize=address,undefined
elif [ "$mode" = "MUSL" ]; then
    echo "Make with musl..." & musl-gcc -w $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_FLAGS -lsupc++
elif [ "$mode" = "SGUI" ]; then
	echo "Make for SGUI..." & $gccbin -DUSE_GUI -DNOUSE_GL $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" -s -static -lXtst -lXext -lX11 -lxcb -lXau -lXdmcp -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
elif [ "$mode" = "GUI" ]; then
    echo "Make for GUI..." & $gccbin -DUSE_GUI -DNOUSE_GL $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_GCC_LIBS -lXtst -lX11 -lpthread
elif [ "$mode" = "GL" ]; then
    echo "Make for GL..." & $gccbin -DUSE_GUI $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_GCC_LIBS -lGL -lXtst -lX11 -lpthread
else
    echo "Make (default)..." & $gccbin $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_FLAGS
fi
[ -z "$gccpath" ] && gccbin=g++ || cd "$currpath"

# === Time ===
echo "Build time: $(($(date +%s) - start_time)) sec."