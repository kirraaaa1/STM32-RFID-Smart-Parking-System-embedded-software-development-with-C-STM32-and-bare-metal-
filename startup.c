#include <stdint.h>

extern uint32_t _estack;   /* Đỉnh Stack */
extern uint32_t _sidata;   /* Đầu vùng data gốc trên Flash */
extern uint32_t _sdata;    /* Đầu vùng data trên RAM */
extern uint32_t _edata;    /* Cuối vùng data trên RAM */
extern uint32_t _sbss;     /* Đầu vùng bss trên RAM */
extern uint32_t _ebss;     /* Cuối vùng bss trên RAM */


extern int main(void);

void Reset_Handler(void);
void Default_Handler(void);

void NMI_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void HardFault_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void MemManage_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void BusFault_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void UsageFault_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void SVC_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void DebugMon_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void PendSV_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void SysTick_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void WWDG_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void PVD_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TAMPER_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void RTC_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void FLASH_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void RCC_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void EXTI0_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void EXTI1_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void EXTI2_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void EXTI3_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void EXTI4_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void DMA1_Channel1_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void DMA1_Channel2_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void DMA1_Channel3_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void DMA1_Channel4_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void DMA1_Channel5_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void DMA1_Channel6_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void DMA1_Channel7_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void ADC1_2_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void USB_HP_CAN1_TX_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void USB_LP_CAN1_RX0_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void CAN1_RX1_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void CAN1_SCE_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void EXTI9_5_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TIM1_BRK_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TIM1_UP_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TIM1_TRG_COM_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TIM1_CC_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TIM2_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TIM3_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TIM4_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void I2C1_EV_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void I2C1_ER_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void I2C2_EV_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void I2C2_ER_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void SPI1_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void SPI2_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void USART1_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void USART2_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void USART3_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void EXTI15_10_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void RTC_Alarm_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void USBWakeUp_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TIM8_BRK_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TIM8_UP_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TIM8_TRG_COM_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TIM8_CC_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void ADC3_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void FSMC_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void SDIO_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TIM5_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void SPI3_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void UART4_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void UART5_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TIM6_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void TIM7_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void DMA2_Channel1_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void DMA2_Channel2_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void DMA2_Channel3_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

void DMA2_Channel4_5_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));

