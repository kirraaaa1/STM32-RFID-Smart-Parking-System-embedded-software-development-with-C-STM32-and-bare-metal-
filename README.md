# STM32 RFID Smart Parking System

A bare-metal RFID-based smart parking gate system developed using the STM32F103RCT6 microcontroller, MFRC522 RFID reader, LCD1602 I2C display, and SG90 servo motor.

The firmware is written entirely in C without STM32 HAL, STM32CubeMX-generated drivers, or an RTOS. Peripheral registers, startup routines, memory layout, communication protocols, and hardware drivers are implemented manually for learning and demonstration purposes.

## Project Overview

This project simulates an automated parking access control system.

When an RFID card is placed near the MFRC522 reader, the STM32 detects the card and reads its UID. The system then verifies whether the card is authorized.

For an accepted card, the SG90 servo rotates approximately 90 degrees to open the parking barrier. After remaining open for about three seconds, the servo returns to its closed position.

The LCD1602 displays information such as:

- System startup status
- MFRC522 connection status
- RFID card UID
- Valid or invalid card notification
- Vehicle entry or exit status
- Number of available parking spaces
- Parking lot full warning

In the current demonstration version, the first RFID card scanned after startup is automatically registered as the authorized card. The UID is stored in SRAM, so it is lost after reset or power loss.

## System Operation

```text
System startup
      |
      v
Initialize SysTick, I2C, LCD, SPI, RFID and Servo
      |
      v
Read MFRC522 VersionReg
      |
      v
Wait for RFID card
      |
      v
Detect card and read UID
      |
      v
Check card authorization
      |
      +----------------------------+
      |                            |
      v                            v
Authorized card               Invalid card
      |                            |
      v                            v
Determine entry or exit       Display rejection
      |
      v
Update parking slot counter
      |
      v
Rotate servo to 90 degrees
      |
      v
Keep barrier open for 3 seconds
      |
      v
Return servo to 0 degrees
      |
      v
Display available parking slots
```

The registered card contains an `is_inside` state:

```text
is_inside = false
→ The vehicle is outside
→ The next scan is treated as vehicle entry

is_inside = true
→ The vehicle is inside
→ The next scan is treated as vehicle exit
```

For vehicle entry:

```text
occupied_slots++
available_slots--
```

For vehicle exit:

```text
occupied_slots--
available_slots++
```

## Main Features

- Bare-metal STM32F103RCT6 firmware
- No HAL or STM32CubeMX-generated peripheral code
- Custom startup file and linker script
- MFRC522 communication through SPI1
- LCD1602 communication through I2C1
- SG90 servo control using TIM2 PWM
- SysTick-based millisecond timing
- RFID card detection
- Four-byte RFID UID reading
- UID comparison and card authorization
- Automatic registration of the first scanned card
- Vehicle entry and exit state tracking
- Parking slot calculation
- Invalid card rejection
- Parking-full detection
- Makefile-based compilation
- ELF, BIN, HEX, MAP, and object file generation

## Hardware Components

| Component | Description |
|---|---|
| STM32F103RCT6 | ARM Cortex-M3 microcontroller |
| MFRC522 | 13.56 MHz RFID reader |
| RFID card or key tag | MIFARE-compatible RFID device |
| LCD1602 | 16x2 character display |
| PCF8574 module | I2C interface for LCD1602 |
| SG90 servo motor | Parking barrier actuator |
| ST-LINK | STM32 programmer and debugger |
| External 5 V supply | Recommended for the servo |
| Breadboard and jumper wires | Prototype connections |

## Pin Connections

### MFRC522 RFID Reader

| MFRC522 Pin | STM32F103RCT6 Pin | Function |
|---|---|---|
| SDA / SS | PA4 | SPI chip select |
| SCK | PA5 | SPI1 clock |
| MISO | PA6 | SPI1 master input |
| MOSI | PA7 | SPI1 master output |
| RST | PB0 | RC522 reset |
| IRQ | Not connected | Polling mode is used |
| 3.3V | 3.3V | Power |
| GND | GND | Common ground |

> The MFRC522 must be powered with 3.3 V. Supplying 5 V may damage the module.

### LCD1602 I2C

| LCD I2C Pin | STM32F103RCT6 Pin | Function |
|---|---|---|
| SCL | PB6 | I2C1 clock |
| SDA | PB7 | I2C1 data |
| VCC | 5V | LCD power |
| GND | GND | Common ground |

