/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>
#line 1 "assembler/sunyat-asm.y"

	/*
	 * Name        : sunyat-asm.y
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
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <assert.h>
    #include <stdint.h>

	#include "token.h"
	#include "../sunyat.h"

	#define MAX_CNT 256

	extern int assembler_pass;	/* sunyat-asm.lex */
	extern char* filename;	/* sunyat-asm.lex */

	uint8_t high_opcode;
	uint8_t high_reg;
	uint8_t low;

	uint8_t msg_data [SIZE_APP_MSG];
	uint8_t ram_data [SIZE_APP_RAM];
	uint8_t address = 0;

	char *variables [MAX_CNT];
	uint8_t variable_addrs [MAX_CNT];
	int variable_cnt = 0;

	char *labels [MAX_CNT];
	uint8_t label_addrs [MAX_CNT];
	int label_cnt = 0;

	char *constants [MAX_CNT];
	uint8_t constant_values [MAX_CNT];
	int constant_cnt = 0;

	int errors_found = 0;

	void message (int err, Token t, const char *primary_err, const char *optional_err) {
		if (err) {
			fprintf (stderr, "Error, ");
		}
		else {
			fprintf (stderr, "Warning, ");
		}
		fprintf (stderr, "line %d:%d - %s", t.line_num, t.char_pos, primary_err);
		if (optional_err != NULL) {
			fprintf (stderr, ": \"%s\"", optional_err);
		}
		fprintf (stderr, "\n");
	}

	void error (Token t, const char *primary_err, const char *optional_err) {
		errors_found++;
		message (1, t, primary_err, optional_err);
	}

	void warning (Token t, const char *primary_warning, const char *optional_warning) {
		message (0, t, primary_warning, optional_warning);
	}

	int variable_pos (const char* s) {
		int result = 0;
		int found = 0;
		while (result < variable_cnt) {
			if (strcmp (variables [result], s) == 0) {
				found = 1;
				break;
			}
			result++;
		}
		if (!found) {
			result = -1;
		}
		return result;
	}

	int constant_pos (const char* s) {
		int result = 0;
		int found = 0;
		while (result < constant_cnt) {
			if (strcmp (constants [result], s) == 0) {
				found = 1;
				break;
			}
			result++;
		}
		if (!found) {
			result = -1;
		}
		return result;
	}

	int label_pos (const char* s) {
		int result = 0;
		int found = 0;
		while (result < label_cnt) {
			if (strcmp (labels [result], s) == 0) {
				found = 1;
				break;
			}
			result++;
		}
		if (!found) {
			result = -1;
		}
		return result;
	}

	void store_instruction () {
		ram_data [address++] = (high_opcode << 3) | (high_reg);
		ram_data [address++] = low;
	}

#line 146 "assembler/sunyat-asm.c"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    ParseTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 53
#define YYACTIONTYPE unsigned char
#define ParseTOKENTYPE Token
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  int yy105;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL
#define ParseARG_PDECL
#define ParseARG_FETCH
#define ParseARG_STORE
#define YYNSTATE 109
#define YYNRULE 62
#define YYERRORSYMBOL 39
#define YYERRSYMDT yy105
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

