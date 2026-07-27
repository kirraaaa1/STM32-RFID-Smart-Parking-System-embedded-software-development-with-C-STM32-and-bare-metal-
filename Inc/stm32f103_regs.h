#ifndef STM32F103_REGS_H
#define STM32F103_REGS_H

#include <stdint.h>

#define RCC_BASE            0x40021000UL

#define RCC_CR              (*(volatile uint32_t *)(RCC_BASE + 0x00UL))
#define RCC_CFGR            (*(volatile uint32_t *)(RCC_BASE + 0x04UL))
#define RCC_APB2ENR         (*(volatile uint32_t *)(RCC_BASE + 0x18UL))
#define RCC_APB1ENR         (*(volatile uint32_t *)(RCC_BASE + 0x1CUL))

/* RCC_APB2ENR bits */
#define RCC_APB2ENR_AFIOEN  (1U << 0)
#define RCC_APB2ENR_IOPAEN  (1U << 2)
#define RCC_APB2ENR_IOPBEN  (1U << 3)
#define RCC_APB2ENR_SPI1EN  (1U << 12)

/* RCC_APB1ENR bits */
#define RCC_APB1ENR_TIM2EN  (1U << 0)
#define RCC_APB1ENR_I2C1EN  (1U << 21)


/* FLASH*/

#define FLASH_BASE          0x40022000UL
#define FLASH_ACR           (*(volatile uint32_t *)(FLASH_BASE + 0x00UL))


/* GPIOA */

#define GPIOA_BASE          0x40010800UL

#define GPIOA_CRL           (*(volatile uint32_t *)(GPIOA_BASE + 0x00UL))
#define GPIOA_CRH           (*(volatile uint32_t *)(GPIOA_BASE + 0x04UL))
#define GPIOA_IDR           (*(volatile uint32_t *)(GPIOA_BASE + 0x08UL))
#define GPIOA_ODR           (*(volatile uint32_t *)(GPIOA_BASE + 0x0CUL))
#define GPIOA_BSRR          (*(volatile uint32_t *)(GPIOA_BASE + 0x10UL))
#define GPIOA_BRR           (*(volatile uint32_t *)(GPIOA_BASE + 0x14UL))


/*GPIOB */

#define GPIOB_BASE          0x40010C00UL

#define GPIOB_CRL           (*(volatile uint32_t *)(GPIOB_BASE + 0x00UL))
#define GPIOB_CRH           (*(volatile uint32_t *)(GPIOB_BASE + 0x04UL))
#define GPIOB_IDR           (*(volatile uint32_t *)(GPIOB_BASE + 0x08UL))
#define GPIOB_ODR           (*(volatile uint32_t *)(GPIOB_BASE + 0x0CUL))
#define GPIOB_BSRR          (*(volatile uint32_t *)(GPIOB_BASE + 0x10UL))
#define GPIOB_BRR           (*(volatile uint32_t *)(GPIOB_BASE + 0x14UL))


/*I2C1*/

#define I2C1_BASE           0x40005400UL

#define I2C1_CR1            (*(volatile uint32_t *)(I2C1_BASE + 0x00UL))
#define I2C1_CR2            (*(volatile uint32_t *)(I2C1_BASE + 0x04UL))
#define I2C1_OAR1           (*(volatile uint32_t *)(I2C1_BASE + 0x08UL))
#define I2C1_OAR2           (*(volatile uint32_t *)(I2C1_BASE + 0x0CUL))
#define I2C1_DR             (*(volatile uint32_t *)(I2C1_BASE + 0x10UL))
#define I2C1_SR1            (*(volatile uint32_t *)(I2C1_BASE + 0x14UL))
#define I2C1_SR2            (*(volatile uint32_t *)(I2C1_BASE + 0x18UL))
#define I2C1_CCR            (*(volatile uint32_t *)(I2C1_BASE + 0x1CUL))
#define I2C1_TRISE          (*(volatile uint32_t *)(I2C1_BASE + 0x20UL))

/* I2C_CR1 bits */
#define I2C_CR1_PE          (1U << 0)
#define I2C_CR1_START       (1U << 8)
#define I2C_CR1_STOP        (1U << 9)
#define I2C_CR1_ACK         (1U << 10)
#define I2C_CR1_SWRST       (1U << 15)

