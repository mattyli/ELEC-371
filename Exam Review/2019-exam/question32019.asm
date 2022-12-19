# Question 3 - 2019 Exam
# bit 5 is the flag bit of status register
# register are wordsized 4 bytes

GetDataFromPort:
    subi    sp, sp, 24
    stw     ra, 20(sp)          # saving the return address
    stw     r2, 16(sp)          # for the list pointer
    stw     r3, 12(sp)          # for N
    stw     r4, 8(sp)           # temp
    stw     r5, 4(sp)           # store the port status
    stw     r6, 0(sp)           # store port data

    movia   r5, PORT_STATUS     # use movia with 32 bit address
    movia   r6, PORT_DATA

    beq     r3, r0, GDFP_end    # if there is nothing in the list

GDFP_loop:
    ldwio   r4, 0(r5)           # load the status from the register
    andi    r4, r4, 0x20        # because (0x20 = 0b100000) the flag bit, if the result is 1, flag is raised
    beq     r4, r0, GDFP_loop   # if flag not raised, keep polling to check

    # flag thrown, store the element

    ldwio   r4, 0(r6)           # get the data from the port
    stw     r4, 0(r2)           # store the data into the list

    addi    r2, r2, 4           # advance list pointer by 4 bytes (1 word)
    subi    r3, r3, 1           # decrement the number of elements left to put into the list
    bge     r3, r0, GDFP_loop

GDFP_end:
    # restore the stack
    addi    sp, sp, 24
    ldw     ra, 20(sp)          # saving the return address
    ldw     r2, 16(sp)          # for the list pointer
    ldw     r3, 12(sp)          # for N
    ldw     r4, 8(sp)           # temp
    ldw     r5, 4(sp)           # store the port status
    ldw     r6, 0(sp)           # store port data
    ret                         # return to the calling program