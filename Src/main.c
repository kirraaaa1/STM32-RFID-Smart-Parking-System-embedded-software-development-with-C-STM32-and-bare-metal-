#include <stdint.h>
#include <stdbool.h>

#include "systick.h"
#include "spi1.h"
#include "rc522.h"
#include "i2c1.h"
#include "lcd_i2c.h"
#include "servo.h"

/* =========================================================
 * Cấu hình hệ thống
 * ========================================================= */

#define CPU_CLOCK_HZ             8000000UL
#define I2C_BUS_SPEED_HZ         100000UL

#define PARKING_TOTAL_SLOTS      4U

#define GATE_OPEN_ANGLE          90U
#define GATE_CLOSE_ANGLE         0U

#define GATE_OPEN_TIME_MS        3000U
#define MESSAGE_TIME_MS          1500U
#define CARD_DEBOUNCE_TIME_MS    1000U


/* =========================================================
 * Thông tin thẻ đã đăng ký
 * ========================================================= */

typedef struct
{
    RC522_UID_t uid;

    /*
     * false: xe đang ở ngoài bãi
     * true : xe đang ở trong bãi
     */
    bool is_inside;

} ParkingCard_t;


/*
 * Trong bản demo này chỉ đăng ký một thẻ.
 */
static ParkingCard_t registered_card;

/*
 * false: chưa có thẻ nào được đăng ký
 * true : đã có một thẻ được đăng ký
 */
static bool card_registered = false;

/*
 * Số xe hiện có trong bãi.
 */
static uint8_t occupied_slots = 0U;


/* =========================================================
 * Hiển thị một số HEX 4-bit
 * ========================================================= */

static void LCD_WriteHexNibble(uint8_t value)
{
    value &= 0x0FU;

    if (value < 10U)
    {
        LCD_WriteChar((char)('0' + value));
    }
    else
    {
        LCD_WriteChar(
            (char)('A' + value - 10U)
        );
    }
}


/* =========================================================
 * Hiển thị một byte dưới dạng HEX
 *
 * Ví dụ:
 * 0x92 → "92"
 * ========================================================= */

static void LCD_WriteHexByte(uint8_t value)
{
    LCD_WriteHexNibble(
        (uint8_t)(value >> 4U)
    );

    LCD_WriteHexNibble(value);
}


/* =========================================================
 * Tính số chỗ còn trống
 * ========================================================= */

static uint8_t Parking_GetFreeSlots(void)
{
    if (occupied_slots >= PARKING_TOTAL_SLOTS)
    {
        return 0U;
    }

    return (uint8_t)(
        PARKING_TOTAL_SLOTS - occupied_slots
    );
}


/* =========================================================
 * Hiển thị trạng thái bãi xe
 * ========================================================= */

static void LCD_DisplayParkingStatus(void)
{
    LCD_Clear();

    LCD_SetCursor(0U, 0U);
    LCD_WriteString("BAI GIU XE");

    LCD_SetCursor(1U, 0U);
    LCD_WriteString("CON TRONG: ");

    LCD_WriteNumber(
        Parking_GetFreeSlots()
    );
}


/* =========================================================
 * Hiển thị UID vừa đọc
 * ========================================================= */

static void LCD_DisplayUID(const RC522_UID_t *uid)
{
    if (uid == 0)
    {
        return;
    }

    LCD_Clear();

    LCD_SetCursor(0U, 0U);
    LCD_WriteString("UID THE:");

    LCD_SetCursor(1U, 0U);

    for (uint8_t i = 0U;
         i < uid->length;
         i++)
    {
        LCD_WriteHexByte(uid->data[i]);

        if (i < (uid->length - 1U))
        {
            LCD_WriteChar(' ');
        }
    }
}


/* =========================================================
 * Sao chép UID
 * ========================================================= */

static bool Parking_CopyUID(
    RC522_UID_t *destination,
    const RC522_UID_t *source)
{
    if ((destination == 0) ||
        (source == 0))
    {
        return false;
    }

    if ((source->length == 0U) ||
        (source->length > RC522_UID_MAX_LENGTH))
    {
        return false;
    }

    destination->length = source->length;

    for (uint8_t i = 0U;
         i < source->length;
         i++)
    {
        destination->data[i] =
            source->data[i];
    }

    return true;
}


