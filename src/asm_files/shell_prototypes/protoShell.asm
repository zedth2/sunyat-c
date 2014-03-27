;------------------------------------
;The SUNYAT Shell
;
;String Delimiter: '%'
;------------------------------------

.message	SUNYAT Shell Prototype
.constant	TERM 	0xFF
.constant	CR	0xD
.constant	LF	0xA
	jmp	!main

!crlf
	.variable	crlf0	CR
	.variable	crlf1	LF
	.variable	crlf2	'%'

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
	.variable cmd0 ' '
	.variable cmd1 ' '
	.variable cmd2 ' '
	.variable cmd3 ' '
	.variable cmd4 ' '
	.variable cmd5 ' '
	.variable cmd6 ' '
	.variable cmd7 ' '
	.variable cmd8 ' '
	.variable cmd9 ' '
	.variable cmd10 ' '
	.variable cmd11 ' '
	.variable cmd12 ' '

;COMMAND DEFS-------------------------------
!cmd_win
	.variable regs0 'w'
	.variable regs1 'i'
	.variable regs2 'n'

!cmd_awr
	.variable awr0 'a'
	.variable awr1 'w'
	.variable awr2 'r'

!cmd_reset
	.variable reset0 'r'
	.variable reset1 'e'
	.variable reset2 's'
	.variable reset3 'e'
	.variable reset4 't'

!cmd_exit
	.variable exit0 'e'
	.variable exit1 'x'
	.variable exit2 'i'
	.variable exit3 't'
;---------------------------------------

;-!main------------------------------------------------------------------------------------------
;-Prints prompt
;-Calls !build_command and gets user input.
;-Calls !get_cmd to assess / run commands.
;-LOOP
;
;------------------------------------------------------------------------------------------------
!main
	mov R0 !prompt
	call !print ;print the prompt
	call !build_command ;get a 'command'
	call !print_line
	;Echo user input-------------------------
	;	mov R0 !command_buffer
	;	call !print
	;	call !print_line
	;----------------------------------------
	call !get_cmd ;Figure out if the user input a real command
	jmp !main ;back to the top
	!exit_shell
	ret
!main_end

;-!build_command---------------------------------------------------------------------------------
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
				jls !get_char ;jump to get_char if entry is Readable (less than 'bell')
				cmp R1 0x7F
				jgr	!get_char ;jump to get_char if entry is Readable (greater than DEL)
			!do_GC_end
		!get_char_end
			cmp R1 LF
			jeq !do_BC_end ;if they pressed "ENTER", don't store it. End function.
			storp R0 R1 ;Drop R1 into location R0. Hopefully within the command_buffer
			add R0 1 ;increment the command_buffer pointer (R0)
			stor TERM R1 ;print character
			jmp !do_BC ;keep getting characters
	!do_BC_end
	mov R1 '%'
	storp R0 R1 ;drop a delimiter
	pop R1 ;return value
	pop	R0 ;return value
	ret
!build_command_end
;------------------------------------------------------------------------------------------------

;-!get_cmd---------------------------------------------------------------------------------------
;-Compares the user's entered command to known system commands and runs them.
;-Runs through a list of commands, compares each of them to user command.
;-Comparisons done by !analyze
;-If !analyze returns a match, run the corresponding code
;
;------------------------------------------------------------------------------------------------
!get_cmd
	push R0
	push R1
	push R2
	push R3

	mov R0 !command_buffer ;R0: userCmd pointer
	loadp R1 R0 ;R1:  userCmd char
	cmp R1 '%'
	jeq !end_of_commands ;make sure command isn't empty
	;cmp R1 ' '
	;jeq !end_of_commands ;make sure command doesn't start with a space (arg delimiter)
	
	;List of known commands --------------------------------------------------------------------
		mov R2 !cmd_win ;REGS
		call !analyze
		jeq !cmd_win_exec

		mov R2 !cmd_awr ;AWR
		call !analyze
		jeq !cmd_awr_exec

		mov R2 !cmd_reset ;RESET
		call !analyze
		jeq !cmd_reset_exec

		mov R2 !cmd_exit ;EXIT
		call !analyze
		jeq !cmd_exit_exec
	;End of known commands ---------------------------------------------------------------------
	!end_of_commands
	pop R3
	pop R2
	pop R1
	pop R0
	ret
!get_cmd_end
;------------------------------------------------------------------------------------------------

;-!analyze---------------------------------------------------------------------------------------
;-(Called by !get_cmd)
;-Compares, character by character, the user's command with a known command.
;-If true, sets R0 to '=' and returns. (Allows !get_cmd to 'know' if cmd should be run or not.)
;
;------------------------------------------------------------------------------------------------
!analyze
	mov R0 !command_buffer
	!analyze_while
		loadp R1 R0
		loadp R3 R2
		cmp R1 '%' ;Hit delimiter, run code.
		jeq !analyze_return
	;cmp R1 ' ' ;Hit arg delimiter, get args
	;jeq !analyze_return
		cmp R1 R3 ;Are these characters the same?
		jne !analyze_return;If not, go to the next command.
		add R0 1
		add R2 1
	jmp !analyze_while
	!analyze_return
		ret
!analyze_end
;------------------------------------------------------------------------------------------------

;-!print-----------------------------------------------------------------------------------------
;-Prints chars individually from a memory location moved into R0
;-Stops when it finds a '%'
;
;-Does not print the '%'
;------------------------------------------------------------------------------------------------
!print
	push R0 ;backup R1
	push R1 ;backup R0
	!while_PP
		loadp R1 R0 ;Load character at address R0 into R1. R0 is an array pointer.
		cmp R1 '%'
		jeq !while_PP_end ;If the character is '%' stop printing.
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

;-command-executions-----------------------------------------------------------------------------
	;WIN - print registers
	!cmd_win_exec
;pop things
		stor TERM R0
		call !print_line
		stor TERM R1
		call !print_line
		stor TERM R2
		call !print_line
		stor TERM R3
		call !print_line
		stor TERM R4
		call !print_line
		stor TERM R5
		call !print_line
		stor TERM R6
		call !print_line
		stor TERM R7
		call !print_line
		jmp !end_of_commands
	!cmd_win_exec_end

	;AWR - Shift window by 1
	!cmd_awr_exec
;pop things
		awr 1
		jmp !end_of_commands
	!cmd_awr_exec_end

	;RESET - Shift window to beginning of registers
	!cmd_reset_exec
;pop things
		swr 3
		jmp !end_of_commands
	!cmd_reset_exec_end

	;EXIT - Exit the SUNYAT
	!cmd_exit_exec
;pop things
		jmp !exit_shell ;need to pop before this, still stuff in the stack.
	!cmd_exit_exec_end
;------------------------------------------------------------------------------------------------

