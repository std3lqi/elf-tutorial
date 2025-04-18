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
    TABLE* table = create_table(1, NULL);
    sprintf(table->entries[0].name, "0x%08lX-%08lX *** ELF Header ***", 0, sizeof(Elf64_Ehdr));
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