#define TIMER_STATUS (volatile unsigned int *) 0x00989690


void interrupt_handler(void){
    // handling for timer interrupt
    unsigned int ipending;                  // local variable for checking
    ipending = NIOS2_READ_IPENDING();       // this is a MACRO --> same as rcdtl    r2, ipending

    if (ipending & 0x1){// IE bit for the timer is bit 0 -> 2^0 = 1
        *TIMER_STATUS = 0;                  // TIMER_STATUS would need to be defined as above
                                            // dereference it and assign it the value of 0, clear interrupt flag
    }
}