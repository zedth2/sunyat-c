.constant	TERM 	0xFF
.constant	CR	0xD
.constant	LF	0xA

jmp !main

!main
;aux 0
mov R0 '3'
stor TERM R0
!main_end
ret