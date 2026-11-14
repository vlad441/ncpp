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

#aarch64-linux-gnu-g++ (apt install g++-aarch64-linux-gnu)
#riscv64-linux-gnu-g++ (apt install g++-riscv64-linux-gnu)
#qemu-riscv64 -L /usr/riscv64-linux-gnu tests
ARCH_FLAG=""
TARGET=""
case "$1" in
    "x64") ARCH_FLAG="-m64"; shift ;;
    "x32") ARCH_FLAG="-m32"; shift ;;
	"ARM64"|"ARM") TARGET="aarch64-linux-gnu"; shift ;;
	"ARM32") TARGET="arm-linux-gnueabihf"; shift ;;
    "RISCV64"|"RISCV") TARGET="riscv64-linux-gnu"; shift ;;
	"RISCV32") TARGET="riscv32-linux-gnu"; shift ;;
    "I386"|"I686") TARGET="i686-linux-gnu"; shift ;;
esac
[[ -n "$TARGET" ]] && [[ "$COMPILER" == "GCC" ]] && CBIN="${TARGET}-$CBIN"
[[ -n "$TARGET" ]] && [[ "$COMPILER" == "CLANG" ]] && ARCH_FLAG="-target $TARGET $ARCH_FLAG"

WARN_FLAGS="-Wall -Wno-misleading-indentation"
#-Wall -Wextra -Wpedantic -Wparentheses -Wconversion -Wfloat-equal -Wuseless-cast -Wnon-virtual-dtor -Woverloaded-virtual -Wundef -Werror
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
#S_DEFLIBS_D="-Wl,-Bstatic -lm -lc -Wl,-Bdynamic"

mode=""
case "$1" in "LIB" | "LIB_DLL" | "LIB_SO" | "FAST" | "SAN" | "SDBG" | "DBG" | "MUSL" | "SGUI" | "GUI" | "GL" | "EXP" | "ALL" | "EXP_SAN" | "NOCRT" | "DLNK" | "COVERAGE" | "DEF")
	mode=$1; shift ;;
esac
[ -z "$1" ] && fname=tests || fname=$1
cppfile="$currCD/$fname.cpp"
exefile="$currCD/$fname"
start_time=$(date +%s)

#if [ -z "$gccpath" ]; then gccbin="${CROSS_PREFIX}g++"; else cd "$gccpath" && gccbin="./${CROSS_PREFIX}g++"; fi
[ -n "$gccpath" ] && cd "$gccpath" && CBIN=./$CBIN
#CBIN="g++4.9"

case "$mode" in
    "LIB") echo "Make via ncpp.a..."; $CBIN -D NCPP_LIB_USE $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" -L"$currCD/../lib" -lncpp $LNK_FLAGS ;;
    "LIB_DLL"|"LIB_SO") echo "Make via ncpp.so..."; $CBIN -D NCPP_LIB_USE $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" -L"$currCD/../lib" -lncpp $D_LNK_FLAGS ;;
	"FAST") echo "Make with -O3 -march=native..."; $CBIN $WARN_FLAGS -O3 -std=$STDCXX $FNO_FLAGS_WH "$cppfile" -o "$exefile" -march=native -mtune=native -flto -funroll-loops -fipa-cp-clone -ffast-math -fomit-frame-pointer $LNK_FLAGS ;;
	"SDBG"|"DBG") echo "Make debug -g -Og..."; $CBIN $WARN_FLAGS -g -Og -std=$STDCXX $FNO_FLAGS_WH "$cppfile" -o "$exefile" $DBG_LNK_FLAGS ;;
	"SAN") echo "Make using sanitizers..."; $CBIN $WARN_FLAGS -g -Og -std=$STDCXX $FNO_FLAGS_WH "$cppfile" -o "$exefile" $GCC_LIBS -lpthread -fsanitize=address,undefined ;;
	"MUSL") echo "Make with musl..."; musl-gcc -w $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_FLAGS -lsupc++ ;;
	"GUI") echo "Make for GUI..."; $CBIN -DUSE_GUI -DNOUSE_GL $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" -s $X11_LIBS $D_LNK_FLAGS ;;
	"SGUI") echo "Make for SGUI..."; $CBIN -DUSE_GUI -DNOUSE_GL $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" -lXtst -lXext -lX11 -lxcb -lXau -lXdmcp $LNK_FLAGS ;;
	"GL") echo "Make for GL..."; $CBIN -DUSE_GUI $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $GL_LIBS $D_LNK_FLAGS ;;
	"EXP"|"ALL") echo "Make for Experimental..."; $CBIN -DUSE_GUI -DUSE_EXPERIMENTAL $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $GL_LIBS $AUDIO_LIBS $D_LNK_FLAGS ;;
	"EXP_SAN") echo "Make using sanitizers (for Experimental)..."; $CBIN -DUSE_GUI -DUSE_EXPERIMENTAL $WARN_FLAGS -g -Og -std=$STDCXX $FNO_FLAGS_WH "$cppfile" -o "$exefile" $GL_LIBS $AUDIO_LIBS -lpthread -fsanitize=address,undefined ;;
	"NOCRT") echo "Make NOCRT..." && $CBIN -DNCPP_NOCRT $WARN_FLAGS -O2 -std=$STDCXX $FNO_FLAGS "$cppfile" -o "$exefile" $ARCH_FLAG -Wl,--entry=_start -s -static -nostdlib ;;
	"DLNK") echo "Make DLNK (Dynamic Lnk)..."; $CBIN $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $D_LNK_FLAGS ;;
	"COVERAGE") echo "Make for --coverage..." && $CBIN --coverage $WARN_FLAGS -g -Og -std=$STDCXX $FNO_FLAGS_WH "$cppfile" -o "$exefile" $LNK_FLAGS && ./$fname || gcov -n -m -f -b $fname.cpp > gcov-coverage.txt || node ../scripts/calc-coverage.js ;;
	*) echo "Make (default)..."; $CBIN $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_FLAGS ;;
esac
#rm -f $fname.gcda $fname.gcno
[ -n "$gccpath" ] && cd "$currCD"

# === Time ===
echo "Build time: $(($(date +%s) - start_time)) sec."