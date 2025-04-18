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

#include "elf-helper.h"

char* get_file_class(unsigned char class) {
    switch (class) {
        case ELFCLASS32:    return "32-Bit";
        case ELFCLASS64:    return "64-Bit";
        default:            return "Invalid";
    }
}

char* get_data_endian(unsigned char encoding) {
    switch (encoding) {
        case ELFDATA2LSB:   return "Little Endian";
        case ELFDATA2MSB:   return "Big Endian";
        default:            return "Invalid";
    }
}

char* get_os_abi(unsigned char abi) {
    switch (abi) {
        case ELFOSABI_NONE: return "UNIX System V ABI";
        case ELFOSABI_GNU:  return "Object uses GNUELF";
        default:            return "Other";
    }
}

char* get_file_type(Elf64_Half type) {
    switch (type) {
        case ET_NONE:   return "NONE";
        case ET_REL:    return "Relocatable File";
        case ET_EXEC:   return "Executable File";
        case ET_DYN:    return "Shared Object File";
        case ET_CORE:   return "Core File";
        default:        return "Unnkown";
    }
}

char* get_machine(Elf64_Half machine) {
    switch (machine) {
        case EM_NONE:   return "NONE";
        case EM_386:    return "Intel 80386";
        case EM_X86_64: return "AMD x86-64 architecture";
        case EM_AARCH64:return "ARM AARCH64";
        default:        return "Other";
    }
}

char* get_section_type_name(Elf64_Word type) {
    switch (type) {
        case SHT_NULL:          return "NULL         ";
        case SHT_PROGBITS:      return "PROGBITS     ";
        case SHT_SYMTAB:        return "SYMTAB       ";
        case SHT_STRTAB:        return "STRTAB       ";
        case SHT_RELA:          return "RELA         ";
        case SHT_HASH:          return "HASH         ";
        case SHT_DYNAMIC:       return "DYNAMIC      ";
        case SHT_NOTE:          return "NOTE         ";
        case SHT_NOBITS:        return "NOBITS       ";
        case SHT_REL:           return "REL          ";
        case SHT_SHLIB:         return "SHLIB        ";
        case SHT_DYNSYM:        return "DYNSYM       ";
        case SHT_INIT_ARRAY:    return "INIT_ARRAY   ";
        case SHT_FINI_ARRAY:    return "FINI_ARRAY   ";
        case SHT_PREINIT_ARRAY: return "PREINIT_ARRAY";
        case SHT_GROUP:         return "GROUP        ";
        case SHT_SYMTAB_SHNDX:  return "SYMTAB_SHNDX ";
        case SHT_GNU_ATTRIBUTES:return "GNU_ATTRIBUTE";
        case SHT_GNU_HASH:      return "GNU_HASH     ";
        case SHT_GNU_LIBLIST:   return "GNU_LIBLIST  ";
        case SHT_GNU_verdef:    return "GNU_VERSION  ";
        case SHT_GNU_verneed:   return "GNU_VERNEEDED";
        case SHT_GNU_versym:    return "GNU_VERSYM   ";
        default:                return "OTHER        ";
    }
}

char* get_segment_type_name(Elf64_Word type) {
    switch (type) {
        case PT_NULL:       return "NULL   ";
        case PT_LOAD:       return "LOAD   ";
        case PT_DYNAMIC:    return "DYNAMIC";
        case PT_INTERP:     return "INTERP ";
        case PT_NOTE:       return "NOTE   ";
        case PT_SHLIB:      return "SHLIB  ";
        case PT_PHDR:       return "PHDR   ";
        case PT_TLS:        return "TLS    ";
        case PT_GNU_EH_FRAME:
                            return "EH_FRAM";
        case PT_GNU_STACK:  return "STACK  ";
        case PT_GNU_RELRO:  return "RELRO  ";
        default:            return "UNKNOWN";
    }
}

char is_segment_readable(Elf64_Word flags) {
    return (flags & PF_R) == 0 ? ' ' : 'R';
}

char is_segment_writable(Elf64_Word flags) {
    return (flags & PF_W) == 0 ? ' ' : 'W';
}

char is_segment_executable(Elf64_Word flags) {
    return (flags & PF_X) == 0 ? ' ' : 'E';
}

char is_section_writable(Elf64_Xword flags) {
    return (flags & SHF_WRITE) == 0 ? ' ' : 'W';
}

char is_section_allocatable(Elf64_Xword flags) {
    return (flags & SHF_ALLOC) == 0 ? ' ' : 'A';
}

char is_section_executable(Elf64_Xword flags) {
    return (flags & SHF_EXECINSTR) == 0 ? ' ' : 'E';
}

char is_section_merged(Elf64_Xword flags) {
    return (flags & SHF_MERGE) == 0 ? ' ' : 'M';
}

char is_section_strings(Elf64_Xword flags) {
    return (flags & SHF_STRINGS) == 0 ? ' ' : 'S';
}

char is_section_info_link(Elf64_Xword flags) {
    return (flags & SHF_INFO_LINK) == 0 ? ' ' : 'L';
}

char is_section_link_order(Elf64_Xword flags) {
    return (flags & SHF_LINK_ORDER) == 0 ? ' ' : 'O';
}

char* get_symbol_binding(unsigned char info) {
    switch (ELF64_ST_BIND(info)) {
        case STB_LOCAL:  return "LOCAL ";
        case STB_GLOBAL: return "GLOBAL";
        case STB_WEAK:   return "WEAK  ";
        default:         return "OTHER ";
    }
}

