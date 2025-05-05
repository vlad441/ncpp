#!/bin/bash
source ./_params.sh
start_time=$(date +%s)

if [ -z "$clangpath" ]; then clangbin=clang++; else cd "$clangpath" && clangbin=./clang++; fi
$clangbin -Wall "$cppfile" -o "$exefile" $gcc_flags $lnk_flags
cd "$currCD"

# === Time ===
echo "Build time: $(($(date +%s) - start_time)) sec."