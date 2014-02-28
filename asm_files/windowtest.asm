	
.constant	SCREEN	0xFF
.constant	KEYB	0xFE

.constant	CR	0xD
.constant	LF	0xA

	jmp	!main

!main

;WINDOW 05 - 12-------------------------------------------------------
	awr 3
;WINDOW 08 - 15-------------------------------------------------R8=111
	mov	R0	7
	awr	8
;WINDOW 16 - 23------------------------------------------------R15=111
	mov	R0	7
	awr	8
;WINDOW 24 - 31------------------------------------------------R24=111
	mov R0	7
	swr	3
;WINDOW 05 - 12-------------------------------------------------------
	stor SCREEN R0 ;8
	awr 1
	stor SCREEN R0 ;9
	awr 1
	stor SCREEN R0 ;10
	awr 1
	stor SCREEN R0 ;11
	awr 1
	stor SCREEN R0 ;12
	awr 1
	stor SCREEN R0 ;13
	awr 1
	stor SCREEN R0 ;14
	awr 1
	stor SCREEN R0 ;15
	awr 1
	stor SCREEN R0 ;16
	
	!main_end