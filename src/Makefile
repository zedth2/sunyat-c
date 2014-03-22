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
sunyat:	sunyat.c sunyat.h build_number.h
	gcc -g -o ../bin/sunyat sunyat.c  -lncurses

#----------------------------------------
# sunyat-asm
#----------------------------------------
sunyat-asm:	lemon sunyat-asm.c lex.yy.c build_number.h assembler/token.h sunyat.h
	gcc -g -o ../bin/sunyat-asm assembler/lex.yy.c assembler/sunyat-asm.c

lemon:
	gcc -o ../bin/lemon ../tools/lemon/lemon.c

lex.yy.c:	assembler/sunyat-asm.lex
	flex assembler/sunyat-asm.lex
	mv lex.yy.c assembler/

sunyat-asm.h:
sunyat-asm.c:	assembler/sunyat-asm.y sunyat-asm.h lemon
	../bin/lemon T=../tools/lemon/lempar.c assembler/sunyat-asm.y

build_number.h:	next_build_number

next_build_number:
	gcc -o ../bin/build_number build_number.c
	../bin/build_number


#----------------------------------------
# Run on make
#----------------------------------------

#windowtest: sunyat windowtest.rom build_number.h
#	./sunyat windowtest.rom
#
#windowtest.rom: sunyat-asm windowtest.asm build_number.h
#	./sunyat-asm windowtest.asm


#----------------------------------------
# clean
#----------------------------------------
clean:
	rm -f ../bin/build_number ../bin/lemon ../bin/sunyat ../bin/sunyat-asm assembler/lex.yy.c assembler/sunyat-asm.c assembler/sunyat-asm.h assembler/sunyat-asm.out
#windowtest.rom
