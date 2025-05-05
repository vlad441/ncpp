# Require musl-dev bineries in your system
# stdc++ can't use musl, need version compiled with musl
musl_include=/usr/include/x86_64-linux-musl
musl_lib=/usr/lib/x86_64-linux-musl

g++ -nostdlib -nostartfiles -I$musl_include -L$musl_lib $musl_lib/crt1.o $musl_lib/crti.o -lc $musl_lib/crtn.o %*