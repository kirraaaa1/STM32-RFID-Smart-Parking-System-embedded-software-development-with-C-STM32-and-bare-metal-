#ifndef I2C1_H
#define I2C1_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Khởi tạo I2C1.
 *
 * Chân mặc định:
 * PB6 → SCL
 * PB7 → SDA
 *
 * pclk1_hz:
 * Tần số clock cấp cho I2C1.
 *
 * bus_speed_hz:
 * Tốc độ I2C mong muốn, thường là 100000 Hz.
 *
 * Ví dụ:
 * I2C1_Init(8000000UL, 100000UL);
 */
void I2C1_Init(
    uint32_t pclk1_hz,
    uint32_t bus_speed_hz
);

/*
 * Gửi một byte đến thiết bị I2C.
 *
 * device_address là địa chỉ 7-bit.
 *
 * Ví dụ LCD:
 * 0x27 hoặc 0x3F
 */
bool I2C1_WriteByte(
    uint8_t device_address,
    uint8_t data
);

/*
 * Gửi nhiều byte đến thiết bị I2C.
 */
bool I2C1_Write(
    uint8_t device_address,
    const uint8_t *data,
    uint32_t length
);

/*
 * Đọc một byte từ thiết bị I2C.
 */
bool I2C1_ReadByte(
    uint8_t device_address,
    uint8_t *data
);

/*
 * Kiểm tra bus I2C có đang bận hay không.
 */
bool I2C1_IsBusy(void);

#endif /* I2C1_H */