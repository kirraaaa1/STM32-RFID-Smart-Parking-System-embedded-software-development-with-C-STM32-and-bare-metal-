#include "servo.h"
#include "stm32f103_regs.h"

/*
 * Servo SG90 thường điều khiển bằng PWM 50 Hz:
 *
 * Chu kỳ: 20 ms = 20000 us
 *
 * 1000 us → khoảng 0 độ
 * 1500 us → khoảng 90 độ
 * 2000 us → khoảng 180 độ
 */

#define SERVO_MIN_PULSE_US       500U
#define SERVO_MAX_PULSE_US       2500U
#define SERVO_PWM_PERIOD_US      20000U

static uint8_t servo_current_angle = 0U;


void Servo_Init(uint32_t timer_clock_hz)
{
    /*
     * Bật clock GPIOA và TIM2.
     */
    RCC_APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC_APB1ENR |= RCC_APB1ENR_TIM2EN;

    /*
     * PA0 = TIM2_CH1.
     *
     * Cấu hình:
     * MODE0 = 10: Output 2 MHz
     * CNF0  = 10: Alternate Function Push-pull
     *
     * Nibble = 0xA
     */
    GPIOA_CRL &= ~(0xFU << 0U);
    GPIOA_CRL |=  (0xAU << 0U);

    /*
     * Tạo timer tick bằng 1 MHz:
     *
     * 1 tick = 1 microsecond.
     *
     * Ví dụ:
     * Timer clock = 8 MHz
     * PSC = 8 - 1 = 7
     */
    TIM2_PSC =
        (timer_clock_hz / 1000000UL) - 1UL;

    /*
     * Chu kỳ PWM 20.000 us = 50 Hz.
     */
    TIM2_ARR = SERVO_PWM_PERIOD_US - 1U;

    /*
     * TIM2 Channel 1:
     *
     * OC1M = 110: PWM Mode 1
     * OC1PE = 1: bật preload CCR1
     */
    TIM2_CCMR1 &= ~(
        (7U << 4U) |
        (1U << 3U)
    );

    TIM2_CCMR1 |=
        (6U << 4U) |
        (1U << 3U);

    /*
     * Bật đầu ra Channel 1.
     */
    TIM2_CCER |= (1U << 0U);

    /*
     * Bật preload cho ARR.
     */
    TIM2_CR1 |= (1U << 7U);

    /*
     * Đặt độ rộng xung đóng cổng ban đầu.
     *
     * 1000 us tương ứng khoảng 0 độ.
     */
    TIM2_CCR1 = SERVO_MIN_PULSE_US;

    /*
     * Nạp PSC, ARR và CCR vào timer.
     */
    TIM2_EGR |= (1U << 0U);

    /*
     * Bật TIM2.
     */
    TIM2_CR1 |= (1U << 0U);

    servo_current_angle = SERVO_GATE_CLOSE_ANGLE;
}


void Servo_SetPulseWidth(uint16_t pulse_width_us)
{
    /*
     * Giới hạn xung để tránh servo chạy vượt phạm vi.
     */
    if (pulse_width_us < SERVO_MIN_PULSE_US)
    {
        pulse_width_us = SERVO_MIN_PULSE_US;
    }

    if (pulse_width_us > SERVO_MAX_PULSE_US)
    {
        pulse_width_us = SERVO_MAX_PULSE_US;
    }

    /*
     * Vì timer tick bằng 1 us:
     *
     * CCR1 = 1500
     * nghĩa là xung mức cao kéo dài 1500 us.
     */
    TIM2_CCR1 = pulse_width_us;
}


void Servo_SetAngle(uint8_t angle)
{
    if (angle > SERVO_MAX_ANGLE)
    {
        angle = SERVO_MAX_ANGLE;
    }

    /*
     * Chuyển góc 0–180 độ thành xung 1000–2000 us.
     *
     * Góc 0:
     * pulse = 1000 us
     *
     * Góc 90:
     * pulse = 1500 us
     *
     * Góc 180:
     * pulse = 2000 us
     */
    uint32_t pulse_width_us =
        SERVO_MIN_PULSE_US +
        (
            ((uint32_t)angle *
             (SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US))
            / SERVO_MAX_ANGLE
        );

    Servo_SetPulseWidth(
        (uint16_t)pulse_width_us
    );

    servo_current_angle = angle;
}


void Servo_OpenGate(void)
{
    /*
     * Quay servo tới 90 độ.
     */
    Servo_SetAngle(SERVO_GATE_OPEN_ANGLE);
}


void Servo_CloseGate(void)
{
    /*
     * Quay servo về 0 độ.
     */
    Servo_SetAngle(SERVO_GATE_CLOSE_ANGLE);
}


uint8_t Servo_GetAngle(void)
{
    return servo_current_angle;
}