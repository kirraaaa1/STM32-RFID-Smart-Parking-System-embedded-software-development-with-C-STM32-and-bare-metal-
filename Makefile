# ============================================================
# Tên chương trình và thư mục
# ============================================================

TARGET      := parking_system

SRC_DIR     := Src
INC_DIR     := Inc
BUILD_DIR   := build

LDSCRIPT    := stm32f103rct6.ld


# ============================================================
# ARM GNU Toolchain
# ============================================================

CC          := arm-none-eabi-gcc
OBJCOPY     := arm-none-eabi-objcopy
SIZE        := arm-none-eabi-size
OBJDUMP     := arm-none-eabi-objdump


# ============================================================
# Các file source trong thư mục Src
# ============================================================

SRC_FILES := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/systick.c \
	$(SRC_DIR)/spi1.c \
	$(SRC_DIR)/rc522.c \
	$(SRC_DIR)/i2c1.c \
	$(SRC_DIR)/lcd_i2c.c \
	$(SRC_DIR)/servo.c \
	$(SRC_DIR)/parking_app.c


# Chuyển Src/main.c thành build/main.o...
SRC_OBJECTS := \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))


# startup.c nằm ngoài thư mục Src
STARTUP_OBJECT := $(BUILD_DIR)/startup.o

OBJS := $(SRC_OBJECTS) $(STARTUP_OBJECT)

DEPS := $(OBJS:.o=.d)


# ============================================================
# Cấu hình STM32F103RCT6
# ============================================================

MCU_FLAGS := \
	-mcpu=cortex-m3 \
	-mthumb


# ============================================================
# Cờ biên dịch
# ============================================================

CFLAGS := \
	$(MCU_FLAGS) \
	-std=gnu11 \
	-Wall \
	-Wextra \
	-O0 \
	-g3 \
	-ffreestanding \
	-ffunction-sections \
	-fdata-sections \
	-MMD \
	-MP \
	-I$(INC_DIR)


# ============================================================
# Cờ linker
# ============================================================

LDFLAGS := \
	$(MCU_FLAGS) \
	-T$(LDSCRIPT) \
	-nostdlib \
	-Wl,--gc-sections \
	-Wl,-Map=$(BUILD_DIR)/$(TARGET).map \
	-Wl,--print-memory-usage

LDLIBS := -lgcc


# ============================================================
# File đầu ra
# ============================================================

ELF_FILE := $(BUILD_DIR)/$(TARGET).elf
BIN_FILE := $(BUILD_DIR)/$(TARGET).bin
HEX_FILE := $(BUILD_DIR)/$(TARGET).hex
LST_FILE := $(BUILD_DIR)/$(TARGET).lst


# ============================================================
# Các target chính
# ============================================================

.PHONY: all clean size disassembly

all: $(BIN_FILE) $(HEX_FILE)
	@echo Build completed successfully
	@echo ELF: $(ELF_FILE)
	@echo BIN: $(BIN_FILE)
	@echo HEX: $(HEX_FILE)


# ============================================================
# Biên dịch file trong Src
#
# Ví dụ:
# Src/main.c -> build/main.o
# ============================================================

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


# ============================================================
# Biên dịch startup.c ở thư mục gốc
# ============================================================

$(BUILD_DIR)/startup.o: startup.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


# ============================================================
# Link các file .o thành file .elf
# ============================================================

$(ELF_FILE): $(OBJS) $(LDSCRIPT)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@
	$(SIZE) $@


# ============================================================
# Chuyển ELF thành BIN
# ============================================================

$(BIN_FILE): $(ELF_FILE)
	$(OBJCOPY) -O binary $< $@


# ============================================================
# Chuyển ELF thành HEX
# ============================================================

$(HEX_FILE): $(ELF_FILE)
	$(OBJCOPY) -O ihex $< $@


# ============================================================
# Tạo thư mục build
# ============================================================

$(BUILD_DIR):
	if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"


# ============================================================
# Kiểm tra dung lượng
# ============================================================

size: $(ELF_FILE)
	$(SIZE) $(ELF_FILE)


# ============================================================
# Xuất mã Assembly
# ============================================================

disassembly: $(ELF_FILE)
	$(OBJDUMP) -d -S $(ELF_FILE) > $(LST_FILE)


# ============================================================
# Xóa kết quả build
# ============================================================

clean:
	if exist "$(BUILD_DIR)" rmdir /S /Q "$(BUILD_DIR)"


# Tự động nạp dependency của header
-include $(DEPS)