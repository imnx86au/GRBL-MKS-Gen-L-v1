#ifndef timekeeper_h
#define timekeeper_h

#include "grbl.h"

void timekeeper_init();

uint32_t get_millis();

uint32_t get_timer_ticks();

uint32_t calculate_dt_micros(uint32_t timer_ticks1, uint32_t timer_ticks2);

#endif