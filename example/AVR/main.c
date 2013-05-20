#include <avr/io.h>
#include <avr/interrupt.h>
#include "kernel/task.h
#include "portable/AVR_GCC/krnl_task_avr.h"
#include "portable/AVR_GCC/krnl_timer_avr.h"
//example for AVR microproccessor
//ALL GOOD :D
typedef struct
{
	task_t task;
	ubyte stack[32];
} task_1;

void task (void)
{
	uchar i = 0;
	
	for(;;)
	{
		i+=10;
	}
}

int main (void)
{
	task_start_sheld(); //init kernel
	int i = 0;
	i = task_create(&(task_1.task), task, (word*)(task_1.stack), 32);
	if(!i) 
	{
		//panic("task did't create");
		return 1;
	}
	return 0;
}