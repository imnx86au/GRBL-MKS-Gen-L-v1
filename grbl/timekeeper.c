#include "timekeeper.h"

volatile uint32_t overflow_offset;

// overflow interrupt for timekeeper - counts timer ticks beyond 16 bits
ISR(TIMER5_OVF_vect) {
	overflow_offset+=(1UL<<16); //use of offset saves calculation time because get_timer_tics is called more often then the overflow interrupt
}

void timekeeper_init() {
  // Configure Timer 5: system timer interrupt
  //TCCR5B &= ~(1<<WGM13); // waveform generation = 0100 = CTC
  //TCCR5B |=  (1<<WGM12);
  TCCR5A =0; // timer outputs disconnected, no waveform generation.
//  TCCR5A &= ~((1<<COM5A1) | (1<<COM5A0) | (1<<COM5B1) | (1<<COM5B0)); // Disconnect OC1 output
  TCCR5B |=  (1<<CS51) | (1<<CS50); // pre-scaler: 1/64 (4 microseconds per tick @ 16MHz)
  TIMSK5 |=  (1<<TOIE5); // enable overflow interrupt  
  timekeeper_reset();
}

void timekeeper_reset()
{
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    TCNT5=0;					// reset the counter
    overflow_offset = 0;		// set the offset to zero
	bit_false(TIFR5,(1<<TOV5));	// clear any pending interrupts
  }
}

uint32_t get_timer_ticks() {
  uint32_t ticks;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
	 ticks = ((uint32_t) TCNT5) + overflow_offset;				// read the timer register and add the overflow offset
	 if (bit_istrue(TIFR5,1<<TOV5))								// during reading and calculating, a new timer overflow occurred that is not handled. read again and add 0x10000 to overflow_offset
	 {
	 ticks = ((uint32_t) TCNT5) + overflow_offset + (1UL<<16); // read again and add 0x10000 to overflow_offset
	 }
  }
  return ticks;
}

