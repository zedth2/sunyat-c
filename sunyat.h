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

#define SIZE_APP_MSG    70  //APP_MSG_SIZE
#define SIZE_APP_RAM    254 //APP_RAM_SIZE

#define REG_PC  0
#define REG_IRH 1
#define REG_IRL 2
#define REG_WIN 3
#define REG_SP  4

#define SIZE_WIN 8
#define SIZE_REG 38h

#define SIZE_APP_ROM    (SIZE_APP_MSG + SIZE_APP_RAM)   //APP_ROM_SIZE (APP_MSG_SIZE + APP_RAM_SIZE)
#define APP_KEYBOARD    0xFE
#define APP_SCREEN      0xFF

//opcodes 
#define OPCODE_MOV_RR   0
#define OPCODE_MOV_RI   1
#define OPCODE_ADD_RR   2
#define OPCODE_ADD_RI   3
#define OPCODE_SUB_RR   4
//#define OPCODE_SUB_RI 5 //OPCODE_SUB_RI + OPCODE_NEG_R were removed to make room for OPCODE_SWR + AWR (windowing)
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
//#define OPCODE_NEG_R  25 //OPCODE_SUB_RI + OPCODE_NEG_R were removed to make room for OPCODE_SWR + AWR (windowing)
#define OPCODE_LOAD_RM  24
#define OPCODE_LOADP_RR 25
#define OPCODE_STOR_MR  26
#define OPCODE_STORP_RR 27
#define OPCODE_PUSH_R   28
#define OPCODE_POP_R    29

#define OPCODE_SWR      30
#define OPCODE_AWR		31

#endif /* _SUNYAT_H_ */
