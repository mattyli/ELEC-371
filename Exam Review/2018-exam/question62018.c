#include "nios2_control.h"

/*
    functions to setup 
        init() x
        printchar() x
        handletimer0
        handletimer1
        interrupt handler   x
        main
*/

/*DEFINE MACROS*/

#define TIMER0_STATUS   ((volatile unsigned int *) 0x00005000)      // from the table
#define TIMER0_CONTROL  ((volatile unsigned int *) 0x00005004)
#define TIMER0_LO       ((volatile unsigned int *) 0x00005008)
#define TIMER0_HI       ((volatile unsigned int *) 0x0000500C)

#define TIMER1_STATUS   ((volatile unsigned int *) 0x00006000)      // following the same pattern as TIMER0
#define TIMER1_CONTROL  ((volatile unsigned int *) 0x00006004)
#define TIMER1_LO       ((volatile unsigned int *) 0x00006008)
#define TIMER1_HI       ((volatile unsigned int *) 0x0000600C)

#define JTAG_DATA       ((volatile unsigned int *) 0x00006C00)
#define JTAG_STATUS     ((volatile unsigned int *) 0x00006C04)

#define INPORT          ((volatile unsigned int *) 0x00006A00)
#define OUTPORT         ((volatile unsigned int *) 0x00006B00)

#define BIT0    0x1
#define BIT1    0x2
#define RAISED  1
#define CLEARED 0

/* GLOBAL VARIABLES + INTIALIZATION */
unsigned int current_temp;
unsigned int quarter_sec_flag = 0;      // flag to indicate that 0.25 sec has elapsed
unsigned int one_sec_flag = 0;          // flag that indicates 1 sec has elapsed

void init(){
    // set up timer0
    *TIMER0_CONTROL = 0x7;      // bits 2,1,0
    *TIMER0_STATUS = 0x1;       // bit 0
    *TIMER0_HI = 50000000;      // 50 MHz * 1 sec = 50 000 000 cycles
    *TIMER0_LO = 50000000 >> 16;// left shift 

    *TIMER1_CONTROL = 0x7;      // bits 2,1,0
    *TIMER1_STATUS = 0x1;       // bit 0
    *TIMER1_HI = 12500000;      // 50 MHz * 0.25 sec = 12 500 000 cycles
    *TIMER1_LO = 12500000 >> 16;// left shift 

    *JTAG_STATUS = 0x1;

    NIOS2_WRITE_IENABLE(0x3);   // timer0 IE = bit 0, timer1 IE = bit 1, enabling interrupts for both timers
    NIOS2_WRITE_STATUS(1);      // good to run
}

void printChar(unsigned int ch){
    unsigned int status;
    do{
        status = *JTAG_STATUS;          // read the JTAG_STATUS
        status = (status & 0xFFFF0000); // checking the upper 16 bits
    } while (status == 0);              // while there is not enough space
    *JTAG_DATA = ch;                    // enough space? can write
}

// handle the temperature control
void handleTimer0(){
    *TIMER0_STATUS = 0x0;               // reset the status, don't need to do anything else
    one_sec_flag = 1;
    if (*INPORT < 98){
        *OUTPORT = 128;
    }
    else if (*INPORT > 102){
        *OUTPORT = 0;
    }
    else{}                              // don't need to adjust temperature
}

void handleTimer1(){
    *TIMER1_STATUS = 0x0;               // reset the status
    quarter_sec_flag = 1;               // a quarter second has elapsed

}

void interruptHandler(){
    unsigned int ipending;
    ipending = NIOS2_READ_IPENDING();     // get the ipending value
    if (ipending & BIT0){
        handleTimer0();                     // because bit0 is ipending for timer 0 
    }
    if (ipending & BIT1){
        handleTimer1();                     // because bit1 is ipending for timer 1 
    }
}

int main(void){
    init();         // set everything up
    while(1){
        current_temp = *INPORT;             // getting the current temperature from the INPORT
        if (quarter_sec_flag == RAISED){
            quarter_sec_flag = CLEARED;     // clear the flag
            printChar('\b');
            if (current_temp < 98){
                printChar('L');
                printChar('\n');
            }
            else if(current_temp >= 98 && current_temp <= 102){
                printChar('-');
                printChar('\n');
            }
            else{
                printChar('H');
                printChar('\n');
            }
        }
    }
    return 0;
}
