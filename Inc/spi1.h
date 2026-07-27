#ifndef SPI1_H
#define SPI1_H

#include <stdint.h>

/*
 * Khởi tạo SPI1 để giao tiếp với RC522.
 *
 * Chân mặc định:
 * PA5 → SCK
 * PA6 → MISO
 * PA7 → MOSI
 *
 * Chế độ:
 * Master
 * SPI Mode 0
 * 8-bit
 * MSB first
 */
void SPI1_Init(void);

/*
 * Gửi một byte và đồng thời nhận một byte qua SPI.
 *
 * SPI truyền và nhận đồng thời:
 * - data: byte gửi đi
 * - return: byte nhận về
 */
uint8_t SPI1_Transfer(uint8_t data);

/*
 * Truyền nhiều byte.
 *
 * tx_buffer có thể bằng NULL nếu chỉ muốn nhận.
 * rx_buffer có thể bằng NULL nếu chỉ muốn gửi.
 */
void SPI1_TransferBuffer(
    const uint8_t *tx_buffer,
    uint8_t *rx_buffer,
    uint32_t length
);

#endif /* SPI1_H */