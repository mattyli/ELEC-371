	.set 	noat
	.text
	.global _start
	.org 0
_start:
	addi r1, r0, 0x1234
	stw	 r1, 0x4000(r0)
	ldw	 r1, 0x4000(r0)
	br	 _start