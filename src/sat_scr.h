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



int setup_ncurses_terminal();
void terminal_init();
void terminal_restore();

#endif /* SAT_SCR_H_ */
