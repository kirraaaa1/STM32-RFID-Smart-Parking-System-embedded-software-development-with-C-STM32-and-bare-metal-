#include "spi1.h"
#include "stm32f103_regs.h"

void SPI1_Init(void)
{
    /*
     * Bật clock GPIOA và SPI1.
     */
    RCC_APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC_APB2ENR |= RCC_APB2ENR_SPI1EN;

    /*
     * Xóa cấu hình PA4, PA5, PA6, PA7.
     */
    GPIOA_CRL &= ~(
        (0xFU << 16) |
        (0xFU << 20) |
        (0xFU << 24) |
        (0xFU << 28)
    );

    /*
     * PA4: GPIO output push-pull 2 MHz.
     * Dùng làm CS của RC522.
     */
    GPIOA_CRL |= (0x2U << 16);

    /*
     * PA5: Alternate Function push-pull 50 MHz.
     * SPI1 SCK.
     */
    GPIOA_CRL |= (0xBU << 20);

    /*
     * PA6: Input floating.
     * SPI1 MISO.
     */
    GPIOA_CRL |= (0x4U << 24);

    /*
     * PA7: Alternate Function push-pull 50 MHz.
     * SPI1 MOSI.
     */
    GPIOA_CRL |= (0xBU << 28);

    /* Không chọn RC522 lúc ban đầu */
    RC522_CS_HIGH();

    /*
     * SPI Mode 0:
     * CPOL = 0
     * CPHA = 0
     *
     * Master
     * Clock chia 8
     * Software NSS
     * Dữ liệu 8-bit
     * MSB trước
     */
    SPI1_CR1 =
        SPI_CR1_MSTR |
        SPI_CR1_BR_DIV8 |
        SPI_CR1_SSM |
        SPI_CR1_SSI;

    SPI1_CR1 |= SPI_CR1_SPE;
}

uint8_t SPI1_Transfer(uint8_t data)
{
    /* Chờ bộ đệm truyền trống */
    while ((SPI1_SR & SPI_SR_TXE) == 0U)
    {
    }

    /*
     * Ghi 8-bit vào thanh ghi SPI.
     * Ép con trỏ uint8_t để tránh truyền 16-bit.
     */
    *(volatile uint8_t *)&SPI1_DR = data;

    /* Chờ nhận được dữ liệu */
    while ((SPI1_SR & SPI_SR_RXNE) == 0U)
    {
    }

    return *(volatile uint8_t *)&SPI1_DR;
}

void SPI1_TransferBuffer(
    const uint8_t *tx_buffer,
    uint8_t *rx_buffer,
    uint32_t length)
{
    for (uint32_t i = 0U; i < length; i++)
    {
        uint8_t transmit_data = 0xFFU;

        if (tx_buffer != 0)
        {
            transmit_data = tx_buffer[i];
        }

        uint8_t received_data = SPI1_Transfer(transmit_data);

        if (rx_buffer != 0)
        {
            rx_buffer[i] = received_data;
        }
    }
}