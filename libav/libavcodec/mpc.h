/*
 * Musepack decoder
 * Copyright (c) 2006 Konstantin Shishkov
 *
 * This file is part of Libav.
 *
 * Libav is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Libav is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Libav; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * Musepack decoder
 * MPEG Audio Layer 1/2 -like codec with frames of 1152 samples
 * divided into 32 subbands.
 */

#ifndef AVCODEC_MPC_H
#define AVCODEC_MPC_H

#include "libavutil/lfg.h"
#include "avcodec.h"
#include "get_bits.h"
#include "dsputil.h"
#include "mpegaudio.h"
#include "mpegaudiodsp.h"

#define BANDS            32
#define SAMPLES_PER_BAND 36
#define MPC_FRAME_SIZE   (BANDS * SAMPLES_PER_BAND)

/** Subband structure - hold all variables for each subband */
typedef struct {
    int msf; ///< mid-stereo flag
    int res[2];
    int scfi[2];
    int scf_idx[2][3];
    int Q[2];
}Band;

typedef struct {
    DSPContext dsp;
    MPADSPContext mpadsp;
    GetBitContext gb;
    int IS, MSS, gapless;
    int lastframelen;
    int maxbands, last_max_band;
    int last_bits_used;
    int oldDSCF[2][BANDS];
    Band bands[BANDS];
    int Q[2][MPC_FRAME_SIZE];
    int cur_frame, frames;
    uint8_t *bits;
    int buf_size;
    AVLFG rnd;
    int frames_to_skip;
    /* for synthesis */
    DECLARE_ALIGNED(16, MPA_INT, synth_buf)[MPA_MAX_CHANNELS][512*2];
    int synth_buf_offset[MPA_MAX_CHANNELS];
    DECLARE_ALIGNED(16, int32_t, sb_samples)[MPA_MAX_CHANNELS][36][SBLIMIT];
} MPCContext;

void ff_mpc_init(void);
void ff_mpc_dequantize_and_synth(MPCContext *c, int maxband, void *dst, int channels);

#endif /* AVCODEC_MPC_H */