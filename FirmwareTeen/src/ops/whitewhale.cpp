#include "../../PrjIncludes.h"
/*#include "whitewhale.h"

#include "ii.h"
*/
const tele_op_t op_WW_PRESET = MAKE_SIMPLE_I2C_OP(WW.PRESET, WW_PRESET);
const tele_op_t op_WW_POS = MAKE_SIMPLE_I2C_OP(WW.POS, WW_POS);
const tele_op_t op_WW_SYNC = MAKE_SIMPLE_I2C_OP(WW.SYNC, WW_SYNC);
const tele_op_t op_WW_START = MAKE_SIMPLE_I2C_OP(WW.START, WW_START);
const tele_op_t op_WW_END = MAKE_SIMPLE_I2C_OP(WW.END, WW_END);
const tele_op_t op_WW_PMODE = MAKE_SIMPLE_I2C_OP(WW.PMODE, WW_PMODE);
const tele_op_t op_WW_PATTERN = MAKE_SIMPLE_I2C_OP(WW.PATTERN, WW_PATTERN);
const tele_op_t op_WW_QPATTERN = MAKE_SIMPLE_I2C_OP(WW.QPATTERN, WW_QPATTERN);
const tele_op_t op_WW_MUTE1 = MAKE_SIMPLE_I2C_OP(WW.MUTE1, WW_MUTE1);
const tele_op_t op_WW_MUTE2 = MAKE_SIMPLE_I2C_OP(WW.MUTE2, WW_MUTE2);
const tele_op_t op_WW_MUTE3 = MAKE_SIMPLE_I2C_OP(WW.MUTE3, WW_MUTE3);
const tele_op_t op_WW_MUTE4 = MAKE_SIMPLE_I2C_OP(WW.MUTE4, WW_MUTE4);
const tele_op_t op_WW_MUTEA = MAKE_SIMPLE_I2C_OP(WW.MUTEA, WW_MUTEA);
const tele_op_t op_WW_MUTEB = MAKE_SIMPLE_I2C_OP(WW.MUTEB, WW_MUTEB);
