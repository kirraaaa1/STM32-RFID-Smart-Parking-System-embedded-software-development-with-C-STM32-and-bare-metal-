#include "rc522.h"
#include "spi1.h"
#include "systick.h"
#include "stm32f103_regs.h"

/* =========================================================
 * Thanh ghi MFRC522
 * ========================================================= */

#define RC522_COMMAND_REG           0x01U
#define RC522_COM_IRQ_REG           0x04U
#define RC522_DIV_IRQ_REG           0x05U
#define RC522_ERROR_REG             0x06U
#define RC522_FIFO_DATA_REG         0x09U
#define RC522_FIFO_LEVEL_REG        0x0AU
#define RC522_CONTROL_REG           0x0CU
#define RC522_BIT_FRAMING_REG       0x0DU
#define RC522_COLL_REG              0x0EU

#define RC522_MODE_REG              0x11U
#define RC522_TX_CONTROL_REG        0x14U
#define RC522_TX_ASK_REG            0x15U

#define RC522_CRC_RESULT_H_REG      0x21U
#define RC522_CRC_RESULT_L_REG      0x22U

#define RC522_T_MODE_REG            0x2AU
#define RC522_T_PRESCALER_REG       0x2BU
#define RC522_T_RELOAD_H_REG        0x2CU
#define RC522_T_RELOAD_L_REG        0x2DU

#define RC522_VERSION_REG           0x37U


/* =========================================================
 * Lệnh điều khiển MFRC522
 * ========================================================= */

#define RC522_CMD_IDLE              0x00U
#define RC522_CMD_CALCULATE_CRC     0x03U
#define RC522_CMD_TRANSCEIVE        0x0CU
#define RC522_CMD_SOFT_RESET        0x0FU


/* =========================================================
 * Lệnh gửi đến thẻ RFID
 * ========================================================= */

#define PICC_CMD_REQA               0x26U
#define PICC_CMD_ANTICOLL_CL1       0x93U
#define PICC_CMD_ANTICOLL_NVB       0x20U
#define PICC_CMD_HALT               0x50U


/* =========================================================
 * Thông số timeout
 * ========================================================= */

#define RC522_COMMUNICATION_TIMEOUT 3000U
#define RC522_CRC_TIMEOUT           5000U


/* =========================================================
 * Hàm nội bộ: ghi một thanh ghi RC522
 * ========================================================= */

static void RC522_WriteRegister(
    uint8_t register_address,
    uint8_t value)
{
    /*
     * Khi ghi:
     *
     * Bit 7 = 0
     * Bit 6:1 = địa chỉ thanh ghi
     * Bit 0 = 0
     */
    uint8_t spi_address =
        (uint8_t)((register_address << 1U) & 0x7EU);

    RC522_CS_LOW();

    SPI1_Transfer(spi_address);
    SPI1_Transfer(value);

    RC522_CS_HIGH();
}


/* =========================================================
 * Hàm nội bộ: đọc một thanh ghi RC522
 * ========================================================= */

static uint8_t RC522_ReadRegister(uint8_t register_address)
{
    /*
     * Khi đọc:
     *
     * Bit 7 = 1
     * Bit 6:1 = địa chỉ thanh ghi
     * Bit 0 = 0
     */
    uint8_t spi_address =
        (uint8_t)(
            ((register_address << 1U) & 0x7EU) |
            0x80U
        );

    RC522_CS_LOW();

    SPI1_Transfer(spi_address);

    uint8_t value = SPI1_Transfer(0x00U);

    RC522_CS_HIGH();

    return value;
}


/* =========================================================
 * Hàm nội bộ: đặt các bit trong thanh ghi
 * ========================================================= */

static void RC522_SetBits(
    uint8_t register_address,
    uint8_t mask)
{
    uint8_t value =
        RC522_ReadRegister(register_address);

    RC522_WriteRegister(
        register_address,
        (uint8_t)(value | mask)
    );
}


/* =========================================================
 * Hàm nội bộ: xóa các bit trong thanh ghi
 * ========================================================= */

static void RC522_ClearBits(
    uint8_t register_address,
    uint8_t mask)
{
    uint8_t value =
        RC522_ReadRegister(register_address);

    RC522_WriteRegister(
        register_address,
        (uint8_t)(value & (uint8_t)(~mask))
    );
}


/* =========================================================
 * Hàm nội bộ: truyền và nhận dữ liệu với thẻ
 * ========================================================= */

