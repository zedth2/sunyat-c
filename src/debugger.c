

#include <ncurses.h>
#include <stdlib.h>
#include "sunyat.h"
#include "debugger.h"



extern uint8_t sunyat_regs [SIZE_REG] ;
extern uint8_t sunyat_ram [SIZE_APP_RAM];

SatWin *reg_win ; ///This is the SatWin holding the registers window.
SatWin *mem_win ; ///This is the SatWin holding the memory window.
extern SatWin *main_win ; ///The main window for the assembly program. Defined in sunyat.{h,c}

int curMode = 0 ; ///This will keep the memory window in whatever mode is was before.

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
SatWin* main_win_debug(){
    printw("Press F1 to exit");
	refresh();
    int max_x = getmaxx(stdscr) ;
    int max_y = getmaxy(stdscr) ;

    main_win = init_SatWin() ;
    main_win->win = newwin(25, 80, 0, 0) ;
    getmaxyx(main_win->win, main_win->max_Y, main_win->max_X) ;

    reg_win = init_SatWin() ;
    reg_win->win = newwin(max_y - 25, 80, 25, 0) ;
    getmaxyx(reg_win->win, reg_win->max_Y, reg_win->max_X) ;

    box(reg_win->win, 0, 0) ;
    scrollok(reg_win->win, TRUE) ;
    wrefresh(reg_win->win) ;

    mem_win = init_SatWin() ;
    mem_win->win = newwin(max_y, max_x - 80, 0, 80) ;
    getmaxyx(mem_win->win, mem_win->max_Y, mem_win->max_X) ;

    box(mem_win->win, 0, 0) ;
    scrollok(mem_win->win, TRUE) ;
    wrefresh(mem_win->win) ;


    return main_win ;
}


/**
 *  Brief:
 *      This will print out the registers and their values. Giving
 *      the system registers proper labels and giving color to the
 *      register window.
 *
 *  Parameters:
 *      win : *SatWin
 *          The window to print everything out to. If NULL
 *          it defaults to reg_win.
 */
void print_reg_win(SatWin *win) {
    if (!win) win = reg_win ;
    erase_box(win) ;
    win->cur_X = 2 ;
    win->cur_Y = 1 ;
    int cnt = 0, strLen = 10 ;
    init_pair(2, COLOR_GREEN, COLOR_BLACK) ;
    for (; SIZE_REG > cnt ; cnt++) {
        switch(cnt){
            case REG_PC:
            {
                mvwprintw(win->win, win->cur_Y, win->cur_X, " PC : 0x%02X ", sunyat_regs[cnt]);
                break ;
            }
            case REG_IRH:
            {
                mvwprintw(win->win, win->cur_Y, win->cur_X, "IRH : 0x%02X ", sunyat_regs[cnt]);
                break ;
            }
            case REG_IRL:
            {
                mvwprintw(win->win, win->cur_Y, win->cur_X, "IRL : 0x%02X ", sunyat_regs[cnt]);
                break ;
            }
            case REG_WIN:
            {
                mvwprintw(win->win, win->cur_Y, win->cur_X, "WIN : 0x%02X ", sunyat_regs[cnt]);
                break ;
            }
            case REG_SP:
            {
                mvwprintw(win->win, win->cur_Y, win->cur_X, " SP : 0x%02X ", sunyat_regs[cnt]);
                break ;
            }
            default:
            {
                if (sunyat_regs[REG_WIN] <= cnt && sunyat_regs[REG_WIN]+SIZE_WIN > cnt){
                    wattron(win->win, COLOR_PAIR(2));
                    mvwprintw(win->win, win->cur_Y, win->cur_X, "R%02d : 0x%02X ", cnt-sunyat_regs[REG_WIN], sunyat_regs[cnt]);
                    //int wcnt = 0 ;
                    //for(; SIZE_WIN > wcnt ; wcnt++) {
                        //mvwprintw(win->win,
                    //}
                    //print_array(win, &(sunyat_regs[cnt]), SIZE_WIN, 0) ;
                    //cnt += SIZE_WIN ;
                    wattroff(win->win, COLOR_PAIR(2));
                } /*else if (sunyat_regs[REG_PC] == cnt || sunyat_regs[REG_PC]-1 == cnt) {
                    wattron(win->win, COLOR_PAIR(2));
                    mvwprintw(win->win, win->cur_Y, win->cur_X, "%02d : 0x%02X ", cnt, sunyat_regs[cnt]);
                    wattroff(win->win, COLOR_PAIR(2));
                }*/
                else {
                    mvwprintw(win->win, win->cur_Y, win->cur_X, "%03d : 0x%02X ", cnt, sunyat_regs[cnt]);
                }
                break ;
            }
        }
        (win->cur_Y)++ ;
        check_cursor(win, strLen) ;
    }
    wrefresh(win->win) ;
}

static void check_cursor(SatWin *win, int strLen) {
    if (win->cur_Y >= (win->max_Y)-2) {
        win->cur_Y = 1 ; //It must be two to get past the box outline.
        win->cur_X += strLen+2 ;
    }
}


