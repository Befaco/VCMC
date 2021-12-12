//////////////////////////////////////
// Adapted from TeleType source code.
// See license on https://github.com/monome/teletype


#include "../../../PrjIncludes.h"


bool tele_opsJFChan[] = { // Command expects channel info
    1, 0, 0, 0, 1, 0,
    0, 1, 0, 0, 1, 0};


/////////////////////////////////////////////////////////////////
// OPS //////////////////////////////////////////////////////////

// If you edit this array, you need to run 'utils/op_enums.py' to update the
// values in 'op_enum.h' so that the order matches.
const tele_op_t *tele_ops[E_OP__LENGTH] = { 
    // variables
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,

    // init
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,

    // turtle
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,

    // metronome
    NULL, NULL, NULL, NULL,

    // patterns
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,

    // queue
    NULL, NULL, NULL,

    // hardware
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL,

    // maths
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL,
    NULL, NULL, NULL,
    NULL, NULL, NULL,
    NULL, NULL,
    NULL, NULL, NULL,

    // stack
    NULL, NULL, NULL, NULL,

    // controlflow
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,

    // delay
    NULL,

    // i2c
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL,

    // whitewhale
    &op_WW_PRESET, &op_WW_POS, &op_WW_SYNC, &op_WW_START, &op_WW_END,
    &op_WW_PMODE, &op_WW_PATTERN, &op_WW_QPATTERN, &op_WW_MUTE1, &op_WW_MUTE2,
    &op_WW_MUTE3, &op_WW_MUTE4, &op_WW_MUTEA, &op_WW_MUTEB,

    // meadowphysics
    &op_MP_PRESET, &op_MP_RESET, &op_MP_STOP,

    // earthsea
    &op_ES_PRESET, &op_ES_MODE, &op_ES_CLOCK, &op_ES_RESET, &op_ES_PATTERN,
    &op_ES_TRANS, &op_ES_STOP, &op_ES_TRIPLE, &op_ES_MAGIC,

    // orca
    &op_OR_TRK, &op_OR_CLK, &op_OR_DIV, &op_OR_PHASE, &op_OR_RST, &op_OR_WGT,
    &op_OR_MUTE, &op_OR_SCALE, &op_OR_BANK, &op_OR_PRESET, &op_OR_RELOAD,
    &op_OR_ROTS, &op_OR_ROTW, &op_OR_GRST, &op_OR_CVA, &op_OR_CVB,

    // ansible
    &op_ANS_G_LED, &op_ANS_G, &op_ANS_G_P,
    &op_ANS_A_LED, &op_ANS_A,
    &op_ANS_APP,
    &op_KR_PRE, &op_KR_PAT, &op_KR_SCALE, &op_KR_PERIOD, &op_KR_POS,
    &op_KR_L_ST, &op_KR_L_LEN, &op_KR_RES, &op_KR_CV, &op_KR_MUTE, &op_KR_TMUTE,
    &op_KR_CLK, &op_KR_PG, &op_KR_CUE, &op_KR_DIR, &op_KR_DUR,
    &op_ME_PRE, &op_ME_RES, &op_ME_STOP, &op_ME_SCALE,
    &op_ME_PERIOD, &op_ME_CV, &op_LV_PRE, &op_LV_RES, &op_LV_POS, &op_LV_L_ST,
    &op_LV_L_LEN, &op_LV_L_DIR, &op_LV_CV, &op_CY_PRE, &op_CY_RES, &op_CY_POS,
    &op_CY_REV, &op_CY_CV, &op_MID_SHIFT, &op_MID_SLEW, &op_ARP_STY,
    &op_ARP_HLD, &op_ARP_RPT, &op_ARP_GT, &op_ARP_DIV, &op_ARP_RES,
    &op_ARP_SHIFT, &op_ARP_SLEW, &op_ARP_FIL, &op_ARP_ROT, &op_ARP_ER,

    // justfriends
    &op_JF_TR, &op_JF_RMODE, &op_JF_RUN, &op_JF_SHIFT, &op_JF_VTR, &op_JF_MODE,
    &op_JF_TICK, &op_JF_VOX, &op_JF_NOTE, &op_JF_GOD, &op_JF_TUNE, &op_JF_QT,

    // W/
    &op_WS_PLAY, &op_WS_REC, &op_WS_CUE, &op_WS_LOOP,

    // telex
    &op_TO_TR, &op_TO_TR_TOG, &op_TO_TR_PULSE, &op_TO_TR_TIME, &op_TO_TR_TIME_S,
    &op_TO_TR_TIME_M, &op_TO_TR_POL, &op_TO_KILL,

    &op_TO_TR_PULSE_DIV, &op_TO_TR_PULSE_MUTE, &op_TO_TR_M_MUL,

    &op_TO_M, &op_TO_M_S, &op_TO_M_M, &op_TO_M_BPM, &op_TO_M_ACT, &op_TO_M_SYNC,
    &op_TO_M_COUNT,

    &op_TO_TR_M, &op_TO_TR_M_S, &op_TO_TR_M_M, &op_TO_TR_M_BPM, &op_TO_TR_M_ACT,
    &op_TO_TR_M_SYNC, &op_TO_TR_WIDTH, &op_TO_TR_M_COUNT,

    &op_TO_CV, &op_TO_CV_SLEW, &op_TO_CV_SLEW_S, &op_TO_CV_SLEW_M,
    &op_TO_CV_SET, &op_TO_CV_OFF, &op_TO_CV_QT, &op_TO_CV_QT_SET, &op_TO_CV_N,
    &op_TO_CV_N_SET, &op_TO_CV_SCALE, &op_TO_CV_LOG,

    &op_TO_CV_INIT, &op_TO_TR_INIT, &op_TO_INIT,

    &op_TO_TR_P, &op_TO_TR_P_DIV, &op_TO_TR_P_MUTE,

    &op_TO_OSC, &op_TO_OSC_SET, &op_TO_OSC_QT, &op_TO_OSC_QT_SET, &op_TO_OSC_FQ,
    &op_TO_OSC_FQ_SET, &op_TO_OSC_N, &op_TO_OSC_N_SET, &op_TO_OSC_LFO,
    &op_TO_OSC_LFO_SET, &op_TO_OSC_WAVE, &op_TO_OSC_SYNC, &op_TO_OSC_PHASE,
    &op_TO_OSC_WIDTH, &op_TO_OSC_RECT, &op_TO_OSC_SLEW, &op_TO_OSC_SLEW_S,
    &op_TO_OSC_SLEW_M, &op_TO_OSC_SCALE, &op_TO_OSC_CYC, &op_TO_OSC_CYC_S,
    &op_TO_OSC_CYC_M, &op_TO_OSC_CYC_SET, &op_TO_OSC_CYC_S_SET,
    &op_TO_OSC_CYC_M_SET, &op_TO_OSC_CTR,

    &op_TO_ENV_ACT, &op_TO_ENV_ATT, &op_TO_ENV_ATT_S, &op_TO_ENV_ATT_M,
    &op_TO_ENV_DEC, &op_TO_ENV_DEC_S, &op_TO_ENV_DEC_M, &op_TO_ENV_TRIG,
    &op_TO_ENV_EOR, &op_TO_ENV_EOC, &op_TO_ENV_LOOP,

    &op_TO_ENV, &op_TO_CV_CALIB, &op_TO_CV_RESET,

    &op_TI_PARAM, &op_TI_PARAM_QT, &op_TI_PARAM_N, &op_TI_PARAM_SCALE,
    &op_TI_PARAM_MAP, &op_TI_IN, &op_TI_IN_QT, &op_TI_IN_N, &op_TI_IN_SCALE,
    &op_TI_IN_MAP, &op_TI_PARAM_CALIB, &op_TI_IN_CALIB, &op_TI_STORE,
    &op_TI_RESET,

    &op_TI_PARAM_INIT, &op_TI_IN_INIT, &op_TI_INIT,

    &op_TI_PRM, &op_TI_PRM_QT, &op_TI_PRM_N, &op_TI_PRM_SCALE, &op_TI_PRM_MAP,
    &op_TI_PRM_INIT,
 
    // fader
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
 
    // ER301
    &op_SC_TR, &op_SC_TR_TOG, &op_SC_TR_PULSE, &op_SC_TR_TIME, &op_SC_TR_POL,
    &op_SC_CV, &op_SC_CV_SLEW, &op_SC_CV_SET, &op_SC_CV_OFF, &op_SC_TR_P,

    // grid
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
 
    // matrixarchate
    &op_MA_SELECT, &op_MA_STEP, &op_MA_RESET, &op_MA_PGM, &op_MA_ON, &op_MA_PON,
    &op_MA_OFF, &op_MA_POFF, &op_MA_SET, &op_MA_PSET, &op_MA_COL, &op_MA_PCOL,
    &op_MA_ROW, &op_MA_PROW, &op_MA_CLR, &op_MA_PCLR,

    // disting ex
    &op_EX, &op_EX_PRESET, &op_EX_PRE, &op_EX_SAVE, &op_EX_RESET, &op_EX_ALG,
    &op_EX_A, &op_EX_CTRL, &op_EX_C, &op_EX_PARAM, &op_EX_P, &op_EX_PV,
    &op_EX_MIN, &op_EX_MAX, &op_EX_REC, &op_EX_PLAY, &op_EX_AL_P, &op_EX_AL_CLK,
    &op_EX_M_CH, &op_EX_M_N, &op_EX_M_NO, &op_EX_M_PB, &op_EX_M_CC,
    &op_EX_M_PRG, &op_EX_M_CLK, &op_EX_M_START, &op_EX_M_STOP, &op_EX_M_CONT,
    &op_EX_SB_CH, &op_EX_SB_N, &op_EX_SB_NO, &op_EX_SB_PB, &op_EX_SB_CC,
    &op_EX_SB_PRG, &op_EX_SB_CLK, &op_EX_SB_START, &op_EX_SB_STOP,
    &op_EX_SB_CONT, &op_EX_VOX_P, &op_EX_VP, &op_EX_VOX, &op_EX_V, &op_EX_VOX_O,
    &op_EX_VO, &op_EX_NOTE, &op_EX_N, &op_EX_NOTE_O, &op_EX_NO, &op_EX_ALLOFF,
    &op_EX_AO, &op_EX_T, &op_EX_TV,

    // seed
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL 
};


