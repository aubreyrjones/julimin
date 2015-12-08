//
// Created by ARJ on 12/5/15.
//

#ifndef JULIMIN_STARTUP_H
#define JULIMIN_STARTUP_H

#ifdef __cplusplus
extern "C" {
#endif
/** Executed immediately upon reset. Run **before** any data initialization is performed. */
void chip_prestart() __attribute__((section(".startup")));

/** Start chip clocks. Run **before** any data initialization is performed. */
void chip_start_core_clocks() __attribute__((section(".startup")));

/** Gives the chip driver an opportunity to move the NVIC table to RAM. */
void chip_move_vectors_to_ram() __attribute__((section(".startup")));

/** Initialize GPIO pins for status LEDs and other proof-of-life peripherals. */
void chip_start_status_indicators() __attribute__((section(".startup")));

/** Set the status LED (if any) on the chip/board. */
void _panic(char const* message) __attribute__((noreturn));;

#ifdef __cplusplus
}
#endif

#endif //JULIMIN_STARTUP_H
