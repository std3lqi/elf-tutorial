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