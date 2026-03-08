#!/bin/bash
currpath=$(pwd)
#gccpath="~/gcc-4.9.2/bin"

WARN_FLAGS="-Wall -Wno-misleading-indentation"
OPT_FLAGS="-O2 -std=c++98 -fno-exceptions -fno-rtti"
LNK_X11_FLAGS="-s -static -lXtst -lXext -lX11 -lxcb -lXau -lXdmcp -Wl,--whole-archive -lpthread -Wl,--no-whole-archive"
LNK_GCC_LIBS="-s -static-libgcc -static-libstdc++"

mode=""
case "$1" in ("LIB" | "LIB_DLL" | "LIB_SO" | "FAST" | "SAN" | "MUSL" | "SGUI" | "GUI" | "GL" | "DEF")
		mode=$1
        shift 1 # Смещаем аргументы на 1: $1 теперь второй аргумент
        ;;
esac
[ -z "$1" ] && fname=gui-testapp || fname=$1
cppfile="$currpath/$fname.cpp"
exefile="$currpath/$fname"
start_time=$(date +%s)

if [ -z "$gccpath" ]; then gccbin=g++; else cd "$gccpath" && gccbin="./g++"; fi
#gccbin="g++4.9"
if [ "$mode" = "SAN" ]; then
	echo "Make for GL using sanitizers..." & $gccbin -DUSE_GUI -DNOUSE_OBJECT -DNOUSE_CRYPTO -DNOUSE_NET $WARN_FLAGS -g -Og -std=c++98 -fno-exceptions -fno-rtti "$cppfile" -o "$exefile" -lGL -lXtst -lX11 -lpthread -fsanitize=address,undefined
elif [ "$mode" = "SGUI" ]; then
	echo "Make for SGUI..." & $gccbin -DUSE_GUI -DNOUSE_GL -DNOUSE_OBJECT -DNOUSE_CRYPTO -DNOUSE_NET $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_X11_FLAGS
elif [ "$mode" = "GUI" ]; then
	echo "Make for GUI..." & $gccbin -DUSE_GUI -DNOUSE_GL -DNOUSE_OBJECT -DNOUSE_CRYPTO -DNOUSE_NET $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_GCC_LIBS -lXtst -lX11 -lpthread
else
    echo "Make for GL..." & $gccbin -DUSE_GUI -DNOUSE_OBJECT -DNOUSE_CRYPTO -DNOUSE_NET $WARN_FLAGS $OPT_FLAGS "$cppfile" -o "$exefile" $LNK_GCC_LIBS -lGL -lXtst -lX11 -lpthread
fi
[ -z "$gccpath" ] && gccbin=g++ || cd "$currpath"

# === Time === 
echo "Build time: $(($(date +%s) - start_time)) sec."