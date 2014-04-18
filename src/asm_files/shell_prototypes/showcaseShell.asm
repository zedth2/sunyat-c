;Savestate
;AWR
;Print Regs
;Populate Regs
;

.constant	TERM 	0xFF
.constant	CR	0xD
.constant	LF	0xA

jmp !main
!command_buffer
	.variable cmd0 ' '
	.variable cmd1 ' '
	.variable cmd2 ' '
	.variable cmd3 ' '
	.variable cmd4 ' '


!main
	call !build_command
	call !translate
!main_end

!translate
	mov R0 !command_buffer
	!analyze_while
		loadp R1 R0
		loadp R3 R2
		cmp R1 0x00 ;Hit delimiter, run code.
		jeq !analyze_return
!translate_end






!build_command ;BC
	push R0
	push R1
	mov R0 !command_buffer ;get pointer
	!bc_while
		!get_character ;gc
			load R1 TERM ;user input
			!gc_while ;check the char (valid ascii)
				cmp R1 0xFF
				jgr !get_character ;char !< NULL
			!gc_while_end
		!get_character_end
		cmp R1 LF
		jeq !bc_while_end ;hit "enter" - we're done here.
		storp R0 R1
		add R0 1 ;increment pointer
		stor TERM R1
		jmp !bc_while
	!bc_while_end
	mov R1 0x00
	storp R0 R1 ;Drop the delimiter (NULL)
	pop R1
	pop	R0
	ret
!build_command_end


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