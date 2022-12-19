#include "led.h"
#include "timer.h"
#include "nios2_control.h"

/*-----------------------------------------------------------------*/
/*             start of application-specific code                  */
/*-----------------------------------------------------------------*/

#define BUTTON (volatile unsigned int *) 0x10000050
#define BUTTON_MASK (volatile unsigned int *) 0x10000058
#define BUTTON_EDGE (volatile unsigned int *) 0x1000005C
#define HEX_DISPLAY (volatile unsigned int *) 0x10000020

// Do note ^ this is a parallel port ok? Technically speaking...
// See for more -> https://ftp.intel.com/Public/Pub/fpgaup/pub/Intel_Material/13.0/Computer_Systems/DE0/DE0_Basic_Computer.pdf

void interrupt_handler(void)
{
   unsigned int ipending;
   ipending = NIOS2_READ_IPENDING();

   if ((ipending & 0b1) == 0b1) {
      // when an interrupt comes in, the ipending will stay on until the device turns
      // off it's signal requesting the interrupt, so we need to tell the timer to turn off it's 1st bit which is
      // related to ITO in this interrupt, telling the timer to call again once times up
      *TIMER_STATUS = *TIMER_STATUS & 0b10;

      // here we are flipping a simple LED on the far right
      // there are two ways of doing this:
      // the flip way
      // *LEDS = (~*LEDS) & 0b1;
      // and the XOR way (which the 2.5 lab asks you to do btw)
      *LEDS = *LEDS ^ 0b1;
   }
   
   if ((ipending & 0b10) == 0b10) {
      unsigned int pressed = *BUTTON_EDGE;
      *BUTTON_EDGE = pressed; // fire back to turn them off
      // *LEDS = *LEDS ^ pressed;
      // ^ uncomment this and it should toggle the 2nd and 3rd LEDS per button
      // I just did this to be sure

      // Like in assembly, doing a -1 will cause a wrap around.
      // each hex digit in display holds 0-7 inputs
      // 4 of that is 32-bits of inputs, and since unsigned ints are 32-bits
      // you get the idea :)
      *HEX_DISPLAY = *HEX_DISPLAY ^ (unsigned int)-1;
   }

   if ((ipending & 0x2000) == 0x2000){// because bit 13 (0010 0000 0000 0000)
      // handle timer 0 interrupt
      // make the LED move (1 light at a time) move R -> L
      
   }

   if ((ipending & 0x4000)== 0x4000){// because bit 14 (0100 0000 0000 0000)
      // handle timer 1 interrupt
   }

   if ((ipending & 0x8000) == 0x8000){// because bit 15 (1000 0000 0000 0000)
      // handle timer2 interrupt
      int charFlag = 0;
      if (charFlag==0){
         // implement JTAG CODE
      }
   }

}

void Init (void)
{
   // TIMER 0 SET:
   *TIMER0_START_LO = 0x4B40;
   *TIMER0_START_HI = 0x004C;
   *TIMER0_STATUS = 0b0;         // clearing the status
   *TIMER0_CONTROL = 0b111;      // setting start, control, ito

   // TIMER 1 SET:
   *TIMER1_START_LO = 0x5E10;
   *TIMER1_START_HI = 0x005F;
   *TIMER1_STATUS = 0b0;         // clearing the status
   *TIMER1_CONTROL = 0b111;      // setting start, control, ito

   // TIMER 2 SET:
   *TIMER2_START_LO = 0x7840;
   *TIMER2_START_HI = 0x017D;
   *TIMER2_STATUS = 0b0;         // clearing the status
   *TIMER2_CONTROL = 0b111;      // setting start, control, ito

   // Push buttons are IRQ 1 -> 2nd bit
   *BUTTON_MASK = 0b110; // enable button 2 & 1 for interrupts (not 0)
   // ^ on the actual board, button 0 is used for a different purpose

   NIOS2_WRITE_STATUS(0b1);            // enables processor interrupts
   NIOS2_WRITE_IENABLE(0b11);          // enables IRQ 0 & 1, our timer & buttons
}


int main (void)
{
    Init ();

   // just as an example of a loop
   int a = 0;
    while (1)
    {
        if (a == 0) {
         a = 1;
      } else {
         a = 0;
      }
    }

    return 0;
}
/*-----------------------------------------------------------------*/
/*              end of application-specific code                   */
/*-----------------------------------------------------------------*/



/*-----------------------------------------------------------------*/


/* 
   exception_handler.c

   This file is a portion of the original code supplied by Altera.

   It has been adapted by N. Manjikian for use in ELEC 371 laboratory work.

   Various unnecessary or extraneous elements have been excluded. For
   example, declarations in C for external functions called from asm()
   instructions are not required because any reference to external names
   in asm() instructions is embedded directly in the output written to
   the assembly-language .s file without any other checks by the C compiler.

   There is one particularly important change: on _reset_, the jump must be
   to the >> _start << location in order to properly initialize the stack
   pointer and to perform other crucial initialization tasks that ensure
   proper C semantics for variable initialization are enforced. The Altera
   version of the code jumped to main(), which will _not_ perform these
   crucial initialization tasks correctly.

   Finally, a reference to control register 'ctl4' in the asm() sequence
   has been replaced with the more meaningful alias 'ipending' for clarity.

   Other than the changes described above, the file contents have also been
   reformatted to fit in 80 columns of text, and comments have been edited.
*/


/* The assembly language code below handles processor reset */
void the_reset (void) __attribute__ ((section (".reset")));

