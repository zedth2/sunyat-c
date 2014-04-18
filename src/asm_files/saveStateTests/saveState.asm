.constant	TERM 	0xFF
.constant	CR	0xD
.constant	LF	0xA

jmp	!main

!crlf
	.variable	crlf0	CR
	.variable	crlf1	LF
	.variable	crlf2	0x00


!main
	mov R0 'F'
	mov R1 'U'
	mov R2 'C'
	mov R3 'K'
	mov R4 ' '
	mov R5 'Y'
	mov R6 'O'
	mov R7 'U'
	aux 0

	stor TERM R0
	stor TERM R1
	stor TERM R2
	stor TERM R3
	stor TERM R4
	stor TERM R5
	stor TERM R6
	stor TERM R7
	
!wait
	
	jmp !wait
!wait_end

	ret
!main_end