The default LCD I2C address is:

```c
#define LCD_I2C_ADDRESS 0x27U
```

Some PCF8574 modules may use:

```c
#define LCD_I2C_ADDRESS 0x3FU
```

### SG90 Servo Motor

| Servo Wire | Connection | Function |
|---|---|---|
| Orange or yellow | PA0 | TIM2 Channel 1 PWM |
| Red | External 5V | Servo power |
| Brown or black | GND | Common ground |

> The servo should use a separate 5 V power supply. The servo power ground and STM32 ground must be connected together.

### ST-LINK

| ST-LINK Pin | STM32F103RCT6 Pin |
|---|---|
| SWDIO | PA13 |
| SWCLK | PA14 |
| GND | GND |
| VTref / 3.3V | 3.3V |
| NRST | NRST |

`BOOT0` should normally be connected to ground so that the STM32 boots from internal Flash.

## Servo PWM Configuration

The SG90 servo is controlled using TIM2 Channel 1 on PA0.

```text
PWM frequency: 50 Hz
PWM period: 20 ms
Timer resolution: approximately 1 microsecond
```

The calibrated pulse-width range is:

```text
500 us  → approximately 0 degrees
1500 us → approximately 90 degrees
2500 us → approximately 180 degrees
```

The barrier positions are configured as:

```c
#define GATE_CLOSE_ANGLE 0U
#define GATE_OPEN_ANGLE  90U
```

When a valid card is detected:

```c
Servo_SetAngle(90U);
Delay_ms(3000U);
Servo_SetAngle(0U);
```

## MFRC522 Communication

The MFRC522 communicates with the STM32 through SPI1 using:

- Master mode
- SPI Mode 0
- 8-bit data frames
- MSB-first transmission
- Software-controlled chip select
- Polling instead of the IRQ output

The RFID driver performs the following operations:

1. Reset the MFRC522.
2. Configure its internal timer.
3. Enable the antenna.
4. Read `VersionReg` to verify SPI communication.
5. Send the `REQA` command to detect a nearby card.
6. Perform Cascade Level 1 anticollision.
7. Read the four-byte UID.
8. Verify the BCC byte.
9. Compare the UID with the registered UID.
10. Send the HALT command after processing.

Typical values returned by `VersionReg` are:

| Decimal | Hexadecimal | Meaning |
|---:|---:|---|
| 145 | `0x91` | MFRC522 Version 1 |
| 146 | `0x92` | MFRC522 Version 2 |
| 136 | `0x88` | Some compatible clone modules |

## Software Architecture

```text
main.c
  |
  +-- systick.c
  |     +-- Millisecond system timer and delay
  |
  +-- spi1.c
  |     +-- SPI1 initialization and byte transfer
  |
  +-- rc522.c
  |     +-- RFID detection and UID reading
  |
  +-- i2c1.c
  |     +-- I2C1 communication
  |
  +-- lcd_i2c.c
  |     +-- LCD1602 display driver
  |
  +-- servo.c
        +-- TIM2 PWM servo control
```

The main application coordinates all hardware drivers, processes RFID cards, tracks parking status, updates the LCD, and controls the barrier.

## Project Structure

