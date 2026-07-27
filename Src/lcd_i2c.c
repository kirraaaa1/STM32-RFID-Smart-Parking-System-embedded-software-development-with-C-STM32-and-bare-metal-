#include "lcd_i2c.h"
#include "i2c1.h"
#include "systick.h"

#define LCD_RS             (1U << 0)
#define LCD_RW             (1U << 1)
#define LCD_EN             (1U << 2)
#define LCD_BACKLIGHT      (1U << 3)

static uint8_t lcd_backlight = LCD_BACKLIGHT;

static void LCD_ExpanderWrite(uint8_t data)
{
    I2C1_WriteByte(
        LCD_I2C_ADDRESS,
        data | lcd_backlight
    );
}

static void LCD_PulseEnable(uint8_t data)
{
    LCD_ExpanderWrite(data | LCD_EN);
    Delay_ms(1U);

    LCD_ExpanderWrite(data & (uint8_t)(~LCD_EN));
    Delay_ms(1U);
}

static void LCD_Write4Bits(uint8_t data)
{
    LCD_ExpanderWrite(data);
    LCD_PulseEnable(data);
}

static void LCD_Send(uint8_t value, uint8_t mode)
{
    uint8_t high_nibble = value & 0xF0U;
    uint8_t low_nibble = (value << 4U) & 0xF0U;

    LCD_Write4Bits(high_nibble | mode);
    LCD_Write4Bits(low_nibble | mode);
}

static void LCD_Command(uint8_t command)
{
    LCD_Send(command, 0U);

    if ((command == 0x01U) || (command == 0x02U))
    {
        Delay_ms(2U);
    }
}

void LCD_Init(void)
{
    Delay_ms(50U);

    /*
     * Chuỗi khởi tạo LCD ở chế độ 4-bit.
     */
    LCD_Write4Bits(0x30U);
    Delay_ms(5U);

    LCD_Write4Bits(0x30U);
    Delay_ms(5U);

    LCD_Write4Bits(0x30U);
    Delay_ms(1U);

    LCD_Write4Bits(0x20U);
    Delay_ms(1U);

    /* 4-bit, 2 dòng, font 5x8 */
    LCD_Command(0x28U);

    /* Bật hiển thị, tắt con trỏ */
    LCD_Command(0x0CU);

    /* Xóa LCD */
    LCD_Command(0x01U);

    /* Tự tăng địa chỉ con trỏ */
    LCD_Command(0x06U);
}

void LCD_Clear(void)
{
    LCD_Command(0x01U);
}

void LCD_Home(void)
{
    LCD_Command(0x02U);
}

void LCD_SetCursor(
    uint8_t row,
    uint8_t column)
{
    uint8_t address;

    if (row == 0U)
    {
        address = column;
    }
    else
    {
        address = 0x40U + column;
    }

    LCD_Command(0x80U | address);
}

void LCD_WriteChar(char character)
{
    LCD_Send((uint8_t)character, LCD_RS);
}

void LCD_WriteString(const char *text)
{
    if (text == 0)
    {
        return;
    }

    while (*text != '\0')
    {
        LCD_WriteChar(*text);
        text++;
    }
}

void LCD_WriteNumber(uint32_t number)
{
    char buffer[11];
    uint32_t index = 0U;

    if (number == 0U)
    {
        LCD_WriteChar('0');
        return;
    }

    while ((number > 0U) && (index < sizeof(buffer)))
    {
        buffer[index] = (char)('0' + (number % 10U));
        number /= 10U;
        index++;
    }

    while (index > 0U)
    {
        index--;
        LCD_WriteChar(buffer[index]);
    }
}

void LCD_WriteSignedNumber(int32_t number)
{
    if (number < 0)
    {
        LCD_WriteChar('-');

        /*
         * Chuyển sang số dương bằng kiểu rộng hơn
         * để tránh lỗi với INT32_MIN.
         */
        LCD_WriteNumber((uint32_t)(-(int64_t)number));
    }
    else
    {
        LCD_WriteNumber((uint32_t)number);
    }
}

void LCD_BacklightOn(void)
{
    lcd_backlight = LCD_BACKLIGHT;
    LCD_ExpanderWrite(0U);
}

void LCD_BacklightOff(void)
{
    lcd_backlight = 0U;
    LCD_ExpanderWrite(0U);
}

void LCD_DisplayOn(void)
{
    LCD_Command(0x0CU);
}

void LCD_DisplayOff(void)
{
    LCD_Command(0x08U);
}
