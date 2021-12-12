
#include "../../PrjIncludes.h"

static void op_VC_CV_get(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);
static void op_VC_CV_set(const void *data, scene_state_t *ss, exec_state_t *es,
                         command_state_t *cs);

const tele_op_t op_VC_CV = MAKE_GET_SET_OP( VC.CV , op_VC_CV_get , op_VC_CV_set, 1, true);



static void op_VC_CV_get(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
       TXReceive(TI, cs, 0, true);
}


static void op_VC_CV_set(const void *NOTUSED(data), scene_state_t *ss,
                         exec_state_t *NOTUSED(es), command_state_t *cs) {
    // port: 1-9
    
}


