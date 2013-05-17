/*
2013
GNU GPL v2 license
Task Manager
Author Gogohia Levan, 1995 year
*/

#ifndef SYSMACRO_H
#define SYSMACRO_H

#define OK  1
#define BAD 0
#define NULL (void*)0
#define true  1
#define false 0
//Kernel type
//#ifndef AVR8 || STM8 || PIC8
typedef unsigned char base_t;
//#endif
//#ifndef MSP430
//typedef unsigned int base_t;
/*#endif
#ifndef ARM32||AVR32||PIC32
typedef unsigned long base_t;
#endif*/
typedef unsigned char uchar;
typedef unsigned char ubyte;
typedef char byte;
typedef unsigned int uword;
typedef int word;
typedef long dword;
typedef unsigned long udword;
typedef void (*func_p)(void); //pointer function task

#endif