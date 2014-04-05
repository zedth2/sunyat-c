
#ifndef _DEBUGER_H_
#define _DEBUGER_H_

#define DEBUG_PAUSE_KEY KEY_BREAK

void memory_win(WINDOW *mem, int x, int y, int wid, int height) ;
WINDOW* main_win_debug() ;
int debug_setup() ;
void write_reg_watcher() ;
void debug_pause() ;

#endif
