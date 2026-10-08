#!/bin/bash
currCD=$(pwd)
#gccpath="~/gcc-4.9.2/bin"

STDCXX="c++98"
case "$1" in
    "C++98") STDCXX="c++98"; shift ;;
    "C++11") STDCXX="c++11"; shift ;;
    "C++17") STDCXX="c++17"; shift ;;
    "C++23") STDCXX="c++23"; shift ;;
esac

#aarch64-linux-gnu-g++ (apt install g++-aarch64-linux-gnu)
#riscv64-linux-gnu-g++ (apt install g++-riscv64-linux-gnu)
#qemu-riscv64 -L /usr/riscv64-linux-gnu tests
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

WARN_FLAGS="-Wall -Wno-misleading-indentation"
FNO_FLAGS="-fno-exceptions -fno-rtti"
FNO_FLAGS_WH="$FNO_FLAGS -fwhole-program"
OPT_FLAGS="-O2 -std=$STDCXX $FNO_FLAGS_WH"

DBG_LNK_FLAGS="-static $ARCH_FLAG -Wl,--whole-archive -lpthread -Wl,--no-whole-archive"
LNK_FLAGS="-s $DBG_LNK_FLAGS"
LNK_GCC_LIBS="-s $ARCH_FLAG -static-libgcc -static-libstdc++"
D_LNK_FLAGS="$LNK_GCC_LIBS -lpthread"

mode=""
case "$1" in "LIB" | "LIB_DLL" | "LIB_SO" | "FAST" | "SAN" | "DBG" | "MUSL" | "SGUI" | "GUI" | "GL" | "EXP" | "EXP_SAN" | "NOCRT" | "DEF")
	mode=$1; shift ;; # Смещаем аргументы на 1: $1 теперь второй аргумент
esac
[ -z "$1" ] && fname=tests || fname=$1
cppfile="$currCD/$fname.cpp"
exefile="$currCD/$fname"
start_time=$(date +%s)

#if [ -z "$gccpath" ]; then gccbin=g++; else cd "$gccpath" && gccbin="./g++"; fi
if [ -z "$gccpath" ]; then gccbin="${CROSS_PREFIX}g++"; else cd "$gccpath" && gccbin="./${CROSS_PREFIX}g++"; fi

#gccbin="g++4.9"
case "$mode" in
    "LIB") echo "Make via ncpp.a..."; $gccbin -D NCPP_LIB_USE $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" -L"$currCD/../lib" -lncpp $LNK_FLAGS ;;
    "LIB_DLL"|"LIB_SO") echo "Make via ncpp.so..."; $gccbin -D NCPP_LIB_USE $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" -L"$currCD/../lib" -lncpp $D_LNK_FLAGS ;;
	"FAST") echo "Make with -O3 -march=native..."; $gccbin $WARN_FLAGS -O3 -std=$STDCXX $FNO_FLAGS_WH "$cppfile" -o "$exefile" -march=native -mtune=native -flto -funroll-loops -fipa-cp-clone -ffast-math -fomit-frame-pointer $LNK_FLAGS ;;
	"DBG") echo "Make debug -g -Og..."; $gccbin $WARN_FLAGS -g -Og -std=$STDCXX $FNO_FLAGS_WH "$cppfile" -o "$exefile" $DBG_LNK_FLAGS ;;
	"SAN") echo "Make using sanitizers..."; $gccbin $WARN_FLAGS -g -Og -std=$STDCXX $FNO_FLAGS_WH "$cppfile" -o "$exefile" -lpthread -fsanitize=address,undefined ;;
	"MUSL") echo "Make with musl..."; musl-gcc -w $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_FLAGS -lsupc++ ;;
	"GUI") echo "Make for GUI..."; $gccbin -DUSE_GUI -DNOUSE_GL $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_GCC_LIBS -lXtst -lX11 -lpthread ;;
	"SGUI") echo "Make for SGUI..."; $gccbin -DUSE_GUI -DNOUSE_GL $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" -s -static -lXtst -lXext -lX11 -lxcb -lXau -lXdmcp -Wl,--whole-archive -lpthread -Wl,--no-whole-archive ;;
	"GL") echo "Make for GL..."; $gccbin -DUSE_GUI $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_GCC_LIBS -lGL -lXtst -lX11 -lpthread ;;
	"EXP") echo "Make for Experimental..."; $gccbin -DUSE_GUI -DUSE_EXPERIMENTAL $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_GCC_LIBS -lGL -lXtst -lX11 -lasound -lpthread ;;
	"EXP_SAN") echo "Make using sanitizers (for Experimental)..."; $gccbin -DUSE_GUI -DUSE_EXPERIMENTAL $WARN_FLAGS -g -Og -std=$STDCXX $FNO_FLAGS_WH "$cppfile" -o "$exefile" -lGL -lXtst -lX11 -lasound -lpthread -fsanitize=address,undefined ;;
	"NOCRT") echo Make NOCRT... & $gccbin -DNCPP_NOCRT $WARN_FLAGS -O2 -std=$STDCXX $FNO_FLAGS "$cppfile" -o "$exefile" $ARCH_FLAG -Wl,--entry=_start -s -static -nostdlib ;;
	*) echo "Make (default)..."; $gccbin $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_FLAGS ;;
esac
[ -z "$gccpath" ] && gccbin=g++ || cd "$currCD"

# === Time ===
echo "Build time: $(($(date +%s) - start_time)) sec."