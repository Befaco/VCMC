#include "../../../PrjIncludes.h"

/*#include "earthsea.h"

#include "ii.h"
*/
const tele_op_t op_ES_PRESET = MAKE_SIMPLE_I2C_OP(ES.PRESET, ES_PRESET);
const tele_op_t op_ES_MODE = MAKE_SIMPLE_I2C_OP(ES.MODE, ES_MODE);
const tele_op_t op_ES_CLOCK = MAKE_SIMPLE_I2C_OP(ES.CLOCK, ES_CLOCK);
const tele_op_t op_ES_RESET = MAKE_SIMPLE_I2C_OP(ES.RESET, ES_RESET);
const tele_op_t op_ES_PATTERN = MAKE_SIMPLE_I2C_OP(ES.PATTERN, ES_PATTERN);
const tele_op_t op_ES_TRANS = MAKE_SIMPLE_I2C_OP(ES.TRANS, ES_TRANS);
const tele_op_t op_ES_STOP = MAKE_SIMPLE_I2C_OP(ES.STOP, ES_STOP);
const tele_op_t op_ES_TRIPLE = MAKE_SIMPLE_I2C_OP(ES.TRIPLE, ES_TRIPLE);
const tele_op_t op_ES_MAGIC = MAKE_SIMPLE_I2C_OP(ES.MAGIC, ES_MAGIC);
