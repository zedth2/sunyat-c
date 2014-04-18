	
.message	Sure hope this doesn't crash.

.constant	SCREEN	0xFE
.constant	KEYB	0xFF

.constant	CR	0xD
.constant	LF	0xA


!crlf
.variable	crlf0	CR
.variable	crlf1	LF
.variable	crlf2	0

!prompt
.variable	prompt0 'E'
.variable	prompt1	'n'
.variable	prompt2	't'
.variable	prompt3	'e'
.variable	prompt4	'r'
.variable	prompt5	' '
.variable	prompt6	'a'
.variable	prompt7	' '
.variable	prompt8	't'
.variable	prompt9	'h'
.variable	prompt10	'i'
.variable	prompt11	'n'
.variable	prompt12	'g'
.variable	prompt13	CR
.variable	prompt14	LF
.variable	prompt15	0

.variable	key

	jmp	!main

!main
	call	!get_data
	call	!print_registers
	
	ret
!main_end


;--------------------------------------------------------------
;Runs through registers, recieving input from read_letter and
;sticking it into them sequentially. Ends with "."
;--------------------------------------------------------------
!get_data
mov	R2	!prompt
call	!print_string
	!do_collect
		call	!read_letter
		mov	R0	R1
		awr	1
	!do_collect_while
		cmp R0 '.'
		jeq	!do_collect
	!do_collect_end
!get_data_end

!read_letter	; (RL)
	push	R3
!do_RL
	load	R3	KEYB
!do_RL_while
	cmp	R3	'z'
	jgr	!do_RL
!do_RL_end
	stor	key	R3
	pop	R3
	ret
!read_letter_end

;------------------------------------------------------------------------------
; print the character string pointed to by R2 (register 2) until the NULL 
; character is found.
;------------------------------------------------------------------------------
!print_string	; (PS)
	push	R2
	push	R3
!while_PS
	loadp	R2	R3
	cmp	R2	0
	jeq	!while_PS_end
	stor	SCREEN	R2
	add	R3	1
	jmp	!while_PS
!while_PS_end
	pop	R3
	pop	R2
!print_string_end


;-------------------------------------
;prints out all of the registers.
;
;-------------------------------------
!print_registers
	swr 0
;WINDOW 05 - 12-------------------------------------------------------
	stor SCREEN R0 ;0
	stor SCREEN R1 ;1
	stor SCREEN R2 ;2
	stor SCREEN R3 ;3
	stor SCREEN R4 ;4
	stor SCREEN R5 ;5
	stor SCREEN R6 ;6
	stor SCREEN R7 ;7
	awr 8
;WINDOW 13 - 20-------------------------------------------------------
	stor SCREEN R0 ;8
	stor SCREEN R1 ;9
	stor SCREEN R2 ;10
	stor SCREEN R3 ;11
	stor SCREEN R4 ;12
	stor SCREEN R5 ;13
	stor SCREEN R6 ;14
	stor SCREEN R7 ;15
	awr 8
;WINDOW 21 - 28-------------------------------------------------------
	stor SCREEN R0 ;16
	stor SCREEN R1 ;17
	stor SCREEN R2 ;18
	stor SCREEN R3 ;19
	stor SCREEN R4 ;20
	stor SCREEN R5 ;21
	stor SCREEN R6 ;22
	stor SCREEN R7 ;23
	awr 8
;WINDOW 29 - 36-------------------------------------------------------
	stor SCREEN R0 ;24
	stor SCREEN R1 ;25
	stor SCREEN R2 ;26
	stor SCREEN R3 ;27
	stor SCREEN R4 ;28
	stor SCREEN R5 ;29
	stor SCREEN R6 ;30
	stor SCREEN R7 ;31
	
!print_registers_end