static bool RC522_Transceive(
    const uint8_t *send_data,
    uint8_t send_length,
    uint8_t *receive_data,
    uint8_t receive_capacity,
    uint16_t *received_bits)
{
    if ((send_data == 0) || (send_length == 0U))
    {
        return false;
    }

    uint32_t timeout =
        RC522_COMMUNICATION_TIMEOUT;

    /*
     * Đưa RC522 về trạng thái nghỉ.
     */
    RC522_WriteRegister(
        RC522_COMMAND_REG,
        RC522_CMD_IDLE
    );

    /*
     * Xóa các cờ ngắt trước đó.
     */
    RC522_WriteRegister(
        RC522_COM_IRQ_REG,
        0x7FU
    );

    /*
     * Xóa toàn bộ FIFO.
     *
     * Bit 7 FlushBuffer = 1.
     */
    RC522_SetBits(
        RC522_FIFO_LEVEL_REG,
        0x80U
    );

    /*
     * Ghi dữ liệu cần gửi vào FIFO.
     */
    for (uint8_t i = 0U; i < send_length; i++)
    {
        RC522_WriteRegister(
            RC522_FIFO_DATA_REG,
            send_data[i]
        );
    }

    /*
     * Bắt đầu lệnh truyền nhận.
     */
    RC522_WriteRegister(
        RC522_COMMAND_REG,
        RC522_CMD_TRANSCEIVE
    );

    /*
     * Bit 7 StartSend = 1.
     */
    RC522_SetBits(
        RC522_BIT_FRAMING_REG,
        0x80U
    );

    uint8_t irq_value = 0U;

    /*
     * Chờ:
     *
     * RxIRq   = bit 5
     * IdleIRq = bit 4
     * TimerIRq = bit 0
     */
    do
    {
        irq_value =
            RC522_ReadRegister(RC522_COM_IRQ_REG);

        /*
         * Timer hết hạn.
         */
        if ((irq_value & 0x01U) != 0U)
        {
            RC522_ClearBits(
                RC522_BIT_FRAMING_REG,
                0x80U
            );

            return false;
        }

        timeout--;
    }
    while (((irq_value & 0x30U) == 0U) &&
           (timeout > 0U));

    /*
     * Xóa StartSend.
     */
    RC522_ClearBits(
        RC522_BIT_FRAMING_REG,
        0x80U
    );

    if (timeout == 0U)
    {
        return false;
    }

    /*
     * Kiểm tra các lỗi:
     *
     * BufferOvfl
     * CollErr
     * CRCErr
     * ParityErr
     * ProtocolErr
     */
    uint8_t error =
        RC522_ReadRegister(RC522_ERROR_REG);

    if ((error & 0x1BU) != 0U)
    {
        return false;
    }

    /*
     * Đọc số byte hiện có trong FIFO.
     */
    uint8_t fifo_length =
        RC522_ReadRegister(RC522_FIFO_LEVEL_REG);

    /*
     * Số bit hợp lệ trong byte cuối cùng.
     */
    uint8_t last_bits =
        (uint8_t)(
            RC522_ReadRegister(RC522_CONTROL_REG) &
            0x07U
        );

    if (received_bits != 0)
    {
        if ((fifo_length > 0U) &&
            (last_bits != 0U))
        {
            *received_bits =
                ((uint16_t)(fifo_length - 1U) * 8U) +
                last_bits;
        }
        else
        {
            *received_bits =
                (uint16_t)fifo_length * 8U;
        }
    }

    /*
     * Không đọc vượt kích thước buffer.
     */
    uint8_t read_length = fifo_length;

    if (read_length > receive_capacity)
    {
        read_length = receive_capacity;
    }

    if (receive_data != 0)
    {
        for (uint8_t i = 0U; i < read_length; i++)
        {
            receive_data[i] =
                RC522_ReadRegister(
                    RC522_FIFO_DATA_REG
                );
        }
    }

    return true;
}


/* =========================================================
 * Hàm nội bộ: tính CRC bằng phần cứng RC522
 * ========================================================= */

