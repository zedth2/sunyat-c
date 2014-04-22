	/*
	 * Name        : debugger.h
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
#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#include "sat_scr.h"

#define DEBUG_PAUSE_KEY 0x1B ///The ESC key

#ifdef __DEBUG_ME_SUNYAT__
    SatWin *printf_debug_win ; ///This is a tempory window to print messages to.
#endif

SatWin *reg_win ; ///This is the SatWin holding the registers window.
SatWin *mem_win ; ///This is the SatWin holding the memory window.


void memory_win(WINDOW *mem, int x, int y, int wid, int height) ;
SatWin* main_win_debug() ;
int debug_setup() ;
void print_reg_win(SatWin *win) ;
int debug_pause() ;
static void erase_box(SatWin *win) ;
void write_mem_win() ;
void print_mem_win(SatWin *win, int mode) ;
void print_to_win(WINDOW *cwin, uint8_t data[], unsigned int len) ;
void write_reg_win() ;
static void check_cursor(SatWin *win, int strLen) ;
static void instruction_to_code(SatWin *win, uint8_t highBits, uint8_t lowBits) ;

static unsigned int cnt_to_reg_win(unsigned int cnt) ;

#endif
