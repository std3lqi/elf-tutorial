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

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <elf.h>
#include <string.h>

#include "elf-reader.h"
#include "elf-table.h"
#include "window.h"
#include "main.h"

extern int window_count;
extern WINDOW_DATA windows_data[MAX_WINDOWS];

#define LEFT_WIN_ID 		"left-win"
#define RIGHT_WIN_ID 		"right-win"

void render_all_windows() {
	for (int i = 0; i < window_count; i++) {
		WINDOW_DATA* win_data = &(windows_data[i]);
		if (strcmp(current_win()->id, LEFT_WIN_ID) == 0 &&
			strcmp(win_data->id, RIGHT_WIN_ID) == 0) {
			int cur_line = current_win()->cur_line;
			if (cur_line == 0) {
				// ELF Header
				set_table(win_data, create_elf_header_table());
			} else if (cur_line == 1) {
				// Program Header Table
				set_table(win_data, create_program_headers_table());
			} else {
				// Show nothing
				set_table(win_data, NULL);
			}
		}
		render_window(win_data);
	}
}

int prepare_windows(TABLE* left_table, TABLE* right_table) {
    
	int screen_height = 26;
	int screen_width = 120;

	int startx = 0;				// Specify the left-upper corner of window
	int starty = 0;				// Specify the left-upper corner of window
	int ch;						// Read char from keyboard

	// Set up ncurses mode
	initscr();										// Initialize screen to ncurses mode
	getmaxyx(stdscr, screen_height, screen_width);	// Get current screen rows and columns
	cbreak();										// Disable line buffering
	keypad(stdscr, TRUE);							// Allow function key input
	noecho();										// Do not display any input from keyboard

	int left_width = 52;							// Width of the left window
	int right_width = screen_width - left_width;	// Width of the right window

	// Create the left window
    WINDOW_DATA* win1 = create_new_win(LEFT_WIN_ID, screen_height, left_width, starty, startx);
	win1->title = "ELF File";
	set_table(win1, left_table);

	// Create the right window
	startx += left_width;							// Adjust startx for the right window
	WINDOW_DATA* win2 = create_new_win(RIGHT_WIN_ID, screen_height, right_width, starty, startx);
	win2->title = "Contents";
	set_table(win2, right_table);

	// Set the left window as the current one
	set_current_win_index(0);

	// Render all windows
	render_all_windows();

	// Event loop
	while((ch = getch()) != CHAR_QUIT)
	{	
		switch(ch)
		{	
			case CHAR_NEXT_WINDOW:
				next_win();
				render_all_windows();
				break;
			case CHAR_TOGGLE_HEX:
				render_all_windows();
				break;
			case CHAR_TOGGLE_EXPAND_SEGMENT:
				render_all_windows();
				break;
			case KEY_UP:
				prev_line(current_win());
				render_all_windows();
				break;
			case KEY_DOWN:
				next_line(current_win());
				render_all_windows();
				break;	
		}
	}
	
	// Exit ncurses mode
	endwin();
	return 0;
}

int main(int argc, char *argv[])
{
	int rc = read_elf_file(argv[1]);
	if (rc < 0) {
		printf("Failed to read file: %d\n", rc);
		return rc;
	}
    TABLE* left_table = create_elf_file_table();
    TABLE* right_table = create_elf_header_table();
	prepare_windows(left_table, right_table);
	destroy_all_windows();
    return 0;
}