/*Bảng vector ngắt.*/
__attribute__((used, section(".isr_vector")))
const uintptr_t isr_vector[] =
{
    (uintptr_t)&_estack,                   /* 0: Initial Stack Pointer */
    (uintptr_t)Reset_Handler,              /* 1: Reset Handler */
    (uintptr_t)NMI_Handler,                /* 2: NMI */
    (uintptr_t)HardFault_Handler,          /* 3: Hard Fault */
    (uintptr_t)MemManage_Handler,          /* 4: Memory Management */
    (uintptr_t)BusFault_Handler,           /* 5: Bus Fault */
    (uintptr_t)UsageFault_Handler,         /* 6: Usage Fault */

    (uintptr_t)0U,                         /* 7: Reserved */
    (uintptr_t)0U,                         /* 8: Reserved */
    (uintptr_t)0U,                         /* 9: Reserved */
    (uintptr_t)0U,                         /* 10: Reserved */

    (uintptr_t)SVC_Handler,                /* 11: SVCall */
    (uintptr_t)DebugMon_Handler,           /* 12: Debug Monitor */

    (uintptr_t)0U,                         /* 13: Reserved */

    (uintptr_t)PendSV_Handler,             /* 14: PendSV */
    (uintptr_t)SysTick_Handler,            /* 15: SysTick */

    (uintptr_t)WWDG_IRQHandler,            /* IRQ 0  */
    (uintptr_t)PVD_IRQHandler,             /* IRQ 1  */
    (uintptr_t)TAMPER_IRQHandler,          /* IRQ 2  */
    (uintptr_t)RTC_IRQHandler,             /* IRQ 3  */
    (uintptr_t)FLASH_IRQHandler,           /* IRQ 4  */
    (uintptr_t)RCC_IRQHandler,             /* IRQ 5  */

    (uintptr_t)EXTI0_IRQHandler,           /* IRQ 6  */
    (uintptr_t)EXTI1_IRQHandler,           /* IRQ 7  */
    (uintptr_t)EXTI2_IRQHandler,           /* IRQ 8  */
    (uintptr_t)EXTI3_IRQHandler,           /* IRQ 9  */
    (uintptr_t)EXTI4_IRQHandler,           /* IRQ 10 */

    (uintptr_t)DMA1_Channel1_IRQHandler,   /* IRQ 11 */
    (uintptr_t)DMA1_Channel2_IRQHandler,   /* IRQ 12 */
    (uintptr_t)DMA1_Channel3_IRQHandler,   /* IRQ 13 */
    (uintptr_t)DMA1_Channel4_IRQHandler,   /* IRQ 14 */
    (uintptr_t)DMA1_Channel5_IRQHandler,   /* IRQ 15 */
    (uintptr_t)DMA1_Channel6_IRQHandler,   /* IRQ 16 */
    (uintptr_t)DMA1_Channel7_IRQHandler,   /* IRQ 17 */

    (uintptr_t)ADC1_2_IRQHandler,          /* IRQ 18 */

    (uintptr_t)USB_HP_CAN1_TX_IRQHandler,  /* IRQ 19 */
    (uintptr_t)USB_LP_CAN1_RX0_IRQHandler, /* IRQ 20 */
    (uintptr_t)CAN1_RX1_IRQHandler,        /* IRQ 21 */
    (uintptr_t)CAN1_SCE_IRQHandler,        /* IRQ 22 */

    (uintptr_t)EXTI9_5_IRQHandler,         /* IRQ 23 */

    (uintptr_t)TIM1_BRK_IRQHandler,        /* IRQ 24 */
    (uintptr_t)TIM1_UP_IRQHandler,         /* IRQ 25 */
    (uintptr_t)TIM1_TRG_COM_IRQHandler,    /* IRQ 26 */
    (uintptr_t)TIM1_CC_IRQHandler,         /* IRQ 27 */

    (uintptr_t)TIM2_IRQHandler,            /* IRQ 28 */
    (uintptr_t)TIM3_IRQHandler,            /* IRQ 29 */
    (uintptr_t)TIM4_IRQHandler,            /* IRQ 30 */

    (uintptr_t)I2C1_EV_IRQHandler,         /* IRQ 31 */
    (uintptr_t)I2C1_ER_IRQHandler,         /* IRQ 32 */
    (uintptr_t)I2C2_EV_IRQHandler,         /* IRQ 33 */
    (uintptr_t)I2C2_ER_IRQHandler,         /* IRQ 34 */

    (uintptr_t)SPI1_IRQHandler,            /* IRQ 35 */
    (uintptr_t)SPI2_IRQHandler,            /* IRQ 36 */

    (uintptr_t)USART1_IRQHandler,          /* IRQ 37 */
    (uintptr_t)USART2_IRQHandler,          /* IRQ 38 */
    (uintptr_t)USART3_IRQHandler,          /* IRQ 39 */

    (uintptr_t)EXTI15_10_IRQHandler,       /* IRQ 40 */
    (uintptr_t)RTC_Alarm_IRQHandler,       /* IRQ 41 */
    (uintptr_t)USBWakeUp_IRQHandler,       /* IRQ 42 */

    (uintptr_t)TIM8_BRK_IRQHandler,        /* IRQ 43 */
    (uintptr_t)TIM8_UP_IRQHandler,         /* IRQ 44 */
    (uintptr_t)TIM8_TRG_COM_IRQHandler,    /* IRQ 45 */
    (uintptr_t)TIM8_CC_IRQHandler,         /* IRQ 46 */

    (uintptr_t)ADC3_IRQHandler,            /* IRQ 47 */
    (uintptr_t)FSMC_IRQHandler,            /* IRQ 48 */
    (uintptr_t)SDIO_IRQHandler,            /* IRQ 49 */

    (uintptr_t)TIM5_IRQHandler,            /* IRQ 50 */
    (uintptr_t)SPI3_IRQHandler,            /* IRQ 51 */
    (uintptr_t)UART4_IRQHandler,           /* IRQ 52 */
    (uintptr_t)UART5_IRQHandler,           /* IRQ 53 */
    (uintptr_t)TIM6_IRQHandler,            /* IRQ 54 */
    (uintptr_t)TIM7_IRQHandler,            /* IRQ 55 */

    (uintptr_t)DMA2_Channel1_IRQHandler,   /* IRQ 56 */
    (uintptr_t)DMA2_Channel2_IRQHandler,   /* IRQ 57 */
    (uintptr_t)DMA2_Channel3_IRQHandler,   /* IRQ 58 */
    (uintptr_t)DMA2_Channel4_5_IRQHandler  /* IRQ 59 */
};

void Default_Handler(void)
{
    while (1)
    {
    }
}

 /* Hàm đầu tiên được chạy sau khi vi điều khiển Reset.*/
void Reset_Handler(void)
{
    uint32_t *pSrc = &_sidata;
    uint32_t *pDst = &_sdata;

    /* Copy Data tu Flash sang RAM de chay */
    while (pDst < &_edata)
    {
        *pDst++ = *pSrc++;

    }

    /* Reset cac bien luu tru trong Bss ve 0 */
    uint32_t *pBss = &_sbss;

    while (pBss < &_ebss)
    {
        *pBss++ = 0U;
    }

    /* Goi chay ham main */
    (void)main();

    /* Treo neu ham main chay 1 la khong co vong lap de thong bao */
    while (1)
    {
    }
}