/* I2C_SR1 bits */
#define I2C_SR1_SB          (1U << 0)
#define I2C_SR1_ADDR        (1U << 1)
#define I2C_SR1_BTF         (1U << 2)
#define I2C_SR1_RXNE        (1U << 6)
#define I2C_SR1_TXE         (1U << 7)

/* I2C_SR2 bits */
#define I2C_SR2_BUSY        (1U << 1)


/*SPI1 */

#define SPI1_BASE           0x40013000UL

#define SPI1_CR1            (*(volatile uint32_t *)(SPI1_BASE + 0x00UL))
#define SPI1_CR2            (*(volatile uint32_t *)(SPI1_BASE + 0x04UL))
#define SPI1_SR             (*(volatile uint32_t *)(SPI1_BASE + 0x08UL))
#define SPI1_DR             (*(volatile uint32_t *)(SPI1_BASE + 0x0CUL))

/* SPI1_CR1 bits */
#define SPI_CR1_CPHA        (1U << 0)
#define SPI_CR1_CPOL        (1U << 1)
#define SPI_CR1_MSTR        (1U << 2)

#define SPI_CR1_BR_DIV2     (0U << 3)
#define SPI_CR1_BR_DIV4     (1U << 3)
#define SPI_CR1_BR_DIV8     (2U << 3)
#define SPI_CR1_BR_DIV16    (3U << 3)
#define SPI_CR1_BR_DIV32    (4U << 3)
#define SPI_CR1_BR_DIV64    (5U << 3)
#define SPI_CR1_BR_DIV128   (6U << 3)
#define SPI_CR1_BR_DIV256   (7U << 3)

#define SPI_CR1_SPE         (1U << 6)
#define SPI_CR1_LSBFIRST    (1U << 7)
#define SPI_CR1_SSI         (1U << 8)
#define SPI_CR1_SSM         (1U << 9)
#define SPI_CR1_DFF         (1U << 11)

/* SPI1_SR bits */
#define SPI_SR_RXNE         (1U << 0)
#define SPI_SR_TXE          (1U << 1)
#define SPI_SR_BSY          (1U << 7)


/*IM2 - dùng PWM điều khiển Servo*/

#define TIM2_BASE           0x40000000UL

#define TIM2_CR1            (*(volatile uint32_t *)(TIM2_BASE + 0x00UL))
#define TIM2_EGR            (*(volatile uint32_t *)(TIM2_BASE + 0x14UL))
#define TIM2_CCMR1          (*(volatile uint32_t *)(TIM2_BASE + 0x18UL))
#define TIM2_CCER           (*(volatile uint32_t *)(TIM2_BASE + 0x20UL))
#define TIM2_PSC            (*(volatile uint32_t *)(TIM2_BASE + 0x28UL))
#define TIM2_ARR            (*(volatile uint32_t *)(TIM2_BASE + 0x2CUL))
#define TIM2_CCR1           (*(volatile uint32_t *)(TIM2_BASE + 0x34UL))


/*Cấu hình chân RC522 */

/*
 * RC522 dùng SPI1:
 *
 * PA4 → NSS/CS
 * PA5 → SCK
 * PA6 → MISO
 * PA7 → MOSI
 * PB0 → RST
 */

#define RC522_CS_PIN        4U
#define RC522_RST_PIN       0U

/* Kéo CS xuống mức 0 */
#define RC522_CS_LOW()      \
    (GPIOA_BSRR = (1U << (RC522_CS_PIN + 16U)))

/* Đưa CS lên mức 1 */
#define RC522_CS_HIGH()     \
    (GPIOA_BSRR = (1U << RC522_CS_PIN))

/* Kéo chân Reset xuống mức 0 */
#define RC522_RST_LOW()     \
    (GPIOB_BSRR = (1U << (RC522_RST_PIN + 16U)))

/* Đưa chân Reset lên mức 1 */
#define RC522_RST_HIGH()    \
    (GPIOB_BSRR = (1U << RC522_RST_PIN))

#endif /* STM32F103_REGS_H */