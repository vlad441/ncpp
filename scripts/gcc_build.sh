#!/bin/bash
source ./_params.sh
#./_gcc_obj_make.sh
cppfile="$currCD/../src/ncpp.cpp"
INCL_DIR="$currCD/../include"
LIB_DIR="$currCD/../lib"
start_time=$(date +%s)
[ -z "$gccpath" ] && gccar=ar || gccar="$gccpath/ar"
mkdir -p "$LIB_DIR"

#cd ../lib/obj || exit 1
#echo "Packing in archive libncpp.a..."
#$gccar rcs ../libncpp.a ncpp-base.o system_fs.o ncpp-system.o net_socket.o net_http.o ncpp-thread.o net_server.o
#cd "$currCD"

if [ -z "$gccpath" ]; then gccbin=g++; else cd "$gccpath" && gccbin="./g++"; fi
echo "Build static-bundle lib..."
$gccbin -D LIB_DIR_BUILD $WARN_FLAGS $OPT_FLAGS -c "$cppfile" -o "$LIB_DIR/ncpp.o"  -I "$INCL_DIR"
ar rcs "$LIB_DIR/libncpp-bundle.a" "$LIB_DIR/ncpp.o"
rm "$LIB_DIR/ncpp.o"

echo "Build dynamic lib..."
$gccbin -D LIB_DIR_BUILD $WARN_FLAGS -fPIC -shared $OPT_FLAGS "$cppfile" -o "$LIB_DIR/libncpp.so" $D_LNK_FLAGS
if [ -z "$gccpath" ]; then gccbin=g++; else cd "$currCD"; fi

# === Time ===
echo "Build time: $(($(date +%s) - start_time)) sec."
