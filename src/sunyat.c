/*
 * Name        : sunyat.c
 * Author      : William "Amos" Confer
 * Description : A simple RISC-like virtual machine designed to demonstrate
 *               one basic way to implement a theoretical ISA.  Because the
 *               SUNYAT is not based on an ISA ever intended for the IC world,
 *               a few unrealistic liberties were taken to keep the
 *               implementation minimal.  For example, "clock ticks" refer to
 *               a complete fetch-decode-execute cycle so that all instructions
 *               (even memory requests) take a single "clock tick" - although
 *               nothing resembling an instruction pipeline is virtualized.
 *               Also, character input (request at address 0xFE) requires an
 *               indeterminate amount of time as it hangs the application until
 *               the user types a character and presses the Enter key... still
 *               counts as only one clock tick :-)
 *
 * Usage       : 1st parameter is a 324 byte ROM image to execute. The first
 *               70 bytes is the application's identification string printed
 *               upon successfull load.  The remaining 254 bytes are the
 *               application "ROM image" which overwrite the entire 254 bytes of
 *               RAM.  Address 0xFE (254) is a key input device and 0xFF (255)
 *               is a character output device (terminal).
 *
 * License     : Copyright (c) 2008--2014 William "Amos" Confer
 *
 *    Permission is hereby granted, free of charge, to any person obtaining a
 *    copy of this software and associated documentation files (the "Software"),
 *    to deal in the Software without restriction, including without limitation
 *    the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *    and/or sell copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included in
 *    all copies or substantial portions of the Software.;
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *    DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <stdint.h>

#include <curses.h>

#ifndef true
#define true TRUE
#endif /* true */

#ifndef false
#define false FALSE
#endif /* false */


#include "sunyat.h"

//////////////////////////////////////////////////

const char MSG_STARTUP [] =
	"\nThe SUNYAT Virtual Machine version 0x0 - (C) 2008, William \"Amos\" Confer\n\nLoading application: %s\n\n";
const char MSG_BAR [] =
	"----------------------------------------";

const char ERR_NCURSES_INIT [] =
	"\tCould not initialize ncurses\n";
const char ERR_NCURSES_CBREAK [] =
	"\tCould not disable character buffering\n";
const char ERR_NCURSES_NODELAY [] =
	"\tCould not disable blocking on \"getch\"\n";
const char ERR_NCURSES_NOECHO [] =
	"\tCould not disable echo\n";
const char ERR_NCURSES_KEYPAD [] =
	"\tCould not enable keypad usage\n";
const char ERR_NCURSES_CURSOR [] =
	"\tCould not modify cursor\n";
const char ERR_NO_APP [] =
	"\tNo application provided...  SUNYAT <filename>\n";
const char ERR_BAD_USAGE [] =
	"\tIncorrect usage... SUNYAT <filename>\n";
const char ERR_BYTE_SIZE [] =
	"\tApplication is not the correct byte size.\n";
const char ERR_FILE_NOT_OPEN [] =
	"\tApplication file could not be opened.\n";

const char ERR_INVALID_PC [] =
	"\tProgram counter is invalid\n";
const char ERR_IMPOSSIBLE_INSTRUCTION [] =
	"\tAttempted to execute an impossible instruction\n";
const char ERR_DIV_ZERO [] =
	"\tDivide by zero error\n";
const char ERR_JMP_RANGE [] =
	"\tJMP instruction targets an out of range address\n";
const char ERR_LOAD [] =
	"\tLOAD instruction uses an out of range address\n";
const char ERR_STOR [] =
	"\tSTOR instruction uses an out of range address\n";
const char ERR_POP [] =
	"\tPOP underflowed the stack\n";
const char ERR_PUSH [] =
	"\tPSH overflowed the stack\n";
const char ERR_CALL_OVERFLOW [] =
	"\tCALL overflowed the stack\n";
const char ERR_CALL_RANGE [] =
	"\tCALL instruction targets an out of range address\n";
const char ERR_WINDOW_RANGE []=
	"\tWindow position out of range. (Acceptable Values: 0-29)\n";

