/*
2013
GNU GPL v2 license
Task Manager
Author Gogohia Levan, 1995 year
*/

#ifndef TASK_H
#define TASK_H
#include "sysmacro.h"

//#include "krnl_task_avr.h"
#define MAX_TASK_NAME 15
#define MAX_TASK_SLEEP (uword)65535
#define IDLE_TASK 0
#define DEFAULT_STACK (byte)32

typedef enum 
{
	TASK_RUNNING,
	TASK_READY,
	TASK_BLOCKED,
	TASK_SLEEP,
} TASK_STATUS;

typedef struct task_t
{	
	word *taskTopStack; //a pointer to the top of the stack
	unsigned char *taskStack; //The stack pointer is determined by the task
	byte taskSizeStack;//stack size
	char taskName[MAX_TASK_NAME]; //name of task
	//base_t taskPriority; //priority
	func_p taskFunc; //function pointer
	base_t taskState; //state task
	uword taskSleep; //time for sleep 
	struct task_t * next_task;
	struct task_t * prev_task;
	base_t taskPid; //number task
} task_t;



/*Do not be afraid of the set of functions. He is very simple.*/
void task_run (void);
void task_ready (task_t * task_struct);
base_t task_create(task_t * tast_struct, func_p func_task, char * name,  unsigned char * stack_point, byte size_stack);
 void task_tick( void ); //This function is enabled in the interrupt. It is necessary to count the ticks to interrupt for sleeping tasks
base_t task_sleep (uword time); //task_sleep
base_t task_delete (task_t * task_struct);
void task_switch ( void ); //My Sheduler
//#ifndef IDLE_TASK == 0
void my_idle_task (void);
//#endif
task_t * task_get_current ( void );
base_t task_count ( void );
base_t task_pid (task_t * task_struct);
base_t task_stop (task_t * task_struct);
base_t task_start (task_t * task_struct);
//char * task_name (task_t * task_struct);
base_t task_sizestack (task_t * task_struct);
base_t task_state (task_t * task_struct);
void shedule_set_state (base_t state);
base_t shedule_get_state (void);
base_t shedule_disable (void);
void shedule_enable (void);

#endif
