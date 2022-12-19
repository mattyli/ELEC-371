/*
    functions to implement:
        init()  x
        printChar() x
        PritHexDigit() x
        UpdateLEDs()
        interruptHandler
        main
*/

#define TIMER_STATUS    ((volatile unsigned int *) 0x00005000)
#define TIMER_CONTROL   ((volatile unsigned int *) 0x00005004)
#define TIMER_LO        ((volatile unsigned int *) 0x00005008)
#define TIMER_HI        ((volatile unsigned int *) 0x0000500C)

#define INPORT          ((volatile unsigned int *) 0x00006A00)
#define OUTPORT         ((volatile unsigned int *) 0x00007E00)

#define JTAG_DATA       ((volatile unsigned int *) 0x0000A800)
#define JTAG_STATUS     ((volatile unsigned int *) 0x0000A804)

unsigned int timer_flag = 0;
unsigned int validPin = 0;

void init(){
    *TIMER_LO = 0x7840;
    *TIMER_HI = 0x017D;
    *TIMER_CONTROL = 0x7;
    *TIMER_STATUS = 0x1;
    
    *JTAG_STATUS = 0x1;
    NIOS2_WRITE_IENABLE(0x3);
    NIOS2_WRITE_STATUS(0x1);
}

void printChar(unsigned int ch){
    unsigned int status;
    do{
        status = *JTAG_STATUS;
        status = (status & 0xFFFF0000);
    } while (status == 0);
    *JTAG_DATA = ch;
}

void printHexDigit(unsigned int ch){
    if (ch < 10){
        printChar('0' + ch);
    }
    else{
        printChar('A' + (ch-10));
    }
}

void updateLEDS(){
    
}