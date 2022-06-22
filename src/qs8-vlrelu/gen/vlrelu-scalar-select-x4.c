// Auto-generated file. Do not edit!
//   Template: src/qs8-vlrelu/scalar-select.c.in
//   Generator: tools/xngen
//
// Copyright 2022 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <xnnpack/math.h>
#include <xnnpack/vlrelu.h>


void xnn_qs8_vlrelu_ukernel__scalar_select_x4(
    size_t n,
    const int8_t* x,
    int8_t* y,
    const union xnn_qs8_lrelu_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  const int32_t vinput_zero_point = params->scalar_select.input_zero_point;
  const int32_t vpositive_multiplier = params->scalar_select.positive_multiplier;
  const int32_t vnegative_multiplier = params->scalar_select.negative_multiplier;
  const int32_t vbias = params->scalar_select.bias;
  for (; n >= 4 * sizeof(int8_t); n -= 4 * sizeof(int8_t)) {
    int32_t vacc0 = (int32_t) x[0];
    int32_t vacc1 = (int32_t) x[1];
    int32_t vacc2 = (int32_t) x[2];
    int32_t vacc3 = (int32_t) x[3];
    x += 4;

    vacc0 -= vinput_zero_point;
    vacc1 -= vinput_zero_point;
    vacc2 -= vinput_zero_point;
    vacc3 -= vinput_zero_point;

    const int32_t vmultiplier0 = XNN_UNPREDICTABLE(vacc0 >= 0) ? vnegative_multiplier : vpositive_multiplier;
    const int32_t vmultiplier1 = XNN_UNPREDICTABLE(vacc1 >= 0) ? vnegative_multiplier : vpositive_multiplier;
    const int32_t vmultiplier2 = XNN_UNPREDICTABLE(vacc2 >= 0) ? vnegative_multiplier : vpositive_multiplier;
    const int32_t vmultiplier3 = XNN_UNPREDICTABLE(vacc3 >= 0) ? vnegative_multiplier : vpositive_multiplier;

    vacc0 = vbias + vacc0 * vmultiplier0;
    vacc1 = vbias + vacc1 * vmultiplier1;
    vacc2 = vbias + vacc2 * vmultiplier2;
    vacc3 = vbias + vacc3 * vmultiplier3;

    int32_t vout0 = asr_s32(vacc0, 8);
    int32_t vout1 = asr_s32(vacc1, 8);
    int32_t vout2 = asr_s32(vacc2, 8);
    int32_t vout3 = asr_s32(vacc3, 8);

    vout0 = math_max_s32(vout0, -128);
    vout1 = math_max_s32(vout1, -128);
    vout2 = math_max_s32(vout2, -128);
    vout3 = math_max_s32(vout3, -128);

    vout0 = math_min_s32(vout0, 127);
    vout1 = math_min_s32(vout1, 127);
    vout2 = math_min_s32(vout2, 127);
    vout3 = math_min_s32(vout3, 127);

    y[0] = (int8_t) vout0;
    y[1] = (int8_t) vout1;
    y[2] = (int8_t) vout2;
    y[3] = (int8_t) vout3;
    y += 4;
  }
  if XNN_UNLIKELY(n != 0) {
    do {
      int32_t vacc = (int32_t) *x++ - vinput_zero_point;
      const int32_t vmultiplier = XNN_UNPREDICTABLE(vacc >= 0) ? vnegative_multiplier : vpositive_multiplier;
      vacc = vbias + vacc * vmultiplier;

      int32_t vout = asr_s32(vacc, 8);
      vout = math_max_s32(vout, -128);
      vout = math_min_s32(vout, 127);
      *y++ = (int8_t) vout;

      n -= sizeof(int8_t);
    } while (n != 0);
  }
}
