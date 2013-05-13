/*
2013
GNU GPL v2 license
Task Manager
Author Gogohia Levan, 1995 year
*/

#include "krnl_timer_avr.h"

void timer_config(void) //Инициализируем таймер
{
	TCCR1B |= (1 << WGM12); // CTC Mode Bit
	OCR1A = (ubyte)((SYSTEM_FREQ / TICK_FREQ) / 64) - 1;	// Set the trigger time / 64 prescaler
	TCCR1B |= (1 << CS11);		// CPU Time / 64 prescaler		
}

void timer_start(void) //Запускаем таймер
{
	TIFR1 &= ~0x02;  // Clear pending interrupts
	TIMSK1 |= 0x02;	// Enable interrupt
	TCCR1B |= (1 << CS10);
}

void timer_stop (void) //Остонавливаем таймер
{
	TIFR1 &= ~0x02;  // Clear interrupt flags
	TIMSK1 &= ~0x02;	// Disable interrupt
	TCCR1B &= ~(1 << CS10);
}

void timer_diseble(void) //Выключить прерывания
{
	TIFR1 &= ~0x02;  // Clear pending interrupts
	TIMSK1 &= ~0x02;	// Disable interrupt
}

void timer_enable(void) //Включить прерывания
{
	TIFR1 &= ~0x02;  // Clear pending interrupts
	TIMSK1 |= 0x02;	// Enable interrupt
}
