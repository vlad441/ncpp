#!/bin/bash
currCD=$(pwd)
gccpath=""
#gccpath="~/gcc-4.9.2/bin"
#clangpath=

COMPILER="GCC"
CBIN="g++"
case "$1" in
    "GCC") COMPILER="GCC"; CBIN="g++"; shift ;;
    "CLANG") COMPILER="CLANG"; CBIN="clang++"; gccpath="$clangpath"; shift ;;
esac

STDCXX="c++98"
case "$1" in
    "C++98") STDCXX="c++98"; shift ;;
    "C++11") STDCXX="c++11"; shift ;;
    "C++17") STDCXX="c++17"; shift ;;
    "C++23") STDCXX="c++23"; shift ;;
esac

ARCH_FLAG=""
CROSS_PREFIX=""
case "$1" in
    "x64") ARCH_FLAG="-m64"; shift ;;
    "x32") ARCH_FLAG="-m32"; shift ;;
	"ARM64"|"ARM") CROSS_PREFIX="aarch64-linux-gnu-"; shift ;;
	"ARM32") CROSS_PREFIX="arm-linux-gnueabihf-"; shift ;;
    "RISCV64"|"RISCV") CROSS_PREFIX="riscv64-linux-gnu-"; shift ;;
	"RISCV32") CROSS_PREFIX="riscv32-linux-gnu-"; shift ;;
    "I386"|"I686") CROSS_PREFIX="i686-linux-gnu-"; shift ;;
esac
CBIN="${CROSS_PREFIX}$CBIN"

WARN_FLAGS="-Wall -Wno-misleading-indentation"
FNO_FLAGS="-fno-exceptions -fno-rtti"
FNO_FLAGS_WH="$FNO_FLAGS -fwhole-program"
#[[ "$COMPILER" == "CLANG" ]] && FNO_FLAGS_WH="$FNO_FLAGS -flto=full -fvisibility=hidden"
OPT_FLAGS="-O2 -std=$STDCXX $FNO_FLAGS_WH"

GCC_LIBS="$ARCH_FLAG -static-libgcc -static-libstdc++"
DBG_LNK_FLAGS="$GCC_LIBS -Wl,--whole-archive -lpthread -Wl,--no-whole-archive"
LNK_FLAGS="-static -s $DBG_LNK_FLAGS"
D_LNK_FLAGS="-s $GCC_LIBS -lpthread"

X11_LIBS="-lXtst -lX11"
GL_LIBS="-lGL $X11_LIBS"
AUDIO_LIBS="-l:libasound.so.2"
#IMAGE_LIBS="-l:libpng16.so.16 -l:libjpeg.so.62"

mode=""
case "$1" in "LIB" | "LIB_DLL" | "LIB_SO" | "FAST" | "SAN" | "DBG" | "MUSL" | "SGUI" | "GUI" | "GL" | "DEF")
	mode=$1; shift ;; # Смещаем аргументы на 1: $1 теперь второй аргумент
esac
[ -z "$1" ] && fname=gui-testapp || fname=$1
cppfile="$currpath/$fname.cpp"
exefile="$currpath/$fname"
start_time=$(date +%s)

[ -n "$gccpath" ] && cd "$gccpath" && CBIN=./$CBIN

case "$mode" in
	"SAN") echo "Make GL using sanitizers..."; $CBIN -DUSE_GUI -DNOUSE_OBJECT -DNOUSE_CRYPTO -DNOUSE_NET $WARN_FLAGS -g -Og -std=c++98 -fno-exceptions -fno-rtti "$cppfile" -o "$exefile" -lGL -lXtst -lX11 -lpthread -fsanitize=address,undefined ;;
	"DBG") echo "Make GL debug ver -g -Og..."; $CBIN $WARN_FLAGS -g -Og -std=$STDCXX -fno-exceptions -fno-rtti "$cppfile" -o "$exefile" -lpthread ;;
	"SGUI") echo "Make for SGUI..."; $CBIN -DUSE_GUI -DNOUSE_GL -DNOUSE_OBJECT -DNOUSE_CRYPTO -DNOUSE_NET $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_X11_FLAGS ;;
	"GUI") echo "Make for GUI..."; $CBIN -DUSE_GUI -DNOUSE_GL -DNOUSE_OBJECT -DNOUSE_CRYPTO -DNOUSE_NET $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_GCC_LIBS -lXtst -lX11 -lpthread ;;
	"GL") echo "Make for GL v0..."; $CBIN -DUSE_GUI -DNOUSE_OBJECT -DNOUSE_CRYPTO -DNOUSE_NET $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_GCC_LIBS -lGL -lX11 -lpthread ;;
	*) echo "Make for GL..."; $CBIN -DUSE_GUI -DNOUSE_OBJECT -DNOUSE_CRYPTO -DNOUSE_NET $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_GCC_LIBS -lGL -lXtst -lX11 -lpthread ;;
esac
[ -n "$gccpath" ] && cd "$currCD"

# === Time === 
echo "Build time: $(($(date +%s) - start_time)) sec."
