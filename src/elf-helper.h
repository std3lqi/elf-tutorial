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
#ifndef ELF_HELPER_H
#define ELF_HELPER_H

#include <elf.h>
#include <stdbool.h>

char* get_file_class(unsigned char class);
char* get_data_endian(unsigned char encoding);
char* get_os_abi(unsigned char abi);
char* get_file_type(Elf64_Half type);
char* get_machine(Elf64_Half machine);
char* get_section_type_name(Elf64_Word type);
char* get_segment_type_name(Elf64_Word type);

char is_segment_readable(Elf64_Word flags);
char is_segment_writable(Elf64_Word flags);
char is_segment_executable(Elf64_Word flags);
char is_section_writable(Elf64_Xword flags);
char is_section_allocatable(Elf64_Xword flags);
char is_section_executable(Elf64_Xword flags);
char is_section_merged(Elf64_Xword flags);
char is_section_strings(Elf64_Xword flags);
char is_section_info_link(Elf64_Xword flags);
char is_section_link_order(Elf64_Xword flags);

char* get_symbol_binding(unsigned char info);
char* get_symbol_type(unsigned char info);
char* get_symbol_visibility(unsigned char other);
char* get_dynamic_tag(Elf64_Sxword tag);
bool use_dynamic_val(Elf64_Sxword tag);
bool use_dynamic_val_for_strtab(Elf64_Sxword tag);
bool use_dynamic_ptr(Elf64_Sxword tag);
char* get_note_os(Elf64_Word os);
char* get_version_def_flags(Elf64_Half flags);
char* get_relocation_type(Elf64_Sxword info);

#endif
