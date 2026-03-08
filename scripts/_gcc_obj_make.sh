source ./_params.sh
cppdir="$(dirname "$(pwd)")"
INCL_DIR="$cppdir/include"
SRC_DIR="$cppdir/src"
OBJ_DIR="$cppdir/lib/obj"
NCPP_H="$SRC_DIR/ncppdef.hpp"

mkdir -p "$cppdir/lib" "$OBJ_DIR"
[ -z "$gccpath" ] && gccbin=g++ || gccbin="$gccpath/g++"
echo $cppdir
compile() { local fname="${1//\//_}"; [[ ! -e "$cppdir/lib/obj/$1.o" ]] && { echo "Compiling $1.cpp..."; $gccbin -D NCPP_LIB_BUILD -include "$NCPP_H" -I "$INCL_DIR" "${@:2}" $OPT_FLAGS -c "$SRC_DIR/$1.cpp" -o "$OBJ_DIR/$fname.o"; }; }

# ==== Base module ====
# compile "ncpp-base"
compile base/print
compile base/utils0
compile base/Array -include base/utils0.hpp
compile base/String -include base/Array.hpp
compile base/dtos -include base/utils0.hpp -include base/String.hpp
# compile base/Buffer.cpp "-include base/utils0.hpp -include base/String.hpp" //Depends: utils0, Array, String, dtos
# compile base/HashMap.cpp //Depends: Array
# ====== ====
# compile sys/utils
# compile sys/fs -include "$NCPP_Hbase"

