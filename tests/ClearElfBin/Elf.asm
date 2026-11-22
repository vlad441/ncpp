;Header:
;  .Magic                                db 0x7f, 0x45, 0x4c, 0x46  ; 7f, 45 = e, 4c = l, 46 = f
;  .Class                                db 0x2                     ; 01 - elf32, 02 - lelf64
;  .Data                                 db 0x1                     ; 01 - litle endian, 02 - big endian
;  .Version                              db 0x1
;  .OS_ABI                               db 0x0
;  .ABI_version                          db 0x0
;  .Reserved                             db 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0

;  .Type                                 dw 0x2                     ; CORE (значение 4)
                                                                   ; DYN (Shared object file), библиотека (значение 3)
                                                                   ; EXEC (Executable file), исполняемый файл (значение 2)
                                                                   ; REL (Relocatable file), файл до линковки (значение 1) 
;  .Machine                              dw 0x3e                    ; 3e - AMD64
;  .File_Version                         dd 0x0
;  .Entry_Point                          dd 0x0                     ; определяет виртуальный адрес точки входа в программу. После загрузки программы в память управление передается на этот адрес.
;  .Start_Of_Program_Headers             dd 0x0                     ; задает смещение от начала файла до начала таблицы заголовков программы (program header table). Информация о таблице заголовков программы будет дана ниже.
;  .Start_Of_Section_Headers             dd 0x0                     ; задает смещение от начала файла до начала таблицы заголовков секций (program section table). Информация о таблице заголовков секций будет дана ниже.
;  .Flags                                dd 0x0                     ; задает дополнительные процессорно-специфичные флаги. В настоящее время значение данного поля должно всегда быть 0.
;  .Size_Of_This_Header                  dw 0x34                    ; хранит размер заголовка ELF-файла. Его значение должно быть равно 52 (sizeof(Elf32_Ehdr)).
;  .Size_of_Program_Headers              dw 0x20                    ; хранит размер одной записи в таблице заголовков программы. Его значение должно быть 32 (sizeof(Elf32_Phdr)) или 0, если таблица заголовков программы пуста.
;  .Number_Of_Program_Headers            dw 0x9                     ; хранит количество записей в таблице заголовков программы.
;  .Size_Of_Section_Header               dw 0x28                    ; хранит размер одной записи в таблице заголовков секций. Его значение должно быть равно 40 (sizeof(Elf32_Shdr)) или 0, если таблица заголовков секций пуста. 
;  .Number_Of_Section_Header             dw 0x1C                    ; хранит количество записей в таблице заголовков секций.
;  .Section_Header_String_Table_Index    dw 0x1B                    ; хранит индекс заголовка секции, которая хранит имена всех секций (см. ниже).
;

BITS 64
org 0x400000

; --- Заголовок ELF64 ---
ehdr:
    db 0x7F, "ELF", 2, 1, 1, 0      ; e_ident (сигнатура и класс 64-бит)
    times 8 db 0                    ; Padding (резерв)
    dw 2                            ; e_type: ET_EXEC (исполняемый)
    dw 0x3E                         ; e_machine: AMD x86-64
    dd 1                            ; e_version: текущая версия
    dq _start                       ; e_entry: точка входа (виртуальный адрес)
    dq phdr - ehdr                  ; e_phoff: смещение программного заголовка
    dq 0                            ; e_shoff: без таблиц секций
    dd 0                            ; e_flags
    dw ehdr_size                    ; e_ehsize: размер этого заголовка (64 байта)
    dw phdr_size                    ; e_phentsize: размер программного заголовка
    dw 1                            ; e_phnum: количество программных заголовков (1)
    dw 0                            ; e_shentsize
    dw 0                            ; e_shnum
    dw 0                            ; e_shstrndx

ehdr_size equ $ - ehdr

; --- Программный заголовок (Program Header) ---
phdr:
    dd 1                            ; p_type: PT_LOAD (загружаемый сегмент)
    dd 5                            ; p_flags: PF_R | PF_X (читать и выполнять)
    dq 0                            ; p_offset: смещение в файле (0)
    dq 0x400000                     ; p_vaddr: виртуальный адрес загрузки
    dq 0x400000                     ; p_paddr: физический адрес
    dq file_size                    ; p_filesz: размер файла в памяти
    dq file_size                    ; p_memsz: размер в памяти
    dq 0x200000                     ; p_align: выравнивание (2MB)

phdr_size equ $ - phdr

; --- Сам исполняемый код ---
_start:
    ; Системный вызов sys_exit (выход из программы в Linux)
    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, 13
    syscall

    mov rax, 60            ; Системный вызов exit (60 в Linux x86_64)
    xor rdi, rdi           ; Код возврата 0
    syscall                ; Инструкция вызова ядра
    msg: db "Hello World!", 0 , 0

file_size equ $ - ehdr