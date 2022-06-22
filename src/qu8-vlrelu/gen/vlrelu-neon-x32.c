// Auto-generated file. Do not edit!
//   Template: src/qs8-vlrelu/neon.c.in
//   Generator: tools/xngen
//
// Copyright 2022 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <arm_neon.h>

#include <xnnpack/common.h>
#include <xnnpack/vlrelu.h>


void xnn_qu8_vlrelu_ukernel__neon_x32(
    size_t n,
    const uint8_t* x,
    uint8_t* y,
    const union xnn_qu8_lrelu_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_OOB_READS
{
  assert(n != 0);
  assert(n % sizeof(uint8_t) == 0);
  assert(x != NULL);
  assert(y != NULL);

  const uint16x8_t vinput_zero_point = vld1q_dup_u16(&params->neon.input_zero_point);
  const int16x8_t vpositive_multiplier = vld1q_dup_s16(&params->neon.positive_multiplier);
  const int16x8_t vnegative_multiplier = vld1q_dup_s16(&params->neon.negative_multiplier);
  const int16x8_t voutput_zero_point = vld1q_dup_s16(&params->neon.output_zero_point);
  for (; n >= 32 * sizeof(uint8_t); n -= 32 * sizeof(uint8_t)) {
    const uint8x16_t vx0 = vld1q_u8(x); x += 16;
    const uint8x16_t vx1 = vld1q_u8(x); x += 16;

    int16x8_t vacc0 = vreinterpretq_s16_u16(vsubw_u8(vinput_zero_point, vget_low_u8(vx0)));
    int16x8_t vacc1 = vreinterpretq_s16_u16(vsubw_u8(vinput_zero_point, vget_high_u8(vx0)));
    int16x8_t vacc2 = vreinterpretq_s16_u16(vsubw_u8(vinput_zero_point, vget_low_u8(vx1)));
    int16x8_t vacc3 = vreinterpretq_s16_u16(vsubw_u8(vinput_zero_point, vget_high_u8(vx1)));

    const uint16x8_t vmask0 = vcltq_s16(vacc0, vmovq_n_s16(0));
    const uint16x8_t vmask1 = vcltq_s16(vacc1, vmovq_n_s16(0));
    const uint16x8_t vmask2 = vcltq_s16(vacc2, vmovq_n_s16(0));
    const uint16x8_t vmask3 = vcltq_s16(vacc3, vmovq_n_s16(0));

    vacc0 = vshlq_n_s16(vacc0, 7);
    vacc1 = vshlq_n_s16(vacc1, 7);
    vacc2 = vshlq_n_s16(vacc2, 7);
    vacc3 = vshlq_n_s16(vacc3, 7);

    const int16x8_t vmultiplier0 = vbslq_s16(vmask0, vnegative_multiplier, vpositive_multiplier);
    const int16x8_t vmultiplier1 = vbslq_s16(vmask1, vnegative_multiplier, vpositive_multiplier);
    const int16x8_t vmultiplier2 = vbslq_s16(vmask2, vnegative_multiplier, vpositive_multiplier);
    const int16x8_t vmultiplier3 = vbslq_s16(vmask3, vnegative_multiplier, vpositive_multiplier);

    vacc0 = vqrdmulhq_s16(vacc0, vmultiplier0);
    vacc1 = vqrdmulhq_s16(vacc1, vmultiplier1);
    vacc2 = vqrdmulhq_s16(vacc2, vmultiplier2);
    vacc3 = vqrdmulhq_s16(vacc3, vmultiplier3);

    vacc0 = vqaddq_s16(vacc0, voutput_zero_point);
    vacc1 = vqaddq_s16(vacc1, voutput_zero_point);
    vacc2 = vqaddq_s16(vacc2, voutput_zero_point);
    vacc3 = vqaddq_s16(vacc3, voutput_zero_point);

    const uint8x16_t vy0 = vcombine_u8(vqmovun_s16(vacc0), vqmovun_s16(vacc1));
    const uint8x16_t vy1 = vcombine_u8(vqmovun_s16(vacc2), vqmovun_s16(vacc3));

    vst1q_u8(y, vy0); y += 16;
    vst1q_u8(y, vy1); y += 16;
  }
  for (; n >= 8 * sizeof(uint8_t); n -= 8 * sizeof(uint8_t)) {
    const uint8x8_t vx = vld1_u8(x); x += 8;
    int16x8_t vacc = vreinterpretq_s16_u16(vsubw_u8(vinput_zero_point, vx));
    const uint16x8_t vmask = vcltq_s16(vacc, vmovq_n_s16(0));
    vacc = vshlq_n_s16(vacc, 7);
    const int16x8_t vmultiplier = vbslq_s16(vmask, vnegative_multiplier, vpositive_multiplier);
    vacc = vqrdmulhq_s16(vacc, vmultiplier);
    vacc = vqaddq_s16(vacc, voutput_zero_point);
    const uint8x8_t vy = vqmovun_s16(vacc);
    vst1_u8(y, vy); y += 8;
  }
  if XNN_UNLIKELY(n != 0) {
    assert(n >= 1 * sizeof(uint8_t));
    assert(n <= 7 * sizeof(uint8_t));

    const uint8x8_t vx = vld1_u8(x);
    int16x8_t vacc = vreinterpretq_s16_u16(vsubw_u8(vinput_zero_point, vx));
    const uint16x8_t vmask = vcltq_s16(vacc, vmovq_n_s16(0));
    vacc = vshlq_n_s16(vacc, 7);
    const int16x8_t vmultiplier = vbslq_s16(vmask, vnegative_multiplier, vpositive_multiplier);
    vacc = vqrdmulhq_s16(vacc, vmultiplier);
    vacc = vqaddq_s16(vacc, voutput_zero_point);
    uint8x8_t vy = vqmovun_s16(vacc);

    if (n & (4 * sizeof(uint8_t))) {
      vst1_lane_u32((void*) y, vreinterpret_u32_u8(vy), 0); y += 4;
      vy = vext_u8(vy, vy, 4);
    }
    if (n & (2 * sizeof(uint8_t))) {
      vst1_lane_u16((void*) y, vreinterpret_u16_u8(vy), 0); y += 2;
      vy = vext_u8(vy, vy, 2);
    }
    if (n & (1 * sizeof(uint8_t))) {
      vst1_lane_u8(y, vy, 0);
    }
  }
}
