# Name        : Makefile
# Author      : William "Amos" Confer
#
# License     : Copyright (c) 2008--2014 William "Amos" Confer
#
#    Permission is hereby granted, free of charge, to any person obtaining a
#    copy of this software and associated documentation files (the "Software"),
#    to deal in the Software without restriction, including without limitation
#    the rights to use, copy, modify, merge, publish, distribute, sublicense,
#    and/or sell copies of the Software, and to permit persons to whom the
#    Software is furnished to do so, subject to the following conditions:
#
#    The above copyright notice and this permission notice shall be included in
#    all copies or substantial portions of the Software.;
#
#    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#    DEALINGS IN THE SOFTWARE.

all:	sunyat sunyat-asm

#windowtest

#----------------------------------------
# sunyat
#----------------------------------------
sunyat:	main.c sunyat.c sunyat.h sat_scr.h sat_scr.c debugger.c debugger.h
	gcc -g -o ../bin/sunyat main.c sunyat.c debugger.c sat_scr.c  -lncurses

#----------------------------------------
# sunyat-asm
#----------------------------------------
sunyat-asm:	lemon sunyat-asm.c lex.yy.c assembler/token.h sunyat.h
	gcc -g -o ../bin/sunyat-asm assembler/lex.yy.c assembler/sunyat-asm.c

lemon:
	gcc -o ../bin/lemon ../tools/lemon/lemon.c

lex.yy.c:	assembler/sunyat-asm.lex
	flex assembler/sunyat-asm.lex
	mv lex.yy.c assembler/

sunyat-asm.h:
sunyat-asm.c:	assembler/sunyat-asm.y sunyat-asm.h lemon
	../bin/lemon T=../tools/lemon/lempar.c assembler/sunyat-asm.y


#----------------------------------------
# clean
#----------------------------------------
clean:
	rm -f ../bin/lemon ../bin/sunyat ../bin/sunyat-asm assembler/lex.yy.c assembler/sunyat-asm.c assembler/sunyat-asm.h assembler/sunyat-asm.out
#windowtest.rom
