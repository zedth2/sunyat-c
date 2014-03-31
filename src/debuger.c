

#include <ncurses.h>
#include "sunyat.h"
#include "debuger.h"

int main(int argc, char *argv[]){
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

	printw("Press F1 to exit");
	refresh();
    int max_x = getmaxx(stdscr) ;
    int max_y = getmaxy(stdscr) ;
    WINDOW *win ;
    win = newwin(25,80,0,0) ;
    box(win, 0, 0) ;
    wrefresh(win) ;

    WINDOW *mem ;
    mem = newwin(max_y, max_x - 80, 0, 80) ;
    box(mem, 0, 0) ;
    wrefresh(mem) ;

    WINDOW *reg_watcher = newwin(max_y - 25, 80, 25, 0) ;
    box(reg_watcher, 0, 0) ;
    wrefresh(reg_watcher) ;



    //WINDOW *wint = newwin(40,80,25,0) ;
    //box(wint, 0, 0) ;
    //wrefresh(wint) ;


    int key,x=0,y=0 ;//= getch() ;
    getmaxyx(stdscr, x, y) ;
    //printw("FUCKHER %d %d", getmaxx(stdscr),getmaxy(stdscr));
    while ((key = getch()) != KEY_F(1)){
        sleep(1) ; }
}


void memory_win(WINDOW *mem, int x, int y, int wid, int height) {
    mem = newwin(wid, height, x, y) ;
    box(mem, 0, 0) ;
    wrefresh(mem) ;
}