static bool RC522_CalculateCRC(
    const uint8_t *data,
    uint8_t length,
    uint8_t result[2])
{
    if ((data == 0) ||
        (length == 0U) ||
        (result == 0))
    {
        return false;
    }

    uint32_t timeout = RC522_CRC_TIMEOUT;

    /*
     * Dừng lệnh hiện tại.
     */
    RC522_WriteRegister(
        RC522_COMMAND_REG,
        RC522_CMD_IDLE
    );

    /*
     * Xóa cờ CRCIRq.
     */
    RC522_ClearBits(
        RC522_DIV_IRQ_REG,
        0x04U
    );

    /*
     * Xóa FIFO.
     */
    RC522_SetBits(
        RC522_FIFO_LEVEL_REG,
        0x80U
    );

    /*
     * Đưa dữ liệu cần tính CRC vào FIFO.
     */
    for (uint8_t i = 0U; i < length; i++)
    {
        RC522_WriteRegister(
            RC522_FIFO_DATA_REG,
            data[i]
        );
    }

    /*
     * Bắt đầu tính CRC.
     */
    RC522_WriteRegister(
        RC522_COMMAND_REG,
        RC522_CMD_CALCULATE_CRC
    );

    /*
     * Chờ CRCIRq = 1.
     */
    while (timeout > 0U)
    {
        uint8_t irq =
            RC522_ReadRegister(RC522_DIV_IRQ_REG);

        if ((irq & 0x04U) != 0U)
        {
            break;
        }

        timeout--;
    }

    /*
     * Trở về Idle.
     */
    RC522_WriteRegister(
        RC522_COMMAND_REG,
        RC522_CMD_IDLE
    );

    if (timeout == 0U)
    {
        return false;
    }

    /*
     * CRC được gửi theo thứ tự Low byte trước,
     * High byte sau.
     */
    result[0] =
        RC522_ReadRegister(
            RC522_CRC_RESULT_L_REG
        );

    result[1] =
        RC522_ReadRegister(
            RC522_CRC_RESULT_H_REG
        );

    return true;
}


/* =========================================================
 * Reset RC522 bằng phần mềm
 * ========================================================= */

void RC522_SoftReset(void)
{
    RC522_WriteRegister(
        RC522_COMMAND_REG,
        RC522_CMD_SOFT_RESET
    );

    Delay_ms(50U);
}


/* =========================================================
 * Bật anten RC522
 * ========================================================= */

void RC522_AntennaOn(void)
{
    uint8_t value =
        RC522_ReadRegister(
            RC522_TX_CONTROL_REG
        );

    if ((value & 0x03U) != 0x03U)
    {
        RC522_SetBits(
            RC522_TX_CONTROL_REG,
            0x03U
        );
    }
}


/* =========================================================
 * Tắt anten RC522
 * ========================================================= */

void RC522_AntennaOff(void)
{
    RC522_ClearBits(
        RC522_TX_CONTROL_REG,
        0x03U
    );
}


/* =========================================================
 * Khởi tạo RC522
 * ========================================================= */

void RC522_Init(void)
{
    /*
     * Bật clock GPIOB vì chân RST dùng PB0.
     */
    RCC_APB2ENR |= RCC_APB2ENR_IOPBEN;

    /*
     * Xóa cấu hình cũ của PB0.
     */
    GPIOB_CRL &= ~(0xFU << 0);

    /*
     * PB0:
     *
     * Output Push-pull
     * Tốc độ 2 MHz
     *
     * MODE = 10
     * CNF  = 00
     * Nibble = 0x2
     */
    GPIOB_CRL |= (0x2U << 0);

    /*
     * Reset phần cứng RC522.
     */
    RC522_RST_LOW();
    Delay_ms(5U);

    RC522_RST_HIGH();
    Delay_ms(50U);

    /*
     * Reset phần mềm.
     */
    RC522_SoftReset();

    /*
     * Cấu hình timer nội bộ của RC522.
     */
    RC522_WriteRegister(
        RC522_T_MODE_REG,
        0x8DU
    );

    RC522_WriteRegister(
        RC522_T_PRESCALER_REG,
        0x3EU
    );

    RC522_WriteRegister(
        RC522_T_RELOAD_L_REG,
        30U
    );

    RC522_WriteRegister(
        RC522_T_RELOAD_H_REG,
        0U
    );

    /*
     * Cấu hình điều chế tín hiệu.
     */
    RC522_WriteRegister(
        RC522_TX_ASK_REG,
        0x40U
    );

    RC522_WriteRegister(
        RC522_MODE_REG,
        0x3DU
    );

    /*
     * Không gửi bit chưa hoàn chỉnh lúc khởi tạo.
     */
    RC522_WriteRegister(
        RC522_BIT_FRAMING_REG,
        0x00U
    );

    /*
     * Bật anten.
     */
    RC522_AntennaOn();
}


/* =========================================================
 * Đọc phiên bản IC MFRC522
 * ========================================================= */

uint8_t RC522_GetVersion(void)
{
    return RC522_ReadRegister(
        RC522_VERSION_REG
    );
}