//////////////////////////////////////////////////


#define STARTUP_PAUSE       3 //Fucking get rid of this.

//Get rid of this shit too ⇓⇓⇓⇓⇓
char app_msg [SIZE_APP_MSG + 1];	/* +1 is to add a guaranteed null terminator */

/**
 *  Brief:
 *      This is the array being all the ram for the SUNYAT.
 */
uint8_t sunyat_ram [SIZE_APP_RAM];

/* //This is already defined and commented in the header so we can probably get rid of it here
 * register file for the sunyat-1.
 * 0:		REG_PC
 * 1:		REG_IRH
 * 2:		REG_IRL
 * 3:		REG_WIN
 * 4:		REG_SP
 * 5-37:	33 General Purpose registers
 */

/**
 *  Brief:
 *      This is the array where the data for alll the registers are stored.
 */
uint8_t sunyat_regs [SIZE_REG] = {
	0, 0, 0, 5,                             /* REG_PC, REG_IRH, REG_IRL, REG_SP */
	SIZE_APP_RAM,                           /* stack grows down from top of RAM */
	'0', '1', '2', '3', '4', '5', '6', '7', /* GPRS no longer default to Amos' wedding date */
	'8', '9', '0', '1', '2', '3', '4', '5', /* GPRS no longer default to Amos' wedding date */
	'6', '7', '8', '9', '0', '1', '2', '3', /* GPRS no longer default to Amos' wedding date */
	'4', '5', '6', '7', '8', '9', '0', '1', /* GPRS no longer default to Amos' wedding date */
};

int sunyat_flag_zero = 0;
int sunyat_flag_sign = 0;

int cursor_row = 0;
int cursor_col = 0;

uint8_t terminal[TERMINAL_HEIGHT][TERMINAL_WIDTH + 1];

bool linefeed_buffered = false;

long int sunyat_clock_ticks = 0;

//////////////////////////////////////////////////
//int setup_terminal();
//I feel like these should be in the header.
static int setup_ncurses_terminal();
static void terminal_init();
static void terminal_restore();

static void sunyat_execute ();

static uint8_t get_opcode ();
static uint8_t get_dreg ();
static uint8_t get_sreg ();
static uint8_t get_mem ();
static int8_t get_imm ();

static void set_flags (int8_t result);


//////////////////////////////////////////////////

int start_sunyat(char *rom){
    clock_t clock_start = clock();

    // test application size
	uint8_t file_buffer [SIZE_APP_ROM];
	FILE *infile = NULL;
	if ((infile = fopen (rom, "rb")) != NULL) {
		// is it at least SIZE_APP_ROM big ?

		if (SIZE_APP_ROM != fread (file_buffer, sizeof (uint8_t), SIZE_APP_ROM, infile)) {
			// not big enough
			printf (ERR_BYTE_SIZE);
			return EXT_ERR_ROM_BIG;
		}
		else {
			// make sure we're at the EOF
			if (fgetc (infile) != EOF) {
				printf (ERR_BYTE_SIZE);
				return EXT_ERR_BYTE_SIZE;
			}
		}
	}
	else {
		// file could not be opened
		printf (ERR_FILE_NOT_OPEN);
		return EXT_ERR_FILE_NOT_OPEN;
	}

		/*
		 * The application image looks good. Let's load it up and run.
		 */

		//close the file... we're done with it now
		fclose (infile);

		// print the application identification message
		memcpy (app_msg, file_buffer, SIZE_APP_MSG);
		app_msg [SIZE_APP_MSG] = '\0';	// make sure the ID message is terminated
		printf ("%s\n%s\n%s\n\n", MSG_BAR, app_msg, MSG_BAR);

		// load RAM from the ROM image
		memcpy (sunyat_ram, file_buffer + SIZE_APP_MSG, SIZE_APP_RAM);

// pause to let user see application info
	while ((clock () - clock_start) / CLOCKS_PER_SEC < 3);

	// get the ncurses terminal going
	if (-1 == setup_ncurses_terminal ()) {
		return EXT_ERR_NCURSES;
	}

	terminal_init();

	// fetch->decode->exceute until returned beyond RAM
	sunyat_execute ();

	// pause to let user see completed application output
	clock_start = clock();
	while ((clock () - clock_start) / CLOCKS_PER_SEC < 3);

	// close the ncurses terminal
	endwin ();

	printf ("\n\nSUNYAT exited after %lu clock cycles\n\n", sunyat_clock_ticks);

    return EXIT_SUCCESS ;
}


