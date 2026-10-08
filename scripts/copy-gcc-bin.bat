:: gcc_mini_builds
:: Use DEF: g++ -m32 -s -static -O2 extra-small.cpp -o extra-small.exe
:: Use CRT: g++ -m32 -s -static -nodefaultlibs -O2 extra-small.cpp -o extra-small.exe -lmingw32 -lmingwex -lmsvcrt -lgcc -lkernel32
:: NOCRT: g++ -s -static -nostdlib -O2 extra-small.cpp -o extra-small.exe -Wl,--entry=_start -lkernel32
:: NOCRT with .def: g++ -nostdlib -O2 extra-small.cpp -o extra-small.exe -Wl,--entry=_start -L"C:\Windows\System32" kernel32.def -lkernel32
:: DEF Size: 47,6 МB | CRT Size: 39,3 МB (Min LIBS32 size = 28,4 МB | Min LIBS64 size = 30,1 МB) | NOCRT size: 19,1 МB (+lkernel32: 21,5 МB)

:: mini-gcc/
:: ├── bin\
:: │   ├── g++           <-- Драйвер (командует парадом)
:: │   ├── as            <-- Ассемблер (из пакета binutils)
:: │   └── ld            <-- Линковщик (из пакета binutils)
:: ├── libexec\gcc\x86_64-w64-mingw32\4.9.2
:: │   ├── cc1           <-- Компилятор С
:: │   ├── cc1plus       <-- Компилятор С++
:: │   └── collect2.exe  <-- Линковщик-посредник
:: ├── lib\gcc\x86_64-w64-mingw32\4.9.2\ and 4.9.2\32
:: │    ├── libgcc.a (-lgcc)  <-- Библиотека поддержки GCC (инструкции проца, возможности C)
:: │    ├── libstdc++.so	  <-- Рантайм плюсов (Опционально для STL)
:: │    └── libgcc_s.so		  <-- Раделяемая версия libgcc.a?
:: ├── lib\gcc\x86_64-w64-mingw32\4.9.2\include
:: │    ├── 
:: ├── x86_64-w64-mingw32\include
:: │    ├── windows.h
:: └── x86_64-w64-mingw32\lib and \lib32
     :: ├── crt2.o
     :: ├── crtbegin.o
     :: ├── crtend.o
	 :: ├── libkernel32.a (-lkernel32)  <-- Статическая библиотека-обертка для kernel32.dll (без нее СОВСЕМ никак. Пытался...)
     :: ├── libmsvcrt.a (-lmsvcrt)      <-- Статическая библиотека-обертка для msvcrt.dll
     :: ├── libmingw32.a (-lmingw32)    <-- Вспомогательная библиотека MinGW (интерфейс сисколов винды)
     :: └── libmingwex.a (-lmingwex)    <-- Расширенная вспомогательная библиотека MinGW Extra.
	 
@echo off
chcp 65001 >nul
setlocal

set SRCDIR=.
set DSTDIR=E:\gcc-min

set COPY_CRT=true
set COPY_LIBS64=true
set COPY_LIBS32=true
set COPY_DEFLIBS=true
set COPY_WINAPI_LIBS=false
set COPY_BINUTILS=false
set COPY_INCLUDE=false

echo GCC Minimizer: === Script copy minimal binary files ===

call :MkDir bin
call :CopyFile bin\g++.exe
call :CopyFile bin\as.exe
call :CopyFile bin\ld.exe

if /i "%COPY_BINUTILS%"=="true" (
	call :CopyFile bin\ar.exe
)     
     
call :MkDir libexec\gcc\x86_64-w64-mingw32\4.9.2
:: call :CopyFile libexec\gcc\x86_64-w64-mingw32\4.9.2\cc1.exe
call :CopyFile libexec\gcc\x86_64-w64-mingw32\4.9.2\cc1plus.exe
call :CopyFile libexec\gcc\x86_64-w64-mingw32\4.9.2\collect2.exe

:: Required DLL: libgmp-3.dll, libcloog-isl-3.dll, libisl-10.dll, libmpc-2.dll, libmpfr-1.dll, liblto_plugin-0.dll
call :CopyFile libexec\gcc\x86_64-w64-mingw32\4.9.2\libgmp-3.dll
call :CopyFile libexec\gcc\x86_64-w64-mingw32\4.9.2\libcloog-isl-3.dll
call :CopyFile libexec\gcc\x86_64-w64-mingw32\4.9.2\libisl-10.dll
call :CopyFile libexec\gcc\x86_64-w64-mingw32\4.9.2\libmpc-2.dll
call :CopyFile libexec\gcc\x86_64-w64-mingw32\4.9.2\libmpfr-1.dll
call :CopyFile libexec\gcc\x86_64-w64-mingw32\4.9.2\liblto_plugin-0.dll

