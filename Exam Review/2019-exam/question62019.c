/*
    Attempt at 2019 Question 6.

    Functions that need to be implemented:
        - init()    x
        - printchar(), printhexdigit() (memorize these) xx
        - main()
        - handletimer() x
        - handleinputport() x
        - interruptHandler() x
*/

// First set up the global variables and such
#include "nios2_control.h"
// Timer

#define TIMER_STATUS ((volatile unsigned int *)      0x00005000)
#define TIMER_CONTROL ((volatile unsigned int *)     0x00005004)
#define TIMER_LO ((volatile unsigned int *)          0x00005008)   // from the table
#define TIMER_HI ((volatile unsigned int *)          0x0000500C)   // from the table

#define JTAG_DATA ((volatile unsigned int *)         0x00006A00)
#define JTAG_STATUS ((volatile unsigned int *)       0x00006A04)

// input and output ports
#define INPORT_DATA ((volatile unsigned int *)       0x00006800)
#define INPORT_STATUS ((volatile unsigned int *)     0x00006804)
#define INPORT_CONTROL ((volatile unsigned int *)    0x00006808)

#define OUTPORT_DATA ((volatile unsigned int *)      0x00006000)     // not sure if this actually needs to be here
#define OUTPORT_STATUS ((volatile unsigned int *)    0x00006004)
#define OUTPORT_CONTROL ((volatile unsigned int *)   0x00006008)

// defining some macros for cleaner code
#define TIMER_BIT 0x1       // (0b1)    bit 0
#define INPUT_BIT 0x2       // (0b10)   bit 1
#define STAT_REG_BIT 0x10   // (0b10000)bit 5
#define RAISED 1
#define CLEARED 0

// flags that we need to define
unsigned int timer_interrupt_counter = 0;
unsigned int one_sec_flag = 0;
unsigned int input_port_flag = 0;
unsigned int input_port_data = 0;
unsigned int input_port_count = 0;
unsigned int glob_sum = 0;
unsigned int glob_count = 0;
unsigned int glob_avg = 0;

void init(){
    // setup the timer
    *TIMER_CONTROL = 0x7;           // could replace with 0b111 (start=1, cont=1, ito=1) (refer to table)
    *TIMER_STATUS = 0x0;            // enables interrupts for the timer
    *TIMER_HI = 6250000;            // automatically selects the top 16 bits
    *TIMER_LO = 6250000 >> 16;      // right shift to get the lower 16 bits

    *OUTPORT_DATA = 0x1;            // the rightmost LED bit
    *INPORT_CONTROL = INPUT_BIT;            

    // enable interrupts for the timer and input port
    NIOS2_WRITE_IENABLE(0x3);       // need to enable bit 0 and bit 1 (0b11 = 0x3)
    NIOS2_WRITE_STATUS(1);
}

void interruptHandler(){
    unsigned int ipending;
    ipending = NIOS2_READ_IPENDING();      // obtain the ipending value
    // Timer Interrupt
    if (ipending & TIMER_BIT){
        handleTimer();
    }

    if (ipending & INPUT_BIT){
        handleInputPort();
    }
}

void handleTimer(){
    *TIMER_STATUS = 0x0;        // reset the timer status
    timer_interrupt_counter++;  // increment the timer counter

    /*
        Timer interrupt interval = 0.125s
        0.125s x 8 = 1.00s
    */
    if (timer_interrupt_counter % 8 == 0){
        one_sec_flag = RAISED;
        timer_interrupt_counter = 0;    // reset this value
    }

    unsigned int output_bits = timer_interrupt_counter % 8;
    *OUTPORT_DATA = 1 << output_bits;   // left shift for the LEDs
}

void handleInputPort(){
    *INPORT_STATUS = 0x0;               // reset the input status
    input_port_data = *INPORT_DATA;     // get the data
    input_port_flag = RAISED;           // set the flag
}

void printChar(unsigned int ch){
    unsigned int status;

    // do while loop, check the status (seeing if we have space), if we can't send data, infinite loop to check
    do{
        status = *JTAG_STATUS;              // check the JTAG status reg
        status = (status & 0xFFFF0000);     // mask the top 16 bits
    } while (status == 0);

    // once we can write the data, write it
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

int main(void){
    init();     // initialize everything

    while(1){
        // infinite loop
        /*
            Per the question, we must check if the flags are thrown:
            If they are thrown, clear them
        */

        /*Check for the inport flag*/
        if (input_port_flag == RAISED){
            input_port_flag = CLEARED;              // clear the flag
            input_port_count++;                     // increasing the count
            glob_sum = glob_sum + input_port_data;
            glob_count++;
            glob_avg = glob_sum/ glob_count;        // computing the average by division
        }

        if (one_sec_flag == RAISED){
            one_sec_flag = CLEARED;
            printChar('\b');
            printChar('\b');            // so they print in the same spot
            unsigned int upper = (unsigned int) (input_port_count > 4) & 0xF;
            unsigned int lower = (unsigned int) input_port_count & 0xF;
            printHexDigit(upper);
            printHexDigit(lower);
            input_port_count = 0;       // reset the count
        }
    }

    return 0;
}



