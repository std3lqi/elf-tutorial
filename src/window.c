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

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "elf-table.h"
#include "window.h"

// All windows
WINDOW_DATA windows_data[MAX_WINDOWS];
int window_count = 0;

static int current_window_index = -1;

WINDOW_DATA *create_new_win(char* id, int height, int width, int starty, int startx)
{
    WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);

    current_window_index = window_count;
    windows_data[current_window_index].id = id;
    windows_data[current_window_index].height = height;
    windows_data[current_window_index].width = width;
    windows_data[current_window_index].win = local_win;
    windows_data[current_window_index].cur_line = 0;
    windows_data[current_window_index].table = NULL;
    window_count++;

	return &(windows_data[current_window_index]);
}

void destroy_all_windows() {
	for (int i = window_count - 1; i >= 0; i--) {
		WINDOW_DATA* win_data = &(windows_data[i]);
        free_table(win_data->table);
		delwin(win_data->win);
        window_count--;
	}
    current_window_index = -1;
}

void set_table(WINDOW_DATA* win_data, TABLE* table) {
    free_table(win_data->table);
    win_data->table = table;
    win_data->visible_start = 0;
    win_data->visible_end = 0;
    if (table) {
        if (table->count > get_table_rows(win_data)) {
            win_data->visible_end = get_table_rows(win_data) - 1;
        } else {
            win_data->visible_end = table->count;
        }
    }
}

void next_win() {
    current_window_index = (current_window_index + 1 + window_count) % window_count;
}

WINDOW_DATA* current_win() {
    return &(windows_data[current_window_index]);
}

void set_current_win_index(int index) {
    current_window_index = index;
}

void adjust_visible_area(WINDOW_DATA* win_data) {
    while (win_data->cur_line > win_data->visible_end) {
        win_data->visible_start++;
        win_data->visible_end++;
    }
    while (win_data->cur_line < win_data->visible_start) {
        win_data->visible_start--;
        win_data->visible_end--;
    }
}

void next_line(WINDOW_DATA* win_data) {
    win_data->cur_line = (win_data->cur_line + 1 + win_data->table->count) % win_data->table->count;
    adjust_visible_area(win_data);
}

void prev_line(WINDOW_DATA* win_data) {
    win_data->cur_line = (win_data->cur_line - 1 + win_data->table->count) % win_data->table->count;
    adjust_visible_area(win_data);
}

char* create_blank_line(int length) {
    char* blank_line = (char*)malloc((length + 1) * sizeof(char));
    memset(blank_line, ' ', length);
    blank_line[length] = '\0';
    return blank_line;
}

int get_table_rows(WINDOW_DATA* win_data) {
    TABLE* table = win_data->table;
    int rows = win_data->height - 2;    // Exclude top/bottom borders 
    if (table->header) {
        rows -= 2;                      // Exclude header and separator line
    }
    return rows;
}

void render_window(WINDOW_DATA* win_data) {
    int starty = win_data->win->_begy;
    int startx = win_data->win->_begx; 
    int height = win_data->height;
    int width  = win_data->width;
    TABLE* table = win_data->table;
    bool overflow = FALSE;

    char* blank_line = create_blank_line(width);
    for (int i = 0; i < height; i++) {
        mvprintw(starty + i, startx, blank_line);        // Clear with blank line
    }
    free(blank_line);
    blank_line = NULL;

    // Render table
    if (table) {
        bool is_current_window = win_data == current_win();
        overflow = table->count > get_table_rows(win_data);
        int rows = table->count;
        if (overflow) {
            rows = get_table_rows(win_data);
        }

        int y = starty + 1;

        // Render header
        if (table->header) {
            mvprintw(y, startx + 2, table->header);         // Output header line
            y++;
            mvhline(y, startx + 1, ACS_HLINE, width - 1);   // Draw separator line
            y++;
        }
        
        for (int i = 0; i < height; i++) {
            if (i < rows) {
                int line_index = win_data->visible_start + i;
                if (is_current_window && line_index == win_data->cur_line) {
                    attron(A_REVERSE);                      // Highlight the current line in the current window
                }
                mvprintw(y + i, startx + 2, table->entries[line_index].name);
                if (is_current_window && line_index == win_data->cur_line) {
                    attroff(A_REVERSE);                     // Disable highlighting
                }
            }
        }
    }

    // Render border
    int endx = startx + width - 1;
    for (int i = 0; i < height; i++) {
        if (i == 0) {
            mvaddch(starty + i, startx, ACS_ULCORNER);
            // Render title
            if (win_data->title) {
                int title_len = strlen(win_data->title);
                mvaddstr(starty + i, startx + 2, win_data->title);
                mvhline(starty + i, startx + 2 + title_len, ACS_HLINE, width - 2 - title_len);
            } else {
                mvhline(starty + i, startx + 1, ACS_HLINE, width - 1);
            }
            mvaddch(starty + i, endx, ACS_URCORNER);
        } else if (i == height - 1) {
            mvaddch(starty + i, startx, ACS_LLCORNER);
            if (overflow) {
                char footer[128];
                int visible_start = win_data->visible_start + 1;
                int visible_end = win_data->visible_end + 1;
                int cur_line = win_data->cur_line + 1;
                sprintf(footer, " %d of %d, %d-%d ", cur_line, table->count, visible_start, visible_end);
                int footer_len = strlen(footer);
                mvaddstr(starty + i, startx + 2, footer);
                mvhline(starty + i, startx + 2 + footer_len, ACS_HLINE, width - 2 - footer_len);
            } else {
                mvhline(starty + i, startx + 1, ACS_HLINE, width - 1);
            }
            mvaddch(starty + i, endx, ACS_LRCORNER);
        } else {
            mvaddch(starty + i, startx, ACS_VLINE);
            mvaddch(starty + i, endx, ACS_VLINE);
        }
    }
}
