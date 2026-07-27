#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <stdint.h>

/*
 * Địa chỉ phổ biến của module PCF8574.
 *
 * Nếu LCD không hoạt động, thử đổi:
 * 0x27 → 0x3F
 */
#define LCD_I2C_ADDRESS      0x27U

#define LCD_ROWS             2U
#define LCD_COLUMNS          16U

/*
 * Khởi tạo LCD 1602 ở chế độ 4-bit thông qua I2C.
 */
void LCD_Init(void);

/*
 * Xóa toàn bộ màn hình.
 */
void LCD_Clear(void);

/*
 * Đưa con trỏ về vị trí đầu màn hình.
 */
void LCD_Home(void);

/*
 * Đặt vị trí con trỏ.
 *
 * row:
 * 0 → dòng 1
 * 1 → dòng 2
 *
 * column:
 * 0 đến 15
 */
void LCD_SetCursor(
    uint8_t row,
    uint8_t column
);

/*
 * Hiển thị một ký tự.
 */
void LCD_WriteChar(char character);

/*
 * Hiển thị một chuỗi ký tự.
 */
void LCD_WriteString(const char *text);

/*
 * Hiển thị số nguyên không dấu.
 */
void LCD_WriteNumber(uint32_t number);

/*
 * Hiển thị số nguyên có dấu.
 */
void LCD_WriteSignedNumber(int32_t number);

/*
 * Bật đèn nền LCD.
 */
void LCD_BacklightOn(void);

/*
 * Tắt đèn nền LCD.
 */
void LCD_BacklightOff(void);

/*
 * Bật hiển thị.
 */
void LCD_DisplayOn(void);

/*
 * Tắt hiển thị.
 */
void LCD_DisplayOff(void);

#endif /* LCD_I2C_H */
