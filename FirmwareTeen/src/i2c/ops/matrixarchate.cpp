#include "ops.h"
/*#include "ops/matrixarchate.h"

#include "helpers.h"
#include "ii.h"
#include "teletype.h"
#include "teletype_io.h"
*/
static void op_MA_SELECT_get(const void *data, scene_state_t *ss,
                             exec_state_t *es, command_state_t *cs);
static void op_MA_SELECT_set(const void *data, scene_state_t *ss,
                             exec_state_t *es, command_state_t *cs);

static void op_MA_STEP_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_MA_RESET_get(const void *data, scene_state_t *ss,
                            exec_state_t *es, command_state_t *cs);
static void op_MA_PGM_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);

static void op_MA_ON_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_MA_PON_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_MA_OFF_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_MA_POFF_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_MA_SET_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_MA_PSET_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);

static void op_MA_COL_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_MA_COL_set(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_MA_PCOL_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_MA_PCOL_set(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_MA_ROW_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_MA_ROW_set(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_MA_PROW_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_MA_PROW_set(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);

static void op_MA_CLR_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_MA_PCLR_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);

const tele_op_t op_MA_SELECT =
    MAKE_GET_SET_OP(MA.SELECT, op_MA_SELECT_get, op_MA_SELECT_set, 0, true);

const tele_op_t op_MA_STEP = MAKE_GET_OP(MA.STEP, op_MA_STEP_get, 0, false);
const tele_op_t op_MA_RESET = MAKE_GET_OP(MA.RESET, op_MA_RESET_get, 0, false);
const tele_op_t op_MA_PGM = MAKE_GET_OP(MA.PGM, op_MA_PGM_get, 1, false);

const tele_op_t op_MA_ON = MAKE_GET_OP(MA.ON, op_MA_ON_get, 2, false);
const tele_op_t op_MA_PON = MAKE_GET_OP(MA.PON, op_MA_PON_get, 3, false);
const tele_op_t op_MA_OFF = MAKE_GET_OP(MA.OFF, op_MA_OFF_get, 2, false);
const tele_op_t op_MA_POFF = MAKE_GET_OP(MA.POFF, op_MA_POFF_get, 3, false);
const tele_op_t op_MA_SET = MAKE_GET_OP(MA.SET, op_MA_SET_get, 3, false);
const tele_op_t op_MA_PSET = MAKE_GET_OP(MA.PSET, op_MA_PSET_get, 4, false);

const tele_op_t op_MA_COL =
    MAKE_GET_SET_OP(MA.COL, op_MA_COL_get, op_MA_COL_set, 1, true);
const tele_op_t op_MA_PCOL =
    MAKE_GET_SET_OP(MA.PCOL, op_MA_PCOL_get, op_MA_PCOL_set, 2, true);
const tele_op_t op_MA_ROW =
    MAKE_GET_SET_OP(MA.ROW, op_MA_ROW_get, op_MA_ROW_set, 1, true);
const tele_op_t op_MA_PROW =
    MAKE_GET_SET_OP(MA.PROW, op_MA_PROW_get, op_MA_PROW_set, 2, true);

const tele_op_t op_MA_CLR = MAKE_GET_OP(MA.CLR, op_MA_CLR_get, 0, false);
const tele_op_t op_MA_PCLR = MAKE_GET_OP(MA.PCLR, op_MA_PCLR_get, 1, false);

u8 selected_ma = 0;

static void ma_set(s16 row, s16 column, s16 value) {
    if (row < 0 || row > 15 || column < 0 || column > 7) return;
    uint8_t d[] = { (uint8_t)(value ? 0b10010000 : 0b10000000), (row << 3) + column, 128 };
    tele_ii_tx(MATRIXARCHATE + selected_ma, d, 3);
}

static void ma_set_pgm(s16 program, s16 row, s16 column, s16 value) {
    if (program < 0 || program > 59 || row < 0 || row > 15 || column < 0 ||
        column > 7)
        return;
    uint8_t d[] = { (uint8_t)(value ? 0b10010000 : 0b10000000), (row << 3) + column,
                    (uint8_t)(program) };
    tele_ii_tx(MATRIXARCHATE + selected_ma, d, 3);
}

static void ma_set_col(s16 column, u16 value) {
    if (column < 0 || column > 7) return;
    uint8_t d[] = { (uint8_t)(0b10110000), column, 128, value & 255, value >> 8 };
    tele_ii_tx(MATRIXARCHATE + selected_ma, d, 5);
}

static void ma_set_col_pgm(s16 program, s16 column, u16 value) {
    if (program < 0 || program > 59 || column < 0 || column > 7) return;
    uint8_t d[] = { 0b10110000, column, program, value & 255, (uint8_t)(value >> 8) };
    tele_ii_tx(MATRIXARCHATE + selected_ma, d, 5);
}

static void ma_set_row(s16 row, u16 value) {
    if (row < 0 || row > 15) return;
    uint8_t d[] = { (uint8_t)(0b10110000), row | 128, 128, (uint8_t)(value & 255), (uint8_t)(value >> 8) };
    tele_ii_tx(MATRIXARCHATE + selected_ma, d, 5);
}

static void ma_set_row_pgm(s16 program, s16 row, u16 value) {
    if (program < 0 || program > 59 || row < 0 || row > 15) return;
    uint8_t d[] = { 0b10110000, row | 128, program, value & 255, value >> 8 };
    tele_ii_tx(MATRIXARCHATE + selected_ma, d, 5);
}

static void op_MA_SELECT_get(const void *NOTUSED(data), scene_state_t *ss,
                             exec_state_t *NOTUSED(es), command_state_t *cs) {
    cs_push(cs, selected_ma + 1);
}

static void op_MA_SELECT_set(const void *NOTUSED(data), scene_state_t *ss,
                             exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 i = cs_pop(cs) - 1;
    if (i < 0 || i > 2) return;
    selected_ma = i;
}

static void op_MA_STEP_get(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    uint8_t d[] = { 0b11111000 };
    tele_ii_tx(MATRIXARCHATE + selected_ma, d, 1);
}

static void op_MA_RESET_get(const void *NOTUSED(data), scene_state_t *ss,
                            exec_state_t *NOTUSED(es), command_state_t *cs) {
    uint8_t d[] = { 0b11111101 };
    tele_ii_tx(MATRIXARCHATE + selected_ma, d, 1);
}

static void op_MA_PGM_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 program = cs_pop(cs) - 1;
    if (program < 0 || program > 59) return;
    uint8_t d[] = { 0b11000000, program };
    tele_ii_tx(MATRIXARCHATE + selected_ma, d, 2);
}

static void op_MA_ON_get(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 row = cs_pop(cs);
    s16 column = cs_pop(cs);
    ma_set(row, column, 1);
}

static void op_MA_PON_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 program = cs_pop(cs) - 1;
    s16 row = cs_pop(cs);
    s16 column = cs_pop(cs);
    ma_set_pgm(program, row, column, 1);
}

