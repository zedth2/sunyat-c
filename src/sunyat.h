/*
 * Name        : sunyat.h
 * Author      : William "Amos" Confer
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

#ifndef _SUNYAT_H_
#define _SUNYAT_H_

int start_sunyat(char *rom);

/*
 * On read:  0 if no key, key code otherwise
 * On write: character output w/ cursor adjust
 */
#define IO_TERMINAL     0xFF



#define SIZE_APP_MSG    70 //Probably pointless

/**
 * Brief:
 *      This will be the size of the ram.
 */
#define SIZE_APP_RAM    0xFF

/**
 *  Brief:
 *      This will be the max size allowed for any load rom file.
 */
#define SIZE_APP_ROM    SIZE_APP_RAM + SIZE_APP_MSG   //APP_ROM_SIZE (APP_MSG_SIZE + APP_RAM_SIZE)


//System Registers
/**
 *  Brief:
 *      This register will contain the current program counter.
 */
#define REG_PC  0

/**
 *  Brief:
 *      This is the register containing the 8 high bits of the currently
 *          loaded instruction.
 */
#define REG_IRH 1

/**
 * Brief:
 *      This is the register containing the 8 low bits of the currently
 *          loaded instruction.
 */
#define REG_IRL 2

/**
 *  Brief:
 *      This register will contain the current starting position of the
 *          register window.
 */
#define REG_WIN 3

/**
 *  Brief:
 *      FILL IN
 */
#define REG_SP  4

/**
 *  Brief:
 *      This is the number of system registers.
 *          It will be used to figure out where the general purpose registers
 *          start.
 */
#define NUM_SYS_REG 5 //Number of system registers (0-4) used for referencing GPRs starting at 0.


//General Purpose Registars
/**
 *  Brief:
 *      This is the total number of registers. It should be the total number
 *          of system registers plus the number of general purpose registers.
 */
#define SIZE_REG (NUM_SYS_REG + 32)

/**
 *  Brief:
 *      This is the starting point of the general purpose registers.
 */
#define REG_GEN_START NUM_SYS_REG

/**
 *  Brief:
 *      This is the ending index of the general purpose registers.
 */
#define REG_GEN_END   SIZE_REG - 1

/**
 *  Brief:
 *      This is the size of the register window.
 */
#define SIZE_WIN 8

/**
 *  Brief:
 *      This is the greatest index that the start of the register window
 *          can be.
 */
#define MAX_WIN_INDEX SIZE_REG - SIZE_WIN

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




//opcodes
#define OPCODE_MOV_RR   0
#define OPCODE_MOV_RI   1
#define OPCODE_ADD_RR   2
#define OPCODE_ADD_RI   3
#define OPCODE_SUB_RR   4
#define OPCODE_MUL_RR   5
#define OPCODE_MUL_RI   6
#define OPCODE_DIV_RR   7
#define OPCODE_DIV_RI   8
#define OPCODE_CMP_RR   9
#define OPCODE_CMP_RI   10
#define OPCODE_JMP_M    11
#define OPCODE_JEQ_M    12
#define OPCODE_JNE_M    13
#define OPCODE_JGR_M    14
#define OPCODE_JLS_M    15
#define OPCODE_CALL_M   16
#define OPCODE_RET      17
#define OPCODE_AND_RR   18
#define OPCODE_AND_RI   19
#define OPCODE_OR_RR    20
#define OPCODE_OR_RI    21
#define OPCODE_XOR_RR   22
#define OPCODE_XOR_RI   23
#define OPCODE_LOAD_RM  24
#define OPCODE_LOADP_RR 25
#define OPCODE_STOR_MR  26
#define OPCODE_STORP_RR 27
#define OPCODE_PUSH_R   28
#define OPCODE_POP_R    29

#define OPCODE_SWR_I	30
#define OPCODE_AWR_I	31


//Exit Code Errors

/**
 *  Brief:
 *      This will get thrown if the system wasn't given any rom file, or
 *          such, to load in.
 */
#define EXT_ERR_NO_FILE_ARG       1

/**
 *  Brief:
 *      This will get thrown if the ROM provided is to large.
 */
#define EXT_ERR_ROM_BIG           2

/**
 *  Brief: //Look at sunyat.c:213
 *      This will get thrown if after reading t
 */
#define EXT_ERR_BYTE_SIZE         3

/**
 *  Brief:
 *      The provided file could not be open.
 */
#define EXT_ERR_FILE_NOT_OPEN     4

/**
 *  Brief:
 *      Will be thrown when theres an error in ncurses.
 */
#define EXT_ERR_NCURSES           5 //This should be expand to cover all ncurses errors.

#endif /* _SUNYAT_H_ */
