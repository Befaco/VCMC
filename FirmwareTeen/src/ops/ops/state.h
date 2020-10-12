// Adapted from TeleType source code.
// See license on https://github.com/monome/teletype

#ifndef _STATE_H_
#define _STATE_H_


#include "command.h"

/////////////////////////////////////////
// state.h
/////////////////////////////////////////
typedef struct {
    bool initializing;
    /*     scene_variables_t variables;
        scene_pattern_t patterns[PATTERN_COUNT];
        scene_delay_t delay;
        scene_stack_op_t stack_op;
        int16_t tr_pulse_timer[TR_COUNT];
        scene_script_t scripts[SCRIPT_COUNT];
        scene_turtle_t turtle;
        bool every_last;
        scene_grid_t grid;
        scene_rand_t rand_states;
        cal_data_t cal; */
    int8_t i2c_op_address;
} scene_state_t;


typedef struct {
    // exec_vars_t variables[EXEC_DEPTH];
    // uint8_t exec_depth;
    bool overflow;
} exec_state_t;

/////////////////////////////////////////

#endif
