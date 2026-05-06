#!/bin/bash
currpath=$(pwd)
#gccpath="~/gcc-4.9.2/bin"

STDCXX="c++98"
case "$1" in
    "C++98") STDCXX="c++98"; shift ;;
    "C++11") STDCXX="c++11"; shift ;;
    "C++17") STDCXX="c++17"; shift ;;
    "C++23") STDCXX="c++23"; shift ;;
esac

ARCH_FLAG=""
case "$1" in
    "x64") ARCH_FLAG="-m64"; shift ;;
    "x32") ARCH_FLAG="-m32"; shift ;;
esac

WARN_FLAGS="-Wall -Wno-misleading-indentation"
OPT_FLAGS="-O2 -std=$STDCXX -fno-exceptions -fno-rtti -fwhole-program"
LNK_X11_FLAGS="-s -static -lXtst -lXext -lX11 -lxcb -lXau -lXdmcp -Wl,--whole-archive -lpthread -Wl,--no-whole-archive"
LNK_GCC_LIBS="-s $ARCH_FLAG -static-libgcc -static-libstdc++"

mode=""
case "$1" in "LIB" | "LIB_DLL" | "LIB_SO" | "FAST" | "SAN" | "DBG" | "MUSL" | "SGUI" | "GUI" | "GL" | "DEF")
	mode=$1; shift ;; # Смещаем аргументы на 1: $1 теперь второй аргумент
esac
[ -z "$1" ] && fname=gui-testapp || fname=$1
cppfile="$currpath/$fname.cpp"
exefile="$currpath/$fname"
start_time=$(date +%s)

if [ -z "$gccpath" ]; then gccbin=g++; else cd "$gccpath" && gccbin="./g++"; fi
#gccbin="g++4.9"
case "$mode" in
	"SAN") echo "Make GL using sanitizers..."; $gccbin -DUSE_GUI -DNOUSE_OBJECT -DNOUSE_CRYPTO -DNOUSE_NET $WARN_FLAGS -g -Og -std=c++98 -fno-exceptions -fno-rtti "$cppfile" -o "$exefile" -lGL -lXtst -lX11 -lpthread -fsanitize=address,undefined ;;
	"DBG") echo "Make GL debug ver -g -Og..."; $gccbin $WARN_FLAGS -g -Og -std=$STDCXX -fno-exceptions -fno-rtti "$cppfile" -o "$exefile" -lpthread ;;
	"SGUI") echo "Make for SGUI..."; $gccbin -DUSE_GUI -DNOUSE_GL -DNOUSE_OBJECT -DNOUSE_CRYPTO -DNOUSE_NET $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_X11_FLAGS ;;
	"GUI") echo "Make for GUI..."; $gccbin -DUSE_GUI -DNOUSE_GL -DNOUSE_OBJECT -DNOUSE_CRYPTO -DNOUSE_NET $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_GCC_LIBS -lXtst -lX11 -lpthread ;;
	*) echo "Make for GL..."; $gccbin -DUSE_GUI -DNOUSE_OBJECT -DNOUSE_CRYPTO -DNOUSE_NET $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_GCC_LIBS -lGL -lXtst -lX11 -lpthread ;;
esac
[ -z "$gccpath" ] && gccbin=g++ || cd "$currpath"

# === Time === 
echo "Build time: $(($(date +%s) - start_time)) sec."