static int setup_ncurses_terminal () {
	if (NULL == initscr ()) {
		printf (ERR_NCURSES_INIT);
		return -1;
	}

	if (ERR == cbreak ()) {
		printf (ERR_NCURSES_CBREAK);
		return -1;
	}

	if (ERR == noecho ()) {
		printf (ERR_NCURSES_NOECHO);
		return -1;
	}

	if (ERR == nodelay (stdscr, true)) {
		printf (ERR_NCURSES_NODELAY);
		return -1;
	}

	if (ERR == keypad (stdscr, true)) {
		printf (ERR_NCURSES_KEYPAD);
		return -1;
	}

	if (ERR == curs_set (1)) {
		printf (ERR_NCURSES_CURSOR);
		return -1;
	}

	return 0;
}



static void terminal_init() {
	int y;
	int x;

	for (y = 0; y < TERMINAL_HEIGHT; y++) {
		for (x = 0; x < TERMINAL_WIDTH; x++) {
			terminal [y][x] = ' ';
		}
		terminal [y][x] = '\0';
	}
}

static void terminal_restore() {
	int y;

	erase ();

	for (y = 0; y < TERMINAL_HEIGHT; y++) {
		mvprintw( y, 0, (const char *)(terminal [y]));
	}

	move (cursor_row, cursor_col);
}

