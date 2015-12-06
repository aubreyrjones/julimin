//
// Created by ARJ on 12/5/15.
//

#ifndef JULIMIN_STARTUP_H
#define JULIMIN_STARTUP_H

#define CPU_CORE_FREQUENCY 48000000

/** Executed immediately upon reset. Run **before** any data initialization is performed. */
void cstart_prestart();

/** Start chip clocks. Run **before** any data initialization is performed. */
void cstart_core_clocks();

/** Initialize ports and other IO. */
void cstart_core_ports();

#endif //JULIMIN_STARTUP_H
