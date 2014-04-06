

#include <ncurses.h>
#include "sunyat.h"
#include "debuger.h"


extern uint8_t sunyat_regs [SIZE_REG] ;
extern uint8_t sunyat_ram [SIZE_APP_RAM];

WINDOW *reg_watcher ;
WINDOW *mem_win ;
/**
 *  brief:
 *      This will do the setup for ncurses for the debugger.
 */
int debug_setup(){
	if (NULL == initscr ()) {
		//printf (ERR_NCURSES_INIT);
		return -1;
	}

	if (ERR == cbreak ()) {
		//printf (ERR_NCURSES_CBREAK);
		return -1;
	}

	if (ERR == noecho ()) {
		//printf (ERR_NCURSES_NOECHO);
		return -1;
	}

	if (ERR == nodelay (stdscr, true)) {
		//printf (ERR_NCURSES_NODELAY);
		return -1;
	}

	if (ERR == keypad (stdscr, true)) {
		//printf (ERR_NCURSES_KEYPAD);
		return -1;
	}

	if (ERR == curs_set (1)) {
		//printf (ERR_NCURSES_CURSOR);
		return -1;
	}
}


/**
 * brief:
 *      This will initialize all the debugger screens and return
 *      the main one that the assemble program should write to.
 *
 * Returns:
 *      ncurses WINDOW
 */
WINDOW* main_win_debug(){
    printw("Press F1 to exit");
	refresh();
    int max_x = getmaxx(stdscr) ;
    int max_y = getmaxy(stdscr) ;
    WINDOW *win ;
    win = newwin(25,80,0,0) ;
    //box(win, 0, 0) ;
    wrefresh(win) ;

    mem_win = newwin(max_y, max_x - 80, 0, 80) ;
    box(mem_win, 0, 0) ;
    wrefresh(mem_win) ;

    reg_watcher = newwin(max_y - 25, 80, 25, 0) ;
    box(reg_watcher, 0, 0) ;
    wrefresh(reg_watcher) ;

    return win ;

    //WINDOW *wint = newwin(40,80,25,0) ;
    //box(wint, 0, 0) ;
    //wrefresh(wint) ;


    //int key,x=0,y=0 ;//= getch() ;
    //getmaxyx(stdscr, x, y) ;
    //printw("FUCKHER %d %d", getmaxx(stdscr),getmaxy(stdscr));
    //while ((key = getch()) != KEY_F(1)){
        //sleep(1) ; }



}



void write_reg_watcher(){
    //int r = 0 ;
    //int x = 0, y = 0 ;
    //getmaxyx(reg_watcher, x, y) ;
    //for(r = 0 ; SIZE_REG > r ; r++) {
        //mvwprintw(reg_watcher, r+2, 2, " %02d : 0x%02X ", r, sunyat_regs[r]) ;
    //}
    //wrefresh(reg_watcher) ;

    //init_pair(1, COLOR_RED, COLOR_BLACK);
    //print_to_win(reg_watcher, sunyat_regs, sunyat_regs[REG_WIN]) ;
    //printf("HERE %d %d \t", sunyat_regs, sunyat_regs[REG_WIN]) ;
	//attron(COLOR_PAIR(1));
    //print_to_win(reg_watcher, sunyat_regs+sunyat_regs[REG_WIN], SIZE_WIN) ;
    //printf("HERE2 %d %d \t", sunyat_regs+sunyat_regs[REG_WIN], SIZE_WIN) ;
	//attroff(COLOR_PAIR(1));
    //print_to_win(reg_watcher, sunyat_regs+(sunyat_regs[REG_WIN]+SIZE_WIN), SIZE_REG-(sunyat_regs[REG_WIN]+SIZE_WIN)) ;
    //printf("HERE3 %d %d \t", sunyat_regs+(sunyat_regs[REG_WIN]+SIZE_WIN), SIZE_REG-(sunyat_regs[REG_WIN]+SIZE_WIN)) ;

    print_to_win(reg_watcher, sunyat_regs, SIZE_REG) ;


}

void write_mem_win(){
    //int x = 0, y = 0 ;
    //getmaxyx(mem_win, x, y) ;
    //printf(" FUCKER X %d Y %d \n", x, y) ;
    //getmaxyx(reg_watcher, x, y) ;
    //printf(" FUCKER_REG X %d Y %d \n", x, y) ;
    print_to_win(mem_win, sunyat_ram, SIZE_APP_RAM) ;

    //int X = 0, Y = 0, cnt = 0, curCol = 2, curRow = 1, strLen = 10, colSpace = 2;
    //getmaxyx(mem_win, X, Y) ;
    //for( ; SIZE_APP_RAM > cnt ; ++cnt, ++curRow) {
        //mvwprintw(mem_win, curRow, curCol, "%03d : 0x%02X ", cnt, sunyat_ram[cnt]) ;
        ////for(curRow = 1 ; Y > curRow ; curRow++){
            ////mvwprintw(mem_win, curRow, curCol, "%03d : 0x%02X ", cnt+curRow-1, *(data+(1-curRow+cnt))) ;
        ////}
        //if (curRow >= X-2) {
            //curRow = 0 ;
            //curCol += strLen+colSpace ;
        //}
    //}
    //wrefresh(mem_win) ;



}

void debug_pause() {
    //printf("PAUSING\n") ;
    wmove(reg_watcher, 1, 1) ;
    while (DEBUG_PAUSE_KEY != getch()){
        sleep(1) ;
        //printf("sleeping\t") ;
    }

}

void print_to_win(WINDOW *cwin, uint8_t data[], int len){
    int X = 0, Y = 0, cnt = 0, curCol = 2, curRow = 1, strLen = 10, colSpace = 2;
    getmaxyx(cwin, X, Y) ;
    for( ; len > cnt ; ++cnt, ++curRow) {
        mvwprintw(cwin, curRow, curCol, "%03d : 0x%02X ", cnt, data[cnt]) ;
        //for(curRow = 1 ; Y > curRow ; curRow++){
            //mvwprintw(cwin, curRow, curCol, "%03d : 0x%02X ", cnt+curRow-1, *(data+(1-curRow+cnt))) ;
        //}
        if (curRow >= X-2) {
            curRow = 0 ;
            curCol += strLen+colSpace ;
        }
    }
    wrefresh(cwin) ;
}

//void memory_win(WINDOW *mem, int x, int y, int wid, int height) {
    //mem_win = newwin(wid, height, x, y) ;
    //box(mem, 0, 0) ;
    //wrefresh(mem) ;
//}
