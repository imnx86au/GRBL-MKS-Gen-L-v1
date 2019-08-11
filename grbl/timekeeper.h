#ifndef timekeeper_h
#define timekeeper_h

#include "grbl.h"

void timekeeper_init();

void timekeeper_reset();

uint32_t get_timer_ticks();

#endif