```text
stm32-rfid-smart-parking/
|
|-- Inc/
|   |-- stm32f103_regs.h
|   |-- systick.h
|   |-- spi1.h
|   |-- rc522.h
|   |-- i2c1.h
|   |-- lcd_i2c.h
|   |-- servo.h
|   `-- parking_app.h
|
|-- Src/
|   |-- main.c
|   |-- systick.c
|   |-- spi1.c
|   |-- rc522.c
|   |-- i2c1.c
|   |-- lcd_i2c.c
|   |-- servo.c
|   `-- parking_app.c
|
|-- startup.c
|-- stm32f103rct6.ld
|-- Makefile
`-- README.md
```

The `Inc` directory contains register definitions, macros, structures, constants, and function prototypes.

The `Src` directory contains the implementation of the peripheral drivers and parking system logic.

`startup.c` contains the interrupt vector table, reset handler, `.data` initialization, `.bss` clearing, and the call to `main()`.

`stm32f103rct6.ld` defines:

```text
Flash: 256 KB starting at 0x08000000
SRAM:   48 KB starting at 0x20000000
```

It also defines the placement of the vector table, executable code, initialized data, uninitialized data, heap, and stack.

## Development Environment

The project was developed using:

- Visual Studio Code
- ARM GNU Toolchain
- GNU Make for Windows
- STM32CubeProgrammer
- ST-LINK
- PowerShell

STM32CubeIDE is not required.

## Building the Project

Verify that the ARM GNU Toolchain is installed:

```powershell
arm-none-eabi-gcc --version
```

On Windows, GNU Make may be available as:

```powershell
mingw32-make --version
```

Clean the previous build:

```powershell
mingw32-make clean
```

Compile the project:

```powershell
mingw32-make
```

After a successful build, the `build` directory contains:

```text
build/
|-- main.o
|-- systick.o
|-- spi1.o
|-- rc522.o
|-- i2c1.o
|-- lcd_i2c.o
|-- servo.o
|-- parking_app.o
|-- startup.o
|-- parking_system.elf
|-- parking_system.bin
|-- parking_system.hex
`-- parking_system.map
```

## Programming the Microcontroller

The firmware can be programmed using STM32CubeProgrammer.

1. Connect the ST-LINK to the STM32.
2. Open STM32CubeProgrammer.
3. Select the `ST-LINK` interface.
4. Select `SWD`.
5. Click **Connect**.
6. Open **Erasing & Programming**.
7. Select:

```text
build/parking_system.elf
```

8. Enable:

```text
Verify programming
Run after programming
```

9. Click **Start Programming**.

The ELF file already contains the correct Flash addresses.

When using the BIN file, set the start address manually:

```text
0x08000000
```

## Current Limitations

The current version is intended as an educational prototype.

- Only one RFID card is automatically registered.
- The card UID is lost after reset or power loss.
- Only four-byte RFID UIDs are supported.
- Vehicle entry and exit are determined by alternating scans.
- No infrared vehicle sensor is used.
- No separate entrance and exit gates are implemented.
- No parking history is stored.
- No real-time clock is included.
- No UART debugging is currently implemented.
- The RFID reader uses polling instead of hardware interrupts.
- Blocking delays are used during barrier operation.

## Future Improvements

Possible improvements include:

- Store authorized UIDs in internal Flash
- Support multiple RFID cards
- Support seven-byte and ten-byte UIDs
- Add infrared vehicle sensors
- Add separate entrance and exit RFID readers
- Close the gate only after the vehicle has passed
- Add a buzzer and status LEDs
- Add UART debugging
- Add a real-time clock
- Record vehicle entry and exit times
- Store parking history in EEPROM or an SD card
- Add ESP8266 or ESP32 connectivity
- Create a web-based monitoring dashboard
- Use the MFRC522 IRQ pin with STM32 EXTI
- Replace blocking delays with a non-blocking state machine
- Add watchdog and communication timeout recovery

## Learning Objectives

This project demonstrates:

- ARM Cortex-M3 startup sequence
- Interrupt vector tables
- Linker scripts
- Flash and SRAM memory organization
- Memory-mapped peripheral registers
- Bare-metal GPIO configuration
- SPI communication
- I2C communication
- Timer-based PWM generation
- SysTick interrupts
- RFID communication protocols
- Modular firmware design
- Embedded state management
- Firmware compilation and linking
- STM32 Flash programming

## Safety Notes

- Do not power the MFRC522 with 5 V.
- Use an external 5 V source for the servo motor.
- Connect all grounds together.
- Avoid using conflicting power sources.
- Test the servo without the barrier attached before calibration.
- Stop increasing the PWM range if the servo reaches its mechanical limit or produces abnormal noise.

## Project Status

The following features are currently working:

- STM32 startup and memory initialization
- SysTick millisecond timing
- LCD1602 I2C display
- MFRC522 SPI communication
- MFRC522 version detection
- RFID card detection
- Four-byte UID reading
- Automatic first-card registration
- UID comparison
- SG90 servo control
- Barrier opening to approximately 90 degrees
- Barrier closing to approximately 0 degrees
- Entry and exit state tracking
- Available parking slot display

## Author

Developed as a bare-metal embedded systems learning project using the STM32F103RCT6 microcontroller.

## License

This project is intended for educational and personal development purposes.

An open-source license such as the MIT License can be added to the repository.