/* The yyzerominor constant is used to initialize instances of
** YYMINORTYPE objects to zero. */
static const YYMINORTYPE yyzerominor = { 0 };

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
*/
#define YY_ACTTAB_COUNT (167)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   109,   96,  100,   16,   93,   94,   45,  108,   47,   46,
 /*    10 */    90,   44,  107,   43,   14,   13,   42,   41,   40,   39,
 /*    20 */    24,   23,   22,   21,   20,   19,   66,   37,   36,   35,
 /*    30 */    34,   33,   32,   31,   17,   28,   26,   25,   12,    2,
 /*    40 */    49,   15,   91,   50,  106,  105,  104,  103,  102,  101,
 /*    50 */    98,   97,   48,   95,   98,   97,   48,   95,   89,   72,
 /*    60 */    74,   38,   87,   98,   97,   48,   95,   98,   97,   48,
 /*    70 */    95,   83,  172,    1,   11,   81,   98,   97,   48,   95,
 /*    80 */    98,   97,   48,   95,   79,   10,    9,    8,   77,   98,
 /*    90 */    97,   48,   95,   98,   97,   48,   95,   65,    7,    6,
 /*   100 */    73,   63,   98,   97,   48,   95,   98,   97,   48,   95,
 /*   110 */    61,    5,    4,    3,   99,   59,   58,   18,   30,   56,
 /*   120 */    55,   27,   54,   53,   52,  173,  173,  173,   75,   71,
 /*   130 */    70,   69,   68,   67,   57,   29,  173,   92,  173,   85,
 /*   140 */    84,   51,   88,   86,  173,   82,   80,  173,  173,  173,
 /*   150 */   173,  173,  173,  173,  173,  173,  173,  173,  173,  173,
 /*   160 */   173,  173,   78,   76,   64,   62,   60,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     0,    5,    2,    3,   49,   50,    6,    1,    8,    9,
 /*    10 */    10,   11,    1,   13,   14,   15,   16,   17,   18,   19,
 /*    20 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*    30 */    30,   31,   32,   33,   34,   35,   36,   37,   38,    5,
 /*    40 */    39,    5,    5,   42,   43,   44,   45,   46,   47,   48,
 /*    50 */     4,    5,    6,    7,    4,    5,    6,    7,   12,    4,
 /*    60 */     5,    6,   12,    4,    5,    6,    7,    4,    5,    6,
 /*    70 */     7,   12,   40,   41,   12,   12,    4,    5,    6,    7,
 /*    80 */     4,    5,    6,    7,   12,   12,   12,   12,   12,    4,
 /*    90 */     5,    6,    7,    4,    5,    6,    7,   12,   12,   12,
 /*   100 */     5,   12,    4,    5,    6,    7,    4,    5,    6,    7,
 /*   110 */    12,   12,   12,   12,   49,   12,   12,   12,   12,   12,
 /*   120 */    12,   12,   12,   12,   12,   52,   52,   52,   51,   51,
 /*   130 */    51,   51,   51,   51,   51,   51,   52,   49,   52,   49,
 /*   140 */    49,   49,   49,   49,   52,   49,   49,   52,   52,   52,
 /*   150 */    52,   52,   52,   52,   52,   52,   52,   52,   52,   52,
 /*   160 */    52,   52,   49,   49,   49,   49,   49,
};
#define YY_SHIFT_USE_DFLT (-5)
#define YY_SHIFT_COUNT (50)
#define YY_SHIFT_MIN   (-4)
#define YY_SHIFT_MAX   (112)
static const signed char yy_shift_ofst[] = {
 /*     0 */    -5,    0,  102,   98,   89,   85,   76,   72,   63,   59,
 /*    10 */    50,   46,  102,  102,  102,  102,  102,   55,   55,   55,
 /*    20 */    55,   55,   55,   55,   55,  112,  111,  110,  109,  108,
 /*    30 */   107,  106,  105,  104,  103,  101,  100,   99,   95,   87,
 /*    40 */    86,   75,   74,   73,   62,   37,   36,   34,   -4,   11,
 /*    50 */     6,
};
#define YY_REDUCE_USE_DFLT (-46)
#define YY_REDUCE_COUNT (24)
#define YY_REDUCE_MIN   (-45)
#define YY_REDUCE_MAX   (117)
static const signed char yy_reduce_ofst[] = {
 /*     0 */    32,    1,  -45,  117,  116,  115,  114,  113,   97,   96,
 /*    10 */    94,   93,   92,   91,   90,   88,   65,   84,   83,   82,
 /*    20 */    81,   80,   79,   78,   77,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   112,  119,  128,  171,  171,  171,  171,  171,  171,  171,
 /*    10 */   171,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*    20 */   171,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*    30 */   171,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*    40 */   171,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*    50 */   171,  167,  166,  165,  164,  163,  162,  161,  160,  159,
 /*    60 */   158,  157,  156,  155,  154,  153,  152,  151,  150,  149,
 /*    70 */   148,  147,  170,  169,  168,  146,  145,  144,  143,  142,
 /*    80 */   141,  140,  139,  138,  137,  136,  135,  134,  133,  132,
 /*    90 */   131,  130,  129,  127,  126,  125,  124,  123,  122,  121,
 /*   100 */   120,  118,  117,  116,  115,  114,  113,  111,  110,
};

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
  int yyerrcnt;                 /* Shifts left before out of the error */
  ParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "EOL",           "MESSAGE",       "ADDR_DIRECTIVE",
  "IMMEDIATE",     "IDENTIFIER",    "LABEL_DIRECTIVE",  "CHARACTER",   
  "VAR_DIRECTIVE",  "CONST_DIRECTIVE",  "NOP",           "MOV",         
  "REGISTER",      "ADD",           "SWR",           "AWR",         
  "SUB",           "MUL",           "DIV",           "CMP",         
  "JMP",           "JEQ",           "JNE",           "JGR",         
  "JLS",           "CALL",          "RET",           "AND",         
  "OR",            "XOR",           "NOT",           "NEG",         
  "LOAD",          "LOADP",         "STOR",          "STORP",       
  "PUSH",          "POP",           "AUX",           "error",       
  "program",       "lines",         "line",          "message_line",
  "address_line",  "variable_line",  "constant_line",  "label_line",  
  "code_line",     "immediate",     "var_value",     "memory",      
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "program ::= lines",
 /*   1 */ "lines ::= lines line EOL",
 /*   2 */ "lines ::= lines error EOL",
 /*   3 */ "lines ::=",
 /*   4 */ "line ::= message_line",
 /*   5 */ "line ::= address_line",
 /*   6 */ "line ::= variable_line",
 /*   7 */ "line ::= constant_line",
 /*   8 */ "line ::= label_line",
 /*   9 */ "line ::= code_line",
 /*  10 */ "line ::=",
 /*  11 */ "message_line ::= MESSAGE",
 /*  12 */ "address_line ::= ADDR_DIRECTIVE immediate",
 /*  13 */ "immediate ::= IMMEDIATE",
 /*  14 */ "immediate ::= IDENTIFIER",
 /*  15 */ "immediate ::= LABEL_DIRECTIVE IDENTIFIER",
 /*  16 */ "immediate ::= CHARACTER",
 /*  17 */ "variable_line ::= VAR_DIRECTIVE IDENTIFIER var_value",
 /*  18 */ "var_value ::= immediate",
 /*  19 */ "var_value ::=",
 /*  20 */ "constant_line ::= CONST_DIRECTIVE IDENTIFIER immediate",
 /*  21 */ "label_line ::= LABEL_DIRECTIVE IDENTIFIER",
 /*  22 */ "code_line ::= NOP",
 /*  23 */ "code_line ::= MOV REGISTER REGISTER",
 /*  24 */ "code_line ::= MOV REGISTER immediate",
 /*  25 */ "code_line ::= ADD REGISTER REGISTER",
 /*  26 */ "code_line ::= ADD REGISTER immediate",
 /*  27 */ "code_line ::= SWR immediate",
 /*  28 */ "code_line ::= AWR immediate",
 /*  29 */ "code_line ::= SUB REGISTER REGISTER",
 /*  30 */ "code_line ::= SUB REGISTER immediate",
 /*  31 */ "code_line ::= MUL REGISTER REGISTER",
 /*  32 */ "code_line ::= MUL REGISTER immediate",
 /*  33 */ "code_line ::= DIV REGISTER REGISTER",
 /*  34 */ "code_line ::= DIV REGISTER immediate",
 /*  35 */ "code_line ::= CMP REGISTER REGISTER",
 /*  36 */ "code_line ::= CMP REGISTER immediate",
 /*  37 */ "code_line ::= JMP memory",
 /*  38 */ "code_line ::= JEQ memory",
 /*  39 */ "code_line ::= JNE memory",
 /*  40 */ "code_line ::= JGR memory",
 /*  41 */ "code_line ::= JLS memory",
 /*  42 */ "code_line ::= CALL memory",
 /*  43 */ "code_line ::= RET",
 /*  44 */ "code_line ::= AND REGISTER REGISTER",
 /*  45 */ "code_line ::= AND REGISTER immediate",
 /*  46 */ "code_line ::= OR REGISTER REGISTER",
 /*  47 */ "code_line ::= OR REGISTER immediate",
 /*  48 */ "code_line ::= XOR REGISTER REGISTER",
 /*  49 */ "code_line ::= XOR REGISTER immediate",
 /*  50 */ "code_line ::= NOT REGISTER",
 /*  51 */ "code_line ::= NEG REGISTER",
 /*  52 */ "code_line ::= LOAD REGISTER memory",
 /*  53 */ "code_line ::= LOADP REGISTER REGISTER",
 /*  54 */ "code_line ::= STOR memory REGISTER",
 /*  55 */ "code_line ::= STORP REGISTER REGISTER",
 /*  56 */ "code_line ::= PUSH REGISTER",
 /*  57 */ "code_line ::= POP REGISTER",
 /*  58 */ "code_line ::= AUX immediate",
 /*  59 */ "memory ::= IDENTIFIER",
 /*  60 */ "memory ::= LABEL_DIRECTIVE IDENTIFIER",
 /*  61 */ "memory ::= IMMEDIATE",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor(pParser, yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from ParseAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>YY_SHIFT_COUNT
   || (i = yy_shift_ofst[stateno])==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    if( iLookAhead>0 ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        return yy_find_shift_action(pParser, iFallback);
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
    }
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser, YYMINORTYPE *yypMinor){
   ParseARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
#line 155 "assembler/sunyat-asm.y"

	fprintf (stderr, "Giving up... parser stack overflow :-x\n");
#line 790 "assembler/sunyat-asm.c"
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer to the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser, yypMinor);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser, yypMinor);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 40, 1 },
  { 41, 3 },
  { 41, 3 },
  { 41, 0 },
  { 42, 1 },
  { 42, 1 },
  { 42, 1 },
  { 42, 1 },
  { 42, 1 },
  { 42, 1 },
  { 42, 0 },
  { 43, 1 },
  { 44, 2 },
  { 49, 1 },
  { 49, 1 },
  { 49, 2 },
  { 49, 1 },
  { 45, 3 },
  { 50, 1 },
  { 50, 0 },
  { 46, 3 },
  { 47, 2 },
  { 48, 1 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 2 },
  { 48, 2 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 2 },
  { 48, 2 },
  { 48, 2 },
  { 48, 2 },
  { 48, 2 },
  { 48, 2 },
  { 48, 1 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 2 },
  { 48, 2 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 3 },
  { 48, 2 },
  { 48, 2 },
  { 48, 2 },
  { 51, 1 },
  { 51, 2 },
  { 51, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  /* Silence complaints from purify about yygotominor being uninitialized
  ** in some cases when it is copied into the stack after the following
  ** switch.  yygotominor is uninitialized when a rule reduces that does
  ** not set the value of its left-hand side nonterminal.  Leaving the
  ** value of the nonterminal uninitialized is utterly harmless as long
  ** as the value is never used.  So really the only thing this code
  ** accomplishes is to quieten purify.  
  **
  ** 2007-01-16:  The wireshark project (www.wireshark.org) reports that
  ** without this code, their parser segfaults.  I'm not sure what there
  ** parser is doing to make this happen.  This is the second bug report
  ** from wireshark this week.  Clearly they are stressing Lemon in ways
  ** that it has not been previously stressed...  (SQLite ticket #2172)
  */
  /*memset(&yygotominor, 0, sizeof(yygotominor));*/
  yygotominor = yyzerominor;


  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
      case 0: /* program ::= lines */
#line 159 "assembler/sunyat-asm.y"
{
	if (errors_found == 0) {
		if (assembler_pass == 2) {
			FILE *outfile = fopen (filename, "wb");
			if (outfile == NULL) {
				fprintf (stderr, "Error - could not open output file for writing.\n");
			}
			else {
				fwrite (msg_data, sizeof (char), SIZE_APP_MSG, outfile);
				fwrite (ram_data, sizeof (char), SIZE_APP_RAM, outfile);
				fclose (outfile);
			}
		}
	}
	else {
		fprintf (stderr, "%d errors found.  No output file generated.\n", errors_found);
	}
}
#line 983 "assembler/sunyat-asm.c"
        break;
      case 9: /* line ::= code_line */
#line 186 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 1) {
		address += 2;
	}
}
#line 992 "assembler/sunyat-asm.c"
        break;
      case 11: /* message_line ::= MESSAGE */
