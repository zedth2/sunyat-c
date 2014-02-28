	
.constant	SCREEN	0xFE
.constant	KEYB	0xFF

.constant	CR	0xD
.constant	LF	0xA

	jmp	!main

!main

;WINDOW 05 - 12-------------------------------------------------------
	mov R0 'S'
	mov R1 't'
	mov R2 'a'
	mov R3 'r'
	mov R4 't'

!main_end


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