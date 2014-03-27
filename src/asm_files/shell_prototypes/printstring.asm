



.message	Shell Groundwork

.constant	TERM	0xFF

.constant	CR	0xD
.constant	LF	0xA

	jmp	!main

!crlf
	.variable	crlf0	CR
	.variable	crlf1	LF
	.variable	crlf2	'%'

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
	.variable prompt11 '%'

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
	.variable sb12 ' '
	.variable sb13 ' '
	.variable sb14 ' '
	.variable sb15 ' '
	.variable sb16 ' '
	.variable sb17 ' '
	.variable sb18 ' '
	.variable sb19 ' '

!main
	mov R0 !prompt
	call !print_prompt ;print the prompt
	
	call !build_command ;get a 'command'
	
	mov R0 !crlf
	call !print_prompt ;print a line

	;Echo user input-------------------------
		mov R0 !command_buffer
		call !print_string ;echo the 'command'
		mov R0 !crlf
		call !print_prompt ;print a line
	;----------------------------------------
	jmp !main ;back to the top
	ret
!main_end


;-!print_prompt----------------------------------------------------------------------------------
;-Prints chars individually from a memory location moved into R0
;-Stops when it finds a '%'
;-Intended for printing !prompt and !crlf
;
;-Does not print the '%'
;------------------------------------------------------------------------------------------------
!print_prompt
	push R0 ;backup R1
	push R1 ;backup R0
	!while_PP
		loadp R1 R0 ;Load character at address R0 into R1. R0 is an array pointer.
		cmp R1 '%'
		jeq !while_PP_end ;If the character is '%' stop printing
		stor TERM R1 ;print character
		add R0 1 ;Increment array pointer
		jmp !while_PP ;keep printing
	!while_PP_end
		pop R1	;return value
		pop R0	;return value
	ret
!print_prompt_end
;------------------------------------------------------------------------------------------------

;-!print_string----------------------------------------------------------------------------------
;-Prints chars individually from a memory location moved into R0
;-Stops when it finds a LF
;-Intended for standard output.
;
;-Does not print the LF
;------------------------------------------------------------------------------------------------
!print_string ;Prints chars until it hits an LF (linefeed)
	push R0 ;backup R1
	push R1 ;backup R0
	!while_PS
		loadp R1 R0 ;Load character at address R0 into R1. R0 is an array pointer
		cmp R1 LF
		jeq !while_PS_end ;If the character is LF stop printing
		stor TERM R1 ;print character
		add R0 1 ;Increment array pointer
		jmp !while_PS ;keep printing
	!while_PS_end
		pop R1 ;return value
		pop R0 ;return value
	ret
!print_string_end
;------------------------------------------------------------------------------------------------

;-!build_command----------------------------------------------------------------------------------
;-Reads chars from terminal (ends when it catches a LF from ENTER)
;-Makes sure they're readable
;-Drops them into !command_buffer
;
;-Stores the LF for delimiting later, but does not print it.
;------------------------------------------------------------------------------------------------
!build_command ;BC
	push R0 ;backup R0
	push R1 ;backup R1
	mov R0 !command_buffer ;R0 becomes string pointer
	!do_BC ;Collects characters and stores them into command_buffer
		!get_char ;GC - WAITS FOR KEYSTROKES and makes sure they are acceptable values
			load R1 TERM ;user inputs character, store it in R1
			!do_GC_while ;makes sure that the entered key is acceptable
				cmp R1 0x07
				jls !get_char ;jump to get_char if entry is Readable (greater than 'bell')
				cmp R1 0x7F
				jgr	!get_char ;jump to get_char if entry is Readable (less than DEL)
				!do_GC_end
		!get_char_end
			storp R0 R1 ;Drop R1 into location R0. Hopefully within the command_buffer
			cmp R1 LF
			jeq !do_BC_end ;if they pressed "ENTER" (we got a LF) stop doing this
			stor TERM R1 ;print character
			add R0 1 ;increment the command_buffer pointer (R0)
		jmp !do_BC
	!do_BC_end
	pop R1 ;return value
	pop	R0 ;return value
	ret
!build_command_end
;------------------------------------------------------------------------------------------------