#line 193 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 1) {
		if (yymsp[0].minor.yy0.token_str == NULL) {

			warning (yymsp[0].minor.yy0, ".MESSAGE string is empty... ignoring.", NULL);
		}
		else if (strlen (yymsp[0].minor.yy0.token_str) > SIZE_APP_MSG) {
			char s[100];
			sprintf (s, ".MESSAGE string longer than %d characters.", SIZE_APP_MSG);
			warning (yymsp[0].minor.yy0, s, NULL);
			memcpy (msg_data, yymsp[0].minor.yy0.token_str, SIZE_APP_MSG);
		}
		else {
			int i;
			memcpy (msg_data, yymsp[0].minor.yy0.token_str, strlen (yymsp[0].minor.yy0.token_str));
			for (i = strlen (yymsp[0].minor.yy0.token_str); i < SIZE_APP_MSG; i++) {
				msg_data [i] = '\0';
			}
		}
	}
}
#line 1017 "assembler/sunyat-asm.c"
        break;
      case 12: /* address_line ::= ADDR_DIRECTIVE immediate */
#line 215 "assembler/sunyat-asm.y"
{
	if ((uint8_t)yymsp[0].minor.yy0.data >= SIZE_APP_RAM) {
		if (assembler_pass == 1) {
			warning (yymsp[0].minor.yy0, ".ADDRESS must be no greater than 253 (inclusive)... ignoring", NULL);
		}
	}
	else {
		address = yymsp[0].minor.yy0.data;
	}
}
#line 1031 "assembler/sunyat-asm.c"
        break;
      case 13: /* immediate ::= IMMEDIATE */
