#-----------------------------------------------------------------------------
# This template source file for ELEC 371 Lab 2 experimentation with interrupts
# also serves as the template for all assembly-language-level coding for
# Nios II interrupt-based programs in this course. DO NOT USE the approach
# shown in the documentation for the DE0 Basic (or Media) Computer. The
# approach illustrated in this template file is far simpler for learning.
#
# Dr. N. Manjikian, Dept. of Elec. and Comp. Eng., Queen's University
#-----------------------------------------------------------------------------
 
	.text		# start a code segment 
 
	.global	_start	# export _start symbol for linker 
 
	.equ JTAG_UART_BASE, 0x10001000
	.equ DATA_OFFSET, 0
	.equ STATUS_OFFSET, 4
	.equ WSPACE_MASK, 0xFFFF
 
#-----------------------------------------------------------------------------
# Define symbols for memory-mapped I/O register addresses and use them in code
#-----------------------------------------------------------------------------
 
# the symbols below are specifically for the Lab 2 pushbutton/LED experiment
 
	.equ	BUTTONS_MASK_REGISTER, 0x10000058
	.equ	BUTTONS_EDGE_REGISTER, 0x1000005C
 
	.equ	LEDS_DATA_REGISTER, 0x10000010
 
#-----------------------------------------------------------------------------
# Define two branch instructions in specific locations at the start of memory
#-----------------------------------------------------------------------------
 
	.org	0x0000	# this is the _reset_ address 
_start:
	br	main	# branch to actual start of main() routine 
 
	.org	0x0020	# this is the _exception/interrupt_ address
 
	br	isr	# branch to start of interrupt service routine 
			# (rather than placing all of the service code here) 
 
#-----------------------------------------------------------------------------
# The actual program code (incl. service routine) can be placed immediately
# after the second branch above, or another .org directive could be used
# to place the program code at a desired address (e.g., 0x0080). It does not
# matter because the _start symbol defines where execution begins, and the
# branch at that location simply force execution to continue where desired.
#-----------------------------------------------------------------------------
 
main:
	movia 	sp, 0x07FFFFC	# initialize stack pointer at last RAM word
 
	movia	r2, NAME_S
	call	PrintString
 
	call Init				# call hw/sw initialization subroutine
 
	mov	r2, r0				# perform any local initialization before main loop 
 
mainloop:
 
	addi r2, r2, 1		# body of main loop (reflecting typical embedded
						#   software organization where execution does not
						#   terminate)
	br mainloop
 
#-----------------------------------------------------------------------------
# This subroutine should encompass preparation of I/O registers as well as
# special processor registers for recognition and processing of interrupt
# requests. Preparation of program data can also be included here.
#-----------------------------------------------------------------------------
 
# body of Init() subroutine
Init:
	subi sp, sp, 8 #Save registers
	stw  r4, 4(sp)
	stw  r5, 0(sp)
 
	#enable interrupts on pushbutton 1 at 0
	movia r4, BUTTONS_MASK_REGISTER # move button mask register address into r4
	movi r5, 0b0010 	 # set an interrupt mask for pushbutton 1
	stwio r5, 0(r4) 	 # mask the interrupt register to enable interrupts for pushbutton 1
 
	#enable Nios II processor interrupts
	movi r6, 0b010 # set interrupt mask bits for bit 1 (pushbutton)
	wrctl ienable, r6 # enables the processor to take interrupts from pushbutton
 
	#enable processor to respond to all interrupts
	movi r6, 1		 #enable all individual external interrupts
	wrctl status, r6 #write to control register
 
	ldw  r5, 0(sp)
	ldw	 r4, 4(sp)
	addi sp, sp, 8
 
	ret
 
#-----------------------------------------------------------------------------
# The code for the interrupt service routine is below. Note that the branch
# instruction at 0x0020 is executed first upon recognition of interrupts,
# and that branch brings the flow of execution to the code below. Therefore,
# the actual code for this routine can be anywhere in memory for convenience.
# This template involves only hardware-generated interrupts. Therefore, the
# return-address adjustment on the ea register is performed unconditionally.
# Programs with software-generated interrupts must check for hardware sources
# to conditionally adjust the ea register (no adjustment for s/w interrupts).
#-----------------------------------------------------------------------------
 
isr: # body of interrupt service routine
	addi	ea, ea, -4	# this is _required_ for hardware interrupts
 
# (save/restore registers that are modified,
#  except ea which must be modified as above)
 
	subi 	sp, sp, 12
	stw     r7, 8(sp)
	stw		r8, 4(sp)
	stw     r9, 0(sp)
 
	movia	r8, BUTTONS_EDGE_REGISTER #address for buttons
    movia	r7, 0xFFFFFFFF
    stwio 	r7, 0(r8) 				  # clear the edge register by writing anywhere 
 
    movia r8,LEDS_DATA_REGISTER 	  #get the address for led status
    ldwio r7,0(r8)					  #load the led status
    movia r9,0xFFFFFFFF
    xor r7,r7,r9					  #flip all the bits
    andi r7,r7,1					  #mask so we only have the first bit
    stwio r7,0(r8)					  #write the led status
 
	ldw 	r7, 8(sp)
	ldw		r8, 4(sp)
	ldw		r9, 0(sp)
	addi    sp, sp, 12
 
	eret		# interrupt service routines end _differently_
				# than normal subroutines; the eret goes back
				# to wherever execution was in the main program
				# (possibly in a subroutine) at the time the
				# interrupt request triggered invocation of
				# the service routine
 
#-----------------------------------------------------------------------------
# Definitions for program data, incl. anything shared between main/isr code
#---------------------------------------------------------------------------
PrintChar:
	subi	sp, sp, 8	# adjust stack pointer down to reserve space
	stw	r3, 4(sp)	# save value of register r3 so it can be a temp
	stw	r4, 0(sp)	# save value of register r4 so it can be a temp
	movia	r3, JTAG_UART_BASE	# point to first memory-mapped I/O register
pc_loop:
	ldwio 	r4, STATUS_OFFSET(r3)	# read bits from status register
	andhi 	r4, r4, WSPACE_MASK	# mask off lower bits to isolate upper bits
	beq 	r4, r0, pc_loop		# if upper bits are zero, loop again
	stwio 	r2, DATA_OFFSET(r3)	# otherwise, write character to data register
	ldw 	r3, 4(sp)	# restore value of r3 from stack
	ldw 	r4, 0(sp)	# restore value of r4 from stack
	addi	sp, sp, 8	#re-adjust stack pointer up to deallocate space
	ret	
 
 
PrintString:
	subi	sp, sp, 16
	stw	ra, 12(sp) # necessary when subroutine calls another subroutine
	stw	r2, 8(sp)
	stw	r3, 4(sp)
	stw	r4, 0(sp)
	mov	r3, r2
 
ps_loop:
	ldb	r4, 0(r3)
 
ps_if:
	bgt	r4, r0, ps_else
 
ps_then:
	br	ps_end_if
 
ps_else:
	mov	r2, r4
	call	PrintChar
	addi	r3, r3, 1
	br	ps_loop
 
ps_end_if:
	ldw	ra, 12(sp)
	ldw	r2, 8(sp)
	ldw	r3, 4(sp)
	ldw	r4, 0(sp)
	addi	sp, sp, 16
	ret
 
#------------------------------------------------------------------------------------------
 
	.org	0x1000		# start should be fine for most small programs
 
	NAME_S: .asciz  "ELEC 371 Lab 2\n"
	COUNT:  .skip   4
 
	.end