/*
	 * Name        : sat_scr.c
	 * Author      : Zac Harvey
	 *
     * License     : Copyright (c) 2014 Zac Harvey
     *
     *    Permission is hereby granted, free of charge, to any person obtaining a
     *    copy of this software and associated documentation files (the "Software"),
     *    to deal in the Software without restriction, including without limitation
     *    the rights to use, copy, modify, merge, publish, distribute, sublicense,
     *    and/or sell copies of the Software, and to permit persons to whom the
     *    Software is furnished to do so, subject to the following conditions:
     *
     *    The above copyright notice and this permission notice shall be included in
     *    all copies or substantial portions of the Software.;
     *
     *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
     *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
     *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
     *    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
     *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
     *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
     *    DEALINGS IN THE SOFTWARE.
	 */
/**
 * Brief:
 *      This will setup the ncurses windows.
 */
#include <stdlib.h>
#include <ncurses.h>
#include "sat_scr.h"
#include "sunyat.h"

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

/**
 *  Brief:
 *      This will execute all the setup functions for ncurses.
 *
 *  Returns : int
 *      Returns 0 if nothing went wrong, -1 otherwise.
 */
int setup_ncurses_terminal () {
	if (NULL == initscr ()) {
		printf (ERR_NCURSES_INIT);
		return -1;
	}

    if (has_colors()) {
        start_color() ;
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
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    wbkgd(stdscr, COLOR_PAIR(1)) ;
	return 0;
}

/**
 *  Brief:
 *      This will initialized the main terminal array to spaces.
 *
 *  Notes:
 *      This should be ripped out and replaced with something.
 *      As of now, 04.21.2014 22:34, when something changes on the
 *      screen the array is changed and ncurses is forced to redraw
 *      the whole screen.
 */
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

/**
 *  Brief:
 *      This will clear out the entire main window.
 */
void terminal_restore() {
	int y;

	erase ();

	for (y = 0; y < TERMINAL_HEIGHT; y++) {
		mvprintw( y, 0, (const char *)(terminal [y]));
	}

	move (cursor_row, cursor_col);
}

/**
 *  Brief:
 *      This will initialize a SatWin. malloc for it then set cur_X and
 *      cur_Y to 0 and set win to NULL.
 *
 *  Returns : SatWin*
 *      The initiliazed SatWin.
 */
SatWin* init_SatWin() {
    SatWin *win = malloc(sizeof(SatWin)) ;
    win->win = NULL ;
    win->cur_X = 0 ;
    win->cur_Y = 0 ;
}

/**
 *  Brief:
 *      This will print the array out to the window.
 *      It will print the value of the array with a counter on the left...
 *      ID  : 0xFF
 *
 *  Parameters:
 *      win : *WINDOW
 *          An ncurses window to print out to.
 *
 *      arr : uint8_t[]
 *          The array of data to print out.
 *
 *      len : unsigned int
 *          The length of the array.
 *
 *      id_start : int
 *          The ID to start with.
 */
int print_array(SatWin *win, uint8_t arr[], unsigned int len, int id_start) {
    int cnt = 0, strLen = 10 ;
    for (cnt = 0 ; len > cnt ; cnt++) {
        mvwprintw(win->win, win->cur_Y, win->cur_X, "%03d : 0x%02X ", id_start, arr[cnt]);
        (win->cur_Y)++ ;
        if (win->cur_Y >= (win->max_Y)-2) {
            win->cur_Y = 1 ; //It must be two to get past the box outline.
            win->cur_X += strLen+2 ;
        }
        id_start++ ;
    }
    wrefresh(win->win) ;
    return id_start ;
}

/**
 *  Brief:
 *      Will reset cur_X and cur_Y to 0.
 *
 *  Parameters:
 *      win : *SatWin
 *          A pointer to the SatWin to reset.
 */
void reset_cur(SatWin *win) {
    win->cur_X = 0 ;
    win->cur_Y = 0 ;
}

/**
 *  Brief:
 *      This will find the max Y and X and height and width respectively.
 *
 *  Parameters:
 *      win : *SatWin
 *          The window to check.
 *
 *      Width : *unsigned int
 *          This will be the number of columns in the window.
 *
 *      Height : *unsigned int
 *          This will be the number of rows in the window.
 */
void get_W_H(SatWin *win, unsigned int *Width, unsigned int *Height) {
    getmaxyx(win->win, *Width, *Height) ;
}