#line 225 "assembler/sunyat-asm.y"
{
	memcpy (&yygotominor.yy0, &yymsp[0].minor.yy0, sizeof (Token));
}
#line 1038 "assembler/sunyat-asm.c"
        break;
      case 14: /* immediate ::= IDENTIFIER */
#line 228 "assembler/sunyat-asm.y"
{
//	if (assembler_pass == 2) {
		int pos = constant_pos (yymsp[0].minor.yy0.token_str);
		if (pos < 0) {
			error (yymsp[0].minor.yy0, "No such constant defined.", yymsp[0].minor.yy0.token_str);
			yygotominor.yy0.data = 0x00;
		}
		else {
			yygotominor.yy0.data = constant_values [pos];
		}
//	}
}
#line 1054 "assembler/sunyat-asm.c"
        break;
      case 15: /* immediate ::= LABEL_DIRECTIVE IDENTIFIER */
#line 240 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		int pos = label_pos (yymsp[0].minor.yy0.token_str);
		if (pos < 0) {
			error (yymsp[0].minor.yy0, "No such label defined.", yymsp[0].minor.yy0.token_str);
			yygotominor.yy0.data = 0x00;
		}
		else {
			yygotominor.yy0.data = label_addrs [pos];
		}
        //printf("DOING LABEL %s %X \n", yymsp[0].minor.yy0.token_str, label_addrs [pos]) ;
	}
}
#line 1071 "assembler/sunyat-asm.c"
        break;
      case 16: /* immediate ::= CHARACTER */
