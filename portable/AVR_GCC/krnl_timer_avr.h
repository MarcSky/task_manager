/*
2013
GNU GPL v2 license
Task Manager
Author Gogohia Levan, 1995 year
*/

#ifndef KRNL_TIMER_AVR_H
#define KRNL_TIMER_AVR_H
#include "task_manager.h"

#define SYSTEM_FREQ 16000000UL
#define TICK_FREQ 1000
#define PRESC 64

void timer_config(void); 
void timer_start(void); 
void timer_stop (void); 
void timer_diseble(void); 
void timer_enable(void); 

#endif