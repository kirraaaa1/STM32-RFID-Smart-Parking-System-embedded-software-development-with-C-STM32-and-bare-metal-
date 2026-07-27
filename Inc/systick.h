#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>
void SysTick_Init(uint32_t core_clock_hz);

/*Trả về số mili giây đã trôi qua kể từ khi khởi động */
uint32_t SysTick_GetMillis(void);void Delay_ms(uint32_t milliseconds);

/* weak SysTick_Handler trong startup.c.*/
void SysTick_Handler(void);

#endif 