/*
2013
GNU GPL v2 license
Task Manager
Author Gogohia Levan, 1995 year
*/

#include "task.h"
//#include "portable/task_manager.h"
//#include "portable/krnl_task_avr.h"
//#include "portable/krnl_timer_avr.h"
//#include <string.h>
//base_t - base type: for AVR - char, for ARM  - long :D
volatile base_t pid = 0; //count of proccess
volatile base_t shedule = false; //state sheduler

typedef struct 
{
	task_t idle;
	ubyte stack[DEFAULT_STACK];
} task_idle_s;

task_idle_s my_task;

void my_idle_task ( void )
{
	uchar i = 0;
	i++;
	if(i>=255) i = 0;
}


extern task_t * current_task; //current_task
task_t * head_task;           //head
task_t * tail_task;			  //tail

void task_init ( void )
{
	//reset all
	current_task = NULL;
	head_task = NULL;
	tail_task  = NULL;
}

void task_run (void)
{
	CS_ENTER();
	task_init();
	//The kernel must perform what ever task!
	task_create(&(my_task.idle), my_idle_task, (word*)(my_task.stack), DEFAULT_STACK);
	CS_EXIT();
}

void task_ready (task_t * task_struct)
{   //Add our task in the tail or head
	if(!head_task) {
		head_task = task_struct;
		tail_task = task_struct;
	} else {
		tail_task -> next_task = task_struct;
		tail_task  = task_struct;
	}
}

base_t task_create(task_t * now_task, func_p func_task, word * stack_point, byte size_stack)
{
	task_t * task_struct = now_task;
	//Create a task! The main part of the manager! :D
	if (!func_task || !task_struct) return BAD;
	
	if(task_count() > 255) return BAD; // now max count of task - 255
	
	CS_ENTER(); //critical section start
	
/*	if(strlen(name) >= MAX_TASK_NAME) //The buffer overflow is bad!
	{
		//task_struct -> taskName = name; //name
	}
	*/
	task_struct -> taskFunc = func_task; //func_task
	task_struct -> taskPid = pid;        //pid
	task_struct -> taskSizeStack = size_stack; //size_stack
	task_struct -> taskStack = stack_point;//stack_point for this task
	task_struct -> taskTopStack = (word*)TOP_OF_STACK(task_struct, size_stack); // init stack for AVR, this is standart stack, i find him in avr lib for winavr
	task_struct -> taskSleep = 0; //time for sleep
	task_struct -> taskState = TASK_READY; //state this task
	task_struct -> next_task = NULL; //next task doesn't exist
	task_initstack(task_struct); //init stack for our task
	
	if (!current_task) //If we do not have task, but it's impossible, becouse we have task_idle, But the need for security check!
	{  
		current_task = task_struct;
	}
	
	if (current_task) //if not first :D
	{
		current_task -> next_task = task_struct;		
	}
	
	task_ready(task_struct); //write our task in head and/or tail
	pid+=1; //count of proccess +1
	
	CS_EXIT(); //crititcal section end
	return OK;
}

void task_tick( void ) //This function is enabled in the interrupt. It is necessary to count the ticks to interrupt for sleeping tasks
{
	task_t * start = head_task;
	task_t * end = tail_task;
	while(start!=end)
	{
		if(start -> taskState == TASK_SLEEP && start -> taskSleep !=0 )
		{
			start -> taskSleep--;
			if(start->taskSleep == 0) {start->taskState = TASK_READY;}
			start=start->next_task;
		}
	}
}

base_t task_sleep (uword time) //task_sleep
{   //Sleep
	if(time >= MAX_TASK_SLEEP) return BAD; //max time for sleep  65535 tick or ~65 secound for AVR, if 1sec = 1000 tick for TIMER 16 bit
	CS_ENTER();
		current_task->taskState = TASK_SLEEP;
		current_task->taskSleep = time;
	CS_EXIT();
	
	task_switch(); //Go next task, this task want sleep :D
	
	return OK;
}	