#line 253 "assembler/sunyat-asm.y"
{
	//if (assembler_pass == 2) {
		yygotominor.yy0.data = yymsp[0].minor.yy0.data;
	//}
}
#line 1080 "assembler/sunyat-asm.c"
        break;
      case 17: /* variable_line ::= VAR_DIRECTIVE IDENTIFIER var_value */
#line 259 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 1) {
		int pos = variable_pos (yymsp[-1].minor.yy0.token_str);
		if (pos >= 0) {
			char s[100];
			sprintf (s, "Variable already defined at address 0x%X.", variable_addrs [pos]);
			error (yymsp[-1].minor.yy0, s, variables [pos]);
		}
		else if (variable_cnt >= MAX_CNT) {
			error (yymsp[-1].minor.yy0, "Too many variables defined.", yymsp[-1].minor.yy0.token_str);
		}
		else if ((pos = constant_pos (yymsp[-1].minor.yy0.token_str)) >= 0 ) {
			char s[100];
			sprintf (s, "Variable name inuse by constant with value 0x%X.", constant_values [pos]);
			error (yymsp[-1].minor.yy0, s, NULL);
		}
		else {
			/* new variable defined */
			int len = strlen (yymsp[-1].minor.yy0.token_str) + 1;
			variables [variable_cnt] = malloc (sizeof (char) * len);
			strcpy (variables [variable_cnt], yymsp[-1].minor.yy0.token_str);
			variable_addrs [variable_cnt] = address;

			variable_cnt++;
			address++;
		}
	}
	else {
        ram_data [address] = yymsp[0].minor.yy0.data;
		address++;
	}
}
#line 1116 "assembler/sunyat-asm.c"
        break;
      case 18: /* var_value ::= immediate */
      case 61: /* memory ::= IMMEDIATE */ yytestcase(yyruleno==61);
#line 291 "assembler/sunyat-asm.y"
{
	yygotominor.yy0.data = yymsp[0].minor.yy0.data;
}
#line 1124 "assembler/sunyat-asm.c"
        break;
      case 19: /* var_value ::= */
#line 294 "assembler/sunyat-asm.y"
{
	yygotominor.yy0.data = 0x00;
}
#line 1131 "assembler/sunyat-asm.c"
        break;
      case 20: /* constant_line ::= CONST_DIRECTIVE IDENTIFIER immediate */
