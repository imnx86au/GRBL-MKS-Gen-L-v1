/*
  threading.c - Handles threading command G33
  Part of Grbl

  Copyright (c) 2014-2016 Sungeun K. Jeon for Gnea Research LLC

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "grbl.h"

volatile uint8_t threading_exec_flags;							// Global realtime executor bitflag variable for spindle synchronization.
volatile uint8_t threading_index_pulse_count;					// Global index pulse counter
volatile uint8_t threading_sync_pulse_count;		// Global synchronization pulse counter
volatile uint32_t threading_sync_Last_timer_tics;				// Time at last sync pulse
volatile uint32_t threading_sync_timer_tics_passed;				// Time passed sync pulse
volatile uint32_t threading_index_Last_timer_tics;				// Time at last index pulse
volatile uint32_t threading_index_timer_tics_passed=0;	    	// Time passed index pulse
volatile uint32_t threading_index_spindle_speed;					// The measured spindle speed used for threading
float threading_mm_per_synchronization_pulse;					// The factor to calculate the feed rate from the spindle speed
volatile float threading_millimeters_target;						// The threading feed target as reported by the planner
volatile float synchronization_millimeters_error;						// The threading feed error calculated at every synchronization pulsee

void ReportMessageUint8(const char *s, uint8_t value)
{
	while (*s)
	  serial_write(*s++);
	print_uint8_base10(value);
}
void ReportMessageFloat(const char *s, float value)
{
	while (*s) 
	  serial_write(*s++);
	printFloat(value,2);
}

// Initializes the G33 threading pass by resetting the timer, spindle counter,
// setting the current z-position as reference and calculating the (next) target position.
void threading_init(float K_value)
{
	threading_mm_per_synchronization_pulse= K_value / (float) settings.sync_pulses_per_revolution;					// Calculate the global mm feed per synchronization pulse value.
	timekeeper_reset();																					//reset the timekeeper to avoid calculation errors when timer overflow occurs (to be sure)
	//ATOMIC_BLOCK(ATOMIC_RESTORESTATE){threading_start_position_steps=sys_position[Z_AXIS];}				//save the current Z-axis position for calculating the actual move. Use atomic to avoid errors due to stepper updates
	threading_reset();				//Sets the target position to zero and calculates the next target position																					
}
// Reset variables to start the threading
void threading_reset()
{
	threading_index_pulse_count=0;	//set the spindle index pulse count to 0
	threading_index_Last_timer_tics=0;
	threading_sync_Last_timer_tics=0;
	threading_sync_pulse_count=0;
	threading_millimeters_target=0;																			//Set this value to 0, will be update at the start of the planner block
	system_clear_threading_exec_flag(0xff);																	//Clear all the bits to avoid executing
}

//Returns the time in 4 useconds tics since the last index pulse
uint32_t TimerTicsPassed(uint32_t last_tics)
{
	uint32_t tics;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){tics=get_timer_ticks()-last_tics;}				//save the current Z-axis position for calculating the actual move. Use atomic to avoid errros due to stepper updates
  return tics;
}
//Returns the time in 4 useconds tics since the last index pulse
uint32_t timer_tics_passed_since_last_index_pulse()
{
	uint32_t tics;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){tics=get_timer_ticks()-threading_index_Last_timer_tics;}				//save the current Z-axis position for calculating the actual move. Use atomic to avoid errros due to stepper updates
	return tics;
	}
// This routine processes the spindle index pin hit by increasing the index pulse counter and calculating the time between pulses
// This speed is used for showing the actual spindle speed in the report
// The not time critical processing should be handled by protocol_exec_rt_system()
// This is signaled by the EXEC_SPINDLE_INDEX flag
// The Z-axis feedrate is synchronized by calling update_planner_feed_rate() when SPINDLE_SYNC_PULSES_PER_ROTATION==1 otherwise this is handled by the synchronization puls event
void process_spindle_index_pulse()
{
	threading_index_timer_tics_passed=get_timer_ticks()-threading_index_Last_timer_tics;	// Calculate the time between index pulses
	threading_index_Last_timer_tics+=threading_index_timer_tics_passed;						// adjust for calculating the next time
	threading_index_pulse_count++;															// Increase the pulse count
	threading_index_spindle_speed = 15000000 / threading_index_timer_tics_passed;			// calculate the spindle speed  at this place (not in the report) reduces the CPU time because a GUI will update more frequently
}

// Processes the synchronization pulses by calculating the time between the synchronization pulses and preparing for the next pulse
//
void process_spindle_synchronization_pulse()
{
	threading_sync_timer_tics_passed=get_timer_ticks()-threading_sync_Last_timer_tics;		// Calculate the time between index pulses
	threading_sync_timer_tics_passed+=threading_sync_timer_tics_passed;						// adjust for calculating the next time
	threading_sync_pulse_count++;															// Increase the synchronization pulse count
}

// This routine does all processing needed to keep the Z-axis in sync with the spindle during a threading pass G33
// This is done only, if the current planner block is a G33 motion indicated by the planner condition PL_COND_FLAG_FEED_PER_REV
// It saves the current Z-axis position
// Calculates the target Z-axis position at the next sync pulse
// Calculates the feed rate needed to be at this position (neglecting acceleration time)
// Sets this feed rate for the current block in the planner
// Recalculates the feed rates for all the blocks in the planner as if a new block was added to the planner que
// If the current block isn't a G33 motion, the synchronization_millimeters_error will be set to zero
void update_planner_feed_rate() {
plan_block_t *plan = plan_get_current_block();
	if (bit_istrue(threading_exec_flags, EXEC_PLANNER_SYNC_PULSE)){				// This bit it set on every spindle/synchronization pulse, independent of a G33 operation is busy
		if ((plan!=NULL) && (plan->condition & PL_COND_FLAG_FEED_PER_REV)) {	// update only during threading
			plan_update_velocity_profile_parameters();							// call plan_compute_profile_nominal_speed() that wil calculate the requested feed rate
			plan_cycle_reinitialize();											// update the feed rates in the blocks
		}
		else synchronization_millimeters_error=0;								// set the error to zero so it can be used in reports
	}
    system_clear_threading_exec_flag( EXEC_PLANNER_SYNC_PULSE);	//set the bit false to prevent processing again
}

// returns true if Spindle sync is active otherwise false
bool spindle_synchronization_active()
{
	plan_block_t *plan = plan_get_current_block();
	if ((plan!=NULL) && (plan->condition & PL_COND_FLAG_FEED_PER_REV)) return true;
	return false;
}
// Reports synchronization error.  just for debugging or checking threading accuracy
void report_synchronization_error()
{
  ReportMessageFloat("  Se:",synchronization_millimeters_error);				//report the threading position error
  report_util_line_feed();
}
