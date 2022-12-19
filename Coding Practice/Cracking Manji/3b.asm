Write a NIOS II assembly-language loop (a complete program is not
required) to transfer data words to the external device connected to the
given port. The words to be transferred begin at location LIST, and the
number of words to be transferred is available at location N (this is the
terminating condition, which changes). The port status flag is in bit 3. 

#### BEGIN LOOP ####
movia r2, LIST          # move the start of the list to 
movia r3, N             # move the address of N into r3
movia r4, STATUS_REG    # the status reg input

trans_loop:
trans_if:
    ldw     r5, 0(r4)               # load STATUS_REG data
    andi    r5, r5, STATUS_FLAG     # masking off unnecessary bits (bit 3 is the flag)
    movi    r6, STATUS_FLAG
    bne     r5, r6, trans_loop      # check if flag was raised

trans_else:
    ldw     r7, 0(r2)               # load the word from the list (0 offset)
    
    addi    r2, r2, 4               # incrementing list pointer
    subi    r3, r3, 1               # decrementing num of words

    movia   r8, DATA_REG
    stwio   r7, 0(r8)               # sending word to port

    bne     r3, r0, trans_loop

trans_end: