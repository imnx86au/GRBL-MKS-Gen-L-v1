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
float threading_z_feed_rate_factor;								// The factor to calculate the feed rate from the spindle speed

//This routine processes the spindle index pin hit by increasing the index pulse counter and calculating the time between pulses
//This speed is used for showing the actual spindle speed in the report
//The not time critical processing should be handled by protocol_exec_rt_system()
//This is signaled by the EXEC_SPINDLE_INDEX flag
void process_spindle_index_pin_hit()
{
	threading_index_timer_tics_passed=get_timer_ticks()-threading_index_Last_timer_tics;
	threading_index_Last_timer_tics+=threading_index_timer_tics_passed;
	threading_index_pulse_count++;
	//calculate the spindle speed  at this place reduces the CPU time because a GUI will update more frequently
	//have to disable this line when all is done for those who set the spindle speed by GRBL
	threading_index_spindle_speed = 15000000 / threading_index_timer_tics_passed;	
	sys.spindle_speed=threading_index_spindle_speed;	
	update_planner_feed_rate((threading_z_feed_rate_factor * (float) 15000000) / (float) threading_index_timer_tics_passed);	//update the feed rate when threading is active
	bit_true(threading_sync_state, EXEC_SPINDLE_INDEX);	//Signal the receive of a spindle index pulse
}

// This routine does all processing needed to keep the Z-axis in sync with the spindle during a threading pass G33
// It saves the current Z-axis position
// Calculates the target Z-axis position at the next sync pulse
// Calculates the feed rate needed to be at this position (neglecting acceleration time)
// Sets this feed rate for the current block in the planner
// Recalculates the feed rates for all the blocks in the planner as if a new block was added to the planner que
// All this is done only, if the current planner block is a G33 motion
//void process_spindle_sync_pin_hit()		//this 
//{
  ////float current_z_position;
  ////memcpy(current_z_position,sys_position[Y_AXIS],sizeof(sys_position[Y_AXIS]));	//Copy the current Z-ax position
  ////threading_target_z_position=sys_position[Y_AXIS]+threading_z_motion_per_sync_pulse;		//this is the starting target for the next synchronization pulse. Nothing is moving so no need for atomic copy
//}

void ReportMessage(const char *s, uint8_t value)
{
	while (*s)
	  serial_write(*s++);
	print_uint8_base10(value);
	report_util_line_feed();
}
void ReportMessagef(const char *s, float value)
{
	while (*s) 
	  serial_write(*s++);
	printFloat(value,2);
	report_util_line_feed();
}

void update_planner_feed_rate(float feed_rate) {
	plan_block_t *plan = plan_get_current_block();
	if ((plan!=NULL) && (plan->condition & PL_COND_FLAG_FEED_PER_REV)) {
		// replace predefined spindle speed with actual speed
		//ReportMessagef("SS: ",threading_index_spindle_speed);
		//ReportMessagef("  PR:",plan->programmed_rate);
	    //report_util_line_feed();
		//plan->spindle_speed = threading_index_spindle_speed;
		//sys.spindle_speed = threading_index_spindle_speed;
		plan->programmed_rate = feed_rate;  //set the feed rate
		plan_update_velocity_profile_parameters();
		plan_cycle_reinitialize();
		//ReportMessagef("SS:",threading_index_spindle_speed);
	}
}

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