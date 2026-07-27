#include "i2c1.h"
#include "stm32f103_regs.h"

#define I2C_TIMEOUT    100000U

static bool I2C1_WaitForFlag(
    volatile uint32_t *status_register,
    uint32_t flag)
{
    uint32_t timeout = I2C_TIMEOUT;

    while (((*status_register) & flag) == 0U)
    {
        if (timeout == 0U)
        {
            return false;
        }

        timeout--;
    }

    return true;
}

void I2C1_Init(
    uint32_t pclk1_hz,
    uint32_t bus_speed_hz)
{
    RCC_APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC_APB1ENR |= RCC_APB1ENR_I2C1EN;

    /*
     * PB6: I2C1 SCL
     * PB7: I2C1 SDA
     *
     * Alternate Function Open-drain 50 MHz:
     * MODE = 11
     * CNF  = 11
     * Nibble = 0xF
     */
    GPIOB_CRL &= ~(
        (0xFU << 24) |
        (0xFU << 28)
    );

    GPIOB_CRL |=
        (0xFU << 24) |
        (0xFU << 28);

    /* Reset I2C1 */
    I2C1_CR1 |= I2C_CR1_SWRST;
    I2C1_CR1 &= ~I2C_CR1_SWRST;

    uint32_t pclk1_mhz = pclk1_hz / 1000000U;

    /*
     * CR2 chứa tần số APB1 theo MHz.
     */
    I2C1_CR2 = pclk1_mhz;

    /*
     * Standard Mode:
     * CCR = PCLK1 / (2 × tốc độ I2C)
     */
    I2C1_CCR = pclk1_hz / (2U * bus_speed_hz);

    /*
     * Standard Mode:
     * TRISE = PCLK1 MHz + 1
     */
    I2C1_TRISE = pclk1_mhz + 1U;

    /* Bật I2C1 */
    I2C1_CR1 |= I2C_CR1_PE;
}

bool I2C1_IsBusy(void)
{
    return (I2C1_SR2 & I2C_SR2_BUSY) != 0U;
}

bool I2C1_WriteByte(
    uint8_t device_address,
    uint8_t data)
{
    uint32_t timeout = I2C_TIMEOUT;

    while (I2C1_IsBusy())
    {
        if (timeout == 0U)
        {
            return false;
        }

        timeout--;
    }

    /* Tạo START */
    I2C1_CR1 |= I2C_CR1_START;

    if (!I2C1_WaitForFlag(&I2C1_SR1, I2C_SR1_SB))
    {
        return false;
    }

    /*
     * Gửi địa chỉ 7-bit và bit Write = 0.
     */
    I2C1_DR = ((uint32_t)device_address << 1U);

    if (!I2C1_WaitForFlag(&I2C1_SR1, I2C_SR1_ADDR))
    {
        I2C1_CR1 |= I2C_CR1_STOP;
        return false;
    }

    /*
     * Đọc SR1 rồi SR2 để xóa cờ ADDR.
     */
    volatile uint32_t temporary;

    temporary = I2C1_SR1;
    temporary = I2C1_SR2;
    (void)temporary;

    if (!I2C1_WaitForFlag(&I2C1_SR1, I2C_SR1_TXE))
    {
        I2C1_CR1 |= I2C_CR1_STOP;
        return false;
    }

    I2C1_DR = data;

    if (!I2C1_WaitForFlag(&I2C1_SR1, I2C_SR1_BTF))
    {
        I2C1_CR1 |= I2C_CR1_STOP;
        return false;
    }

    /* Tạo STOP */
    I2C1_CR1 |= I2C_CR1_STOP;

    return true;
}

bool I2C1_Write(
    uint8_t device_address,
    const uint8_t *data,
    uint32_t length)
{
    if ((data == 0) || (length == 0U))
    {
        return false;
    }

    for (uint32_t i = 0U; i < length; i++)
    {
        if (!I2C1_WriteByte(device_address, data[i]))
        {
            return false;
        }
    }

    return true;
}

bool I2C1_ReadByte(
    uint8_t device_address,
    uint8_t *data)
{
    /*
     * Chưa cần cho LCD vì LCD chỉ nhận dữ liệu.
     */
    (void)device_address;
    (void)data;

    return false;
}