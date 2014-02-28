;asm used to test register windowing in SUNYAT

.constant	SCREEN	0xFF
.constant	KEYB	0xFE

.constant	CR	0xD
.constant	LF	0xA

!windex
.variable	windex0 '0'
.variable	windex1 '1'
.variable	windex2 '2'
.variable	windex3 '3'
.variable	windex4 '4'
.variable	windex5 '5'
.variable	windex6 '6'
.variable	windex7 '7'

!where
.variable	where0	'R'
.variable	where1	'e'
.variable	where2	'g'
.variable	where3	'i'
.variable	where4	's'
.variable	where5	't'
.variable	where6	'e'
.variable	where7	'r'
.variable	where8	'|'
.variable	where9	'R'
.variable	where10	'e'
.variable	where11	'l'
.variable	where12	'a'
.variable	where13	't'
.variable	where14	'i'
.variable	where15	'v'
.variable	where16	'e'
.variable	where17	'|'
.variable	where18	'V'
.variable	where19	'a'
.variable	where20	'l'
.variable	where21	'u'
.variable	where22	'e'
.variable	where23 0


	jmp	!main

!main
	
;WINDOW 5 - 12--------------------------------------------------------
	awr	-5 ;Go to the zeroth register. (Offset Zero)
;WINDOW 0 - 7-----------------------------------------------------R5=0
	mov R5	R3 ;register 5 (GPR 0) has the offset in it.
	awr 5
;WINDOW 5 - 12--------------------------------------------------------
	mov	R1	12
	mov R2	23
;WINDOW 5 - 12--------------------------------------------------------
	swr 10
;WINDOW 15 - 23-------------------------------------------------------
	awr - 15

	call !whereami


	;call	!whereami

!main_end


!whereami

!whereami_end





;-------------------------------------------------------------------------
;Prints stats on the curent window
;-Current window position: REG_WIN (system register) 3
;-Each register:
;-Register Number | Relative position (to zeroth window index) | Value
;-------------------------------------------------------------------------

!printwindow;Window 0-7 (assumed)-------------------------------------R3=0	
	mov	R6	!where
	call	!print_string
	;printf("Register|Relative|Value")
	stor	SCREEN	R0
	stor	SCREEN	R1
	stor	SCREEN	R2
	stor	SCREEN	R3
	stor	SCREEN	R4
	stor	SCREEN	R5
	stor	SCREEN	R6
	stor	SCREEN	R7
!printwindow_end

;------------------------------------------------------------------------------
; print the character string pointed to by R6 until the NULL
; character is found.
;------------------------------------------------------------------------------
!print_string
	push	R5
	push	R6
	
	!while_PS
		loadp	R5	R6
		cmp	R5	0
		jeq	!while_PS_end
		stor	SCREEN	R6
		add	R6	1
		jmp	!while_PS
	!while_PS_end
	
	pop	R6
	pop	R5
	ret
!print_string_end