/* =========================================================
 * Đăng ký thẻ đầu tiên
 * ========================================================= */

static bool Parking_RegisterFirstCard(
    const RC522_UID_t *uid)
{
    if (!Parking_CopyUID(
            &registered_card.uid,
            uid))
    {
        return false;
    }

    registered_card.is_inside = false;
    card_registered = true;

    LCD_Clear();

    LCD_SetCursor(0U, 0U);
    LCD_WriteString("DA DANG KY THE");

    LCD_SetCursor(1U, 0U);
    LCD_WriteString("THE HOP LE");

    Delay_ms(MESSAGE_TIME_MS);

    return true;
}


/* =========================================================
 * Kiểm tra UID có phải thẻ đã đăng ký không
 * ========================================================= */

static bool Parking_IsAuthorizedCard(
    const RC522_UID_t *uid)
{
    if (!card_registered)
    {
        return false;
    }

    return RC522_UIDCompare(
        uid,
        &registered_card.uid
    );
}


/* =========================================================
 * Mở servo 90 độ rồi đóng về 0 độ
 * ========================================================= */

static void Gate_OpenAndClose(void)
{
    /*
     * Mở thanh chắn.
     */
    Servo_SetAngle(GATE_OPEN_ANGLE);

    /*
     * Giữ cổng mở trong 3 giây.
     */
    Delay_ms(GATE_OPEN_TIME_MS);

    /*
     * Đóng thanh chắn.
     */
    Servo_SetAngle(GATE_CLOSE_ANGLE);

    /*
     * Chờ servo quay về vị trí đóng.
     */
    Delay_ms(1000U);
}


/* =========================================================
 * Xử lý xe vào
 * ========================================================= */

static void Parking_ProcessEntry(void)
{
    /*
     * Không cho xe vào nếu bãi đã đầy.
     */
    if (occupied_slots >= PARKING_TOTAL_SLOTS)
    {
        LCD_Clear();

        LCD_SetCursor(0U, 0U);
        LCD_WriteString("BAI DA DAY");

        LCD_SetCursor(1U, 0U);
        LCD_WriteString("KHONG THE VAO");

        Delay_ms(2000U);

        return;
    }

    registered_card.is_inside = true;
    occupied_slots++;

    LCD_Clear();

    LCD_SetCursor(0U, 0U);
    LCD_WriteString("THE HOP LE");

    LCD_SetCursor(1U, 0U);
    LCD_WriteString("MOI XE VAO");

    Gate_OpenAndClose();
}


/* =========================================================
 * Xử lý xe ra
 * ========================================================= */

static void Parking_ProcessExit(void)
{
    registered_card.is_inside = false;

    if (occupied_slots > 0U)
    {
        occupied_slots--;
    }

    LCD_Clear();

    LCD_SetCursor(0U, 0U);
    LCD_WriteString("THE HOP LE");

    LCD_SetCursor(1U, 0U);
    LCD_WriteString("MOI XE RA");

    Gate_OpenAndClose();
}


/* =========================================================
 * Xử lý UID vừa đọc
 * ========================================================= */

static void Parking_ProcessCard(
    const RC522_UID_t *detected_uid)
{
    if (detected_uid == 0)
    {
        return;
    }

    /*
     * Chưa có thẻ đăng ký:
     * lấy thẻ đầu tiên làm thẻ hợp lệ.
     */
    if (!card_registered)
    {
        if (!Parking_RegisterFirstCard(
                detected_uid))
        {
            LCD_Clear();

            LCD_SetCursor(0U, 0U);
            LCD_WriteString("LOI DANG KY THE");

            Delay_ms(MESSAGE_TIME_MS);

            return;
        }

        /*
         * Sau khi đăng ký, lần quét đầu tiên
         * được xử lý luôn là xe vào.
         */
        Parking_ProcessEntry();

        return;
    }

    /*
     * Đã có thẻ đăng ký nhưng UID không trùng.
     */
    if (!Parking_IsAuthorizedCard(
            detected_uid))
    {
        LCD_Clear();

        LCD_SetCursor(0U, 0U);
        LCD_WriteString("THE KHONG HOPLE");

        LCD_SetCursor(1U, 0U);
        LCD_WriteString("TU CHOI MO CONG");

        Delay_ms(2000U);

        return;
    }

    /*
     * Thẻ đúng và xe đang ở ngoài:
     * xử lý xe vào.
     */
    if (!registered_card.is_inside)
    {
        Parking_ProcessEntry();
    }
    else
    {
        /*
         * Thẻ đúng và xe đang trong bãi:
         * xử lý xe ra.
         */
        Parking_ProcessExit();
    }
}