#line 298 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 1) {
		int pos = constant_pos (yymsp[-1].minor.yy0.token_str);
		if (pos >= 0) {
			char s[100];
			sprintf (s, "Constant already defined as 0x%X.", constant_values [pos]);
			error (yymsp[-1].minor.yy0, s, NULL);
		}
		else if (constant_cnt >= MAX_CNT) {
			error (yymsp[-1].minor.yy0, "Too many constants defined.", yymsp[-1].minor.yy0.token_str);
		}
		else if ((pos = variable_pos (yymsp[-1].minor.yy0.token_str)) >= 0 ) {
			char s[100];
			sprintf (s, "Constant name inuse by variable at address 0x%X.", variable_addrs [pos]);
			error (yymsp[-1].minor.yy0, s, NULL);
		}
		else {
			/* new const defined */
			int len = strlen (yymsp[-1].minor.yy0.token_str) + 1;
			constants [constant_cnt] = malloc (sizeof (char) * len);
			strcpy (constants [constant_cnt], yymsp[-1].minor.yy0.token_str);
			constant_values [constant_cnt] = yymsp[0].minor.yy0.data;
			constant_cnt++;
		}
	}
}
#line 1161 "assembler/sunyat-asm.c"
        break;
      case 21: /* label_line ::= LABEL_DIRECTIVE IDENTIFIER */
#line 325 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 1) {
		int pos = label_pos (yymsp[0].minor.yy0.token_str);
		if (pos >= 0) {
			char s[100];
			sprintf (s, "Label already defined at address 0x%X.", label_addrs [pos]);
			error (yymsp[0].minor.yy0, s, yymsp[0].minor.yy0.token_str);
		}
		else if (label_cnt >= MAX_CNT) {
			error (yymsp[0].minor.yy0, "Too many labels defined.", yymsp[0].minor.yy0.token_str);
		}
		else {
			/* new label defined */
			int len = strlen (yymsp[0].minor.yy0.token_str) + 1;
			labels [label_cnt] = malloc (sizeof (char) * len);
			strcpy (labels [label_cnt], yymsp[0].minor.yy0.token_str);
			label_addrs [label_cnt] = address;
			label_cnt++;
		}
	}
}
#line 1186 "assembler/sunyat-asm.c"
        break;
      case 22: /* code_line ::= NOP */
#line 347 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		/* same as JMP to next line */
		high_opcode = OPCODE_JMP_M;
		high_reg = 0;
		low = address + 2;
		store_instruction ();
	}
}
#line 1199 "assembler/sunyat-asm.c"
        break;
      case 23: /* code_line ::= MOV REGISTER REGISTER */
#line 357 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_MOV_RR;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1211 "assembler/sunyat-asm.c"
        break;
      case 24: /* code_line ::= MOV REGISTER immediate */
#line 366 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_MOV_RI;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1223 "assembler/sunyat-asm.c"
        break;
      case 25: /* code_line ::= ADD REGISTER REGISTER */
#line 375 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_ADD_RR;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1235 "assembler/sunyat-asm.c"
        break;
      case 26: /* code_line ::= ADD REGISTER immediate */
#line 384 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_ADD_RI;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1247 "assembler/sunyat-asm.c"
        break;
      case 27: /* code_line ::= SWR immediate */
#line 393 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_SWR_I;
		high_reg = 0 ; //dst.data;
		if (yymsp[0].minor.yy0.data > MAX_WIN_INDEX) {
            error (yymsp[0].minor.yy0, "Your set of the beginning window was to large.", yymsp[0].minor.yy0.token_str) ;
        }
        low = (uint8_t)yymsp[0].minor.yy0.data ;
		store_instruction ();
	}
}
#line 1262 "assembler/sunyat-asm.c"
        break;
      case 28: /* code_line ::= AWR immediate */
#line 405 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_AWR_I;
		high_reg = 0 ; //dst.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1274 "assembler/sunyat-asm.c"
        break;
      case 29: /* code_line ::= SUB REGISTER REGISTER */
#line 415 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_ADD_RR;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)(-yymsp[0].minor.yy0.data);
		store_instruction ();
	}
}
#line 1286 "assembler/sunyat-asm.c"
        break;
      case 30: /* code_line ::= SUB REGISTER immediate */
#line 426 "assembler/sunyat-asm.y"
{ //THIS IS A PSEUDO-INSTRUCTION
	if (assembler_pass == 2) {
		high_opcode = OPCODE_ADD_RI;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)(-yymsp[0].minor.yy0.data);
		store_instruction ();
	}
}
#line 1298 "assembler/sunyat-asm.c"
        break;
      case 31: /* code_line ::= MUL REGISTER REGISTER */
#line 435 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_MUL_RR;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1310 "assembler/sunyat-asm.c"
        break;
      case 32: /* code_line ::= MUL REGISTER immediate */
#line 444 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_MUL_RI;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1322 "assembler/sunyat-asm.c"
        break;
      case 33: /* code_line ::= DIV REGISTER REGISTER */
#line 453 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_DIV_RR;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1334 "assembler/sunyat-asm.c"
        break;
      case 34: /* code_line ::= DIV REGISTER immediate */
#line 462 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_DIV_RI;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1346 "assembler/sunyat-asm.c"
        break;
      case 35: /* code_line ::= CMP REGISTER REGISTER */
