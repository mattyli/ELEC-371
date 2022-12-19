ISR:
    subi    ea, ea, 4   # for interrupts
    subi    sp, sp, 12  # saving the stack
    stw     ra, 8(sp)
    stw     r2, 4(sp)   # to store ipending value
    stw     r3, 0(sp)   # to store the and result

    rdctl   r2, ipending    # read the ipending in

interruptX:
    andi    r3, r2, 0x10            # X assigned to bit 4 (0x10 = 0b10000)
    beq     r3, r0, interruptY      # if the signals don't match, check if Y threw interrupt
    call    HandleIntX
    br      end                     # handled the interrupt, branch to end

interruptY:
    andi    r3, r2, 0x20            # Y is assigned bit 5 (0x20 = 0b100000)
    beq     r3, r0, end             # if Y didnt throw, go to end
    call    HandleIntY              # handle the interrupt
                                    # falls through to end
end:
    # restore the stack

    ldw     ra, 8(sp)
    ldw     r2, 4(sp)
    ldw     r3, 0(sp)
    addi    sp, sp, 12

    eret


