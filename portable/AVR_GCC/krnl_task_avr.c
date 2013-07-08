/*
2013
GNU GPL v2 license
Task Manager
Author Gogohia Levan, 1995 year
*/
#include <avr\io.h>
#include <avr\interrupt.h>
#include "krnl_task_avr.h"
#include "krnl_timer_avr.h"

task_t * current_task;

void task_initstack (task_t * task_struct)
{
	// Initialize the stack for the task.
	ubyte usAddr;
	uchar *pucStack;
	byte i;

	// Get the stack pointer.
	pucStack = (uchar*)current_task->taskTopStack;
	
	// Get the "return" address for the function
	usAddr = (ubyte)(current_task -> taskFunc);
	
	// clear the stack...
	for (i = 0; i < task_struct -> taskSizeStack; i++)
	{
		current_task -> taskStack[i] = 0xFF;
	}
		
	// 1st - push start address...
	PUSH_TO_STACK(pucStack, (uchar)(usAddr & 0x00FF)); // hi
	PUSH_TO_STACK(pucStack, (uchar)((usAddr >> 8) & 0x00FF)); //low
	
	// 2nd - push R0
	PUSH_TO_STACK(pucStack, 0x00);	// R0
	
	// Push status register
	PUSH_TO_STACK(pucStack, 0x80);  // SR
	PUSH_TO_STACK(pucStack, 0x00);  // R1

	// Push other registers
	for (i = 2; i <= 23; i++) //R2-R23
	{
		PUSH_TO_STACK(pucStack, i);
	}

	// Assume that the task handle is the first and only function parameter to a task
	PUSH_TO_STACK(pucStack, (uchar)((ubyte)current_task) & 0x00FF);	//R24
	PUSH_TO_STACK(pucStack, (uchar)(((ubyte)current_task)>>8) & 0x00FF); //R25

	// Push the rest of the fake stack variables.
	for (i = 26; i <=31; i++)
	{
		PUSH_TO_STACK(pucStack, i);
	}
	// Set the top o' the stack
	current_task -> taskTopStack = pucStack;
}



void task_start_sheld(void)
{
	current_task = NULL;
	timer_config(); //config timer1
	task_run(); //task_run
	shedule_set_state(true);//enable sheduler
	timer_start();  //start timer1
	/*task_switch(); //go in sheduler
	Task_RestoreContext(); //get context for task*/
}

void task_yield ( void ) __attribute__ ( ( naked ) );
void task_yield ( void )
{
	Task_SaveContext();
	task_tick(); 
	task_switch(); 
	Task_RestoreContext();
	ASM("ret"); //The output from the function
}

ISR(TIMER1_COMPA_vect)
{
	base_t state_sheduler = 0;
	state_sheduler = shedule_get_state();
	if(state_sheduler)
	{
		task_yield();
		//ASM("reti");//The output from the interrupt
	}
	ASM("reti");
}
