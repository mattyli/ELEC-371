ISR:
    subi    ea, ea, 4       # because exception handling
    subi    sp, sp, 12      # save the stack
    stw     ra, 8(sp)       # save the return address
    stw     r2, 4(sp)       # for ipending
    stw     r3, 0(sp)       # temp

    rdctl   r2, ipending    # read the ipending bit

check_X:
    # assuming that device X has ipending bit (bit 0)
    andi    r3, r2, 0x1     # 0x1 is bit 0 of ipending
    beq     r3, r0, end
    call    HandleDeviceX

end: 
    # restore the stack
    addi    sp, sp, 12      # save the stack
    ldw     ra, 8(sp)       # save the return address
    ldw     r2, 4(sp)       # for ipending
    ldw     r3, 0(sp)       # temp
    eret                    # return from the ISR
