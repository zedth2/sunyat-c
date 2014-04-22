/*
	 * Name        : sat_scr.h
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

#ifndef SAT_SCR_H_
#define SAT_SCR_H_

#include <stdint.h>


/**
 *  Brief:
 *      This is the minimal width the terminal can be. Once it becomes
 *          less than this an error message pops up.
 */
#define TERMINAL_WIDTH  80

/**
 *  Brief:
 *      This is the minimal height the terminal can be. Once it becomes
 *          less than this an error message pops up.
 */
#define TERMINAL_HEIGHT 24

/**
 *  Brief:
 *      The size of the tab character to be used in ncures.
 */
#define TAB_SIZE         4



/**
 *  Brief:
 *      This will contain a window for sunyat.
 *
 *  Properties:
 *      win : *WINDOW
 *          A pointer to an ncurses window.
 *      cur_X : int
 *          The X position of the cursor.
 *      cur_Y : int
 *          The Y position of the cursor.
 *
 *
 *
 *
 */

typedef struct _SatWin {
    WINDOW *win ;
    int cur_X ;
    int cur_Y ;
    unsigned int max_X ;
    unsigned int max_Y ;
} SatWin ;




int setup_ncurses_terminal();
void terminal_init();
void terminal_restore();
SatWin* init_SatWin() ;
int print_array(SatWin *win, uint8_t arr[], unsigned int len, int id_start) ;
void get_W_H(SatWin *win, unsigned int *Width, unsigned int *Height) ;
//void print_array_regs(SatWin *win, uint8_t arr[], int len) ;
void print_sys_reg(SatWin *win, uint8_t arr[]) ;
#endif /* SAT_SCR_H_ */