void write_mem_win(){
    print_mem_win(mem_win, curMode) ;
}


/**
 *  Brief:
 *      This will print data out to win. The function assumes it's the
 *      ncurses window showing memory.
 *
 *  Parameters:
 *      win : *SatWin
 *          A pointer to the window to print to.
 *
 *      arr : uint8_t[]
 *          The array to print out to the window.
 *
 *      mode : int
 *          This will set the mode to print out.
 *              0 : Will print out all memory with IRL and IRH colored.
 *              1 : Will print a list of memory with IRL and IRH in the middle.
 *                  These will be decoded and reconstruct a code line.
 */
void print_mem_win(SatWin *win, int mode) {
    if(!win) win = mem_win ;
    init_pair(2, COLOR_GREEN, COLOR_BLACK) ;
    win->cur_X = 2 ;
    win->cur_Y = 1 ;
    if (1 == mode) {
        erase_box(win) ;
        int irh = sunyat_regs[REG_PC]-2, irl = sunyat_regs[REG_PC]-1, id = (irh % 2) + (irh - ((win->max_Y) / 2)) ;
        if (id < 0) id = 0 ;
        //else if(id == 0 || id % 2 == 0) id++ ;
        for (;(win->max_Y)-1 > win->cur_Y && SIZE_APP_RAM > id ; id += 2) {
            if (irh == id) {
                wattron(win->win, COLOR_PAIR(2)) ;
                mvwprintw(win->win, win->cur_Y, win->cur_X, "IRH : 0x%02X_%02X ", sunyat_ram[id], sunyat_ram[id+1]) ;
                win->cur_X += 13 ;
                instruction_to_code(win, sunyat_ram[id], sunyat_ram[id+1]) ;
                win->cur_X = 2 ;
                wattroff(win->win, COLOR_PAIR(2)) ;
            } /*else if (irl == id) {
                wattron(win->win, COLOR_PAIR(2)) ;
                mvwprintw(win->win, win->cur_Y, win->cur_X, "IRL : 0x%02X ", sunyat_ram[id]) ;
                win->cur_X += 13 ;
                instruction_to_code(win, sunyat_ram[id], sunyat_ram[id+1]) ;
                win->cur_X = 2 ;
                wattroff(win->win, COLOR_PAIR(2)) ;
            } */else {
                mvwprintw(win->win, win->cur_Y, win->cur_X, "%03d : 0x%02X_%02X ", id, sunyat_ram[id], sunyat_ram[id+1]) ;
                win->cur_X += 13 ;
                instruction_to_code(win, sunyat_ram[id], sunyat_ram[id+1]) ;
                win->cur_X = 2 ;
            }
            win->cur_Y++ ;
            //check_cursor(win, 10) ;
        }
        curMode = 1 ;
    }
    else { //Default to 0
        if (curMode != 0) erase_box(win) ;
        int id = 0, strLen = 10 ;
        id = print_array(win, sunyat_ram, sunyat_regs[REG_PC]-2, id) ;
        wattron(win->win, COLOR_PAIR(2)) ;
        mvwprintw(win->win, win->cur_Y, win->cur_X, "IRH : 0x%02X ", sunyat_ram[sunyat_regs[REG_PC]-2]) ;
        win->cur_Y++ ;
        check_cursor(win, strLen) ;
        mvwprintw(win->win, win->cur_Y, win->cur_X, "IRL : 0x%02X ", sunyat_ram[sunyat_regs[REG_PC]-1]) ;
        win->cur_Y++ ;
        check_cursor(win, strLen) ;
        wattroff(win->win, COLOR_PAIR(2)) ;
        id = print_array(win, &(sunyat_ram[sunyat_regs[REG_PC]]), 1+SIZE_APP_RAM-sunyat_regs[REG_PC], sunyat_regs[REG_PC]) ;
        curMode = 0 ;
    }
    wrefresh(win->win) ;
}




int debug_pause() {
    //printf("PAUSING\n") ;
    wmove(reg_win->win, 1, 1) ;
    int curKey = 0, pause_again = 0 ;
    while (DEBUG_PAUSE_KEY != (curKey = getch()) && !pause_again){
        if (KEY_F(7) == curKey) {
            print_mem_win(mem_win, !curMode) ;
        } else if (KEY_F(8) == curKey) {
            pause_again = 1 ;
        }
        sleep(1) ;
        //printf("sleeping\t") ;
    }
    erase_box(mem_win) ;
    curMode = (pause_again && curMode) ;
    return pause_again ;
}

static void erase_box(SatWin *win) {
    werase(win->win) ;
    box(win->win, 0, 0) ;

}

