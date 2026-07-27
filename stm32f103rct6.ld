ENTRY(Reset_Handler) /* Entry Point */

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 256K /* khai báo dung lương và địa chỉ FLASH */
    SRAM (rwx) : ORIGIN = 0X20000000, LENGTH = 48K /* khai báo dung lương và địa chỉ SRAM */
}

_estack = ORIGIN(SRAM) + LENGTH(SRAM)  /* chuyển đến địa chị cuối sram */

_Min_Heap_Size = 0x200; /* cấp 512bytes cho Heap */
_Min_Stack_Size = 0x400; /* cấp 1024bytes cho stack */
 
SECTIONS
{
    .text : /* gôm file .o thành mã máy */
    {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    *(.text*)
    *(.rodata*)
    . = ALIGN(4);
    _etext = .;
    } > FLASH

    .data :/*dùng cho biến toàn cục gán giá trị ban đầu  */
    {
    . = ALIGN(4);
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
    } > SRAM AT> FLASH

    .bss :/*dùng cho biến toàn cục chưa được gán giá trị ban đầu  */
    {
    . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    . = ALIGN(4);
    _ebss = .;
    } > SRAM

    .ARM.attributes 0 : /*gom thông tin mô tả file được biên dịch cho kiến trúc ARM nào, ABI nào và các thuộc tính tương thích của object file.*/
    {
    *(.ARM.attributes)
    }

    ._user_heap_stack : /*dành trước một lượng RAM tối thiểu cho Heap và Stack, giúp linker phát hiện khi chương trình dùng quá nhiều RAM*/
    {
    . = ALIGN(8);
    PROVIDE ( end = .);
    PROVIDE ( _end = .);
    . = . + 0x200; /* cấp 512 bytes cho Heap */
    . = . + 0x400; /* cấp 1024 bytes cho Heap */
    . = ALIGN(8);
    } > RAM
}
