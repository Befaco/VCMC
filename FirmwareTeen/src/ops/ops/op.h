#ifndef _OPS_OP_H_
#define _OPS_OP_H_


#include <stdbool.h>
#include <stddef.h>

#include "command.h"
#include "op_enum.h"
#include "state.h"
extern void tele_ii_tx(uint8_t addr, uint8_t *data, uint8_t l);
extern void tele_ii_rx(uint8_t addr, uint8_t *data, uint8_t l);

#ifdef __GNUC__
#define NOTUSED(x) UNUSED_##x __attribute__((__unused__))
#else
#define NOTUSED(x) UNUSED_##x
#endif

/////////////////////////////////////////
// op.h
/////////////////////////////////////////
typedef struct {
    const char *name;
    void (*const get)(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
    void (*const set)(const void *data, scene_state_t *ss, exec_state_t *es,
                      command_state_t *cs);
    const uint8_t params;
    const bool returns;
    const void *data;
} tele_op_t;


// Get only ops
#define MAKE_GET_OP(n, g, p, r)                                       \
    {                                                                 \
        .name = #n, .get = g, .set = NULL, .params = p, .returns = r, \
        .data = NULL                                                  \
    }

// Get & set ops
#define MAKE_GET_SET_OP(n, g, s, p, r) \
    { .name = #n, .get = g, .set = s, .params = p, .returns = r, .data = NULL }

// Variables, peek & poke
#define MAKE_SIMPLE_VARIABLE_OP(n, v)                                    \
    {                                                                    \
        .name = #n, .get = op_peek_i16, .set = op_poke_i16, .params = 0, \
        .returns = 1, .data = (void *)offsetof(scene_state_t, v)         \
    }

// Alias one OP to another
#define MAKE_ALIAS_OP(n, g, s, p, r) \
    { .name = #n, .get = g, .set = s, .params = p, .returns = r, .data = NULL }

// Simple I2C op (to support the original Trilogy modules)
#define MAKE_SIMPLE_I2C_OP(n, v)                                    \
    {                                                               \
        .name = #n, .get = op_simple_i2c, .set = NULL, .params = 1, \
        .returns = 0, .data = (void *)v                             \
    }

void op_simple_i2c(const void *data, scene_state_t *ss, exec_state_t *es,
                   command_state_t *cs);

// Mods
#define MAKE_MOD(n, f, p) \
    { .name = #n, .func = f, .params = p }
//////////////////////////////

#endif
