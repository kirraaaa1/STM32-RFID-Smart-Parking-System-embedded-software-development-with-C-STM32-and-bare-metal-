# STM32 RFID Smart Parking System

A bare-metal smart parking gate system developed with the STM32F103RCT6 microcontroller. The project uses an MFRC522 RFID reader for card identification, an LCD1602 with an I2C interface for status display, and an SG90 servo motor to control the parking barrier.

The firmware is written in C without STM32 HAL or STM32CubeMX-generated code. Peripheral registers, startup routines, memory layout, communication protocols, and device drivers are implemented manually for educational purposes.

---

## Project Overview

This project simulates a small RFID-based parking access control system.

When an RFID card is placed near the MFRC522 reader, the STM32 reads its UID and determines whether the card is authorized. For an accepted card, the servo motor rotates approximately 90 degrees to open the parking barrier. After a short delay, the servo returns to the closed position.

The LCD displays system information such as:

- System startup status
- MFRC522 connection status
- RFID card UID
- Card authorization result
- Vehicle entry or exit status
- Number of available parking spaces
- Parking lot full notification

The current demonstration version automatically registers the first RFID card scanned after startup. This card is then used as the authorized parking card.

---

## Main Features

- Bare-metal STM32F103RCT6 firmware
- No HAL or STM32CubeMX-generated peripheral code
- MFRC522 RFID communication through SPI1
- LCD1602 communication through I2C1
- SG90 servo motor control using TIM2 PWM
- SysTick-based millisecond delay
- RFID UID detection and comparison
- Automatic registration of the first scanned RFID card
- Vehicle entry and exit state tracking
- Available parking slot calculation
- Invalid card rejection
- Parking lot full detection
- Custom startup file and linker script
- Makefile-based compilation using ARM GNU Toolchain
- ELF, BIN, HEX, MAP, and object file generation

---

## Hardware Components

| Component | Description |
|---|---|
| STM32F103RCT6 | ARM Cortex-M3 microcontroller |
| MFRC522 | 13.56 MHz RFID reader module |
| RFID card or key tag | MIFARE-compatible RFID device |
| LCD1602 | 16x2 character LCD |
| PCF8574 I2C module | I2C interface for LCD1602 |
| SG90 servo motor | Parking barrier actuator |
| ST-LINK | Programmer and debugger |
| External 5 V supply | Recommended for the servo motor |
| Jumper wires | Hardware connections |
| Breadboard | Prototype assembly |

---

## Pin Connections

### MFRC522 to STM32F103RCT6

| MFRC522 Pin | STM32 Pin | Function |
|---|---|---|
| SDA / SS | PA4 | SPI chip select |
| SCK | PA5 | SPI1 clock |
| MISO | PA6 | SPI1 master input |
| MOSI | PA7 | SPI1 master output |
| RST | PB0 | RC522 reset |
| IRQ | Not connected | Interrupt is not used |
| 3.3V | 3.3V | Power supply |
| GND | GND | Common ground |

> The MFRC522 must be powered with 3.3 V. Supplying 5 V may damage the module.

### LCD1602 I2C to STM32F103RCT6

| LCD I2C Pin | STM32 Pin | Function |
|---|---|---|
| SCL | PB6 | I2C1 clock |
| SDA | PB7 | I2C1 data |
| VCC | 5V | LCD power |
| GND | GND | Common ground |

The default LCD I2C address used by the firmware is:

```c
#define LCD_I2C_ADDRESS 0x27U
