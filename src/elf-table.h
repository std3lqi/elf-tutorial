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

#ifndef ELF_HEADER_H
#define ELF_HEADER_H

#include <elf.h>

struct table_entry {
    char name[1024];
};
typedef struct table_entry TABLE_ENTRY;

struct table {
    int count;                      // How many entries
    char* header;                   // Table header
    TABLE_ENTRY* entries;           // Table entries showing ELF information
};
typedef struct table TABLE;

void free_table(TABLE* table);

TABLE* create_elf_file_table();
TABLE* create_elf_header_table();
TABLE* create_program_headers_table();
TABLE* create_section_headers_table();
TABLE* create_string_table(Elf64_Shdr* shdr);
TABLE* create_symbol_table(Elf64_Shdr* shdr);
TABLE* create_dynamic_table(Elf64_Shdr* shdr);
TABLE* create_hash_table(Elf64_Shdr* shdr);
TABLE* create_gnu_hash_table(Elf64_Shdr* shdr);
TABLE* create_note_table(Elf64_Shdr* shdr);
TABLE* create_gnu_verdef_table(Elf64_Shdr* shdr);
TABLE* create_gnu_versym_table(Elf64_Shdr* shdr);
TABLE* create_gnu_verneed_table(Elf64_Shdr* shdr);
TABLE* create_rela_table(Elf64_Shdr* shdr);
TABLE* create_hex_table(Elf64_Shdr* shdr);

#endif
