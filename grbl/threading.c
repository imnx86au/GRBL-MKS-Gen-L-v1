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

volatile uint8_t threading_sync_state;							// Global realtime executor bitflag variable for spindle synchronization.
volatile uint32_t threading_index_pulse_count;					// Global index pulse counter
volatile uint32_t threading_synchronization_pulse_count;		// Global synchronization pulse counter
volatile uint32_t threading_sync_Last_timer_tics;				// Time at last sync pulse
volatile uint32_t threading_sync_timer_tics_passed;				// Time passed sync pulse
volatile uint32_t threading_index_Last_timer_tics;				// Time at last index pulse
volatile uint32_t threading_index_timer_tics_passed=0;	    	// Time passed index pulse
volatile float threading_target_z_position;						// The Z-axis position to reach at the next synchronization pulse
volatile float threading_index_spindle_speed;					// The measured spindle speed used for threading
float threading_z_motion_per_sync_pulse;						// Distance the Z-axis has to travel at each synchronization pulse

//This routine processes the spindle index pin hit by increasing the index pulse counter and calculating the time between pulses
//used by showing the actual spindle speed in the report
//The not time critical processing should be handled by protocol_exec_rt_system()
//This is signaled by the EXEC_SPINDLE_INDEX flag
void process_spindle_index_pin_hit()
{
	threading_index_timer_tics_passed=get_timer_ticks()-threading_index_Last_timer_tics;
	threading_index_Last_timer_tics+=threading_index_timer_tics_passed;
	threading_index_pulse_count++;
	threading_index_spindle_speed=15000000 / threading_index_timer_tics_passed;	//calculate the spindle speed  at this place reduces the CPU time because a GUI will update more frequently
	sys.spindle_speed=threading_index_spindle_speed;
	bit_true(threading_sync_state, EXEC_SPINDLE_INDEX);	//Signal the receive of a spindle index pulse
}

//float z_motion_per_sync_pulse;
//float threading_spindle_speed;	

// This routine does all processing needed to keep the Z-axis in sync with the spindle during a threading pass G33
// It saves the current Z-axis position
// Calculates the target Z-axis position at the next sync pulse
// Calculates the feed rate needed to be at this position (neglecting acceleration time)
// Sets this feed rate for the current block in the planner
// Recalculates the feed rates for all the blocks in the planner as if a new block was added to the planner que
// All this is done only, if the current planner block is a G33 motion
void process_spindle_sync_pin_hit()		//this 
{
  float current_z_position;
  //memcpy(current_z_position,sys_position[Y_AXIS],sizeof(sys_position[Y_AXIS]));	//Copy the current Z-ax position
  //threading_target_z_position=sys_position[Y_AXIS]+threading_z_motion_per_sync_pulse;		//this is the starting target for the next synchronization pulse. Nothing is moving so no need for atomic copy
	
	
}

//void spindle_sync_update() {
	//plan_block_t *plan = plan_get_current_block();
	//if ((plan!=NULL) && (plan->condition & PL_COND_FLAG_FEED_PER_REV)) {
		//float actual_spindle_speed = spindle_get_speed();
		//// replace predefined spindle speed with actual speed
		//plan->spindle_speed = actual_spindle_speed;
		//plan_update_velocity_profile_parameters();
		//plan_cycle_reinitialize();
	//}
//}

// Prints synchronization state.
void report_synchronization_state()
{
	printPgmString(PSTR("Si: "));
	print_uint32_base10(threading_index_pulse_count);
	printPgmString(PSTR("|Ss: "));
	print_uint32_base10(threading_synchronization_pulse_count);
	printPgmString(PSTR("|Sp: "));
	print_uint32_base10(threading_sync_timer_tics_passed);
	printPgmString(PSTR("|Ip: "));
	print_uint32_base10(threading_index_timer_tics_passed);
	//report_RPM_state();
	report_util_line_feed();
}
////return the RPM based on the time between spindle index pulses
//uint32_t RPM()
//{
	//uint32_t tmp;
	//ATOMIC_BLOCK(ATOMIC_RESTORESTATE){		// Avoid changing during calculations so store temporary
		//tmp = threading_index_timer_tics_passed;
	//}
	////if (tmp < (uint32_t) 150000000)			// >> 1 RPM
	////return  0;
	//return (uint32_t) 15000000  / (tmp);		//every tic is 4 us, in stead of multiplying the time passed, divide the 60.000.0000 (const 1 second) by 4
//}