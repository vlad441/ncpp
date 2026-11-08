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
OPT_FLAGS="-O2 -std=$STDCXX $FNO_FLAGS"

GCC_LIBS="$ARCH_FLAG -static-libgcc -static-libstdc++"
DBG_LNK_FLAGS="$GCC_LIBS -Wl,--whole-archive -lpthread -Wl,--no-whole-archive"
LNK_FLAGS="-static -s $DBG_LNK_FLAGS"
D_LNK_FLAGS="-s $GCC_LIBS -lpthread"

X11_LIBS="-lXtst -lX11"
GL_LIBS="-lGL $X11_LIBS"
AUDIO_LIBS="-l:libasound.so.2"
#IMAGE_LIBS="-l:libpng16.so.16 -l:libjpeg.so.62"

mode=""
case "$1" in "LIB" | "LIB_DLL" | "LIB_SO" | "FAST" | "SAN" | "SDBG" | "DBG" | "MUSL" | "SGUI" | "GUI" | "GL" | "EXP" | "EXP_SAN" | "NOCRT" | "DLNK" | "DEF")
	mode=$1; shift ;;
esac

cppfile="$currCD/../src/ncpp.cpp"
#exefile="$currCD/../$fname"
INCL_DIR="$currCD/../include"
LIB_DIR="$currCD/../lib"
mkdir -p "$LIB_DIR"

#source ./_params.sh
#source ./_gcc_obj_make.sh
#[ -z "$gccpath" ] && ARBIN=ar || ARBIN="$gccpath/ar"

start_time=$(date +%s)

[ -n "$gccpath" ] && cd "$gccpath"

#cd ../lib/obj || exit 1
#echo "Packing in archive libncpp.a..."
#$ARBIN rcs ../libncpp.a ncpp-base.o system_fs.o ncpp-system.o net_socket.o net_http.o ncpp-thread.o net_server.o
#cd "$currCD"

echo "Build static-bundle lib..."
$CBIN -D LIB_DIR_BUILD $WARN_FLAGS $OPT_FLAGS -c "$cppfile" -o "$LIB_DIR/ncpp.o" -I "$INCL_DIR"
ar rcs "$LIB_DIR/libncpp-bundle.a" "$LIB_DIR/ncpp.o"
rm "$LIB_DIR/ncpp.o"

echo "Build dynamic lib..."
$CBIN -D LIB_DIR_BUILD $WARN_FLAGS -fPIC -shared $OPT_FLAGS "$cppfile" -o "$LIB_DIR/libncpp.so" $D_LNK_FLAGS

[ -n "$gccpath" ] && cd "$currCD"

# === Time ===
echo "Build time: $(($(date +%s) - start_time)) sec."
