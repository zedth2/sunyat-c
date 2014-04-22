

#include <ncurses.h>
#include <stdlib.h>
#include "sunyat.h"
#include "debugger.h"



extern uint8_t sunyat_regs [SIZE_REG] ;
extern uint8_t sunyat_ram [SIZE_APP_RAM];



extern SatWin *main_win ; ///The main window for the assembly program. Defined in sunyat.{h,c}

int curMode = 0 ; ///This will keep the memory window in whatever mode is was before.

/**
 *  brief:
 *      This will do the setup for ncurses for the debugger.
 *
 *  Returns : int
 *      A -1 if an error occured, 0 otherwise.
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
    //init_pair(1, COLOR_WHITE, COLOR_BLACK);
    //wbkgd(stdscr, COLOR_PAIR(1)) ;
	return 0;

    return 0 ;
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
    //if (max_x <= 80 || max_y <= 25) {
        //printf("WINDOW NOT BIG ENOUGH %d %d \n", max_x, max_y) ;
        //return NULL ;
    //}
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
    mem_win->win = newwin(max_y/2, max_x - 80, 0, 80) ;
    getmaxyx(mem_win->win, mem_win->max_Y, mem_win->max_X) ;

    box(mem_win->win, 0, 0) ;
    scrollok(mem_win->win, TRUE) ;
    wrefresh(mem_win->win) ;

    printf_debug_win = init_SatWin() ;
    printf_debug_win->win = newwin(max_y/2, max_x - 80, max_y/2, 80) ;

    box(printf_debug_win->win, 0, 0) ;
    scrollok(printf_debug_win->win, TRUE) ;
    wrefresh(printf_debug_win->win) ;
    printf_debug_win->cur_X = 2 ;
    printf_debug_win->cur_Y = 2 ;


    //if (mem_win->win == NULL || reg_win->win == NULL) {
        //printf("WINDOWS FAILED TO BE CREATED FOR DEBUGGER.\n") ;
        //scanf ( "%d\n", &max_x) ;
        //return NULL ;
    //}

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
    unsigned int cnt = 0, strLen = 10, reg_win_cnt = 0 ;
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
                if (REG_GEN_START <= cnt || REG_GEN_END >= cnt) {
                    reg_win_cnt = cnt_to_reg_win(cnt) ;
                    if (SIZE_WIN > reg_win_cnt) {
                        wattron(win->win, COLOR_PAIR(2));
                        mvwprintw(win->win, win->cur_Y, win->cur_X, "R%02d : 0x%02X ", reg_win_cnt, sunyat_regs[cnt]);
                        wattroff(win->win, COLOR_PAIR(2));
                    } else {
                        mvwprintw(win->win, win->cur_Y, win->cur_X, "%03d : 0x%02X ", cnt, sunyat_regs[cnt]);
                    }
                }
                break ;
            }
        }
        (win->cur_Y)++ ;
        check_cursor(win, strLen) ;
    }
    wrefresh(win->win) ;
}

/**
 *  Brief:
 *      Gets a number as input and returns the window value of the number.
 *
 *  Parameter:
 *      cnt : unsigned int
 *          The number to get the associated window position of.
 *
 *  Returns : unsigned int
 *      The number return can be greater than the size of the window.
 */
static unsigned int cnt_to_reg_win(unsigned int cnt) {
    unsigned int rw = sunyat_regs[REG_WIN] - NUM_SYS_REG ;
    unsigned int base = NUM_GEN_REG - rw + (cnt - NUM_SYS_REG)  ;
    if (base >= NUM_GEN_REG) {
        return base - NUM_GEN_REG ;
    }
    return base ;
}

/**
 *  Brief:
 *      This does a check to see if the cursor is outside the bounds of then
 *      boxed window of ncurses. If it detects the value is beyond the window
 *      then it resets Y to 1 and X to strLen + 2.
 *      The strLen is the length of the string being printed out in the
 *      columns so that the cursor is reset to the next column to the right.
 *
 *  Parameters:
 *      win : *SatWin
 *          The window to check against.
 *
 *      strLen : int
 *          The length of the string being printed through the columns.
 */
static void check_cursor(SatWin *win, int strLen) {
    if (win->cur_Y >= (win->max_Y)-2) {
        win->cur_Y = 1 ; //It must be two to get past the box outline.
        win->cur_X += strLen+2 ;
    }
}


/**
 *  Brief:
 *      This is just an abstracted call to print_mem_win so that
 *      it can maintain the current memory window mode without
 *      the caller needing to do any logic.
 */
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
//printf("write_mem_win %X %d", sunyat_regs[REG_PC], curMode) ;
    //exit(102) ;
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

/**
 *  Brief:
 *      This will run an infinite loop effectively pausing the excution of
 *      the calling system.
 *      The user can press F8 to continue for one cycle. F7 will change
 *      the memory window mode between a list of all memory and the current
 *      memory being executed.
 *
 *  Returns : int
 *      The returning value has the meaning if the pause should continue
 *      after the next instruction.
 */
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
        //sleep(1) ;
        //printf("sleeping\t") ;
    }
    erase_box(mem_win) ;
    curMode = (pause_again && curMode) ;
    return pause_again ;
}

/**
 *  Brief:
 *      This will erase the passed in window and redraw it with a box.
 *      But does not do a refresh.
 *
 *  Parameters:
 *      win : *SatWin
 *          The window to operate on.
 */
static void erase_box(SatWin *win) {
    werase(win->win) ;
    box(win->win, 0, 0) ;

}

/**
 *  Brief:
 *      This will deconstruct the high and low bits and print out the
 *      assembly code line to the screen.
 *
 *  Parameters:
 *      win : *SatWin
 *          The window to print the data to.
 *
 *      highBits : uint8_t
 *          The high eight bits for the instruction.
 *
 *      lowBits : uint8_t
 *          The low eight bits for the instruction.
 */
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

/**
 *  Brief:
 *      This will print the array out to the window.
 *      It will print the value of the array with a counter on the left...
 *
 *      255 : 0xFF
 *
 *  Parameters:
 *      cwin : *WINDOW
 *          An ncurses window to print out to.
 *
 *      data : uint8_t[]
 *          The array of data to print out.
 *
 *      len : unsigned int
 *          The length of the array.
 */
void print_to_win(WINDOW *cwin, uint8_t data[], unsigned int len){
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