/* =========================================================
 * Kiểm tra kết nối RC522
 * ========================================================= */

static bool RC522_CheckConnection(void)
{
    uint8_t version =
        RC522_GetVersion();

    LCD_Clear();

    LCD_SetCursor(0U, 0U);
    LCD_WriteString("RC522 VERSION");

    LCD_SetCursor(1U, 0U);
    LCD_WriteString("0x");

    LCD_WriteHexByte(version);

    Delay_ms(MESSAGE_TIME_MS);

    /*
     * 0x91: MFRC522 Version 1
     * 0x92: MFRC522 Version 2
     * 0x88: một số module clone
     */
    return (
        (version == 0x91U) ||
        (version == 0x92U) ||
        (version == 0x88U)
    );
}


/* =========================================================
 * MAIN
 * ========================================================= */

int main(void)
{
    /*
     * Khởi tạo SysTick 1 ms.
     */
    SysTick_Init(CPU_CLOCK_HZ);

    /*
     * Khởi tạo LCD I2C.
     */
    I2C1_Init(
        CPU_CLOCK_HZ,
        I2C_BUS_SPEED_HZ
    );

    LCD_Init();

    LCD_Clear();

    LCD_SetCursor(0U, 0U);
    LCD_WriteString("DANG KHOI DONG");

    /*
     * Khởi tạo servo trên PA0 - TIM2_CH1.
     */
    Servo_Init(CPU_CLOCK_HZ);

    /*
     * Đóng cổng ban đầu.
     */
    Servo_SetAngle(GATE_CLOSE_ANGLE);

    /*
     * Khởi tạo SPI1 và RC522.
     */
    SPI1_Init();
    RC522_Init();

    Delay_ms(500U);

    /*
     * Kiểm tra RC522.
     */
    if (!RC522_CheckConnection())
    {
        LCD_Clear();

        LCD_SetCursor(0U, 0U);
        LCD_WriteString("LOI RC522");

        LCD_SetCursor(1U, 0U);
        LCD_WriteString("KIEM TRA DAY");

        while (1)
        {
            /*
             * Dừng tại đây khi RC522 lỗi.
             */
        }
    }

    /*
     * Trạng thái ban đầu.
     */
    card_registered = false;
    occupied_slots = 0U;

    LCD_Clear();

    LCD_SetCursor(0U, 0U);
    LCD_WriteString("QUET THE DAU");

    LCD_SetCursor(1U, 0U);
    LCD_WriteString("DE DANG KY");

    while (1)
    {
        RC522_UID_t detected_uid;

        /*
         * Chưa có thẻ thì tiếp tục chờ.
         */
        if (!RC522_IsCardPresent())
        {
            continue;
        }

        /*
         * Có thẻ nhưng đọc UID thất bại.
         */
        if (!RC522_ReadUID(
                &detected_uid))
        {
            LCD_Clear();

            LCD_SetCursor(0U, 0U);
            LCD_WriteString("LOI DOC THE");

            Delay_ms(MESSAGE_TIME_MS);

            if (!card_registered)
            {
                LCD_Clear();

                LCD_SetCursor(0U, 0U);
                LCD_WriteString("QUET THE DAU");

                LCD_SetCursor(1U, 0U);
                LCD_WriteString("DE DANG KY");
            }
            else
            {
                LCD_DisplayParkingStatus();
            }

            continue;
        }

        /*
         * Hiển thị UID vừa đọc.
         */
        LCD_DisplayUID(&detected_uid);

        Delay_ms(1000U);

        /*
         * Đăng ký hoặc xử lý xe vào/ra.
         */
        Parking_ProcessCard(
            &detected_uid
        );

        /*
         * Đưa thẻ về trạng thái Halt.
         */
        RC522_Halt();

        /*
         * Cập nhật số chỗ còn trống.
         */
        LCD_DisplayParkingStatus();

        /*
         * Chống đọc thẻ liên tục.
         */
        Delay_ms(CARD_DEBOUNCE_TIME_MS);
    }
}