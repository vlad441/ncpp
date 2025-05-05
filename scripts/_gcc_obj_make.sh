source ./_params.sh
cppdir="$(dirname "$(pwd)")"
ncpp_include="$cppdir/include"
ncpp_src="$cppdir/src"
ncpp_obj="$cppdir/lib/obj"
NCPP_H="$ncpp_src/ncpp.cpp.h"
NCPP_Hbase="$ncpp_include/ncpp-base.h"

mkdir -p "$cppdir/lib" "$ncpp_obj"
[ -z "$gccpath" ] && gccbin=g++ || gccbin="$gccpath/g++"
echo $cppdir
compile() { local fname="${1//\//_}"; [[ ! -e "$cppdir/lib/$1.o" ]] && { echo "Compiling $1.cpp..."; $gccbin -include "$NCPP_H" "${@:2}" -c "$ncpp_src/$1.cpp" -o "$ncpp_obj/$fname.o" $gcc_flags; }; }

compile "ncpp-base"
compile "system/fs" -include "$NCPP_Hbase"
# compile "ncpp-system"
# compile "net/socket"
# compile "net/http"
# compile "experimantal/ncpp-thread"
# compile "net/server"
# compile "ncpp-object"
# compile "ncpp-bigint"
# compile "ncpp-crypto"
# compile "gui/gui"

