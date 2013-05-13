/*
2013
GNU GPL v2 license
Task Manager
Author Gogohia Levan, 1995 year
*/

#ifndef KRNL_TASK_AVR_H
#define KRNL_TASK_AVR_H
#define RTOS_RUN TIMER1_COMPA_vect

#define ASM(x)		asm volatile(x);
//! Status register define - map to 0x003F
#define SR_			0x3F
//! Stack pointer define
#define SPH_		0x3E
#define SPL_		0x3D

//---------------------------------------------------------------------------
//! Macro to find the top of a stack given its size and top address
#define TOP_OF_STACK(x, y)		(uchar*) ( ((byte)x) + (y-1) )
//! Push a value y to the stack pointer x and decrement the stack pointer
#define PUSH_TO_STACK(x, y)		*x = y; x--;

//---------------------------------------------------------------------------
//! Save the context of the task
#define Task_SaveContext() \
ASM("push r0"); \
ASM("in r0, __SREG__"); \
ASM("cli"); \
ASM("push r0"); \
ASM("push r1"); \
ASM("clr r1"); \
ASM("push r2"); \
ASM("push r3"); \
ASM("push r4"); \
ASM("push r5"); \
ASM("push r6"); \
ASM("push r7"); \
ASM("push r8"); \
ASM("push r9"); \
ASM("push r10"); \
ASM("push r11"); \
ASM("push r12"); \
ASM("push r13"); \
ASM("push r14"); \
ASM("push r15"); \
ASM("push r16"); \
ASM("push r17"); \
ASM("push r18"); \
ASM("push r19"); \
ASM("push r20"); \
ASM("push r21"); \
ASM("push r22"); \
ASM("push r23"); \
ASM("push r24"); \
ASM("push r25"); \
ASM("push r26"); \
ASM("push r27"); \
ASM("push r28"); \
ASM("push r29"); \
ASM("push r30"); \
ASM("push r31"); \
ASM("lds r26, current_task"); \
ASM("lds r27, current_task + 1"); \
ASM("in	r0, 0x3D"); \
ASM("st	x+, r0"); \
ASM("in	r0, 0x3E"); \
ASM("st	x+, r0"); 

//---------------------------------------------------------------------------
//! Restore the context of the task
#define Task_RestoreContext() \
ASM("lds r26, current_task"); \
ASM("lds r27, current_task + 1");\
ASM("ld	 r28, x+"); \
ASM("out 0x3D, r28"); \
ASM("ld	 r29, x+"); \
ASM("out 0x3E, r29"); \
ASM("pop r31"); \
ASM("pop r30"); \
ASM("pop r29"); \
ASM("pop r28"); \
ASM("pop r27"); \
ASM("pop r26"); \
ASM("pop r25"); \
ASM("pop r24"); \
ASM("pop r23"); \
ASM("pop r22"); \
ASM("pop r21"); \
ASM("pop r20"); \
ASM("pop r19"); \
ASM("pop r18"); \
ASM("pop r17"); \
ASM("pop r16"); \
ASM("pop r15"); \
ASM("pop r14"); \
ASM("pop r13"); \
ASM("pop r12"); \
ASM("pop r11"); \
ASM("pop r10"); \
ASM("pop r9"); \
ASM("pop r8"); \
ASM("pop r7"); \
ASM("pop r6"); \
ASM("pop r5"); \
ASM("pop r4"); \
ASM("pop r3"); \
ASM("pop r2"); \
ASM("pop r1"); \
ASM("pop r0"); \
ASM("out __SREG__, r0"); \
ASM("pop r0"); 

//------------------------------------------------------------------------

#define ENABLE_INTS()		ASM("sei");
#define DISABLE_INTS()		ASM("cli");

//Enter in the critical section (save status register, disable interrupts 
inline void CS_ENTER( void )
{
	volatile uchar x;
	x = _SFR_IO8(SR_)
	DISABLE_INTS();
}

//Exit critical section - load status register and enable interrupts
inline void CS_EXIT ( void ) 
{
	_SFR_IO8(SR_) = x;
	ENABLE_INTS();
}

#endif