void op_simple_i2c(const void *data, scene_state_t *NOTUSED(ss),
                   exec_state_t *NOTUSED(es), command_state_t *cs) {
    int16_t message = (intptr_t)data;
    int16_t value = cs_pop(cs);

    uint8_t address = message & 0xF0;
    uint8_t message_type = message & 0xFF;

    uint8_t buffer[3] = { message_type, value >> 8, value & 0xFF };

    tele_ii_tx(address, buffer, 3);
}




/*
const tele_op_t *tele_opsJF[] = {
        // justfriends
        &op_JF_TR, &op_JF_RMODE, &op_JF_RUN, &op_JF_SHIFT, &op_JF_VTR, &op_JF_MODE,
        &op_JF_TICK, &op_JF_VOX, &op_JF_NOTE, &op_JF_GOD, &op_JF_TUNE, &op_JF_QT
        };

const tele_op_t *tele_opsTO[] = {
    // telex TO
    &op_TO_TR, &op_TO_TR_TOG, &op_TO_TR_PULSE, &op_TO_TR_TIME, &op_TO_TR_TIME_S,
    &op_TO_TR_TIME_M, &op_TO_TR_POL, &op_TO_KILL,

    &op_TO_TR_PULSE_DIV, &op_TO_TR_PULSE_MUTE, &op_TO_TR_M_MUL,

    &op_TO_M, &op_TO_M_S, &op_TO_M_M, &op_TO_M_BPM, &op_TO_M_ACT, &op_TO_M_SYNC,
    &op_TO_M_COUNT,

    &op_TO_TR_M, &op_TO_TR_M_S, &op_TO_TR_M_M, &op_TO_TR_M_BPM, &op_TO_TR_M_ACT,
    &op_TO_TR_M_SYNC, &op_TO_TR_WIDTH, &op_TO_TR_M_COUNT,

    &op_TO_CV, &op_TO_CV_SLEW, &op_TO_CV_SLEW_S, &op_TO_CV_SLEW_M,
    &op_TO_CV_SET, &op_TO_CV_OFF, &op_TO_CV_QT, &op_TO_CV_QT_SET, &op_TO_CV_N,
    &op_TO_CV_N_SET, &op_TO_CV_SCALE, &op_TO_CV_LOG,

    &op_TO_CV_INIT, &op_TO_TR_INIT, &op_TO_INIT,

    &op_TO_TR_P, &op_TO_TR_P_DIV, &op_TO_TR_P_MUTE,

    &op_TO_OSC, &op_TO_OSC_SET, &op_TO_OSC_QT, &op_TO_OSC_QT_SET, &op_TO_OSC_FQ,
    &op_TO_OSC_FQ_SET, &op_TO_OSC_N, &op_TO_OSC_N_SET, &op_TO_OSC_LFO,
    &op_TO_OSC_LFO_SET, &op_TO_OSC_WAVE, &op_TO_OSC_SYNC, &op_TO_OSC_PHASE,
    &op_TO_OSC_WIDTH, &op_TO_OSC_RECT, &op_TO_OSC_SLEW, &op_TO_OSC_SLEW_S,
    &op_TO_OSC_SLEW_M, &op_TO_OSC_SCALE, &op_TO_OSC_CYC, &op_TO_OSC_CYC_S,
    &op_TO_OSC_CYC_M, &op_TO_OSC_CYC_SET, &op_TO_OSC_CYC_S_SET,
    &op_TO_OSC_CYC_M_SET, &op_TO_OSC_CTR,

    &op_TO_ENV_ACT, &op_TO_ENV_ATT, &op_TO_ENV_ATT_S, &op_TO_ENV_ATT_M,
    &op_TO_ENV_DEC, &op_TO_ENV_DEC_S, &op_TO_ENV_DEC_M, &op_TO_ENV_TRIG,
    &op_TO_ENV_EOR, &op_TO_ENV_EOC, &op_TO_ENV_LOOP,

    &op_TO_ENV, &op_TO_CV_CALIB, &op_TO_CV_RESET,
};
 const tele_op_t *tele_opsTi[] = {
    // telex TI
    &op_TI_PARAM, &op_TI_PARAM_QT, &op_TI_PARAM_N, &op_TI_PARAM_SCALE,
    &op_TI_PARAM_MAP, &op_TI_IN, &op_TI_IN_QT, &op_TI_IN_N, &op_TI_IN_SCALE,
    &op_TI_IN_MAP, &op_TI_PARAM_CALIB, &op_TI_IN_CALIB, &op_TI_STORE,
    &op_TI_RESET,

    &op_TI_PARAM_INIT, &op_TI_IN_INIT, &op_TI_INIT,

    &op_TI_PRM, &op_TI_PRM_QT, &op_TI_PRM_N, &op_TI_PRM_SCALE, &op_TI_PRM_MAP,
    &op_TI_PRM_INIT
    };
*/
