/*SECTIONS
{
    . = 0x400000;
    __image_start = .;
    __header_start = .;
    .header : { *(.header) }
    __entry_point = main;
    .text : { 
        *(.text.startup)
        *(.text) 
    }
    .data : { *(.data) }
    .rodata : { *(.rodata) }
    __image_end = .;

    __file_size = __image_end - __image_start;
    /DISCARD/ : {
        *(.*)
    }
}*/
OUTPUT_FORMAT("binary")
/*ENTRY(main)*/
SECTIONS
{
    __base = 0x400000;
    . = __base;

    /* === Elf64_Ehdr (64 байта) === */
    .elf_header : {
        BYTE(0x7f); BYTE(0x45); BYTE(0x4c); BYTE(0x46); /* \x7fELF */
        BYTE(2);                /* 64-bit */
        BYTE(1);                /* Little-endian */
        BYTE(1);                /* ELF version */
        BYTE(0);                /* OS ABI (System V) */
        QUAD(0);                /* 8 байт padding */
        SHORT(2);               /* e_type: ET_EXEC (исполняемый) */
        SHORT(0x3e);            /* e_machine: x86-64 */
        LONG(1);                /* e_version */
        QUAD(_start);           /* e_entry: адрес точки входа (подставится сам!) */
        QUAD(64);               /* e_phoff: таблица сегментов сразу после Ehdr */
        QUAD(0);                /* e_shoff: таблицы секций нет */
        LONG(0);                /* e_flags */
        SHORT(64);              /* e_ehsize: размер этого заголовка */
        SHORT(56);              /* e_phentsize: размер записи Phdr */
        SHORT(1);               /* e_phnum: 1 сегмент */
        SHORT(0); SHORT(0); SHORT(0); /* e_shentsize, e_shnum, e_shstrndx */
    }

    /* === Elf64_Phdr (56 байт) === */
    .program_header : {
        LONG(1);                /* p_type: PT_LOAD */
        LONG(5);                /* p_flags: R (4) + X (1) = 5 */
        QUAD(0);                /* p_offset: грузим файл с самого начала (0) */
        QUAD(__base);           /* p_vaddr: виртуальный адрес 0x400000 */
        QUAD(__base);           /* p_paddr */
        QUAD(__file_size);      /* p_filesz: размер в файле (посчитается ниже!) */
        QUAD(__file_size);      /* p_memsz: размер в памяти */
        QUAD(0x1000);           /* p_align: выравнивание страницы */
    }

    /* === Твой код и данные из C++ === */
    .text : { 
        *(.text) 
    }
    .data : { *(.data) }
    .rodata : { *(.rodata) }

    __file_size = . - __base;

    /DISCARD/ : {
        *(.comment)
        *(.eh_frame*)
        *(.note*)
        *(.*)
    }
}
