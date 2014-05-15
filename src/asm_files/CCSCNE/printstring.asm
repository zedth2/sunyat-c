
.message	Prints Strings.

.constant	TERM	0xFF

.constant	CR	0xD
.constant	LF	0xA
.constant	NULL 0x00

	jmp	!main

!crlf
	.variable	crlf0	CR
	.variable	crlf1	LF
	.variable	crlf2	NULL ;used as a string delimiter

.variable	key

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
	.variable prompt11 NULL

!command_buffer
	.variable sb0 ' '
	.variable sb1 ' '
	.variable sb2 ' '
	.variable sb3 ' '
	.variable sb4 ' '
	.variable sb5 ' '
	.variable sb6 ' '
	.variable sb7 ' '
	.variable sb8 ' '
	.variable sb9 ' '
	.variable sb10 ' '
	.variable sb11 ' '

!main
	mov R0 !prompt
	call !print
	
	call !build_command ;get a 'command'
	
	mov R0 !crlf
	call !print_line

	;Echo user input-------------------------
		mov R0 !command_buffer
		call !print
		call !print_line
	;----------------------------------------
	jmp !main ;back to the top
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
;-!build_command----------------------------------------------------------------------------------
; Builds a 'command' string, stored in 'command buffer'
;-Reads chars from terminal (ends when it catches a LF from ENTER)
;-Makes sure they're readable
;-Drops them into !command_buffer
;
;-Stores the LF for delimiting later, but does not print it.
;------------------------------------------------------------------------------------------------
!build_command ;BC
	push R0
	push R1
	mov R0 !command_buffer ;get pointer
	!bc_while
		!get_character ;gc
			load R1 TERM ;user input
			!gc_while ;check the char (valid ascii)
				cmp R1 0x00
				jls !get_character ;char !< NULL
				cmp R1 0x7F
				jgr	!get_character ;char !> DEL
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
;------------------------------------------------------------------------------------------------