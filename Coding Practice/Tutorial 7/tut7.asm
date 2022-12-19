# interrupt handler
.equ    TIMER_STATUS    0x99999999
interrupt_handler:
    subi    sp, sp, 4
    stw     r12, 0(sp)

    movia   r12, TIMER_STATUS       # changing the flag to switch the interrupt (this counts as 2 instrs. )
    stwio   r0, 0(r12)              # changing the least significant bit (IE bit of TIMER_STATUS)

    # restore the stack
    ldw     r12, 0(sp)
    addi    sp, sp, 4
    ret                             # return to the calling program

/*
    freq = 50 MHz
    period (cycle time) = 1/50 MHz = 20 ns

    1 instr = 5 cycles = 5 * 20 = 100 ns

    interval = 1 ms

    1 ms / 100 ns( per instr) = 10, 000 instrs in 1 ms

    76/ 10, 000 * 100% = 0.76%
*/