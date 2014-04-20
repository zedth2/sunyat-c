.constant	TERM 	0xFF
.constant	CR	0xD
.constant	LF	0xA

jmp !main

!main
mov R0 1
mul R0 -20
stor TERM R0

aux 1

!main_end
ret