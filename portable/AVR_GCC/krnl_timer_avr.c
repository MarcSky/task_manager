/*
2013
GNU GPL v2 license
Task Manager
Author Gogohia Levan, 1995 year
*/

#include "krnl_timer_avr.h"

#include <avr\io.h>
#include <avr\interrupt.h>

void timer_config(void) //Инициализируем таймер
{
	TCCR1B |= (1 << WGM12); // CTC Mode Bit
	OCR1A = ((SYSTEM_FREQ / TICK_FREQ) / PRESC) - 1;	// Set the trigger time / 64 prescaler
	TCCR1B |= (1 << CS11)|(1<<CS10);		//		
}

void timer_start(void) //Запускаем таймер
{
	TIFR &=~(1<<OCF1A);  // Clear pending interrupts
	TIMSK |=(1<<OCIE1A);	// Enable interrupt
	TCCR1B |= (1 << CS10)|(1<<CS11);
}

void timer_stop (void) //Остонавливаем таймер
{
	TIFR &=~(1<<OCF1A);  // Clear interrupt flags
	TIMSK &=~(1<<OCIE1A);	// Disable interrupt
	//ASM("cli");
	TCCR1B &= ~(1 << CS10)|(1<<CS11);
}

void timer_diseble(void) //Выключить прерывания
{
	//ASM("cli");
	TIFR &= ~0x02;  // Clear pending interrupts
	TIMSK &= ~0x02;	// Disable interrupt
}

void timer_enable(void) //Включить прерывания
{	
	//ASM("sei");
	TIFR &= ~0x02;  // Clear pending interrupts
	TIMSK |= 0x02;	// Enable interrupt
}