static void instruction_to_code(SatWin *win, uint8_t highBits, uint8_t lowBits) {
    switch(get_opcode(highBits)) {
    case OPCODE_MOV_RR:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " MOV R%d R%d ", get_dreg(highBits), get_sreg(lowBits)) ;
		break ;
    }
    case OPCODE_MOV_RI:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " MOV R%d %d ", get_dreg(highBits), get_imm(lowBits)) ;
		break ;
    }
    case OPCODE_ADD_RR:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " ADD R%d R%d ", get_dreg(highBits), get_sreg(lowBits)) ;
		break ;
    }
    case OPCODE_ADD_RI:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " ADD R%d %d ", get_dreg(highBits), get_imm(lowBits)) ;
		break ;
    }
    case OPCODE_SUB_RR:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " SUB R%d R%d ", get_dreg(highBits), get_sreg(lowBits)) ;
		break ;
    }
    case OPCODE_MUL_RR:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " MUL R%d R%d ", get_dreg(highBits), get_sreg(lowBits)) ;
		break ;
    }
    case OPCODE_MUL_RI:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " MUL R%d %d ", get_dreg(highBits), get_imm(lowBits)) ;
		break ;
    }
    case OPCODE_DIV_RR:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " DIV R%d R%d ", get_dreg(highBits), get_sreg(lowBits)) ;
		break ;
    }
    case OPCODE_DIV_RI:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " DIV R%d %d ", get_dreg(highBits), get_imm(lowBits)) ;
		break ;
    }
    case OPCODE_CMP_RR:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " CMP R%d R%d ", get_dreg(highBits), get_sreg(lowBits)) ;
		break ;
    }
    case OPCODE_CMP_RI:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " CMP R%d %d ", get_dreg(highBits), get_imm(lowBits)) ;
		break ;
    }
    case OPCODE_JMP_M:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " JMP 0x%X ", get_mem(lowBits)) ;
		break ;
    }
    case OPCODE_JEQ_M:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " JEQ 0x%X ", get_mem(lowBits)) ;
		break ;
    }
    case OPCODE_JNE_M:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " JNE 0x%X ", get_mem(lowBits)) ;
		break ;
    }
    case OPCODE_JGR_M:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " JGR 0x%X ", get_mem(lowBits)) ;
		break ;
    }
    case OPCODE_JLS_M:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " JLS 0x%X ", get_mem(lowBits)) ;
		break ;
    }
    case OPCODE_CALL_M:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " CALL %d ", get_mem(lowBits)) ;
		break ;
    }
    case OPCODE_RET:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " RET ") ;
		break ;
    }
    case OPCODE_AND_RR:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " AND R%d R%d ", get_dreg(highBits), get_sreg(lowBits)) ;
		break ;
    }
    case OPCODE_AND_RI:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " AND R%d %d ", get_dreg(highBits), get_imm(lowBits)) ;
		break ;
    }
    case OPCODE_OR_RR:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " OR R%d R%d ", get_dreg(highBits), get_sreg(lowBits)) ;
		break ;
    }
    case OPCODE_OR_RI:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " OR R%d %d ", get_dreg(highBits), get_imm(lowBits)) ;
		break ;
    }
    case OPCODE_XOR_RR:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " XOR R%d R%d ", get_dreg(highBits) - sunyat_regs[REG_WIN], get_sreg(lowBits) - sunyat_regs[REG_WIN]) ;
		break ;
    }
    case OPCODE_XOR_RI:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " XOR R%d %d ", get_dreg(highBits), get_imm(lowBits)) ;
		break ;
    }
    case OPCODE_LOAD_RM:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " LOAD R%d %d ", get_dreg(highBits), get_mem(lowBits)) ;
		break ;

    }
    case OPCODE_LOADP_RR:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " LOADP R%d R%d ", get_dreg(highBits), get_sreg(lowBits)) ;
		break ;
    }
    case OPCODE_STOR_MR:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " STOR %d R%d ", get_mem(lowBits), get_sreg(lowBits)) ;
		break ;
    }
    case OPCODE_STORP_RR:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " STORP R%d R%d ", get_dreg(highBits), get_sreg(lowBits)) ;
		break ;
    }
    case OPCODE_STACKER_R:
    {
        int imm = get_imm(lowBits) ;
        if (imm) { //Pop
            mvwprintw(win->win, win->cur_Y, win->cur_X, " POP R%d ", get_dreg(highBits)) ;
        } else { //Push
            mvwprintw(win->win, win->cur_Y, win->cur_X, " PUSH R%d ", get_dreg(highBits)) ;
        }
		break ;
    }
    case OPCODE_SWR_I:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " SWR %d ", get_imm(lowBits)) ;
		break ;
    }
    case OPCODE_AWR_I:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " AWR %d ", get_imm(lowBits)) ;
		break ;
    }
    case OPCODE_AUX_I:
    {
        mvwprintw(win->win, win->cur_Y, win->cur_X, " AUX %d ", get_imm(lowBits)) ;
		break ;
    }
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

void print_to_win_lbls(WINDOW *cwin, uint8_t data[], int len, char *lbls, int lbl_len){

}

void move_cursor(WINDOW *cwin){

}

//void memory_win(WINDOW *mem, int x, int y, int wid, int height) {
    //mem_win = newwin(wid, height, x, y) ;
    //box(mem, 0, 0) ;
    //wrefresh(mem) ;
//}