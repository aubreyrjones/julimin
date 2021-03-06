/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2014 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

MEMORY
{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 62K
	RAM  (rwx) : ORIGIN = 0x1FFFF800, LENGTH = 8K
}

SECTIONS
{
	.nvic 0x0 : {
		PROVIDE(_s_flash_nvic = .);
		KEEP(*(.vectors))
		PROVIDE(_e_flash_nvic = .);
	} > FLASH

	.flashconfig 0x400 : {
		KEEP(*(.flashconfig))
	} > FLASH

	.startup : {
		*(.startup)
	} > FLASH


	.ARM.extab : {
		*(.ARM.extab*)
	} > FLASH

    .ARM.exidx : {
     	PROVIDE(__exidx_start = .);
     	*(.ARM.exidx*)
    	PROVIDE(__exidx_end = .);
    } > FLASH

	.text : {
		. = ALIGN(4);
		KEEP(*(_start))
		*(.text .text*)
	} > FLASH

	.preinit_array : {
		. = ALIGN(4);
		PROVIDE_HIDDEN (__preinit_array_start = .);
    	KEEP (*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
    } > FLASH

	.init_array : {
		. = ALIGN(4);
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);
	} > FLASH

	.fini_array : {
		. = ALIGN(4);
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		PROVIDE_HIDDEN (__fini_array_end = .);
	} > FLASH

	.rodata : {
		. = ALIGN(4);
		*(.rdata*)
		*(.rodata*)
	} > FLASH

	_etext = .;

	/* RAM below here. */

	.ramnvic (NOLOAD) : {
		PROVIDE(_s_ram_nvic = .);
		KEEP(*(.ramnvic));
		PROVIDE(_e_ram_nvic = .);
	} > RAM

	.data : AT(_etext) {
		. = ALIGN(4);
		PROVIDE(_sdata = .);
		KEEP(*(.data .data.*))
		PROVIDE(_edata = .);
	} > RAM

	.bss : {
		. = ALIGN(4);
		PROVIDE(_sbss = .);
		PROVIDE(__bss_start__ = .);
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		PROVIDE(_ebss = .);
		PROVIDE(__bss_end = .);
		PROVIDE(__bss_end__ = .);
	} > RAM

	_heapStart = .;

	_estack = ORIGIN(RAM) + LENGTH(RAM);
}
