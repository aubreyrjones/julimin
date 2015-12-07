//
// Created by ARJ on 12/5/15.
//

#ifndef JULIMIN_STARTUP_H
#define JULIMIN_STARTUP_H

/** Executed immediately upon reset. Run **before** any data initialization is performed. */
void chip_prestart() __attribute__((section(".startup")));

/** Start chip clocks. Run **before** any data initialization is performed. */
void chip_start_core_clocks() __attribute__((section(".startup")));

/** Gives the chip driver an opportunity to move the NVIC table to RAM. */
void chip_move_vectors_to_ram() __attribute__((section(".startup")));

/** Initialize GPIO pins for status LEDs and other proof-of-life peripherals. */
void chip_start_status_indicators() __attribute__((section(".startup")));

#endif //JULIMIN_STARTUP_H
