#include "systick.h"

/* Thanh ghi SysTick của Cortex-M3 */
#define SYSTICK_CTRL    (*(volatile uint32_t *)0xE000E010UL)
#define SYSTICK_LOAD    (*(volatile uint32_t *)0xE000E014UL)
#define SYSTICK_VAL     (*(volatile uint32_t *)0xE000E018UL)

/* Bit trong SysTick CTRL */
#define SYSTICK_ENABLE      (1U << 0)
#define SYSTICK_TICKINT     (1U << 1)
#define SYSTICK_CLKSOURCE   (1U << 2)

static volatile uint32_t systick_milliseconds = 0U;

void SysTick_Init(uint32_t core_clock_hz)
{
    /* Tắt SysTick trước khi cấu hình */
    SYSTICK_CTRL = 0U;

    /*
     * Tạo ngắt mỗi 1 ms.
     * Ví dụ 8 MHz:
     * 8.000.000 / 1.000 = 8.000
     */
    SYSTICK_LOAD = (core_clock_hz / 1000U) - 1U;

    /* Xóa giá trị đếm hiện tại */
    SYSTICK_VAL = 0U;

    systick_milliseconds = 0U;

    /* Dùng clock CPU, bật ngắt và bật SysTick */
    SYSTICK_CTRL =
        SYSTICK_CLKSOURCE |
        SYSTICK_TICKINT |
        SYSTICK_ENABLE;
}

void SysTick_Handler(void)
{
    systick_milliseconds++;
}

uint32_t SysTick_GetMillis(void)
{
    return systick_milliseconds;
}

void Delay_ms(uint32_t milliseconds)
{
    uint32_t start_time = SysTick_GetMillis();

    while ((SysTick_GetMillis() - start_time) < milliseconds)
    {
        /* Chờ */
    }
}