#line 471 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_CMP_RR;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1358 "assembler/sunyat-asm.c"
        break;
      case 36: /* code_line ::= CMP REGISTER immediate */
#line 480 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_CMP_RI;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1370 "assembler/sunyat-asm.c"
        break;
      case 37: /* code_line ::= JMP memory */
#line 489 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_JMP_M;
		high_reg = 0;
		low = yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1382 "assembler/sunyat-asm.c"
        break;
      case 38: /* code_line ::= JEQ memory */
#line 498 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_JEQ_M;
		high_reg = 0;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1394 "assembler/sunyat-asm.c"
        break;
      case 39: /* code_line ::= JNE memory */
#line 507 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_JNE_M;
		high_reg = 0;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1406 "assembler/sunyat-asm.c"
        break;
      case 40: /* code_line ::= JGR memory */
#line 516 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_JGR_M;
		high_reg = 0;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1418 "assembler/sunyat-asm.c"
        break;
      case 41: /* code_line ::= JLS memory */
#line 525 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_JLS_M;
		high_reg = 0;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1430 "assembler/sunyat-asm.c"
        break;
      case 42: /* code_line ::= CALL memory */
#line 534 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_CALL_M;
		high_reg = 0;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1442 "assembler/sunyat-asm.c"
        break;
      case 43: /* code_line ::= RET */
#line 543 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_RET;
		high_reg = 0;
		low = 0;
		store_instruction ();
	}
}
#line 1454 "assembler/sunyat-asm.c"
        break;
      case 44: /* code_line ::= AND REGISTER REGISTER */
#line 552 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_AND_RR;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1466 "assembler/sunyat-asm.c"
        break;
      case 45: /* code_line ::= AND REGISTER immediate */
#line 561 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_AND_RI;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1478 "assembler/sunyat-asm.c"
        break;
      case 46: /* code_line ::= OR REGISTER REGISTER */
#line 570 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_OR_RR;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1490 "assembler/sunyat-asm.c"
        break;
      case 47: /* code_line ::= OR REGISTER immediate */
#line 579 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_OR_RI;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1502 "assembler/sunyat-asm.c"
        break;
      case 48: /* code_line ::= XOR REGISTER REGISTER */
#line 588 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_XOR_RR;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1514 "assembler/sunyat-asm.c"
        break;
      case 49: /* code_line ::= XOR REGISTER immediate */
#line 597 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_XOR_RI;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1526 "assembler/sunyat-asm.c"
        break;
      case 50: /* code_line ::= NOT REGISTER */
#line 606 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		/* same as XOR REG 0xFF */
		high_opcode = OPCODE_XOR_RI;
		high_reg = yymsp[0].minor.yy0.data;
		low = 0xFF;
		store_instruction ();
	}
}
#line 1539 "assembler/sunyat-asm.c"
        break;
      case 51: /* code_line ::= NEG REGISTER */
#line 618 "assembler/sunyat-asm.y"
{ //THIS IS A PSEUDO-INSTRUCTION
	if (assembler_pass == 2) {
		high_opcode = OPCODE_MUL_RI;
		high_reg = yymsp[0].minor.yy0.data;
		low = -1;
		store_instruction ();
	}
}
#line 1551 "assembler/sunyat-asm.c"
        break;
      case 52: /* code_line ::= LOAD REGISTER memory */
#line 627 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_LOAD_RM;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1563 "assembler/sunyat-asm.c"
        break;
      case 53: /* code_line ::= LOADP REGISTER REGISTER */
#line 636 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_LOADP_RR;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1575 "assembler/sunyat-asm.c"
        break;
      case 54: /* code_line ::= STOR memory REGISTER */
#line 645 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_STOR_MR;
		high_reg = yymsp[0].minor.yy0.data; /* yymsp[0].minor.yy0 and yymsp[-1].minor.yy0 are reverse in the encoding */
		low = (uint8_t)yymsp[-1].minor.yy0.data;
		store_instruction ();
	}
}
#line 1587 "assembler/sunyat-asm.c"
        break;
      case 55: /* code_line ::= STORP REGISTER REGISTER */
#line 654 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_STORP_RR;
		high_reg = yymsp[-1].minor.yy0.data;
		low = (uint8_t)yymsp[0].minor.yy0.data;
		store_instruction ();
	}
}
#line 1599 "assembler/sunyat-asm.c"
        break;
      case 56: /* code_line ::= PUSH REGISTER */
