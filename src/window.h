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

#ifndef WINDOW_H
#define WINDOW_H

#include <ncurses.h>
#include "elf-table.h"

#define MAX_WINDOWS 16

struct win_data {
    char* id;               // Window identifier
    char* title;            // Title
    WINDOW* win;            // Ncurses WINDOW object
    int height;             // Height
    int width;              // Width
    int cur_line;           // Current line index
    int visible_start;      // Start line of the visible area
    int visible_end;        // End line of the visible area
    TABLE* table;           // Contents shown in window
};
typedef struct win_data WINDOW_DATA;

WINDOW_DATA *create_new_win(char* id, int height, int width, int starty, int startx);
void destroy_all_windows();
void next_win();
WINDOW_DATA* current_win();
void set_current_win_index(int index);
void next_line(WINDOW_DATA* win_data);
void prev_line(WINDOW_DATA* win_data);

void set_table(WINDOW_DATA* win_data, TABLE* table);
int get_table_rows(WINDOW_DATA* win_data);
void render_window(WINDOW_DATA* win_data);

#endif
