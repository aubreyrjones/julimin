//
// Created by ARJ on 12/5/15.
//

#ifndef JULIMIN_STARTUP_H
#define JULIMIN_STARTUP_H

/** Executed immediately upon reset. Run **before** any data initialization is performed. */
void cstart_prestart() __attribute__((section(".startup")));

/** Start chip clocks. Run **before** any data initialization is performed. */
void cstart_core_clocks() __attribute__((section(".startup")));

/** Initialize GPIO pins for LEDs and other proof-of-life peripherals. */
void cstart_core_ports() __attribute__((section(".startup")));

#endif //JULIMIN_STARTUP_H