#line 663 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_STACKER_R;
		high_reg = yymsp[0].minor.yy0.data;
		low = 0;
		store_instruction ();
	}
}
#line 1611 "assembler/sunyat-asm.c"
        break;
      case 57: /* code_line ::= POP REGISTER */
#line 672 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_STACKER_R;
		high_reg = yymsp[0].minor.yy0.data;
		low = 1;
		store_instruction ();
	}
}
#line 1623 "assembler/sunyat-asm.c"
        break;
      case 58: /* code_line ::= AUX immediate */
#line 681 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		high_opcode = OPCODE_AUX_I;
		high_reg = 0 ; //dst.data;
		if (yymsp[0].minor.yy0.data < 0 || yymsp[0].minor.yy0.data > 7) {
            error (yymsp[0].minor.yy0, "0-7 are acceptable values.", yymsp[0].minor.yy0.token_str) ;
        }
        low = (uint8_t)yymsp[0].minor.yy0.data ;
		store_instruction ();
	}
}
#line 1638 "assembler/sunyat-asm.c"
        break;
      case 59: /* memory ::= IDENTIFIER */
#line 695 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		int pos = variable_pos (yymsp[0].minor.yy0.token_str);
		if (pos < 0) {
			/* not a var, check for const */
			pos = constant_pos (yymsp[0].minor.yy0.token_str);
			if (pos < 0) {
				error (yymsp[0].minor.yy0, "No such variable or constant defined.", yymsp[0].minor.yy0.token_str);
				yygotominor.yy0.data = 0x00;
			}
			else {
				yygotominor.yy0.data = constant_values [pos];
			}
		}
		else {
			yygotominor.yy0.data = variable_addrs [pos];
		}
	}
}
#line 1661 "assembler/sunyat-asm.c"
        break;
      case 60: /* memory ::= LABEL_DIRECTIVE IDENTIFIER */
#line 714 "assembler/sunyat-asm.y"
{
	if (assembler_pass == 2) {
		int pos = label_pos (yymsp[0].minor.yy0.token_str);
		if (pos < 0) {
			error (yymsp[0].minor.yy0, "No such label defined.", yymsp[0].minor.yy0.token_str);
			yygotominor.yy0.data = 0x00;
		}
		else {
			yygotominor.yy0.data = label_addrs [pos];
		}
	}
}
#line 1677 "assembler/sunyat-asm.c"
        break;
      default:
      /* (1) lines ::= lines line EOL */ yytestcase(yyruleno==1);
      /* (2) lines ::= lines error EOL */ yytestcase(yyruleno==2);
      /* (3) lines ::= */ yytestcase(yyruleno==3);
      /* (4) line ::= message_line */ yytestcase(yyruleno==4);
      /* (5) line ::= address_line */ yytestcase(yyruleno==5);
      /* (6) line ::= variable_line */ yytestcase(yyruleno==6);
      /* (7) line ::= constant_line */ yytestcase(yyruleno==7);
      /* (8) line ::= label_line */ yytestcase(yyruleno==8);
      /* (10) line ::= */ yytestcase(yyruleno==10);
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact < YYNSTATE ){
#ifdef NDEBUG
    /* If we are not debugging and the reduce action popped at least
    ** one element off the stack, then we can push the new element back
    ** onto the stack here, and skip the stack overflow test in yy_shift().
    ** That gives a significant speed improvement. */
    if( yysize ){
      yypParser->yyidx++;
      yymsp -= yysize-1;
      yymsp->stateno = (YYACTIONTYPE)yyact;
      yymsp->major = (YYCODETYPE)yygoto;
      yymsp->minor = yygotominor;
    }else
#endif
    {
      yy_shift(yypParser,yyact,yygoto,&yygotominor);
    }
  }else{
    assert( yyact == YYNSTATE + YYNRULE + 1 );
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
#line 151 "assembler/sunyat-asm.y"

	fprintf (stderr, "Giving up... the parser has gotten completely lost :-x\n");
#line 1737 "assembler/sunyat-asm.c"
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 145 "assembler/sunyat-asm.y"

	fprintf (stderr, "Syntax error, line %d:%d - \"%s\"\n", TOKEN.line_num
	, TOKEN.char_pos, TOKEN.long_str);
	errors_found++;
#line 1757 "assembler/sunyat-asm.c"
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      /*memset(&yyminorunion, 0, sizeof(yyminorunion));*/
      yyminorunion = yyzerominor;
      yyStackOverflow(yypParser, &yyminorunion);
      return;
    }
#endif
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact<YYNSTATE ){
      assert( !yyendofinput );  /* Impossible to shift the $ token */
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      yymajor = YYNOCODE;
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor,yyminorunion);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}