static void op_MA_OFF_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 row = cs_pop(cs);
    s16 column = cs_pop(cs);
    ma_set(row, column, 0);
}

static void op_MA_POFF_get(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 program = cs_pop(cs) - 1;
    s16 row = cs_pop(cs);
    s16 column = cs_pop(cs);
    ma_set_pgm(program, row, column, 0);
}

static void op_MA_SET_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 row = cs_pop(cs);
    s16 column = cs_pop(cs);
    s16 value = cs_pop(cs);
    ma_set(row, column, value);
}

static void op_MA_PSET_get(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 program = cs_pop(cs) - 1;
    s16 row = cs_pop(cs);
    s16 column = cs_pop(cs);
    s16 value = cs_pop(cs);
    ma_set_pgm(program, row, column, value);
}

static void op_MA_COL_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 column = cs_pop(cs);
    u16 value = 0;
    if (column >= 0 && column <= 7) {
        uint8_t d[] = { 0b11110101, column, 128 };
        tele_ii_tx(MATRIXARCHATE + selected_ma, d, 3);
        d[0] = 0;
        d[1] = 0;
        tele_ii_rx(MATRIXARCHATE + selected_ma, d, 2);
        value = (d[1] << 8) + d[0];
    }
    cs_push(cs, value);
}

static void op_MA_COL_set(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 column = cs_pop(cs);
    u16 value = cs_pop(cs);
    ma_set_col(column, value);
}

static void op_MA_PCOL_get(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 program = cs_pop(cs) - 1;
    s16 column = cs_pop(cs);
    u16 value = 0;
    if (column >= 0 && column <= 7 && program >= 0 && program <= 59) {
        uint8_t d[] = { 0b11110101, column, program };
        tele_ii_tx(MATRIXARCHATE + selected_ma, d, 3);
        d[0] = 0;
        d[1] = 0;
        tele_ii_rx(MATRIXARCHATE + selected_ma, d, 2);
        value = (d[1] << 8) + d[0];
    }
    cs_push(cs, value);
}

static void op_MA_PCOL_set(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 program = cs_pop(cs) - 1;
    s16 column = cs_pop(cs);
    u16 value = cs_pop(cs);
    ma_set_col_pgm(program, column, value);
}

static void op_MA_ROW_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 row = cs_pop(cs);
    u16 value = 0;
    if (row >= 0 && row <= 15) {
        uint8_t d[] = { 0b11110101, row | 128, 128 };
        tele_ii_tx(MATRIXARCHATE + selected_ma, d, 3);
        d[0] = 0;
        d[1] = 0;
        tele_ii_rx(MATRIXARCHATE + selected_ma, d, 2);
        value = (d[1] << 8) + d[0];
    }
    cs_push(cs, value);
}

static void op_MA_ROW_set(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 row = cs_pop(cs);
    u16 value = cs_pop(cs);
    ma_set_row(row, value);
}

static void op_MA_PROW_get(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 program = cs_pop(cs) - 1;
    s16 row = cs_pop(cs);
    u16 value = 0;
    if (row >= 0 && row <= 15 && program >= 0 && program <= 59) {
        uint8_t d[] = { 0b11110101, row | 128, program };
        tele_ii_tx(MATRIXARCHATE + selected_ma, d, 3);
        d[0] = 0;
        d[1] = 0;
        tele_ii_rx(MATRIXARCHATE + selected_ma, d, 2);
        value = (d[1] << 8) + d[0];
    }
    cs_push(cs, value);
}

static void op_MA_PROW_set(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 program = cs_pop(cs) - 1;
    s16 row = cs_pop(cs);
    u16 value = cs_pop(cs);
    ma_set_row_pgm(program, row, value);
}

static void op_MA_CLR_get(const void *NOTUSED(data), scene_state_t *ss,
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    for (u8 i = 0; i < 8; i++) ma_set_col(i, 0);
}

static void op_MA_PCLR_get(const void *NOTUSED(data), scene_state_t *ss,
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    s16 program = cs_pop(cs) - 1;
    for (u8 i = 0; i < 8; i++) ma_set_col_pgm(program, i, 0);
}
