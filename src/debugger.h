
#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#include "sat_scr.h"

#define DEBUG_PAUSE_KEY 0x1B

SatWin *printf_debug_win ; ///This is a tempory window to print messages to.


void memory_win(WINDOW *mem, int x, int y, int wid, int height) ;
SatWin* main_win_debug() ;
int debug_setup() ;
void print_reg_win(SatWin *win) ;
int debug_pause() ;
static void erase_box(SatWin *win) ;
void write_mem_win() ;
void print_mem_win(SatWin *win, int mode) ;
void print_to_win(WINDOW *cwin, uint8_t *data, int len) ;
void write_reg_win() ;
static void check_cursor(SatWin *win, int strLen) ;
static void instruction_to_code(SatWin *win, uint8_t highBits, uint8_t lowBits) ;

static unsigned int cnt_to_reg_win(unsigned int cnt) ;

#endif
