/*
    Functions to implement
        init
        printchar
        interruptHandler
        main
*/

#define TIMER_STATUS    ((volatile unsigned int *) 0x5000)
#define TIMER_CONTROL   ((volatile unsigned int *) 0x5004)
#define TIMER_LO        ((volatile unsigned int *) 0x5008)
#define TIMER_HI        ((volatile unsigned int *) 0x500C)

#define LEDS            ((volatile unsigned int *) 0x6000)
#define INPORT          ((volatile unsigned int *) 0x6800)

#define JTAG_DATA       ((volatile unsigned int *) 0x6A00)
#define JTAG_STATUS     ((volatile unsigned int *) 0x6A04)
#define RAISED          1
#define CLEARED         0

unsigned int glob_interrupt_count, high_count, low_count;
unsigned int one_sec_flag;
unsigned int data;

void init(){
    *TIMER_STATUS = 0x0;    // clearing interrupts
    *TIMER_CONTROL = 0x7;   // setting control, cont, ito
    *TIMER_HI = 12500000 & 0xFFFF0000;  // want the top 16 bits
    *TIMER_LO = 12500000 & 0x0000FFFF;  // want the lower 16 bits

    glob_interrupt_count = high_count = low_count = 0;  // initializing the global vars
    one_sec_flag = 0;
    NIOS2_WRITE_IENABLE(0x1);   // enabling timer interrupts
    NIOS2_WRITE_STATUS(0x1);    // enable global interrupts for the processor
}

void printChar(unsigned int ch){
    unsigned int status;
    do {
        status = *JTAG_STATUS;
        status = status & 0xFFFF0000;   // masking the upper 16 bits, to check for free space
    }while(status == 0);
    *JTAG_DATA = ch;
}

void handleTimer(){
    *TIMER_STATUS = 0x0;    // clear the IRQ
    if (*LEDS == 0x3){      // 0x3 = 0b00000011 (reached the least signficant)
        *LEDS = 0xC0;       // 0xC0 = 0b11000000 (reset to the most significant)
    }
    else{
        *LEDS >>= 1;        // bitshift them down 
    }
    glob_interrupt_count++; // increment the global interrupt count
    if (glob_interrupt_count%4 == 0){
        // notify 1 sec elapsed
        one_sec_flag = 1;     // increment this flag every 1 second
        glob_interrupt_count = 0;   // save memory
    }
}

void interrutHandler(){
    unsigned int ipending;
    ipending = NIOS2_READ_IPENDING();   // get ipending
    if (ipending & 0x1){
        handleTimer();
    }
}

int main(void){
    while(1){
        if (one_sec_flag == RAISED){    // if this flag is raised
            one_sec_flag = CLEARED;     // clear the flag
            data = *INPORT;             // get the inport data
            if (data >= 128){
                high_count++;
                printChar('H');
            }
            else{
                low_count++;
                printChar('L');
            }
        }
    }
    return 0;
}