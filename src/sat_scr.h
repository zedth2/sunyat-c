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
} SatWin ;




int setup_ncurses_terminal();
void terminal_init();
void terminal_restore();
SatWin* init_SatWin() ;
int print_array(SatWin *win, uint8_t arr[], int len, int id_start);
void get_W_H(SatWin *win, unsigned int *Width, unsigned int *Height) ;
void print_array_regs(SatWin *win, uint8_t arr[], int len) ;
#endif /* SAT_SCR_H_ */
