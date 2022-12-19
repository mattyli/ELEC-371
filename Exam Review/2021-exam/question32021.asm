# Question 3 2021
SendDataToPort:
    # first save the stack
    subi    sp, sp, 24 
    stw     ra, 20(sp)       # save the return address
    stw     r2, 16(sp)       # list pointer
    stw     r3, 12(sp)       # N
    stw     r4, 8(sp)        # utility register
    stw     r5, 4(sp)        # for status
    stw     r6, 0(sp)        # for data

    movia   r5, OUTPORT_STATUS  # because 32 bit address
    movia   r6, OUTPORT_DATA

    beq     r3, r0, SDTP_end    # if there is nothing to put into the list

SDTP_loop:
    ldw     r4, 0(r5)           # load the status
    andi    r4, r4, 0x20        # because bit 4 is the flag bit (0x20 = 0b10000)
    beq     r4, r0, SDTP_loop   # status flag not raised? keep polling

    ldw     r4, 0(r2)           # fetch the list element
    stwio   r4, 0(r6)           # store the element to the DATA

    addi    r2, r2, 4           # 4 bytes = 1 word
    subi    r3, r3, 1           # decrement the counter
    bne     r3, r0, SDTP_loop   # keep going if we still have elements

SDTP_end:
    # restore the stack
    addi    sp, sp, 24 
    ldw     ra, 20(sp)       # save the return address
    ldw     r2, 16(sp)       # list pointer
    ldw     r3, 12(sp)       # N
    ldw     r4, 8(sp)        # utility register
    ldw     r5, 4(sp)        # for status
    ldw     r6, 0(sp)        # for data
    ret                     # return to the calling program