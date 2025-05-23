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
    Elf64_Ehdr* header = get_elf_header();
    TABLE* table = create_table(3 + header->e_shnum, NULL);
    char* string_table = get_section_name_string_table();
    int i = 0;
    sprintf(table->entries[i++].name, "0x%08lX-%08lX *** ELF Header ***", 0, sizeof(Elf64_Ehdr));
    sprintf(table->entries[i++].name, "0x%08lX-%08lX *** Program Header Table ***",
        header->e_phoff, header->e_phoff + header->e_phentsize * header->e_phnum);
    for (int j = 0; j < header->e_shnum; j++) {
        Elf64_Shdr* shdr = get_section_header(j);
        char* name = string_table + shdr->sh_name;
        Elf64_Xword size = shdr->sh_size;
        if (shdr->sh_type == SHT_NOBITS) {
            size = 0;
        }
        sprintf(table->entries[i++].name, "0x%08lX-%08lX %s %s",
            shdr->sh_offset, shdr->sh_offset + size,
            get_section_type_name(shdr->sh_type), name);
    }
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
    char* title = "Index|NAME            | TYPE        |FLG| VIRT ADDR| OFFSET   |SIZE |LNK|INF|ALIGN|ENTSZ";
    //            "   1: .init            PROGBITS      E A 0x004005F0 0x000005F0 0x018 000 000 0x004 000"
    TABLE* table = create_table(header->e_shnum, title);
    Elf64_Shdr* shdr = (Elf64_Shdr*)get_buffer(header->e_shoff);
    char* string_table = get_section_name_string_table();
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
        char* name = string_table + shdr->sh_name;
        sprintf(
            table->entries[i].name,
            "%4d: %-16s %s %c%c%c 0x%08lX 0x%08lX 0x%03X %03d %03d 0x%03X %03d",
            i,
            name,
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

TABLE* create_string_table(Elf64_Shdr* shdr) {
    char* title = "Index|String";
    int count = 0;
    char* string_table = get_buffer(shdr->sh_offset);
    for (int i = 0; i < shdr->sh_size; i++) {
        if (string_table[i] == '\0') {
            count++;
        }
    }
    int index = 0;  // Table entry index
    int s_begin = 0;// String start position
    TABLE* table = create_table(count, title);
    for (int i = 0; i < shdr->sh_size; i++) {
        if (string_table[i] == '\0') {
            count++;
            sprintf(
                table->entries[index].name,
                "%4d: %s",
                index,
                string_table + s_begin
            );
            s_begin = i + 1;
            index++;
        }
    }
    return table;
}

TABLE* create_symbol_table(Elf64_Shdr* shdr) {
    char* title = "Index| TYPE  | BIND |VISIBILITY|SHNDX| VALUE    | SIZE | Name";
    //            "   26: FILE    LOCAL  DEFAULT    65521 0x00000000 0x0000 crt1.o"
    int count = shdr->sh_size / shdr->sh_entsize;
    TABLE* table = create_table(count, title);
    Elf64_Sym* sym = (Elf64_Sym*) get_buffer(shdr->sh_offset);
    Elf64_Shdr* shdr_string_table = get_section_header(shdr->sh_link);
    char* string_table = (char*)get_buffer(shdr_string_table->sh_offset);
    for (int i = 0; i < count; i++) {
        // Elf64_Word st_name; /* Symbol name (string tbl index) */
        // unsigned char st_info; /* Symbol type and binding */
        // unsigned char st_other; /* Symbol visibility */
        // Elf64_Section st_shndx /* Section index */
        // Elf64_Addr st_value; /* Symbol value */
        // Elf64_Xword st_size; /* Symbol size */        
        sprintf(
            table->entries[i].name,
            "%4d: %s %s %s % 6d 0x%08lX 0x%04X %s",
            i,
            get_symbol_type(sym->st_info),
            get_symbol_binding(sym->st_info),
            get_symbol_visibility(sym->st_other),
            sym->st_shndx,
            sym->st_value,
            sym->st_size,
            string_table + sym->st_name
        );
        sym++;
    }
    return table;
}

TABLE* create_dynamic_table(Elf64_Shdr* shdr) {
    char* title = "Index| TAGVALUE | TAG      | VALUE";
    //            ""
    int count = shdr->sh_size / shdr->sh_entsize;
    TABLE* table = create_table(count, title);
    Elf64_Dyn* dyn = (Elf64_Dyn*) get_buffer(shdr->sh_offset);
    Elf64_Shdr* shdr_string_table = get_section_header(shdr->sh_link);
    char* string_table = (char*)get_buffer(shdr_string_table->sh_offset);
    for (int i = 0; i < count; i++) {
        char s[1024];
        sprintf(s, "0x%08lX", dyn->d_un.d_ptr);
        if (dyn->d_tag == DT_NEEDED) {
            sprintf(s, "%s", string_table + dyn->d_un.d_val);
        }
        sprintf(
            table->entries[i].name,
            "% 4d: 0x%08X %s %s",
            i,
            dyn->d_tag,
            get_dynamic_tag(dyn->d_tag),
            s
        );
        dyn++;
    }
    return table;
}

TABLE* create_hash_table(Elf64_Shdr* shdr) {
    Elf64_Word* array = (Elf64_Word*) get_buffer(shdr->sh_offset);
    Elf64_Word nbucket = array[0];
    Elf64_Word nchain = array[1];
    int count = 2 + nbucket;
    int bucket_base = 2;
    int chain_base = bucket_base + nbucket;
    TABLE* table = create_table(count, NULL);
    int i = 0;
    sprintf(table->entries[i++].name, "Buckets   : %d", nbucket);
    sprintf(table->entries[i++].name, "Chain     : %d", nchain);
    for (int j = 0; j < nbucket; j++) {
        int symbols = 0;
        int symbol_index = array[bucket_base + j];
        while (symbol_index > 0) {
            symbols++;
            symbol_index = array[chain_base + symbol_index];
        }
        sprintf(table->entries[i++].name, "Bucket[%02d]: %d", j, symbols);
    }
    return table;
}

TABLE* create_gnu_hash_table(Elf64_Shdr* shdr) {
    Elf64_Word* array = (Elf64_Word*) get_buffer(shdr->sh_offset);
    Elf64_Xword* array64 = (Elf64_Xword*) get_buffer(shdr->sh_offset);
    Elf64_Word nbucket = array[0];
    Elf64_Word sym_offset = array[1];
    Elf64_Word bloom_size = array[2];
    Elf64_Word bloom_shift = array[3];
    int count = 4 + bloom_size + nbucket;
    int bloom_base = 4;
    int bloom_base_64 = bloom_base / 2;
    int bucket_base = bloom_base + bloom_size * 2;
    int chain_base = bucket_base + nbucket;
    TABLE* table = create_table(count, NULL);
    int i = 0;
    sprintf(table->entries[i++].name, "Buckets      : %d", nbucket);
    sprintf(table->entries[i++].name, "Symbol Offset: %d", sym_offset);
    sprintf(table->entries[i++].name, "Bloom Size   : %d", bloom_size);
    sprintf(table->entries[i++].name, "Bloom Shift  : %d", bloom_shift);
    for (int j = 0; j < bloom_size; j ++) {
        Elf64_Xword mask = array64[bloom_base_64 + j];
        sprintf(table->entries[i++].name, "Bloom[%02d]    : 0x%016lX", j, mask);
    }
    for (int j = 0; j < nbucket; j++) {
        int symbols = 1;
        int symbol_index = array[bucket_base + j];
        int chain_index = symbol_index - sym_offset;
        while ((array[chain_base + chain_index] & 0x01) == 0) {
            symbols++;
            chain_index++;
        }
        sprintf(table->entries[i++].name, "Bucket[%02d]   : %d", j, symbols);
    }
    return table;
}

TABLE* create_note_table(Elf64_Shdr* shdr) {
    int count = 0;
    Elf64_Word offset = shdr->sh_offset;
    while (offset < shdr->sh_offset + shdr->sh_size) {
        Elf64_Nhdr* nhdr = (Elf64_Nhdr*)get_buffer(offset);
        offset += sizeof(Elf64_Nhdr) + nhdr->n_namesz + nhdr->n_descsz;
        count++;
    }
    offset = shdr->sh_offset;
    TABLE* table = create_table(count, NULL);
    for (int i = 0; i < count; i++) {
        Elf64_Nhdr* nhdr = (Elf64_Nhdr*)get_buffer(offset);
        offset += sizeof(Elf64_Nhdr);
        char* name = (char*)get_buffer(offset);
        offset += nhdr->n_namesz;
        Elf64_Word* desc = (Elf64_Word*)get_buffer(offset);
        offset += nhdr->n_descsz;

        char descriptor[1024];
        descriptor[0] = '\0';
        switch(nhdr->n_type) {
            case NT_GNU_ABI_TAG:
                sprintf(table->entries[i++].name, "%d: %s ABI %s %d.%d.%d",
                    i, name, get_note_os(desc[0]), desc[1], desc[2], desc[3]
                );
                break;
            case NT_GNU_BUILD_ID:
                Elf64_Word* desc = (Elf64_Word*)get_buffer(offset);
                for (int i = 0; i < nhdr->n_descsz; i++) {
                    sprintf(descriptor + i * 2, "%02X", desc[i]);
                }
                sprintf(table->entries[i++].name, "%d: %s BUILD ID %s",
                    i, name, descriptor
                );
                break;
            case NT_GNU_PROPERTY_TYPE_0:
                Elf64_Word* w = (Elf64_Word*)desc;
                if (w[0] == GNU_PROPERTY_X86_FEATURE_1_AND) {
                    char* shstk = "";
                    char* ibt = "";
                    if (w[2] & GNU_PROPERTY_X86_FEATURE_1_IBT) {
                        ibt = "IBT";
                    }
                    if (w[2] * GNU_PROPERTY_X86_FEATURE_1_SHSTK) {
                        shstk = "SHSTK";
                    }
                    sprintf(descriptor, "X86 Features: %s %s", ibt, shstk);
                    sprintf(table->entries[i++].name, "%d: %s PROPERTY %s",
                        i, name, descriptor
                    );                    
                    break;
                }
            case NT_GNU_GOLD_VERSION:
            case NT_GNU_HWCAP:
            default:
                sprintf(table->entries[i++].name, "%d: %s %d %d",
                    i, name, nhdr->n_type, nhdr->n_descsz
                );
        }
    }
    return table;
}

TABLE* create_gnu_verdef_table(Elf64_Shdr* shdr) {
    int count = 0;
    Elf64_Word offset = shdr->sh_offset;
    Elf64_Verdef* vd = NULL;
    do {
        vd = (Elf64_Verdef*)get_buffer(offset);
        count++;
        offset += vd->vd_next;
    } while (vd->vd_next > 0);

    char* title = "Index|VER|FLAG|NDX|CNT|HASH      |AUX";
    TABLE* table = create_table(count, title);

    Elf64_Shdr* shdr_string_table = get_section_header(shdr->sh_link);
    char* string_table = (char*)get_buffer(shdr_string_table->sh_offset);

    int i = 0;
    offset = shdr->sh_offset;
    do {
        vd = (Elf64_Verdef*)get_buffer(offset);
        // Elf64_Half vd_version; /* Version revision */
        // Elf64_Half vd_flags; /* Version information */
        // Elf64_Half vd_ndx; /* Version Index */
        // Elf64_Half vd_cnt; /* Number of associated aux entries */
        // Elf64_Word vd_hash; /* Version name hash value */
        // Elf64_Word vd_aux; /* Offset in bytes to verdaux array */
        // Elf64_Word vd_next; /* Offset in bytes to next verdef entry */
        char aux[1024]; aux[0] = '\0';
        Elf64_Off offset_aux = offset + vd->vd_aux;
        Elf64_Verdaux* vda = NULL;
        do {
            vda = (Elf64_Verdaux*)get_buffer(offset_aux);
            char* aux_name = string_table + vda->vda_name;
            sprintf(aux, "%s %s", aux, aux_name);
            offset_aux += vda->vda_next;
        } while (vda->vda_next > 0);
        sprintf(table->entries[i].name, "%4d: %3d %s %3d %3d 0x%08X %s",
            i,
            vd->vd_version,
            get_version_def_flags(vd->vd_flags),
            vd->vd_ndx,
            vd->vd_cnt,
            vd->vd_hash,
            aux
        );
        i++;
        offset += vd->vd_next;
    } while (vd->vd_next > 0);

    return table;
}

TABLE* create_gnu_versym_table(Elf64_Shdr* shdr) {
    int count = shdr->sh_size / shdr->sh_entsize;
    char* title = "Index|Version Index Number";
    TABLE* table = create_table(count, title);
    Elf64_Versym* sym = (Elf64_Versym*)get_buffer(shdr->sh_offset);
    for (int i = 0; i < count; i++) {
        sprintf(table->entries[i].name, "%4d: %d", i, sym[i]);
    }
    return table;
}

TABLE* create_gnu_verneed_table(Elf64_Shdr* shdr) {
    int count = 0;
    Elf64_Word offset = shdr->sh_offset;
    Elf64_Verneed* vn = NULL;
    do {
        vn = (Elf64_Verneed*)get_buffer(offset);
        count += vn->vn_cnt;
        offset += vn->vn_next;
    } while (vn->vn_next > 0);

    Elf64_Shdr* shdr_string_table = get_section_header(shdr->sh_link);
    char* string_table = (char*)get_buffer(shdr_string_table->sh_offset);

    char* title = "Index|VER|CNT|AUX HASH  |FLAG|OTH|FILE/VER NAME";
    TABLE* table = create_table(count, title);
    int i = 0;
    offset = shdr->sh_offset;
    do {
        // Elf64_Half vn_version; /* Version of structure */
        // Elf64_Half vn_cnt; /* Number of associated aux entries */
        // Elf64_Word vn_file; /* Offset of filename for this dependency */
        // Elf64_Word vn_aux; /* Offset in bytes to vernaux array */
        // Elf64_Word vn_next; /* Offset in bytes to next verneed entry */        
        vn = (Elf64_Verneed*)get_buffer(offset);
        Elf64_Off offset_aux = offset + vn->vn_aux;
        Elf64_Vernaux* vna = NULL;
        do {
            vna = (Elf64_Vernaux*)get_buffer(offset_aux);
            // Elf64_Word vna_hash; /* Hash value of dependency name */
            // Elf64_Half vna_flags; /* Dependency specific information */
            // Elf64_Half vna_other; /* Unused */
            // Elf64_Word vna_name; /* Dependency name string offset */
            // Elf64_Word vna_next; /* Offset in bytes to next vernaux entry */            
            sprintf(table->entries[i].name, "%4d: %3d %3d 0x%08X %s %3d %s %s",
                i,
                vn->vn_version,
                vn->vn_cnt,
                vna->vna_hash,
                get_version_def_flags(vna->vna_flags),
                vna->vna_other,
                string_table + vn->vn_file,
                string_table + vna->vna_name
            );
            i++;
            offset_aux += vna->vna_next;
        } while (vna->vna_next > 0);
        offset += vn->vn_next;
    } while (vn->vn_next > 0);
    return table;
}

TABLE* create_rela_table(Elf64_Shdr* shdr) {
    int count = shdr->sh_size / shdr->sh_entsize;
    char* title = "Index|OFFSET    |TYPE|SYM|ADDEND    |RELOCATION TYPE";
    TABLE* table = create_table(count, title);
    Elf64_Rela* rela = (Elf64_Rela*)get_buffer(shdr->sh_offset);
    for (int i = 0; i < count; i++) {
        // Elf64_Addr r_offset; /* Address */
        // Elf64_Xword r_info; /* Relocation type and symbol index */
        // Elf64_Sxword r_addend; /* Addend */        
        sprintf(table->entries[i].name, "%4d: 0x%08lX %4d %3d 0x%08lX %s",
            i,
            rela->r_offset,
            ELF64_R_TYPE(rela->r_info),
            ELF64_R_SYM(rela->r_info),
            rela->r_addend,
            get_relocation_type(rela->r_info)
        );
        rela++;
    }
    return table; 
}

TABLE* create_hex_table(Elf64_Shdr* shdr) {
    unsigned char* buf = get_buffer(shdr->sh_offset);
    int size = shdr->sh_size;
    int count = (size + 16 - 1) / 16;
    char* table_header = "Offset | 00 01 02 03 04 05 06 07 | 08 09 0A 0B 0C 0D 0E 0F";
    TABLE* table = create_table(count, table_header);

    for (int i = 0; i < table->count; i++) {
        char * row = table->entries[i].name;
        sprintf(row, "0x%04X  ", i * 16);
        row += 8;
        int cols = 16;
        if (i * 16 + 16 > size) {
            cols = size - i * 16;
        }
        for (int j = 0; j < cols; j ++) {
            if (j == 8) {
                sprintf(row, " |");    
                row += 2;
            }
            sprintf(row, " %02X", buf[j]);
            row += 3;
        }
        buf += 16;
    }
    return table;
}