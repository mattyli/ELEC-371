/*===================================================================

   Template single/combined C file for ELEC 371 laboratory activity

   October 2021  Dr. N. Manjikian

   C programs for Nios II systems can be prepared with multiple
   separate files in project defined with the Monitor Program.

   A single C file, however, is necessary for the Web-based simulator
   because the current version of the Web-based tool handles a single
   file in its editor window.

   This single file merges together several .h header files and
   the exception_handler.c file with the assembly-language code
   for saving/restoring registers and handling the ea adjustment.

   The .h header file contents are positioned at the beginning of
   this template file, and the exception_handler.c file contents
   are positioned at the end of this template file. There are
   comments with "----" dividers and original-file identification
   to aid in understanding the different portions.

   In the middle of this template file is the portion that can be
   customized for a particular application. For interrupt support,
   there is an empty interrupt_handler() function. There is an
   empty Init() function and a simple main() function. Other
   functions can be added as necessary in front of main().

==================================================================*/



/*-----------------------------------------------------------------*/

#ifndef _NIOS2_CONTROL_H_
#define _NIOS2_CONTROL_H_


/* 
   This file defines macros that utilize the built-in functions
   in Altera's adaption of the GCC compiler to generate the
   control-register-access instructions for the Nios II processor.

   For ELEC 371 laboratory work, it has been modified by N. Manjikian.

   It should be noted that the manner in which the built-in functions
   are used below _differs_ from the manner in which they are shown in
   the DE0 Basic Computer reference document from Altera. The key
   difference is for _read_ operations. The conventional method of
   defining macros that read a value is to have the value returned,
   like a function. The method used in the original Altera code is
   not conventional. The second main difference is a simplification
   that avoids the use of a do..while loop found in the Altera version.
   The do..while loops may have been introduced for a specific reason (e.g.,
   to perhaps prevent the compiler from optimizing away the calls to
   the built-in functions, or to allow the assignment of the return
   value to a named variable). For the purposes of laboratory work,
   the revision of the macros by the instructor is appropriate, and
   the compiler-generated code is correct.

   The official names for the processor control register are ctl0, ctl1, ...
   These registers are more conveniently identified by their aliases,
   such as status, estatus, ...  Documentation on the Nios II processor
   provides the association between the official names and the aliases.

   Not all of the possible control-register accesses are covered by
   the macros below. The ones most relevant for ELEC 371 laboratory work
   are included. If access to the other control registers is required,
   additional macros could easily be introduced, based on the existing ones.

   The number on the right side of each macro definition refers to the
   corresponding 'ctl_' processor control register. The __builtin_wrctl()
   function passes the ctl register number and a value to the compiler for
   use in generating the appropriate assembly-language instruction.
   The __builtin_rdctl() function passes only the register number, and
   it _returns_ a value. The instructor-revised read macros then return
   this result as the return value of the macro for use by the C program.
*/


#define NIOS2_WRITE_STATUS(value)  (__builtin_wrctl (0, value))

#define NIOS2_READ_IENABLE()	   (__builtin_rdctl (3))

#define NIOS2_WRITE_IENABLE(value) (__builtin_wrctl (3, value))

#define NIOS2_READ_IPENDING()	   (__builtin_rdctl (4))


#endif /* _NIOS2_CONTROL_H_ */