char* get_symbol_type(unsigned char info) {
    switch (ELF64_ST_TYPE(info)) {
        case STT_NOTYPE: return "NONE   ";
        case STT_OBJECT: return "OBJECT ";
        case STT_FUNC:   return "FUNC   ";
        case STT_SECTION:return "SECTION";
        case STT_FILE:   return "FILE   ";
        default:         return "OTHER  ";
    }
}

char* get_symbol_visibility(unsigned char other) {
    switch (ELF64_ST_VISIBILITY(other)) {
        case STV_DEFAULT:   return "DEFAULT  ";
        case STV_INTERNAL:  return "INTERNAL ";
        case STV_HIDDEN:    return "HIDDEN   ";
        case STV_PROTECTED: return "PROTECTED";
    }
}

char* get_dynamic_tag(Elf64_Sxword tag) {
    switch (tag) {
        case DT_NULL:     return "NULL      ";
        case DT_NEEDED:   return "NEEDED    ";
        case DT_PLTRELSZ: return "PLTRELSZ  ";
        case DT_PLTGOT:   return "PLTGOT    ";
        case DT_HASH:     return "HASH      ";
        case DT_STRTAB:   return "STRTAB    ";
        case DT_SYMTAB:   return "SYMTAB    ";
        case DT_RELA:     return "RELA      ";
        case DT_RELASZ:   return "RELASZ    ";
        case DT_RELAENT:  return "RELAENT   ";
        case DT_STRSZ:    return "STRSZ     ";
        case DT_SYMENT:   return "SYMENT    ";
        case DT_INIT:     return "INIT      ";
        case DT_FINI:     return "FINI      ";
        case DT_SONAME:   return "SONAME    ";
        case DT_RPATH:    return "RPATH     ";
        case DT_SYMBOLIC: return "SYMBOLIC  ";
        case DT_REL:      return "REL       ";
        case DT_RELSZ:    return "RELSZ     ";
        case DT_RELENT:   return "RELENT    ";
        case DT_PLTREL:   return "PLTREL    ";
        case DT_DEBUG:    return "DEBUG     ";
        case DT_TEXTREL:  return "TEXTREL   ";
        case DT_JMPREL:   return "JMPREL    ";
        case DT_BIND_NOW: return "BIND_NOW  ";
        case DT_INIT_ARRAY:
                          return "INIT_ARR  ";
        case DT_FINI_ARRAY:
                          return "FINI_ARR  ";
        case DT_INIT_ARRAYSZ:
                          return "INIT_ARRSZ";
        case DT_FINI_ARRAYSZ:
                          return "FINI_ARRSZ";
        default:          return "OTHER     ";
    }
}

bool use_dynamic_val(Elf64_Sxword tag) {
    switch (tag) {
        case DT_NEEDED:
        case DT_PLTRELSZ:
        case DT_RELASZ:
        case DT_RELAENT:
        case DT_STRSZ:
        case DT_SYMENT:
        case DT_SONAME:
        case DT_RPATH:
        case DT_RELSZ:
        case DT_RELENT:
        case DT_PLTREL:   return true;
        default:          return false;
    }
}

bool use_dynamic_val_for_strtab(Elf64_Sxword tag) {
    switch (tag) {
        case DT_NEEDED:
        case DT_SONAME:
        case DT_RPATH:    return true;
        default:          return false;
    }
}

bool use_dynamic_ptr(Elf64_Sxword tag) {
    switch (tag) {
        case DT_PLTGOT:
        case DT_HASH:
        case DT_STRTAB:
        case DT_SYMTAB:
        case DT_RELA:
        case DT_INIT:
        case DT_FINI:
        case DT_REL:
        case DT_DEBUG:
        case DT_JMPREL:
        case DT_INIT_ARRAY:
        case DT_FINI_ARRAY:
        case DT_INIT_ARRAYSZ:
        case DT_FINI_ARRAYSZ:
                          return true;
        default:          return false;
    }
}

char* get_note_os(Elf64_Word os) {
    switch (os) {
        case ELF_NOTE_OS_LINUX:     return "LINUX   ";
        case ELF_NOTE_OS_GNU:       return "GNU     ";
        case ELF_NOTE_OS_SOLARIS2:  return "SOLARIS2";
        case ELF_NOTE_OS_FREEBSD:   return "FREEBSD ";
        default:                    return "OTHER   ";
    } 
}

char* get_version_def_flags(Elf64_Half flags) {
    switch(flags) {
        case VER_FLG_WEAK: return "WEAK";
        case VER_FLG_BASE: return "BASE";
        default:           return "NONE";
    }
}

#define DEFINE_CASE(V) case V: return #V;

char* get_relocation_type(Elf64_Sxword info) {
    switch (ELF64_R_TYPE(info)) {
        DEFINE_CASE(R_X86_64_NONE)
        DEFINE_CASE(R_X86_64_64)
        DEFINE_CASE(R_X86_64_PC32)
        DEFINE_CASE(R_X86_64_GOT32)
        DEFINE_CASE(R_X86_64_PLT32)
        DEFINE_CASE(R_X86_64_COPY)
        DEFINE_CASE(R_X86_64_GLOB_DAT)
        DEFINE_CASE(R_X86_64_JUMP_SLOT)
        DEFINE_CASE(R_X86_64_RELATIVE)
        DEFINE_CASE(R_AARCH64_GLOB_DAT)
        DEFINE_CASE(R_AARCH64_JUMP_SLOT)
        default: return "UNKNOWN";
    }
}