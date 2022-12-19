# question 4 - 2018 

ISR:
    subi    ea, ea, 4           # because interrupt
    subi    sp, sp, 12          # stack
    stw     ra, 8(sp)           # save the return address
    stw     r2, 4(sp)           # to store the ipending bit
    stw     r3, 0(sp)           # general purpose

    rdctl   r2, ipending        # read the ipending register 

# assume that ipending 0 = PORT A, ipending 1 = PORT B

check_A:
    andi    r3, r2, 0x1         # 0x1 = 0b1
    beq     r3, r0, check_B     # A did not generate interrupt
    call    HandlePortA
    br      end

check_B:
    andi    r3, r2, 0x2         # 0x2 = 0b10
    beq     r3, r0, end         # no interrupt
    call    HandlePortB

end:
    # restore the stack
    addi    sp, sp, 12          # stack
    ldw     ra, 8(sp)           # save the return address
    ldw     r2, 4(sp)           # to store the ipending bit
    ldw     r3, 0(sp)           # general purpose
    eret                        # because we are handling exceptions