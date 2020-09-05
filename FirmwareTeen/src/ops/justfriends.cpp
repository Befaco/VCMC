//////////////////////////////////////
// Adapted from TeleType source code.
// See license on https://github.com/monome/teletype


#include "../../PrjIncludes.h"
/*
#include "ops/justfriends.h"

#include "helpers.h"
#include "ii.h"
#include "teletype_io.h"
*/
#ifdef USEI2C

////////////////////////////////

static void op_JF_TR_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_JF_RMODE_get(const void *data, scene_state_t *ss,
                            exec_state_t *es, command_state_t *cs);
static void op_JF_RUN_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_JF_SHIFT_get(const void *data, scene_state_t *ss,
                            exec_state_t *es, command_state_t *cs);
static void op_JF_VTR_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_JF_MODE_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_JF_TICK_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_JF_VOX_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_JF_NOTE_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_JF_GOD_get(const void *data, scene_state_t *ss, exec_state_t *es,
                          command_state_t *cs);
static void op_JF_TUNE_get(const void *data, scene_state_t *ss,
                           exec_state_t *es, command_state_t *cs);
static void op_JF_QT_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);

// clang-format off
const tele_op_t op_JF_TR    = MAKE_GET_OP(JF.TR    , op_JF_TR_get    , 2, false);
const tele_op_t op_JF_RMODE = MAKE_GET_OP(JF.RMODE , op_JF_RMODE_get , 1, false);
const tele_op_t op_JF_RUN   = MAKE_GET_OP(JF.RUN   , op_JF_RUN_get   , 1, false);
const tele_op_t op_JF_SHIFT = MAKE_GET_OP(JF.SHIFT , op_JF_SHIFT_get , 1, false);
const tele_op_t op_JF_VTR   = MAKE_GET_OP(JF.VTR   , op_JF_VTR_get   , 2, false);
const tele_op_t op_JF_MODE  = MAKE_GET_OP(JF.MODE  , op_JF_MODE_get  , 1, false);
const tele_op_t op_JF_TICK  = MAKE_GET_OP(JF.TICK  , op_JF_TICK_get  , 1, false);
const tele_op_t op_JF_VOX   = MAKE_GET_OP(JF.VOX   , op_JF_VOX_get   , 3, false);
const tele_op_t op_JF_NOTE  = MAKE_GET_OP(JF.NOTE  , op_JF_NOTE_get  , 2, false);
const tele_op_t op_JF_GOD   = MAKE_GET_OP(JF.GOD   , op_JF_GOD_get   , 1, false);
const tele_op_t op_JF_TUNE  = MAKE_GET_OP(JF.TUNE  , op_JF_TUNE_get  , 3, false);
const tele_op_t op_JF_QT    = MAKE_GET_OP(JF.QT    , op_JF_QT_get    , 1, false);
// clang-format on


static void op_JF_TR_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    uint8_t a = cs_pop(cs);
    uint8_t b = cs_pop(cs);
    uint8_t d[] = { JF_TR, a, b };
    tele_ii_tx(JF_ADDR, d, 3);
}

static void op_JF_RMODE_get(const void *NOTUSED(data),
                            scene_state_t *NOTUSED(ss),
                            exec_state_t *NOTUSED(es), command_state_t *cs) {
    uint8_t a = cs_pop(cs);
    uint8_t d[] = { JF_RMODE, a };
    tele_ii_tx(JF_ADDR, d, 2);
}

static void op_JF_RUN_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    uint8_t a = cs_pop(cs);
    uint8_t d[] = { JF_RUN, a >> 8, a & 0xff };
    tele_ii_tx(JF_ADDR, d, 3);
}

static void op_JF_SHIFT_get(const void *NOTUSED(data),
                            scene_state_t *NOTUSED(ss),
                            exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    uint8_t d[] = { JF_SHIFT, a >> 8, a & 0x00ff };
    tele_ii_tx(JF_ADDR, d, 3);
}

static void op_JF_VTR_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    uint8_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    uint8_t d[] = { JF_VTR, a, b >> 8, b & 0xff };
    tele_ii_tx(JF_ADDR, d, 4);
}

static void op_JF_MODE_get(const void *NOTUSED(data),
                           scene_state_t *NOTUSED(ss),
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    uint8_t a = cs_pop(cs);
    uint8_t d[] = { JF_MODE, a };
    tele_ii_tx(JF_ADDR, d, 2);
}

static void op_JF_TICK_get(const void *NOTUSED(data),
                           scene_state_t *NOTUSED(ss),
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    uint8_t a = cs_pop(cs);
    uint8_t d[] = { JF_TICK, a };
    tele_ii_tx(JF_ADDR, d, 2);
}

static void op_JF_VOX_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    int16_t c = cs_pop(cs);
    uint8_t d[] = { JF_VOX, a, b >> 8, b & 0xff, c >> 8, c & 0xff };
    tele_ii_tx(JF_ADDR, d, 6);
}

static void op_JF_NOTE_get(const void *NOTUSED(data),
                           scene_state_t *NOTUSED(ss),
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t a = cs_pop(cs);
    int16_t b = cs_pop(cs);
    uint8_t d[] = { JF_NOTE, a >> 8, a & 0xff, b >> 8, b & 0xff };
    tele_ii_tx(JF_ADDR, d, 5);
}

static void op_JF_GOD_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                          exec_state_t *NOTUSED(es), command_state_t *cs) {
    uint8_t a = cs_pop(cs);
    uint8_t d[] = { JF_GOD, a };
    tele_ii_tx(JF_ADDR, d, 2);
}

static void op_JF_TUNE_get(const void *NOTUSED(data),
                           scene_state_t *NOTUSED(ss),
                           exec_state_t *NOTUSED(es), command_state_t *cs) {
    uint8_t a = cs_pop(cs);
    uint8_t b = cs_pop(cs);
    uint8_t c = cs_pop(cs);
    uint8_t d[] = { JF_TUNE, a, b, c };
    tele_ii_tx(JF_ADDR, d, 4);
}

static void op_JF_QT_get(const void *NOTUSED(data), scene_state_t *NOTUSED(ss),
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    uint8_t a = cs_pop(cs);
    uint8_t d[] = { JF_QT, a };
    tele_ii_tx(JF_ADDR, d, 2);
}

#endif