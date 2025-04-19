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

#ifndef ELF_READER_H
#define ELF_READER_H

#include <elf.h>

int read_elf_file(const char* file_path);
void close_elf_file();
unsigned char* get_buffer(int offset);

Elf64_Ehdr* get_elf_header();
Elf64_Shdr* get_section_header(int index);
char* get_section_name_string_table();
#endif