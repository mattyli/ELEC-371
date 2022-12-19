/*-----------------------------------------------------------------*/

#ifndef _TIMER_H_
#define _TIMER_H_


/* define pointer macros for accessing the timer interface registers */


// NEED TO FIX THE MACROS
#define TIMER_STATUS	((volatile unsigned int *) 0x10002000)
#define TIMER_CONTROL	((volatile unsigned int *) 0x10002004)
#define TIMER_START_LO	((volatile unsigned int *) 0x10002008)
#define TIMER_START_HI	((volatile unsigned int *) 0x1000200C)
#define TIMER_SNAP_LO	((volatile unsigned int *) 0x10002010)
#define TIMER_SNAP_HI	((volatile unsigned int *) 0x10002014)

// Timer 0
#define TIMER0_STATUS	((volatile unsigned int *) 0x10004000)
#define TIMER0_CONTROL	((volatile unsigned int *) 0x10004004)
#define TIMER0_START_LO	((volatile unsigned int *) 0x10004008)
#define TIMER0_START_HI	((volatile unsigned int *) 0x1000400C)

// Timer 1
#define TIMER1_STATUS	((volatile unsigned int *) 0x10004020)
#define TIMER1_CONTROL	((volatile unsigned int *) 0x10004024)
#define TIMER1_START_LO	((volatile unsigned int *) 0x10004028)
#define TIMER1_START_HI	((volatile unsigned int *) 0x1000402C)

// Timer 2
#define TIMER2_STATUS	((volatile unsigned int *) 0x10004060)
#define TIMER2_CONTROL	((volatile unsigned int *) 0x10004064)
#define TIMER2_START_LO	((volatile unsigned int *) 0x10004068)
#define TIMER2_START_HI	((volatile unsigned int *) 0x1000406C)


/* define a bit pattern reflecting the position of the timeout (TO) bit
   in the timer status register */

#define TIMER_TO_BIT 0x1


#endif /* _TIMER_H_ */
/*-----------------------------------------------------------------*/