:: То самое "курица и яйцо" - библиотека-обертка для kernel32.dll
call :MkDir x86_64-w64-mingw32\lib
call :CopyFile x86_64-w64-mingw32\lib\libkernel32.a
call :MkDir x86_64-w64-mingw32\lib32
call :CopyFile x86_64-w64-mingw32\lib32\libkernel32.a

if /i "%COPY_CRT%"=="true" if /i "%COPY_LIBS64%"=="true" (
	call :MkDir lib\gcc\x86_64-w64-mingw32\4.9.2
	call :CopyFile lib\gcc\x86_64-w64-mingw32\4.9.2\libgcc.a
	:: call :CopyFile lib\gcc\x86_64-w64-mingw32\4.9.2\32\libstdc++.so
	:: call :CopyFile lib\gcc\x86_64-w64-mingw32\4.9.2\32\libgcc_s.so
	
	call :MkDir x86_64-w64-mingw32\lib
	call :CopyFile x86_64-w64-mingw32\lib\crt2.o
	call :CopyFile x86_64-w64-mingw32\lib\crtbegin.o
	call :CopyFile x86_64-w64-mingw32\lib\crtend.o
	::call :CopyFile x86_64-w64-mingw32\lib\libkernel32.a
	call :CopyFile x86_64-w64-mingw32\lib\libmsvcrt.a
	call :CopyFile x86_64-w64-mingw32\lib\libmingw32.a
	call :CopyFile x86_64-w64-mingw32\lib\libmingwex.a
)

if /i "%COPY_CRT%"=="true" if /i "%COPY_LIBS32%"=="true" (
	call :MkDir lib\gcc\x86_64-w64-mingw32\4.9.2\32
	call :CopyFile lib\gcc\x86_64-w64-mingw32\4.9.2\32\libgcc.a

	call :MkDir x86_64-w64-mingw32\lib32
	call :CopyFile x86_64-w64-mingw32\lib32\crt2.o
	call :CopyFile x86_64-w64-mingw32\lib32\crtbegin.o
	call :CopyFile x86_64-w64-mingw32\lib32\crtend.o
	::call :CopyFile x86_64-w64-mingw32\lib32\libkernel32.a
	call :CopyFile x86_64-w64-mingw32\lib32\libmsvcrt.a
	call :CopyFile x86_64-w64-mingw32\lib32\libmingw32.a
	call :CopyFile x86_64-w64-mingw32\lib32\libmingwex.a
)

if /i "%COPY_INCLUDE%"=="true" (
	REM call :MkDir lib\gcc\x86_64-w64-mingw32\4.9.2\include
	REM │    ├──

	REM call :MkDir x86_64-w64-mingw32\include
	REM call :CopyFile x86_64-w64-mingw32\include\windows.h
)

if /i "%COPY_DEFLIBS%"=="true" if /i "%COPY_LIBS64%"=="true" (
	call :CopyFile lib\gcc\x86_64-w64-mingw32\4.9.2\libstdc++.a
	call :CopyFile x86_64-w64-mingw32\lib\libmoldname.a
	call :CopyFile x86_64-w64-mingw32\lib\libpthread.a
	call :CopyFile x86_64-w64-mingw32\lib\libadvapi32.a
	call :CopyFile x86_64-w64-mingw32\lib\libshell32.a
	call :CopyFile x86_64-w64-mingw32\lib\libuser32.a
)

if /i "%COPY_DEFLIBS%"=="true" if /i "%COPY_LIBS32%"=="true" (
	call :CopyFile lib\gcc\x86_64-w64-mingw32\4.9.2\32\libstdc++.a
	call :CopyFile x86_64-w64-mingw32\lib32\libmoldname.a
	call :CopyFile x86_64-w64-mingw32\lib32\libpthread.a
	call :CopyFile x86_64-w64-mingw32\lib32\libadvapi32.a
	call :CopyFile x86_64-w64-mingw32\lib32\libshell32.a
	call :CopyFile x86_64-w64-mingw32\lib32\libuser32.a
)

call :CopyFile copy-gcc-bin.bat

goto :EOF

:MkDir
mkdir "%DSTDIR%\%~1" 2>nul & exit /b

:: // ---Функция: копирует файл с сохранением пути
:CopyFile
set "RELPATH=%~1"
:: mkdir "%DSTDIR%\%RELPATH%" 2>nul &
copy /Y "%SRCDIR%\%RELPATH%" "%DSTDIR%\%RELPATH%" >nul && echo [+] %RELPATH%
exit /b
