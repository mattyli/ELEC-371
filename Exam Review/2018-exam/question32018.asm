# question 3 2018

SendDataToPort:
    # save the stack
    subi    sp, sp, 24 
    stw     ra, 20(sp)      # save the return address
    stw     r2, 16(sp)      # pointer to the list
    stw     r3, 12(sp)      # number of elements
    stw     r4, 8(sp)       # general purpose register
    stw     r5, 4(sp)       # PORT DATA
    stw     r6, 0(sp)       # PORT STATUS

    # move the port data and status, because we have 16 bit address, can use movi rather than movia
    movi    r5, PORT_DATA
    movi    r6, PORT_STATUS

    # skip to the end if there is nothing to put in 
    beq     r3, r0, SDTP_end
SDTP_loop:
    ldwio   r4, 0(r6)           # get the PORT_STATUS
    andi    r4, r4, 0x4         # isolating the flag bit (0x4 = 0b100) (bit 2)
    beq     r4, r0, SDTP_loop   # keep polling if the flag has not been raised

    # flag has been raised, send data to the port
    
    ldw     r4, 0(r2)           # load data from the list into r4
    stwio   r4, 0(r5)           # send the data to the port (via load instruction)

    addi    r2, r2, 4           # advance the list by 4 bytes (because wordsized)
    subi    r3, r3, 1           # decrement the number of elements
    bne     r3, r0, SDTP_loop   # still have elements? keep going

SDTP_end:
    # RESTORE THE STACK
    addi    sp, sp, 24 
    ldw     ra, 20(sp)      # save the return address
    ldw     r2, 16(sp)      # pointer to the list
    ldw     r3, 12(sp)      # number of elements
    ldw     r4, 8(sp)       # general purpose register
    ldw     r5, 4(sp)       # PORT DATA
    ldw     r6, 0(sp)       # PORT STATUS

    ret                     # return to the calling program/ subroutine