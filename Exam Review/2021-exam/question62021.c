/*
    functions to implement
        init x
        printchar x
        handletimer0
        handletimer1
        interrupt handler
        main
*/

#define TIMER0_STATUS           ((volatile unsigned int *) 0x00005000)
#define TIMER0_CONTROL          ((volatile unsigned int *) 0x00005004)
#define TIMER0_LO               ((volatile unsigned int *) 0x00005008)
#define TIMER0_HI               ((volatile unsigned int *) 0x0000500C)

#define TIMER1_STATUS           ((volatile unsigned int *) 0x00006000)
#define TIMER1_CONTROL          ((volatile unsigned int *) 0x00006004)
#define TIMER1_LO               ((volatile unsigned int *) 0x00006008)
#define TIMER1_HI               ((volatile unsigned int *) 0x0000600C)

#define INPORT                  ((volatile unsigned int *) 0x00006700)
#define OUTPORT                 ((volatile unsigned int *) 0x00006800)

#define JTAG_DATA               ((volatile unsigned int *) 0x00006A00)
#define JTAG_STATUS             ((volatile unsigned int *) 0x00006A04)

#define RAISED 1
#define CLEARED 0

unsigned int TIMER1_FLAG = 0;
unsigned int TIMER0_FLAG = 0;
unsigned int glob_average = 0;
unsigned int glob_count = 0;

void init(){
    *TIMER0_STATUS = 0x1;
    *TIMER0_CONTROL = 0x7;
    *TIMER0_HI = 6250000;
    *TIMER0_LO = 6250000 >> 16;     // bit shift 

    *TIMER1_STATUS = 0x1;
    *TIMER1_CONTROL = 0x7;
    *TIMER1_HI = 25000000;
    *TIMER1_LO = 25000000 >> 16;     // bit shift 
    
    NIOS2_WRITE_IENABLE(0x60);   // bit 5 and 6 for timer 0 and 1 respectively, 0b1100000
    NIOS2_WRITE_STATUS(1);
}

void printChar(unsigned int ch){
    unsigned int status;
    do{
        status = (*JTAG_STATUS & 0xFFFF0000);
    }while(status == 0);
    *JTAG_DATA = ch;
}

void handleTimer0(){

}

void handleTimer1(){

}

void interruptHandler(){
    unsigned int ipending;
    ipending = NIOS2_READ_IPENDING();       // get the ipending value
    if (ipending & 0x20){
        // bit 5 activated
        handleTimer0();
    }
    if (ipending & 0x40){
        // bit 6 activated
        handleTimer0();
    }
}

int main(void){
    init();
    while(1){
        
    }
    return 0;
}