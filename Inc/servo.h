#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

/*
 * Góc của thanh chắn.
 *
 * Có thể đổi hai giá trị này tùy cách lắp servo.
 */
#define SERVO_GATE_CLOSE_ANGLE    0U
#define SERVO_GATE_OPEN_ANGLE     90U

#define SERVO_MIN_ANGLE           0U
#define SERVO_MAX_ANGLE           180U

/*
 * Khởi tạo PA0 và TIM2 Channel 1 để tạo PWM 50 Hz.
 *
 * timer_clock_hz:
 * - 8000000UL nếu STM32 chạy HSI 8 MHz
 * - 72000000UL nếu TIM2 chạy 72 MHz
 */
void Servo_Init(uint32_t timer_clock_hz);

/* Điều khiển servo đến góc từ 0 đến 180 độ */
void Servo_SetAngle(uint8_t angle);

/* Điều khiển trực tiếp độ rộng xung PWM */
void Servo_SetPulseWidth(uint16_t pulse_width_us);

/* Quay servo tới góc mở 90 độ */
void Servo_OpenGate(void);

/* Quay servo về góc đóng 0 độ */
void Servo_CloseGate(void);

/* Lấy góc servo hiện tại */
uint8_t Servo_GetAngle(void);

#endif /* SERVO_H */