;------------------------------------
;The SUNYAT Shell
;'Fast and Loose' edition
;Uses hard-coded memory addresses instead of string analysis.
;------------------------------------


;.message	SUNYAT Shell 'Fast and Loose' Prototype
.constant	TERM 	0xFF
.constant	CR	0xD
.constant	LF	0xA


jmp !main
!cmd_table
	.variable	cmd_exit_0	!cmd_exit
	.variable	cmd_reset_1	!cmd_reset
	.variable	cmd_awr_2	!cmd_awr
	.variable	cmd_populate_reg_3	!cmd_populate_reg
	.variable	cmd_savestate_4	!cmd_savestate
;commands--------------------------------------------------------
!cmd_exit
	jmp !exit_shell
!cmd_reset
	mov R0 'R'
	STOR TERM R0
	swr 5
	jmp !get_cmd_end
!cmd_awr
	awr 1
	jmp !get_cmd_end
!cmd_populate_reg
	mov R7 'Z'
	jmp !get_cmd_end
!cmd_savestate
	aux 0
	jmp !get_cmd_end

!cmd_win_end;
;end of commands-------------------------------------------------

!crlf
	.variable	crlf0	CR
	.variable	crlf1	LF
	.variable	crlf2	0x00
!prompt
	.variable prompt0 '['
	.variable prompt1 'm'
	.variable prompt2 'e'
	.variable prompt3 '@'
	.variable prompt4 'S'
	.variable prompt5 'U'
	.variable prompt6 'N'
	.variable prompt7 'Y'
	.variable prompt8 'A'
	.variable prompt9 'T'
	.variable prompt10 ']'
	.variable prompt11 0x00
!main
	mov R0 !prompt
	call !print ;print the prompt
	!get_cmd
		load R0 TERM
		!gc_while
			cmp R0 '0'
			jls !get_cmd
			cmp R0 '9'
			jgr	!get_cmd
		!gc_while_end
		stor TERM R0
		call !print_line
	add R0 -46 ;0 = 48(ascii) !table is location 2.
	;stor TERM R0
	;call !print_line
	loadp R1 R0
	;stor TERM R1
	;call !print_line
	push R1
	ret
	!get_cmd_end

	call !print_line
jmp !main ;back to the top
	!exit_shell
	ret
!main_end


;-!print-----------------------------------------------------------------------------------------
;-Prints chars individually from a memory location moved into R0
;-Stops when it finds a 0x00
;
;-Does not print the 0x00
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
	ret
!print_end
;------------------------------------------------------------------------------------------------

;-!print_line------------------------------------------------------------------------------------
;-Prints a newline (!crlf) when called
;------------------------------------------------------------------------------------------------
!print_line
	push R0
	mov R0 !crlf
	call !print
	pop R0
	ret
!print_line_end
;------------------------------------------------------------------------------------------------