/* =========================================================
 * Kiểm tra có thẻ ở gần đầu đọc không
 * ========================================================= */

bool RC522_IsCardPresent(void)
{
    uint8_t command = PICC_CMD_REQA;

    uint8_t response[2] = {0U, 0U};

    uint16_t response_bits = 0U;

    /*
     * Lệnh REQA chỉ gửi 7 bit.
     *
     * TxLastBits = 7.
     */
    RC522_WriteRegister(
        RC522_BIT_FRAMING_REG,
        0x07U
    );

    bool success = RC522_Transceive(
        &command,
        1U,
        response,
        sizeof(response),
        &response_bits
    );

    /*
     * Khôi phục truyền đủ 8 bit.
     */
    RC522_WriteRegister(
        RC522_BIT_FRAMING_REG,
        0x00U
    );

    /*
     * Thẻ phải trả về ATQA dài 2 byte = 16 bit.
     */
    return success && (response_bits == 16U);
}


/* =========================================================
 * Đọc UID 4 byte của thẻ
 * ========================================================= */

bool RC522_ReadUID(RC522_UID_t *uid)
{
    if (uid == 0)
    {
        return false;
    }

    /*
     * Anticollision Cascade Level 1:
     *
     * 0x93 0x20
     */
    uint8_t command[2] =
    {
        PICC_CMD_ANTICOLL_CL1,
        PICC_CMD_ANTICOLL_NVB
    };

    /*
     * Phản hồi:
     *
     * response[0] = UID byte 0
     * response[1] = UID byte 1
     * response[2] = UID byte 2
     * response[3] = UID byte 3
     * response[4] = BCC
     */
    uint8_t response[5] =
    {
        0U, 0U, 0U, 0U, 0U
    };

    uint16_t response_bits = 0U;

    /*
     * Gửi đầy đủ từng byte.
     */
    RC522_WriteRegister(
        RC522_BIT_FRAMING_REG,
        0x00U
    );

    /*
     * ValuesAfterColl = 0.
     *
     * Không giữ lại các bit sau khi xảy ra collision.
     */
    RC522_ClearBits(
        RC522_COLL_REG,
        0x80U
    );

    bool success = RC522_Transceive(
        command,
        sizeof(command),
        response,
        sizeof(response),
        &response_bits
    );

    /*
     * UID 4 byte + BCC 1 byte = 40 bit.
     */
    if ((!success) ||
        (response_bits != 40U))
    {
        return false;
    }

    /*
     * Kiểm tra BCC.
     */
    uint8_t calculated_bcc =
        response[0] ^
        response[1] ^
        response[2] ^
        response[3];

    if (calculated_bcc != response[4])
    {
        return false;
    }

    /*
     * Lưu UID.
     */
    uid->length = 4U;

    for (uint8_t i = 0U; i < 4U; i++)
    {
        uid->data[i] = response[i];
    }

    return true;
}


/* =========================================================
 * So sánh hai UID
 * ========================================================= */

bool RC522_UIDCompare(
    const RC522_UID_t *uid1,
    const RC522_UID_t *uid2)
{
    if ((uid1 == 0) || (uid2 == 0))
    {
        return false;
    }

    if (uid1->length != uid2->length)
    {
        return false;
    }

    for (uint8_t i = 0U;
         i < uid1->length;
         i++)
    {
        if (uid1->data[i] != uid2->data[i])
        {
            return false;
        }
    }

    return true;
}


/* =========================================================
 * Đưa thẻ về trạng thái HALT
 * ========================================================= */

void RC522_Halt(void)
{
    /*
     * Lệnh HALT:
     *
     * Byte 0 = 0x50
     * Byte 1 = 0x00
     * Byte 2 = CRC Low
     * Byte 3 = CRC High
     */
    uint8_t buffer[4];

    buffer[0] = PICC_CMD_HALT;
    buffer[1] = 0x00U;

    uint8_t crc[2];

    if (!RC522_CalculateCRC(
            buffer,
            2U,
            crc))
    {
        return;
    }

    buffer[2] = crc[0];
    buffer[3] = crc[1];

    uint16_t received_bits = 0U;

    RC522_WriteRegister(
        RC522_BIT_FRAMING_REG,
        0x00U
    );

    /*
     * Thẻ hợp lệ thường không phản hồi lệnh HALT.
     * Do đó bỏ qua kết quả trả về.
     */
    (void)RC522_Transceive(
        buffer,
        sizeof(buffer),
        0,
        0U,
        &received_bits
    );
}