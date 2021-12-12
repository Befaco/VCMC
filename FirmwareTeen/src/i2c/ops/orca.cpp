#include "../../../PrjIncludes.h"

/*
#include "orca.h"

#include "ii.h"
*/
const tele_op_t op_OR_TRK = MAKE_SIMPLE_I2C_OP(OR.TRK, ORCA_TRACK);
const tele_op_t op_OR_CLK = MAKE_SIMPLE_I2C_OP(OR.CLK, ORCA_CLOCK);
const tele_op_t op_OR_DIV = MAKE_SIMPLE_I2C_OP(OR.DIV, ORCA_DIVISOR);
const tele_op_t op_OR_PHASE = MAKE_SIMPLE_I2C_OP(OR.PHASE, ORCA_PHASE);
const tele_op_t op_OR_RST = MAKE_SIMPLE_I2C_OP(OR.RST, ORCA_RESET);
const tele_op_t op_OR_WGT = MAKE_SIMPLE_I2C_OP(OR.WGT, ORCA_WEIGHT);
const tele_op_t op_OR_MUTE = MAKE_SIMPLE_I2C_OP(OR.MUTE, ORCA_MUTE);
const tele_op_t op_OR_SCALE = MAKE_SIMPLE_I2C_OP(OR.SCALE, ORCA_SCALE);
const tele_op_t op_OR_BANK = MAKE_SIMPLE_I2C_OP(OR.BANK, ORCA_BANK);
const tele_op_t op_OR_PRESET = MAKE_SIMPLE_I2C_OP(OR.PRESET, ORCA_PRESET);
const tele_op_t op_OR_RELOAD = MAKE_SIMPLE_I2C_OP(OR.RELOAD, ORCA_RELOAD);
const tele_op_t op_OR_ROTS = MAKE_SIMPLE_I2C_OP(OR.ROTS, ORCA_ROTATES);
const tele_op_t op_OR_ROTW = MAKE_SIMPLE_I2C_OP(OR.ROTW, ORCA_ROTATEW);
const tele_op_t op_OR_GRST = MAKE_SIMPLE_I2C_OP(OR.GRST, ORCA_GRESET);
const tele_op_t op_OR_CVA = MAKE_SIMPLE_I2C_OP(OR.CVA, ORCA_CVA);
const tele_op_t op_OR_CVB = MAKE_SIMPLE_I2C_OP(OR.CVB, ORCA_CVB);
