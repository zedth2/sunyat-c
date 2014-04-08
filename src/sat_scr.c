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
#include <stdlib.h>
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

    if (has_colors()) {
        start_color() ;

    }
    init_pair(1, COLOR_RED, COLOR_BLACK);

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

SatWin* init_SatWin() {
    SatWin *win = malloc(sizeof(SatWin)) ;
    win->win = NULL ;
    win->cur_X = 0 ;
    win->cur_Y = 0 ;
}


int print_array(SatWin *win, uint8_t arr[], int len, int id_start) {
    int W = 0, H = 0, cnt = 0, strLen = 10 ;
    get_W_H(win, &W, &H) ;
    for (cnt = 0 ; len > cnt ; cnt++) {
        mvwprintw(win->win, win->cur_Y, win->cur_X, "%03d : 0x%02X ", id_start, arr[cnt]);
        (win->cur_Y)++ ;
        if (win->cur_Y >= W-2) {
            win->cur_Y = 1 ; //It must be two to get past the box outline.
            win->cur_X += strLen+win->cur_X ;
        }
        id_start++ ;
    }
    wrefresh(win->win) ;
    return id_start ;
}

void reset_cur(SatWin *win) {
    win->cur_X = 0 ;
    win->cur_Y = 0 ;
}

void get_W_H(SatWin *win, unsigned int *Width, unsigned int *Height) {
    getmaxyx(win->win, *Width, *Height) ;
}
