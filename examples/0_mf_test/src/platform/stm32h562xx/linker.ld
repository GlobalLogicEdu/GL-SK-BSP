/**
 * @file    linker.ld
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

MEMORY
{
    FLASH    (rx)    : ORIGIN = 0x08000000, LENGTH = 1024K

    /* Split SRAM Blocks */
    SRAM1    (xrw)   : ORIGIN = 0x20000000, LENGTH = 256K /* 256-Kbyte SRAM1 */
    SRAM2    (xrw)   : ORIGIN = 0x20040000, LENGTH = 64K  /* 64-Kbyte SRAM2 with ECC */
    SRAM3    (xrw)   : ORIGIN = 0x20050000, LENGTH = 320K /* 320-Kbyte SRAM3 with optional ECC */
}

_estack = ORIGIN(SRAM1) + LENGTH(SRAM1);
_sstack = _estack - 10K;

SECTIONS
{
    .isr_vectors :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vectors))
        . = ALIGN(4);
    } >FLASH

    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

    /* 1. Main Application Data, Heap, and Stack go into SRAM1 */
    .data :
    {
        . = ALIGN(4);
        _data_start = .; /* The target start destination address inside SRAM */
        *(.data)
        *(.data*)
        _data_end = .; /* The end address marker boundary inside SRAM */
        . = ALIGN(4);
    } >SRAM1 AT> FLASH

    /* Get the exact LOAD address of the data segment inside FLASH */
    _si_data = LOADADDR(.data);

    .bss :
    {
        . = ALIGN(4);
        _bss_start = .;
        *(.bss)
        *(.bss*)
        _bss_end = .;
        . = ALIGN(4);
    } >SRAM1

    /* 2. Custom Section for SRAM2 (Great for fast execution or DMA) */
    .sram2_data :
    {
        . = ALIGN(4);
        *(.sram2_data*)
        . = ALIGN(4);
    } >SRAM2

    /* 3. Custom Section for SRAM3 (Great for large structures, graphics buffers, or RTOS heaps) */
    .sram3_data :
    {
        . = ALIGN(4);
        *(.sram3_data*)
        . = ALIGN(4);
    } >SRAM3
}

GROUP(
   libgcc.a
   libg.a
   libc.a
   libm.a
   libnosys.a
)
