#include <avr/io.h>
#include <avr/interrupt.h>
#include "task.h"
#include "task_manager.h"
//example for AVR microproccessor
//ALL GOOD :D
typedef struct
{
	task_t task;
	ubyte stack[32];
} task_1;

void task (void * pthis)
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
	i = task_create((&(task_1.task)), (func_p)task, "FIRST_TASK!", (word*)(task_1.stack), 32);
	if(!i) 
	{
		//panic("task did't create");
		return 1;
	}
	return 0;
}