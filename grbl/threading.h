/*
  threading.h - Handles threading command G33
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

#ifndef threading_h
#define threading_h

extern volatile uint8_t threading_sync_state;							// Global real time executor bitflag variable for spindle synchronization.
extern volatile uint32_t threading_synchronization_pulse_count;			// Global synchronization pulse counter
extern volatile uint32_t threading_sync_Last_timer_tics;				// Time at last sync pulse
extern volatile uint32_t threading_sync_timer_tics_passed;				// Time passed sync pulse
extern volatile float threading_sync_spindle_speed;						// The spindle speed calculated from the sync index pulses. Used for synchronizing the Z-axis.
extern volatile uint32_t threading_index_pulse_count;					// Global index pulse counter
extern volatile uint32_t threading_index_Last_timer_tics;				// Time at last index pulse
extern volatile uint32_t threading_index_timer_tics_passed;				// Time passed index pulse
extern volatile float threading_index_spindle_speed;					// The spindle speed calculated from the spindle index pulses. Used for displaying the real spindle speed.
extern volatile float threading_target_z_position;						// The Z-axis postion to reach at the next synchronization pulse
extern float threading_z_motion_per_sync_pulse;						    // Z-axis motion at each sync pulse. Is not declared as volatile because it is not updated by an ISR routine.

void process_spindle_index_pin_hit();
void report_synchronization_state();
//uint32_t RPM();
#endif