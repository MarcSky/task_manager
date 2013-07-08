#include <avr/io.h>
#include <avr/interrupt.h>
#include "task.h"
#include "sysmacro.h"
#include "krnl_task_avr.h"
#include "debug.h"

typedef struct
{
	task_t task_;
	ubyte stack[DEFAULT_STACK];
} task1;

typedef struct
{
	task_t task;
	ubyte stack[DEFAULT_STACK];
} task2;

task1 my_task1;
task2 my_task2;

void task1 (void)
{
	uchar i = 0;
	
	for(;;)
	{
		i+=10;
	}
}

void task2 (void)
{
	uchar a = 0;
	
	for(;;)
	{
		a+=10;
	}
}

int main (void)
{
	init_usart();
	task_start_sheld(); //init kernel
	sei();
	int z = task_create(&(my_task1.task_), (func_p)task1, "FIRST_TASK!", (word*)(my_task1.stack), DEFAULT_STACK);
	int j = task_create(&(my_task2.task), (func_p)task2, "SECOND_TASK!",(word*)(my_task2.stack), DEFAULT_STACK);
	
	if(z!=0) Debug_str("task1 ok");
	if(j!=0) Debug_str("task2 ok");
	
	return 0;
}