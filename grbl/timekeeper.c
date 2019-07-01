#include "timekeeper.h"

uint32_t overflows;

// overflow interrupt for timekeeper - counts timer ticks beyond 16 bits
ISR(TIMER5_OVF_vect) {
	overflows++;
}

void timekeeper_init() {
  // Configure Timer 5: system timer interrupt
  //TCCR5B &= ~(1<<WGM13); // waveform generation = 0100 = CTC
  //TCCR5B |=  (1<<WGM12);
  TCCR5A =0; // timer outputs disconnected, no waveform generation.
  TCCR5A &= ~((1<<COM5A1) | (1<<COM5A0) | (1<<COM5B1) | (1<<COM5B0)); // Disconnect OC1 output
  TCCR5B |=  (1<<CS11) | (1<<CS10); // pre-scaler: 1/64 (4 microseconds per tick @ 16MHz)
  TIMSK5 |=  (1<<TOIE5); // enable overflow interrupt  
  overflows = 0;
}

uint32_t get_millis() {
	// each timer tick are 4 microseconds
	return (get_timer_ticks()/(uint32_t)250);
}

uint32_t get_timer_ticks(){
	uint32_t ticks = TCNT5;
	return ticks+((overflows)<<16) ;
}

uint32_t calculate_dt_micros(uint32_t timer_ticks1, uint32_t timer_ticks2) {
	return ((timer_ticks2 - timer_ticks1)*4); // TODO: test if this works even if there was a 32bit overflow between the timestamps
}

