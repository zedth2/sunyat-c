;sunyat_hex.asm------------------------------
;Written by: Ben Foster
;
;Prompts users for 8-bit decimal numbers and
;converts them to hex using a bitwise AND for
;the low bits and a synthesized 4-bit Right
;Shift for the highs.
;
;'q' to exit.
;
;--------------------------------------------


.message "'q' to exit"
.constant	TERM 	0xFF
.constant	CR	0xD
.constant	LF	0xA

jmp !main

!crlf
	.variable	crlf0	CR
	.variable	crlf1	LF
	.variable	crlf2	0x00

!hex_table
	.variable hex0 '0'
	.variable hex1 '1'
	.variable hex2 '2'
	.variable hex3 '3'
	.variable hex4 '4'
	.variable hex5 '5'
	.variable hex6 '6'
	.variable hex7 '7'
	.variable hex8 '8'
	.variable hex9 '9'
	.variable hex10 'A'
	.variable hex11 'B'
	.variable hex12 'C'
	.variable hex13 'D'
	.variable hex14 'E'
	.variable hex15 'F'

!prompt
	.variable prompt0 'E'
	.variable prompt1 'n'
	.variable prompt2 't'
	.variable prompt3 'e'
	.variable prompt4 'r'
	.variable prompt5 ' '
	.variable prompt6 'a'
	.variable prompt7 ' '
	.variable prompt8 'n'
	.variable prompt9 'u'
	.variable prompt10 'm'
	.variable prompt11 'b'
	.variable prompt12 'e'
	.variable prompt13 'r'
	.variable prompt14 ':'
	.variable prompt15 0x00

!ox
	.variable ox0 '0'
	.variable ox1 'x'
	.variable ox3 0x00


;main-----------------------------------------------------------------------------------
;Prints the prompt, calls !getNum and !analyze
;Loops
;---------------------------------------------------------------------------------------
!main
	mov R0 !prompt
	call !print
	call !getNum	;returns user entered number into R7
	call !analyze
	jmp !main
!main_end
ret
;---------------------------------------------------------------------------------------


	;getNum---------------------------------------------------------------------------------
	;Builds a number out of user-entered characters
	;
	;---------------------------------------------------------------------------------------
	!getNum
		push R0
		push R1
		mul R7 0	;Make sure R7 is Zero
		
		;getNum_getDigit-------------------------------
		;Make sure chars entered are acceptable
		;---------------------------------------------
		!getNum_getDigit
			load R0 TERM
			cmp R0 LF
			jeq !getNum_end
			cmp R0 'q'
			jeq !bye
			cmp R0 '0'
			jls !getNum_getDigit
			cmp R0 '9'
			jgr !getNum_getDigit
		!getNum_getDigit_end
		;---------------------------------------------

		;getNum_buildNum------------------------------
		;Build a number in R7 out of chars entered.
		;---------------------------------------------
		!getNum_buildNum
			stor TERM R0
			add R0 -48	;Character - Integer ascii offset ('48' = 0)
			mul R7 10	;Tack zeros on to running value
			add R7 R0	;Add current char to running value.

			;This doesn't work for some reason.-----------------------------------------------
			;cmp R7 255	;If the number they've entered so far is too big, throw an error.
			;jgr !err_TooBig
			;cmp R7 99	;If the number they've entered is 3 digits, leave the loop.
			;jgr !getNum_end
			;---------------------------------------------------------------------------------

			jmp !getNum_getDigit ;Otherwise, keep getting digits.

		!getNum_buildNum_end
		;---------------------------------------------

	!getNum_end
		pop R1
		pop R0
		call !print_line
	ret
	;---------------------------------------------------------------------------------------


	;analyze--------------------------------------------------------------------------------
	;Convert 8-bit number to hex and print it.
	;
	;---------------------------------------------------------------------------------------
	!analyze
		push R3
		push R4
		mov R2 R7	;copy R7 into R2

		;get the low bits---
		and R7 0b0000_1111
		;-------------------

		;get the high bits----
		and R2 0b1111_0000
		div R2 16
		;---------------------

		mov R3 !hex_table
		mov R4 !hex_table
		add R4 R2 ;high bits
		add R3 R7 ;low bits
		loadp R7 R3
		loadp R2 R4
		mov R0 !ox
		call !print
		stor TERM R2 ;high
		stor TERM R7 ;low
	!analyze_end
		pop R3
		pop R4
		call !print_line
	ret
	;---------------------------------------------------------------------------------------


;-!print_line------------------------------------------------------------------------------------
;-Prints a newline (!crlf) when called
;------------------------------------------------------------------------------------------------
!print_line
	push R0
	mov R0 !crlf
	call !print
	pop R0
!print_line_end
ret
;------------------------------------------------------------------------------------------------

;-!print-----------------------------------------------------------------------------------------
;-Prints chars individually from a memory location moved into R0
;-Stops when it finds a 0x00
;------------------------------------------------------------------------------------------------
!print
	push R0 ;backup R1
	push R1 ;backup R0
	!while_PP
		loadp R1 R0 ;Load character at address R0 into R1. R0 is an array pointer.
		cmp R1 0x00
		jeq !while_PP_end ;If the character is 0x00 stop printing.
		stor TERM R1 ;print character
		add R0 1 ;Increment array pointer
		jmp !while_PP ;keep printing
	!while_PP_end
	pop R1	;return value
	pop R0	;return value
!print_end
ret
;------------------------------------------------------------------------------------------------

;Exit----------
!bye
	pop R1
	pop R0
	pop R3	;address in !main
	mov R0 !goodbye
	call !print
	jmp !main_end

!goodbye
	.variable goodbye0 'B'
	.variable goodbye1 'y'
	.variable goodbye2 'e'
	.variable goodbye3 0x00

;ERRORS------------
!err_TooBig
	pop R1
	pop R0
	pop R3
	call !print_line
	push R0
	mov R0 !tooBig
	call !print
	call !print_line
	pop R0
	jmp !main

	!tooBig
		.variable tooBig0 't'
		.variable tooBig1 'o'
		.variable tooBig2 'o'
		.variable tooBig3 ' '
		.variable tooBig4 'b'
		.variable tooBig5 'i'
		.variable tooBig6 'g'