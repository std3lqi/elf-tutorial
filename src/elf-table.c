/* 
  This file is part of ELF Tutorial program.

  ELF Tutorial program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by the Free 
  Software Foundation, either version 3 of the License, or (at your option) any 
  later version.

  This program is distributed in the hope that it will be useful, but WITHOUT 
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with 
  this program. If not, see <https://www.gnu.org/licenses/>. 
*/

#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <stdio.h>
#include "elf-table.h"
#include "elf-reader.h"
#include "elf-helper.h"

TABLE* create_table(int rows, char* header) {
    TABLE* table = (TABLE*) malloc(sizeof(TABLE));
    table->count = rows;
    table->header = header;
    table->entries = malloc(table->count * sizeof(TABLE_ENTRY));
    return table;
}

void free_table(TABLE* table) {
    if (table) {
        free(table->entries);
        free(table);
    }
}

TABLE* create_elf_file_table() {
    TABLE* table = create_table(3, NULL);
    Elf64_Ehdr* header = get_elf_header();
    int i = 0;
    sprintf(table->entries[i++].name, "0x%08lX-%08lX *** ELF Header ***", 0, sizeof(Elf64_Ehdr));
    sprintf(table->entries[i++].name, "0x%08lX-%08lX *** Program Header Table ***",
        header->e_phoff, header->e_phoff + header->e_phentsize * header->e_phnum);
    sprintf(table->entries[i++].name, "0x%08lX-%08lX *** Section Header Table ***",
        header->e_shoff, header->e_shoff + header->e_shentsize * header->e_shnum);
    return table;
}

TABLE* create_elf_header_table() {
    TABLE* table = create_table(21, NULL);
    Elf64_Ehdr* header = get_elf_header();
    unsigned char* s = header->e_ident;
    int i = 0;
    sprintf(table->entries[i++].name, "Magic Number : 0x%02X %c %c %c", s[0], s[1], s[2], s[3]);
    sprintf(table->entries[i++].name, "File Class   : %s", get_file_class(s[4]));
    sprintf(table->entries[i++].name, "Data Encoding: %s", get_data_endian(s[5]));
    sprintf(table->entries[i++].name, "Version      : %d", s[6]);
    sprintf(table->entries[i++].name, "OS/ABI       : %s", get_os_abi(s[7]));
    sprintf(table->entries[i++].name, "ABI Version  : %d", s[8]);
    sprintf(table->entries[i++].name, "File Type    : %s", get_file_type(header->e_type));
    sprintf(table->entries[i++].name, "Architecture : %s", get_machine(header->e_machine));
    sprintf(table->entries[i++].name, "File Version : %d", header->e_version);
    sprintf(table->entries[i++].name, "Entry Point  : 0x%08lX", header->e_entry);
    sprintf(table->entries[i++].name, "Program Header Table");
    sprintf(table->entries[i++].name, "  Offset     : 0x%08lX", header->e_phoff);
    sprintf(table->entries[i++].name, "  Entry Size : %d", header->e_phentsize);
    sprintf(table->entries[i++].name, "  Entry Count: %d", header->e_phnum);
    sprintf(table->entries[i++].name, "Section Header Table");
    sprintf(table->entries[i++].name, "  Offset     : 0x%08lX", header->e_shoff);
    sprintf(table->entries[i++].name, "  Entry Size : %d", header->e_shentsize);
    sprintf(table->entries[i++].name, "  Entry Count: %d", header->e_shnum);
    sprintf(table->entries[i++].name, "Proc Flags   : 0x%08lX", header->e_flags);
    sprintf(table->entries[i++].name, "ELF Header Sz: %d", header->e_ehsize);
    sprintf(table->entries[i++].name, "Sect Str Idx : %d", header->e_shstrndx);

    return table;
}

TABLE* create_program_headers_table() {
    Elf64_Ehdr* header = get_elf_header();
    char* title = "TYPE   |FLG| ALIGN  |FILESZ| FILE OFF |MEMSZ |VIRT ADDR |PHYS ADDR";
    //            "PHDR      R 0x000008 0x01F8 0x00000040 0x01F8 0x00400040 0x00400040"
    TABLE* table = create_table(header->e_phnum, title);
    Elf64_Phdr* phdr = (Elf64_Phdr*)get_buffer(header->e_phoff);
    for (int i = 0; i < header->e_phnum; i++) {
        // Elf64_Word p_type; /* Segment type */
        // Elf64_Word p_flags; /* Segment flags */
        // Elf64_Off p_offset; /* Segment file offset */
        // Elf64_Addr p_vaddr; /* Segment virtual address */
        // Elf64_Addr p_paddr; /* Segment physical address */
        // Elf64_Xword p_filesz; /* Segment size in file */
        // Elf64_Xword p_memsz; /* Segment size in memory */
        // Elf64_Xword p_align; /* Segment alignment */        
        sprintf(
            table->entries[i].name,
            "%s %c%c%c 0x%06lX 0x%04X 0x%08lX 0x%04X 0x%08lX 0x%08lX",
            get_segment_type_name(phdr->p_type),
            is_segment_executable(phdr->p_flags),
            is_segment_writable(phdr->p_flags),
            is_segment_readable(phdr->p_flags),
            phdr->p_align,
            phdr->p_filesz,
            phdr->p_offset,
            phdr->p_memsz,
            phdr->p_vaddr,
            phdr->p_paddr
        );
        phdr++;
    }
    return table;
}

TABLE* create_section_headers_table() {
    Elf64_Ehdr* header = get_elf_header();
    char* title = "NAM| TYPE        |FLG| VIRT ADDR| OFFSET   |SIZE |LNK|INF|ALIGN|ENTSZ";
    //            "123 PROGBITS      E A 0x004005F0 0x000005F0 0x018 000 000 0x004 000"
    TABLE* table = create_table(header->e_shnum, title);
    Elf64_Shdr* shdr = (Elf64_Shdr*)get_buffer(header->e_shoff);
    for (int i = 0; i < header->e_shnum; i++) {
        // Elf64_Word sh_name; /* Section name (string tbl index) */
        // Elf64_Word sh_type; /* Section type */
        // Elf64_Xword sh_flags; /* Section flags */
        // Elf64_Addr sh_addr; /* Section virtual addr at execution */
        // Elf64_Off sh_offset; /* Section file offset */
        // Elf64_Xword sh_size; /* Section size in bytes */
        // Elf64_Word sh_link; /* Link to another section */
        // Elf64_Word sh_info; /* Additional section information */
        // Elf64_Xword sh_addralign; /* Section alignment */
        // Elf64_Xword sh_entsize; /* Entry size if section holds table */
        sprintf(
            table->entries[i].name,
            "%03d %s %c%c%c 0x%08lX 0x%08lX 0x%03X %03d %03d 0x%03X %03d",
            shdr->sh_name,
            get_section_type_name(shdr->sh_type),
            is_section_executable(shdr->sh_flags),
            is_section_writable(shdr->sh_flags),
            is_section_allocatable(shdr->sh_flags),
            shdr->sh_addr,
            shdr->sh_offset,
            shdr->sh_size,
            shdr->sh_link,
            shdr->sh_info,
            shdr->sh_addralign,
            shdr->sh_entsize
        );
        shdr++;
    }
    return table;
}