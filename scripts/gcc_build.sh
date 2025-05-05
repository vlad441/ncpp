#!/bin/bash
source ./_params.sh
#./_gcc_obj_make.sh
cppfile="$currCD/../src/ncpp.cpp"
ncpp_include="$currCD/../include"
ncpp_lib="$currCD/../lib"
start_time=$(date +%s)
[ -z "$gccpath" ] && gccar=ar || gccar="$gccpath/ar"
mkdir -p "$ncpp_lib"

#cd ../lib/obj || exit 1
#echo "Packing in archive libncpp.a..."
#$gccar rcs ../libncpp.a ncpp-base.o system_fs.o ncpp-system.o net_socket.o net_http.o ncpp-thread.o net_server.o
#cd "$currCD"

if [ -z "$gccpath" ]; then gccbin=g++; else cd "$gccpath" && gccbin="./g++"; fi
echo "Build static-bundle lib..."
$gccbin -Wall -c "$cppfile" -o "$ncpp_lib/ncpp.o" $gcc_flags -I "$ncpp_include"
ar rcs "$ncpp_lib/libncpp-bundle.a" "$ncpp_lib/ncpp.o"
rm "$ncpp_lib/ncpp.o"

echo "Build dynamic lib..."
$gccbin -fPIC -shared "$cppfile" -o "$ncpp_lib/libncpp.so" $gcc_flags $dll_lnk_flags
if [ -z "$gccpath" ]; then gccbin=g++; else cd "$currCD"; fi

# === Time ===
echo "Build time: $(($(date +%s) - start_time)) sec."
