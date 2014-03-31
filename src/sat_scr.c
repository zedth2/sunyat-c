/**
 * Author: Zachary Harvey
 * Brief:
 *      This will setup the ncurses windows.
 *
 *
 *
 *
 *
 */
#include <ncurses.h>
#include "sat_scr.h"

const char ERR_NCURSES_INIT [] =
	"\tCould not initialize ncurses\n";
const char ERR_NCURSES_CBREAK [] =
	"\tCould not disable character buffering\n";
const char ERR_NCURSES_NODELAY [] =
	"\tCould not disable blocking on \"getch\"\n";
const char ERR_NCURSES_NOECHO [] =
	"\tCould not disable echo\n";
const char ERR_NCURSES_KEYPAD [] =
	"\tCould not enable keypad usage\n";
const char ERR_NCURSES_CURSOR [] =
	"\tCould not modify cursor\n";

int cursor_row = 0;
int cursor_col = 0;
uint8_t terminal[TERMINAL_HEIGHT][TERMINAL_WIDTH + 1];

int setup_ncurses_terminal () {
	if (NULL == initscr ()) {
		printf (ERR_NCURSES_INIT);
		return -1;
	}

	if (ERR == cbreak ()) {
		printf (ERR_NCURSES_CBREAK);
		return -1;
	}

	if (ERR == noecho ()) {
		printf (ERR_NCURSES_NOECHO);
		return -1;
	}

	if (ERR == nodelay (stdscr, true)) {
		printf (ERR_NCURSES_NODELAY);
		return -1;
	}

	if (ERR == keypad (stdscr, true)) {
		printf (ERR_NCURSES_KEYPAD);
		return -1;
	}

	if (ERR == curs_set (1)) {
		printf (ERR_NCURSES_CURSOR);
		return -1;
	}

	return 0;
}



void terminal_init() {
	int y;
	int x;

	for (y = 0; y < TERMINAL_HEIGHT; y++) {
		for (x = 0; x < TERMINAL_WIDTH; x++) {
			terminal [y][x] = ' ';
		}
		terminal [y][x] = '\0';
	}
}

void terminal_restore() {
	int y;

	erase ();

	for (y = 0; y < TERMINAL_HEIGHT; y++) {
		mvprintw( y, 0, (const char *)(terminal [y]));
	}

	move (cursor_row, cursor_col);
}
