
#ifndef _DEBUGER_H_
#define _DEBUGER_H_

#include "sat_scr.h"

#define DEBUG_PAUSE_KEY 0x1B

void memory_win(WINDOW *mem, int x, int y, int wid, int height) ;
SatWin* main_win_debug() ;
int debug_setup() ;
void print_reg_win(SatWin *win) ;
int debug_pause(uint8_t opcode, uint8_t sreg, uint8_t dreg, uint8_t mem, int8_t imm, uint8_t cmp_result) ;
void write_mem_win() ;
void print_mem_win(SatWin *win, int mode) ;
void print_to_win(WINDOW *cwin, uint8_t *data, int len) ;
void write_reg_win() ;
static void check_cursor(SatWin *win, int strLen) ;
static void instruction_to_code(SatWin *win) ;
#endif
