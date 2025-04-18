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

#include "elf-reader.h"
#include <stdio.h>
#include <stdlib.h>

static unsigned char* buffer;

int read_elf_file(const char* file_path) {
  FILE* f = fopen(file_path, "rb");
  if (f == NULL) {
    return -1;
  }

  // Get file size
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);

  // Read file contents into buffer
  buffer = (unsigned char*) malloc(size);
  int bytes_read = fread(buffer, 1, size, f);
  if (bytes_read != size) {
    free(buffer);
    buffer = NULL;
    fclose(f);
    return -2;
  }

  fclose(f);
  return 0;
}

void close_elf_file() {
  free(buffer);
  buffer = NULL;
}

unsigned char* get_buffer(int offset) {
  return buffer + offset;
}

Elf64_Ehdr* get_elf_header() {
  return (Elf64_Ehdr*) get_buffer(0);
}
