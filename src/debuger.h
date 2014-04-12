
#ifndef _DEBUGER_H_
#define _DEBUGER_H_

#define DEBUG_PAUSE_KEY 0x1B

void memory_win(WINDOW *mem, int x, int y, int wid, int height) ;
WINDOW* main_win_debug() ;
int debug_setup() ;
void write_reg_watcher() ;
void debug_pause() ;
void write_mem_win() ;
void print_to_win(WINDOW *cwin, uint8_t *data, int len) ;
static void make_reg_labels(char *lbls[SIZE_WIN]);
#endif
