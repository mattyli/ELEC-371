ISR:
    addi    ea, ea, 4
    # save the stack 
    subi    sp, sp, 12 
    stw     ra, 8(sp)   # save the return address
    stw     r2, 4(sp)   # for ipending
    stw     r3, 0(sp)   # for the bit patteern

    rdctl   r2, ipending    # read ipending into r2

checkA:
    andi    r3, r2, 0x40    # because A is assigned bit 6 (0b1000000)
    beq     r3, r0, checkB

    call    HandleIntA
    br      end

checkB:
    andi    r3, r2, 0x80    # because B is assigned bit 7 (0b10000000)
    beq     r3, r0, end
    call    HandleIntB

end:
    # restore the stack
    addi    sp, sp, 12 
    ldw     ra, 8(sp)   # save the return address
    ldw     r2, 4(sp)   # for ipending
    ldw     r3, 0(sp)   # for the bit patteern

    eret                # return from the exception