base_t task_delete (task_t * task_struct)
{  
	if(task_struct == NULL /*|| task_struct == task_idle_s*/) return BAD;
	
	CS_ENTER();
	//delete task
	task_t * prev;
	task_t * next;
	prev = task_struct -> prev_task;
	next = task_struct -> next_task;
	prev -> next_task = next;
	next -> prev_task = prev;
	
	pid-=1; //minus proccess
	CS_EXIT();
	
	return OK;
}

//I am a little unsure of the correct operation of the scheduler, but I think that it works well.
void task_switch ( void ) //My Sheduler
{
	task_t * start = head_task;
	task_t * end   = tail_task;
		
	if(shedule)
	{
		if(start == NULL) //if we don't have task, for example, we did not create task_idle
		{   
			//This serious error
			//for AVR i'm send in usart this string
			//panic("task doesn't exist");
			return;
		}
		if(start == end) //If we have only one task, example - task_idle or we went on all the tasks
		{
			if(pid == 1) 
			{
				current_task = start; //starting task_idle;
				(current_task->taskFunc)(); // starting function of task
			}
			else //if count of task > 1
			{    //Begin to run on the tasks of the meat
				start = head_task;
				current_task = start;
				(current_task->taskFunc)(); // starting function of task
				while(start!=end)
				{
					if(start->taskState == TASK_RUNNING && start->taskFunc != my_idle_task)
					{
						start -> taskState = TASK_READY;
						start -> taskSleep = 0;
						start =  start -> next_task;
					}
				}
			}
		
		}
		else //if we have > 1 task and we dont have end of task;
		{
			while(start!=end)
			{
				if(start->taskState == TASK_READY && start -> taskFunc != my_idle_task)
				{
					current_task = start;
					start = start -> next_task;
					current_task -> taskState = TASK_RUNNING;
					(current_task->taskFunc)(); // starting function of task
					//break;
				}
			}
		}
	}
	else 
	{
		//panic("shedule offline");
		return;
	}
}
/*
Mini API for this task_manager
*/

task_t * task_get_current ( void )
{
	return current_task;
}
base_t task_count ( void )
{
	return pid; //get count proccess
}

base_t task_pid (task_t * task_struct) 
{	//get number our proccess
	if(task_struct == NULL) return BAD;
	else return task_struct -> taskPid;
}

base_t task_stop (task_t * task_struct)
{	//stop out task
	if(task_struct == NULL) return BAD;
	
	CS_ENTER();
		task_struct -> taskState = TASK_BLOCKED;
	CS_EXIT();
	
	return OK;
}

base_t task_start (task_t * task_struct)
{	//start our task
	if(task_struct == NULL) return BAD;
	CS_ENTER();
		task_struct -> taskState = TASK_READY;
	CS_EXIT();
	
	return OK;
}

/*uchar * task_name (task_t * task_struct)
{	
	if(task_struct) 
	{
		return task_struct -> taskName;
	}
	else
	{
		return NULL;
	}
}*/

base_t task_sizestack (task_t * task_struct)
{
	if(task_struct)
	{
		return task_struct -> taskSizeStack;
	} 
	else
	{
		return 0;
	}
}

base_t task_state (task_t * task_struct)
{
	if(task_struct)
	{
		return task_struct -> taskState; 
	}
	else
	{
		return TASK_DOEST_EXIST;
	}
}

void shedule_set_state (base_t state)
{
	shedule = state;
}

base_t shedule_get_state (void)
{
	return shedule;
}

base_t shedule_disable (void)
{
	base_t data; 
	CS_ENTER();
	data = shedule_get_state();
	shedule_set_state(false);
	CS_EXIT();
	return data;
}

void shedule_enable (void)
{
	CS_ENTER();
	shedule_set_state(true);
	CS_EXIT();
}