static void sunyat_execute () {
	bool terminal_too_small_prev_cycle = false;

	for (;;) {
		uint8_t opcode;
		uint8_t sreg;
		uint8_t dreg;
		uint8_t mem;
		int8_t imm;
		uint8_t cmp_result;

		int current_width;
		int current_height;

		move (cursor_row, cursor_col);

		getmaxyx (stdscr, current_height, current_width);

		if (current_width < TERMINAL_WIDTH || current_height < TERMINAL_HEIGHT) {
			int x;
			int y;

			/*
			 * What's important is it is true when this cycle is
			 * the previous cycle next cycle :-P
			 */
			terminal_too_small_prev_cycle = true;

			for (y = 0; y < current_height; y++) {
				move (y, 0);
				for (x = 0; x < current_width; x++) {
					addch ('@');
				}
			}
			int cx = current_width  / 2;
			int cy = current_height / 2;

			mvprintw(cy-1, cx-10, "                    ");
			mvprintw(  cy, cx-10, "  Window too small  ");
			mvprintw(cy+1, cx-10, " resize to >= 80x24 ");
			mvprintw(cy+2, cx-10, "                    ");
			refresh();
			continue;
		}

		if (terminal_too_small_prev_cycle) {
			terminal_restore ();
			terminal_too_small_prev_cycle = false;
			refresh ();
		}

		sunyat_clock_ticks++;

		/*
		 * FETCH
		 */

		// make sure the PC is valid... -2 because each instruction is 2 bytes
		if (sunyat_regs [REG_PC] > (SIZE_APP_RAM - 2)) {
			printf (ERR_INVALID_PC);
			return;
		}

		sunyat_regs [REG_IRH] = sunyat_ram [sunyat_regs [REG_PC]++];
		sunyat_regs [REG_IRL] = sunyat_ram [sunyat_regs [REG_PC]++];

		/*
		 * DECODE
		 */
		opcode = get_opcode ();
		sreg = get_sreg () + sunyat_regs [REG_WIN]; //This should be all we need for the offset of the window.
		dreg = get_dreg () + sunyat_regs [REG_WIN];
        //printf("Accessing %d and des to %d \n", sreg, dreg) ;
		mem = get_mem ();
		imm = get_imm ();

		/*
		 * EXECUTE
		 */
		switch (opcode) {
		case OPCODE_MOV_RR:
			sunyat_regs [dreg] = sunyat_regs [sreg];
			break;
		case OPCODE_MOV_RI:
			sunyat_regs [dreg] = imm;
			break;
		case OPCODE_ADD_RR:
			// this should work on signed values as well
			sunyat_regs [dreg] = sunyat_regs [dreg] + sunyat_regs [sreg];
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_ADD_RI:
			// this should work on signed values as well
			sunyat_regs [dreg] = sunyat_regs [dreg] + imm;
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_MUL_RR:
			// this should work on signed values as well
			sunyat_regs [dreg] = sunyat_regs [dreg] * sunyat_regs [sreg];
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_MUL_RI:
			// this should work on signed values as well
			sunyat_regs [dreg] = sunyat_regs [dreg] * imm;
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_DIV_RR:
			// this should work on signed values as well
			if (sunyat_regs [sreg] == 0) {
				printf (ERR_DIV_ZERO);
				return;
			}
			sunyat_regs [dreg] = sunyat_regs [dreg] / sunyat_regs [sreg];
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_DIV_RI:
			// this should work on signed values as well
			if (imm == 0) {
				printf (ERR_DIV_ZERO);
				return;
			}
			sunyat_regs [dreg] = sunyat_regs [dreg] / imm;
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_CMP_RR:
			// this should work on signed values as well
			cmp_result = sunyat_regs [dreg] - sunyat_regs [sreg];
			set_flags (cmp_result);
			break;
		case OPCODE_CMP_RI:
			// this should work on signed values as well
			cmp_result = sunyat_regs [dreg] - imm;
			set_flags (cmp_result);
			break;
		case OPCODE_JMP_M:
			if (mem >= SIZE_APP_RAM) {
				printf (ERR_JMP_RANGE );
				return;
			}
			sunyat_regs [REG_PC] = mem;
			break;
		case OPCODE_JEQ_M:
			if (sunyat_flag_zero)
				sunyat_regs [REG_PC] = mem;
			break;
		case OPCODE_JNE_M:
			if (!sunyat_flag_zero)
				sunyat_regs [REG_PC] = mem;
			break;
		case OPCODE_JGR_M:
			if (!sunyat_flag_zero && !sunyat_flag_sign)
				sunyat_regs [REG_PC] = mem;
			break;
		case OPCODE_JLS_M:
			if (sunyat_flag_sign)
				sunyat_regs [REG_PC] = mem;
			break;
			if (sunyat_regs [REG_SP] <= 0) {
				printf (ERR_CALL_OVERFLOW);
				return;
			}
			if (mem >= SIZE_APP_RAM) {
				printf (ERR_CALL_RANGE);
				return;
			}
			sunyat_regs [REG_SP]--;
			sunyat_ram [sunyat_regs [REG_SP]] = sunyat_regs [REG_PC];
			sunyat_regs [REG_PC] = mem;
			break;
		case OPCODE_CALL_M:
			if (sunyat_regs [REG_SP] <= 0) {
				printf (ERR_CALL_OVERFLOW);
				return;
			}
			if (mem >= SIZE_APP_RAM) {
				printf (ERR_CALL_RANGE);
				return;
			}
			sunyat_regs [REG_SP]--;
			sunyat_ram [sunyat_regs [REG_SP]] = sunyat_regs [REG_PC];
			sunyat_regs [REG_PC] = mem;
			break;
		case OPCODE_RET:
			if (sunyat_regs [REG_SP] >= SIZE_APP_RAM)
				return;
			sunyat_regs [REG_PC] = sunyat_ram [sunyat_regs [REG_SP]];
			sunyat_regs [REG_SP]++;
			break;
		case OPCODE_AND_RR:
			sunyat_regs [dreg] = sunyat_regs [dreg] & sunyat_regs [sreg];
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_AND_RI:
			sunyat_regs [dreg] = sunyat_regs [dreg] & imm;
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_OR_RR:
			sunyat_regs [dreg] = sunyat_regs [dreg] | sunyat_regs [sreg];
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_OR_RI:
			sunyat_regs [dreg] = sunyat_regs [dreg] | imm;
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_XOR_RR:
			sunyat_regs [dreg] = sunyat_regs [dreg] ^ sunyat_regs [sreg];
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_XOR_RI:
			sunyat_regs [dreg] = sunyat_regs [dreg] ^ imm;
			set_flags (sunyat_regs [dreg]);
			break;
		case OPCODE_LOAD_RM:
            if (mem < SIZE_APP_RAM)
				sunyat_regs [dreg] = sunyat_ram [mem];
			else if (mem == IO_TERMINAL)
				if (!linefeed_buffered)
				{
					sunyat_regs [dreg] = getch ();
					switch ((int) sunyat_regs [dreg])
					{
						case KEY_ENTER:
						sunyat_regs [dreg] = (uint8_t) 0xD;
						linefeed_buffered = true;
						break;

						case ERR:
						sunyat_regs [dreg] = (uint8_t) 0;
						break;

						default:
						break;
					}
				}
				else
				{
					sunyat_regs [dreg] = (uint8_t) 0xA;
					linefeed_buffered = false;
				}
			else {
				printf (ERR_LOAD);
				return;
			}
			break;
        case OPCODE_LOADP_RR:
			if (sunyat_regs [sreg] < SIZE_APP_RAM)
				sunyat_regs [dreg] = sunyat_ram [sunyat_regs [sreg]];
			else if (sunyat_ram [sunyat_regs [sreg]] == IO_TERMINAL)
				if(!linefeed_buffered)
				{
					sunyat_regs [dreg] = getch ();
					switch ((int) sunyat_regs [dreg])
					{
						case KEY_ENTER:
						sunyat_regs [dreg] = (uint8_t) 0xD;
						linefeed_buffered = true;
						break;

						case ERR:
						sunyat_regs [dreg] = (uint8_t) 0;
						break;

						default:
						break;
					}
				}
				else
				{
					sunyat_regs [dreg] = (uint8_t) 0xA;
					linefeed_buffered = false;
				}
			else {
				printf (ERR_LOAD);
				return;
			}
			break;
		case OPCODE_STOR_MR:
			if (mem < SIZE_APP_RAM)
				sunyat_ram [mem] = sunyat_regs [dreg]; //yes, dreg is correct for this one
			else if (mem == IO_TERMINAL) {
				char c = sunyat_regs [dreg];
				switch (c) {
				case 0x9: //horizontal tab
					cursor_col += TAB_SIZE - (cursor_col % TAB_SIZE);
					if (cursor_col >= TERMINAL_WIDTH) {
						cursor_col = 0;
						cursor_row = (cursor_row + 1) % TERMINAL_HEIGHT;
					}
					break;
				case 0xD: // carriage return
					cursor_col = 0;
					break;
				case 0xA: // line feed
					cursor_row = (cursor_row + 1) % TERMINAL_HEIGHT;
					break;
				default:
					if (isprint (c)) {
						printw ("%c", c);
						terminal[cursor_row][cursor_col] = c;

						cursor_col++;
						if (cursor_col >= TERMINAL_WIDTH) {
							cursor_col = 0;
							cursor_row = (cursor_row + 1) % TERMINAL_HEIGHT;
						}
					}
					else {
						printw ("<0x%02X>", c);
						terminal[cursor_row][cursor_col] = ' ';

					}
				}
				mvprintw (cursor_row, cursor_col, "");
				refresh ();
			}
			else {
				printf (ERR_STOR);
				return;
			}
			break;
		case OPCODE_STORP_RR:
			if (sunyat_regs [dreg] < SIZE_APP_RAM)
				sunyat_ram [sunyat_regs [dreg]] = sunyat_regs [sreg];
			else if (sunyat_regs [dreg] == IO_TERMINAL) {
				char c = sunyat_regs [dreg];
				switch (c) {
				case 0x9: //horizontal tab
					cursor_col += TAB_SIZE - (cursor_col % TAB_SIZE);
					if (cursor_col >= TERMINAL_WIDTH) {
						cursor_col = 0;
						cursor_row = (cursor_row + 1) % TERMINAL_HEIGHT;
					}
					break;
				case 0xD: // carriage return
					cursor_col = 0;
					break;
				case 0xA: // line feed
					cursor_row = (cursor_row + 1) % TERMINAL_HEIGHT;
					break;
				default:
					if (isprint (c)) {
						printw ("%c", c);
						terminal[cursor_row][cursor_col] = c;

						cursor_col++;
						if (cursor_col >= TERMINAL_WIDTH) {
							cursor_col = 0;
							cursor_row = (cursor_row + 1) % TERMINAL_HEIGHT;
						}
					}
					else {
						printw ("<0x%02X>", c);
						terminal[cursor_row][cursor_col] = ' ';
					}
					refresh ();
				}
				mvprintw (cursor_row, cursor_col, "");
				refresh ();
			}
			break;
		/*case OPCODE_PUSH_R:
			if (sunyat_regs [REG_SP] <= 0)
			{
				printf (ERR_PUSH);
				return;
			}
			sunyat_regs [REG_SP]--;
			sunyat_ram [sunyat_regs [REG_SP]] = sunyat_regs [dreg]; //yes, dreg is correct for this one
			break;
		case OPCODE_POP_R:
			if (sunyat_regs [REG_SP] >= SIZE_APP_RAM) {
				printf (ERR_POP);
				return;
			}
			sunyat_regs [dreg] = sunyat_ram [sunyat_regs [REG_SP]];
			sunyat_regs [REG_SP]++;
			break;*/
        case OPCODE_STACKER_R:
            if (0 == imm) { //Push
                if (sunyat_regs [REG_SP] <= 0)
                {
                    printf (ERR_PUSH);
                    return;
                }
                sunyat_regs [REG_SP]--;
                sunyat_ram [sunyat_regs [REG_SP]] = sunyat_regs [dreg]; //yes, dreg is correct for this one
            } else { //Pop
                if (sunyat_regs [REG_SP] >= SIZE_APP_RAM) {
                    printf (ERR_POP);
                    return;
                }
                sunyat_regs [dreg] = sunyat_ram [sunyat_regs [REG_SP]];
                sunyat_regs [REG_SP]++;
            }
            break ;
		//Windowing opcodes
		case OPCODE_SWR_I:
			{
                //printf("Changing swr %d \n", imm + REG_GEN_START) ;
				if (imm > MAX_WIN_INDEX)
				{
					printf(ERR_WINDOW_RANGE);
				}
				sunyat_regs[REG_WIN] = imm + REG_GEN_START;
				break;
			}
		case OPCODE_AWR_I: //This has the greatest chance of going beyond MAX_WIN_INDEX
		{
			if ((sunyat_regs[REG_WIN] = sunyat_regs[REG_WIN] + imm) > MAX_WIN_INDEX)
			{
				printf(ERR_WINDOW_RANGE);
			}
			break;
		}



		default:
			// This should be impossible since every opcode is accounted for
			printf (ERR_IMPOSSIBLE_INSTRUCTION);
			printf ("opcode = %d\n", opcode);
			return;
			break;
		}

	}
}

static uint8_t get_opcode () {
	return sunyat_regs [REG_IRH] >> 3; // top 5 bits are opcode
}

static uint8_t get_dreg () {
	return sunyat_regs [REG_IRH] & 0x07; // bottom 3 bits are dreg
}

static uint8_t get_sreg () {
	return sunyat_regs [REG_IRL] & 0x07; // bottom 3 bits are sreg
}

static uint8_t get_mem () {
	return sunyat_regs [REG_IRL];
}

static int8_t get_imm () {
	return (int8_t)(sunyat_regs [REG_IRL]);
}

static void set_flags (int8_t result) {
	if (result == 0) {
		sunyat_flag_zero = 1;
		sunyat_flag_sign = 0;
	}
	else if (result > 0) {
		sunyat_flag_zero = 0;
		sunyat_flag_sign = 0;
	}
	else {
		sunyat_flag_zero = 0;
		sunyat_flag_sign = 1;
	}
}