/*****************************************************************************
 * Reset code. By giving the code a section attribute with the name ".reset" *
 * we allow the linker program to locate this code at the proper reset vector*
 * address. This code jumps to _startup_ code for C program, _not_ main().   *
 *****************************************************************************/

void the_reset (void)
{
  asm (".set noat");         /* the .set commands are included to prevent */
  asm (".set nobreak");      /* warning messages from the assembler */
  asm ("movia r2, _start");  /* jump to the C language _startup_ code */
  asm ("jmp r2");            /* (_not_ main, as in the original Altera file) */
}

/* The assembly language code below handles exception processing. This
 * code should not be modified; instead, the C language code in the normal
 * function interrupt_handler() [which is called from the code below]
 * can be modified as needed for a given application.
 */

void the_exception (void) __attribute__ ((section (".exceptions")));

/*****************************************************************************
 * Exceptions code. By giving the code a section attribute with the name     *
 * ".exceptions" we allow the linker program to locate this code at the      *
 * proper exceptions vector address. This code calls the interrupt handler   *
 * and later returns from the exception to the main program.                 *
 *****************************************************************************/

void the_exception (void)
{
  asm (".set noat");         /* the .set commands are included to prevent */
  asm (".set nobreak");      /* warning messages from the assembler */
  asm ("subi sp, sp, 128");
  asm ("stw  et, 96(sp)");
  asm ("rdctl et, ipending"); /* changed 'ctl4' to 'ipending' for clarity */
  asm ("beq  et, r0, SKIP_EA_DEC");   /* Not a hardware interrupt, */
  asm ("subi ea, ea, 4");             /* so decrement ea by one instruction */ 
  asm ("SKIP_EA_DEC:");
  asm ("stw	r1,  4(sp)"); /* Save all registers */
  asm ("stw	r2,  8(sp)");
  asm ("stw	r3,  12(sp)");
  asm ("stw	r4,  16(sp)");
  asm ("stw	r5,  20(sp)");
  asm ("stw	r6,  24(sp)");
  asm ("stw	r7,  28(sp)");
  asm ("stw	r8,  32(sp)");
  asm ("stw	r9,  36(sp)");
  asm ("stw	r10, 40(sp)");
  asm ("stw	r11, 44(sp)");
  asm ("stw	r12, 48(sp)");
  asm ("stw	r13, 52(sp)");
  asm ("stw	r14, 56(sp)");
  asm ("stw	r15, 60(sp)");
  asm ("stw	r16, 64(sp)");
  asm ("stw	r17, 68(sp)");
  asm ("stw	r18, 72(sp)");
  asm ("stw	r19, 76(sp)");
  asm ("stw	r20, 80(sp)");
  asm ("stw	r21, 84(sp)");
  asm ("stw	r22, 88(sp)");
  asm ("stw	r23, 92(sp)");
  asm ("stw	r25, 100(sp)"); /* r25 = bt (r24 = et, saved above) */
  asm ("stw	r26, 104(sp)"); /* r26 = gp */
  /* skip saving r27 because it is sp, and there is no point in saving sp */
  asm ("stw	r28, 112(sp)"); /* r28 = fp */
  asm ("stw	r29, 116(sp)"); /* r29 = ea */
  asm ("stw	r30, 120(sp)"); /* r30 = ba */
  asm ("stw	r31, 124(sp)"); /* r31 = ra */
  asm ("addi	fp,  sp, 128"); /* frame pointer adjustment */

  asm ("call	interrupt_handler"); /* call normal function */

  asm ("ldw	r1,  4(sp)"); /* Restore all registers */
  asm ("ldw	r2,  8(sp)");
  asm ("ldw	r3,  12(sp)");
  asm ("ldw	r4,  16(sp)");
  asm ("ldw	r5,  20(sp)");
  asm ("ldw	r6,  24(sp)");
  asm ("ldw	r7,  28(sp)");
  asm ("ldw	r8,  32(sp)");
  asm ("ldw	r9,  36(sp)");
  asm ("ldw	r10, 40(sp)");
  asm ("ldw	r11, 44(sp)");
  asm ("ldw	r12, 48(sp)");
  asm ("ldw	r13, 52(sp)");
  asm ("ldw	r14, 56(sp)");
  asm ("ldw	r15, 60(sp)");
  asm ("ldw	r16, 64(sp)");
  asm ("ldw	r17, 68(sp)");
  asm ("ldw	r18, 72(sp)");
  asm ("ldw	r19, 76(sp)");
  asm ("ldw	r20, 80(sp)");
  asm ("ldw	r21, 84(sp)");
  asm ("ldw	r22, 88(sp)");
  asm ("ldw	r23, 92(sp)");
  asm ("ldw	r24, 96(sp)");
  asm ("ldw	r25, 100(sp)");
  asm ("ldw	r26, 104(sp)");
  /* skip r27 because it is sp, and we did not save this on the stack */
  asm ("ldw	r28, 112(sp)");
  asm ("ldw	r29, 116(sp)");
  asm ("ldw	r30, 120(sp)");
  asm ("ldw	r31, 124(sp)");

  asm ("addi	sp,  sp, 128");

  asm ("eret"); /* return from exception */

  /* Note that the C compiler will still generate the 'standard'
     end-of-normal-function code with a normal return-from-subroutine
     instruction. But with the above eret instruction embedded
     in the final output from the compiler, that end-of-function code
     will never be executed.
   */ 
}