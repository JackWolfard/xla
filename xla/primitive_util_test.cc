/* Copyright 2018 The OpenXLA Authors.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "xla/primitive_util.h"

#include <string>

#include "xla/hlo/testlib/test.h"
#include "xla/hlo/testlib/test_helpers.h"
#include "xla/xla_data.pb.h"
#include "tsl/platform/statusor.h"

namespace xla {
namespace {

TEST(PrimitiveUtilTest, StringToPrimitiveType) {
  auto expect_ok_and_equal = [](const std::string& str,
                                PrimitiveType expected) {
    TF_ASSERT_OK_AND_ASSIGN(PrimitiveType actual,
                            primitive_util::StringToPrimitiveType(str));
    EXPECT_EQ(expected, actual);
  };
  expect_ok_and_equal("f32", F32);
  expect_ok_and_equal("tuple", TUPLE);
  expect_ok_and_equal("pred", PRED);
  expect_ok_and_equal("s32", S32);

  EXPECT_IS_NOT_OK(primitive_util::StringToPrimitiveType("F32").status());
  EXPECT_IS_NOT_OK(primitive_util::StringToPrimitiveType("Pred").status());
  EXPECT_IS_NOT_OK(primitive_util::StringToPrimitiveType("preD").status());
}

TEST(PrimitiveUtilTest, FloatTypes) {
  EXPECT_EQ(primitive_util::SignificandWidth(F32), 24);
  EXPECT_EQ(primitive_util::SignificandWidth(BF16), 8);
  EXPECT_EQ(primitive_util::ExponentWidth(F32), 8);
  EXPECT_EQ(primitive_util::ExponentWidth(BF16), 8);
  EXPECT_EQ(primitive_util::UnderflowExponent(F32), -125);
  EXPECT_EQ(primitive_util::UnderflowExponent(BF16), -125);
  EXPECT_EQ(primitive_util::OverflowExponent(F32), 128);
  EXPECT_EQ(primitive_util::OverflowExponent(BF16), 128);
}

TEST(PrimitiveUtilTest, CastPreservesValues) {
  bool expecteds[PrimitiveType_ARRAYSIZE][PrimitiveType_ARRAYSIZE];
  expecteds[PRED][PRED] = expecteds[PRED][S1] = true;
  expecteds[PRED][S2] = expecteds[PRED][S4] = true;
  expecteds[PRED][S8] = expecteds[PRED][S16] = true;
  expecteds[PRED][S32] = expecteds[PRED][S64] = true;
  expecteds[PRED][U1] = expecteds[PRED][U2] = true;
  expecteds[PRED][U4] = expecteds[PRED][U8] = true;
  expecteds[PRED][U16] = expecteds[PRED][U32] = true;
  expecteds[PRED][U64] = expecteds[PRED][F16] = true;
  expecteds[PRED][F32] = expecteds[PRED][F64] = true;
  expecteds[PRED][C64] = expecteds[PRED][BF16] = true;
  expecteds[PRED][C128] = expecteds[PRED][F8E5M2] = true;
  expecteds[PRED][F8E4M3] = expecteds[PRED][F8E4M3FN] = true;
  expecteds[PRED][F8E4M3B11FNUZ] = expecteds[PRED][F8E5M2FNUZ] = true;
  expecteds[PRED][F8E4M3FNUZ] = expecteds[PRED][F8E3M4] = true;
  expecteds[PRED][F4E2M1FN] = true;
  expecteds[PRED][F8E8M0FNU] = false;
  expecteds[S1][PRED] = false;
  expecteds[S1][S1] = true;
  expecteds[S1][S2] = true;
  expecteds[S1][S4] = true;
  expecteds[S1][S8] = true;
  expecteds[S1][S16] = true;
  expecteds[S1][S32] = true;
  expecteds[S1][S64] = true;
  expecteds[S1][U1] = false;
  expecteds[S1][U2] = false;
  expecteds[S1][U4] = false;
  expecteds[S1][U8] = false;
  expecteds[S1][U16] = false;
  expecteds[S1][U32] = false;
  expecteds[S1][U64] = false;
  expecteds[S1][F16] = true;
  expecteds[S1][F32] = true;
  expecteds[S1][F64] = true;
  expecteds[S1][C64] = true;
  expecteds[S1][BF16] = true;
  expecteds[S1][C128] = true;
  expecteds[S1][F4E2M1FN] = true;
  expecteds[S1][F8E5M2] = true;
  expecteds[S1][F8E4M3] = true;
  expecteds[S1][F8E4M3FN] = true;
  expecteds[S1][F8E4M3B11FNUZ] = true;
  expecteds[S1][F8E5M2FNUZ] = true;
  expecteds[S1][F8E4M3FNUZ] = true;
  expecteds[S1][F8E3M4] = true;
  expecteds[S1][F8E8M0FNU] = false;
  expecteds[S2][PRED] = false;
  expecteds[S2][S1] = false;
  expecteds[S2][S2] = true;
  expecteds[S2][S4] = true;
  expecteds[S2][S8] = true;
  expecteds[S2][S16] = true;
  expecteds[S2][S32] = true;
  expecteds[S2][S64] = true;
  expecteds[S2][U1] = false;
  expecteds[S2][U2] = false;
  expecteds[S2][U4] = false;
  expecteds[S2][U8] = false;
  expecteds[S2][U16] = false;
  expecteds[S2][U32] = false;
  expecteds[S2][U64] = false;
  expecteds[S2][F16] = true;
  expecteds[S2][F32] = true;
  expecteds[S2][F64] = true;
  expecteds[S2][C64] = true;
  expecteds[S2][BF16] = true;
  expecteds[S2][C128] = true;
  expecteds[S2][F4E2M1FN] = true;
  expecteds[S2][F8E5M2] = true;
  expecteds[S2][F8E4M3] = true;
  expecteds[S2][F8E4M3FN] = true;
  expecteds[S2][F8E4M3B11FNUZ] = true;
  expecteds[S2][F8E5M2FNUZ] = true;
  expecteds[S2][F8E4M3FNUZ] = true;
  expecteds[S2][F8E3M4] = true;
  expecteds[S2][F8E8M0FNU] = false;
  expecteds[S4][PRED] = false;
  expecteds[S4][S1] = false;
  expecteds[S4][S2] = false;
  expecteds[S4][S4] = true;
  expecteds[S4][S8] = true;
  expecteds[S4][S16] = true;
  expecteds[S4][S32] = true;
  expecteds[S4][S64] = true;
  expecteds[S4][U1] = false;
  expecteds[S4][U2] = false;
  expecteds[S4][U4] = false;
  expecteds[S4][U8] = false;
  expecteds[S4][U16] = false;
  expecteds[S4][U32] = false;
  expecteds[S4][U64] = false;
  expecteds[S4][F16] = true;
  expecteds[S4][F32] = true;
  expecteds[S4][F64] = true;
  expecteds[S4][C64] = true;
  expecteds[S4][BF16] = true;
  expecteds[S4][C128] = true;
  expecteds[S4][F4E2M1FN] = false;
  expecteds[S4][F8E5M2] = true;
  expecteds[S4][F8E4M3] = true;
  expecteds[S4][F8E4M3FN] = true;
  expecteds[S4][F8E4M3B11FNUZ] = true;
  expecteds[S4][F8E5M2FNUZ] = true;
  expecteds[S4][F8E4M3FNUZ] = true;
  expecteds[S4][F8E3M4] = true;
  expecteds[S4][F8E8M0FNU] = false;
  expecteds[S8][PRED] = false;
  expecteds[S8][S1] = false;
  expecteds[S8][S2] = false;
  expecteds[S8][S4] = false;
  expecteds[S8][S8] = true;
  expecteds[S8][S16] = true;
  expecteds[S8][S32] = true;
  expecteds[S8][S64] = true;
  expecteds[S8][U1] = false;
  expecteds[S8][U2] = false;
  expecteds[S8][U4] = false;
  expecteds[S8][U8] = false;
  expecteds[S8][U16] = false;
  expecteds[S8][U32] = false;
  expecteds[S8][U64] = false;
  expecteds[S8][F16] = true;
  expecteds[S8][F32] = true;
  expecteds[S8][F64] = true;
  expecteds[S8][C64] = true;
  expecteds[S8][BF16] = true;
  expecteds[S8][C128] = true;
  expecteds[S8][F4E2M1FN] = false;
  expecteds[S8][F8E5M2] = false;
  expecteds[S8][F8E4M3] = false;
  expecteds[S8][F8E4M3FN] = false;
  expecteds[S8][F8E4M3B11FNUZ] = false;
  expecteds[S8][F8E5M2FNUZ] = false;
  expecteds[S8][F8E4M3FNUZ] = false;
  expecteds[S8][F8E3M4] = false;
  expecteds[S8][F8E8M0FNU] = false;
  expecteds[S16][PRED] = false;
  expecteds[S16][S1] = false;
  expecteds[S16][S2] = false;
  expecteds[S16][S4] = false;
  expecteds[S16][S8] = false;
  expecteds[S16][S16] = true;
  expecteds[S16][S32] = true;
  expecteds[S16][S64] = true;
  expecteds[S16][U1] = false;
  expecteds[S16][U2] = false;
  expecteds[S16][U4] = false;
  expecteds[S16][U8] = false;
  expecteds[S16][U16] = false;
  expecteds[S16][U32] = false;
  expecteds[S16][U64] = false;
  expecteds[S16][F16] = false;
  expecteds[S16][F32] = true;
  expecteds[S16][F64] = true;
  expecteds[S16][C64] = true;
  expecteds[S16][BF16] = false;
  expecteds[S16][C128] = true;
  expecteds[S16][F4E2M1FN] = false;
  expecteds[S16][F8E5M2] = false;
  expecteds[S16][F8E4M3] = false;
  expecteds[S16][F8E4M3FN] = false;
  expecteds[S16][F8E4M3B11FNUZ] = false;
  expecteds[S16][F8E5M2FNUZ] = false;
  expecteds[S16][F8E4M3FNUZ] = false;
  expecteds[S16][F8E3M4] = false;
  expecteds[S16][F8E8M0FNU] = false;
  expecteds[S32][PRED] = false;
  expecteds[S32][S1] = false;
  expecteds[S32][S2] = false;
  expecteds[S32][S4] = false;
  expecteds[S32][S8] = false;
  expecteds[S32][S16] = false;
  expecteds[S32][S32] = true;
  expecteds[S32][S64] = true;
  expecteds[S32][U1] = false;
  expecteds[S32][U2] = false;
  expecteds[S32][U4] = false;
  expecteds[S32][U8] = false;
  expecteds[S32][U16] = false;
  expecteds[S32][U32] = false;
  expecteds[S32][U64] = false;
  expecteds[S32][F16] = false;
  expecteds[S32][F32] = false;
  expecteds[S32][F64] = true;
  expecteds[S32][C64] = false;
  expecteds[S32][BF16] = false;
  expecteds[S32][C128] = true;
  expecteds[S32][F4E2M1FN] = false;
  expecteds[S32][F8E5M2] = false;
  expecteds[S32][F8E4M3] = false;
  expecteds[S32][F8E4M3FN] = false;
  expecteds[S32][F8E4M3B11FNUZ] = false;
  expecteds[S32][F8E5M2FNUZ] = false;
  expecteds[S32][F8E4M3FNUZ] = false;
  expecteds[S32][F8E3M4] = false;
  expecteds[S32][F8E8M0FNU] = false;
  expecteds[S64][PRED] = false;
  expecteds[S64][S1] = false;
  expecteds[S64][S2] = false;
  expecteds[S64][S4] = false;
  expecteds[S64][S8] = false;
  expecteds[S64][S16] = false;
  expecteds[S64][S32] = false;
  expecteds[S64][S64] = true;
  expecteds[S64][U1] = false;
  expecteds[S64][U2] = false;
  expecteds[S64][U4] = false;
  expecteds[S64][U8] = false;
  expecteds[S64][U16] = false;
  expecteds[S64][U32] = false;
  expecteds[S64][U64] = false;
  expecteds[S64][F16] = false;
  expecteds[S64][F32] = false;
  expecteds[S64][F64] = false;
  expecteds[S64][C64] = false;
  expecteds[S64][BF16] = false;
  expecteds[S64][C128] = false;
  expecteds[S64][F4E2M1FN] = false;
  expecteds[S64][F8E5M2] = false;
  expecteds[S64][F8E4M3] = false;
  expecteds[S64][F8E4M3FN] = false;
  expecteds[S64][F8E4M3B11FNUZ] = false;
  expecteds[S64][F8E5M2FNUZ] = false;
  expecteds[S64][F8E4M3FNUZ] = false;
  expecteds[S64][F8E3M4] = false;
  expecteds[S64][F8E8M0FNU] = false;
  expecteds[U1][PRED] = false;
  expecteds[U1][S1] = false;
  expecteds[U1][S2] = true;
  expecteds[U1][S4] = true;
  expecteds[U1][S8] = true;
  expecteds[U1][S16] = true;
  expecteds[U1][S32] = true;
  expecteds[U1][S64] = true;
  expecteds[U1][U1] = true;
  expecteds[U1][U2] = true;
  expecteds[U1][U4] = true;
  expecteds[U1][U8] = true;
  expecteds[U1][U16] = true;
  expecteds[U1][U32] = true;
  expecteds[U1][U64] = true;
  expecteds[U1][F16] = true;
  expecteds[U1][F32] = true;
  expecteds[U1][F64] = true;
  expecteds[U1][C64] = true;
  expecteds[U1][BF16] = true;
  expecteds[U1][C128] = true;
  expecteds[U1][F4E2M1FN] = true;
  expecteds[U1][F8E5M2] = true;
  expecteds[U1][F8E4M3] = true;
  expecteds[U1][F8E4M3FN] = true;
  expecteds[U1][F8E4M3B11FNUZ] = true;
  expecteds[U1][F8E5M2FNUZ] = true;
  expecteds[U1][F8E4M3FNUZ] = true;
  expecteds[U1][F8E3M4] = true;
  expecteds[U1][F8E8M0FNU] = false;
  expecteds[U2][PRED] = false;
  expecteds[U2][S1] = false;
  expecteds[U2][S2] = false;
  expecteds[U2][S4] = true;
  expecteds[U2][S8] = true;
  expecteds[U2][S16] = true;
  expecteds[U2][S32] = true;
  expecteds[U2][S64] = true;
  expecteds[U2][U1] = false;
  expecteds[U2][U2] = true;
  expecteds[U2][U4] = true;
  expecteds[U2][U8] = true;
  expecteds[U2][U16] = true;
  expecteds[U2][U32] = true;
  expecteds[U2][U64] = true;
  expecteds[U2][F16] = true;
  expecteds[U2][F32] = true;
  expecteds[U2][F64] = true;
  expecteds[U2][C64] = true;
  expecteds[U2][BF16] = true;
  expecteds[U2][C128] = true;
  expecteds[U2][F4E2M1FN] = true;
  expecteds[U2][F8E5M2] = true;
  expecteds[U2][F8E4M3] = true;
  expecteds[U2][F8E4M3FN] = true;
  expecteds[U2][F8E4M3B11FNUZ] = true;
  expecteds[U2][F8E5M2FNUZ] = true;
  expecteds[U2][F8E4M3FNUZ] = true;
  expecteds[U2][F8E3M4] = true;
  expecteds[U2][F8E8M0FNU] = false;
  expecteds[U4][PRED] = false;
  expecteds[U4][S1] = false;
  expecteds[U4][S2] = false;
  expecteds[U4][S4] = false;
  expecteds[U4][S8] = true;
  expecteds[U4][S16] = true;
  expecteds[U4][S32] = true;
  expecteds[U4][S64] = true;
  expecteds[U4][U1] = false;
  expecteds[U4][U2] = false;
  expecteds[U4][U4] = true;
  expecteds[U4][U8] = true;
  expecteds[U4][U16] = true;
  expecteds[U4][U32] = true;
  expecteds[U4][U64] = true;
  expecteds[U4][F16] = true;
  expecteds[U4][F32] = true;
  expecteds[U4][F64] = true;
  expecteds[U4][C64] = true;
  expecteds[U4][BF16] = true;
  expecteds[U4][C128] = true;
  expecteds[U4][F4E2M1FN] = false;
  expecteds[U4][F8E5M2] = false;
  expecteds[U4][F8E4M3] = true;
  expecteds[U4][F8E4M3FN] = true;
  expecteds[U4][F8E4M3B11FNUZ] = true;
  expecteds[U4][F8E5M2FNUZ] = false;
  expecteds[U4][F8E4M3FNUZ] = true;
  expecteds[U4][F8E3M4] = true;
  expecteds[U4][F8E8M0FNU] = false;
  expecteds[U8][PRED] = false;
  expecteds[U8][S1] = false;
  expecteds[U8][S2] = false;
  expecteds[U8][S4] = false;
  expecteds[U8][S8] = false;
  expecteds[U8][S16] = true;
  expecteds[U8][S32] = true;
  expecteds[U8][S64] = true;
  expecteds[U8][U1] = false;
  expecteds[U8][U2] = false;
  expecteds[U8][U4] = false;
  expecteds[U8][U8] = true;
  expecteds[U8][U16] = true;
  expecteds[U8][U32] = true;
  expecteds[U8][U64] = true;
  expecteds[U8][F16] = true;
  expecteds[U8][F32] = true;
  expecteds[U8][F64] = true;
  expecteds[U8][C64] = true;
  expecteds[U8][BF16] = true;
  expecteds[U8][C128] = true;
  expecteds[U8][F4E2M1FN] = false;
  expecteds[U8][F8E5M2] = false;
  expecteds[U8][F8E4M3] = false;
  expecteds[U8][F8E4M3FN] = false;
  expecteds[U8][F8E4M3B11FNUZ] = false;
  expecteds[U8][F8E5M2FNUZ] = false;
  expecteds[U8][F8E4M3FNUZ] = false;
  expecteds[U8][F8E3M4] = false;
  expecteds[U8][F8E8M0FNU] = false;
  expecteds[U16][PRED] = false;
  expecteds[U16][S1] = false;
  expecteds[U16][S2] = false;
  expecteds[U16][S4] = false;
  expecteds[U16][S8] = false;
  expecteds[U16][S16] = false;
  expecteds[U16][S32] = true;
  expecteds[U16][S64] = true;
  expecteds[U16][U1] = false;
  expecteds[U16][U2] = false;
  expecteds[U16][U4] = false;
  expecteds[U16][U8] = false;
  expecteds[U16][U16] = true;
  expecteds[U16][U32] = true;
  expecteds[U16][U64] = true;
  expecteds[U16][F16] = false;
  expecteds[U16][F32] = true;
  expecteds[U16][F64] = true;
  expecteds[U16][C64] = true;
  expecteds[U16][BF16] = false;
  expecteds[U16][C128] = true;
  expecteds[U16][F4E2M1FN] = false;
  expecteds[U16][F8E5M2] = false;
  expecteds[U16][F8E4M3] = false;
  expecteds[U16][F8E4M3FN] = false;
  expecteds[U16][F8E4M3B11FNUZ] = false;
  expecteds[U16][F8E5M2FNUZ] = false;
  expecteds[U16][F8E4M3FNUZ] = false;
  expecteds[U16][F8E3M4] = false;
  expecteds[U16][F8E8M0FNU] = false;
  expecteds[U32][PRED] = false;
  expecteds[U32][S1] = false;
  expecteds[U32][S2] = false;
  expecteds[U32][S4] = false;
  expecteds[U32][S8] = false;
  expecteds[U32][S16] = false;
  expecteds[U32][S32] = false;
  expecteds[U32][S64] = true;
  expecteds[U32][U1] = false;
  expecteds[U32][U2] = false;
  expecteds[U32][U4] = false;
  expecteds[U32][U8] = false;
  expecteds[U32][U16] = false;
  expecteds[U32][U32] = true;
  expecteds[U32][U64] = true;
  expecteds[U32][F16] = false;
  expecteds[U32][F32] = false;
  expecteds[U32][F64] = true;
  expecteds[U32][C64] = false;
  expecteds[U32][BF16] = false;
  expecteds[U32][C128] = true;
  expecteds[U32][F4E2M1FN] = false;
  expecteds[U32][F8E5M2] = false;
  expecteds[U32][F8E4M3] = false;
  expecteds[U32][F8E4M3FN] = false;
  expecteds[U32][F8E4M3B11FNUZ] = false;
  expecteds[U32][F8E5M2FNUZ] = false;
  expecteds[U32][F8E4M3FNUZ] = false;
  expecteds[U32][F8E3M4] = false;
  expecteds[U32][F8E8M0FNU] = false;
  expecteds[U64][PRED] = false;
  expecteds[U64][S1] = false;
  expecteds[U64][S2] = false;
  expecteds[U64][S4] = false;
  expecteds[U64][S8] = false;
  expecteds[U64][S16] = false;
  expecteds[U64][S32] = false;
  expecteds[U64][S64] = false;
  expecteds[U64][U1] = false;
  expecteds[U64][U2] = false;
  expecteds[U64][U4] = false;
  expecteds[U64][U8] = false;
  expecteds[U64][U16] = false;
  expecteds[U64][U32] = false;
  expecteds[U64][U64] = true;
  expecteds[U64][F16] = false;
  expecteds[U64][F32] = false;
  expecteds[U64][F64] = false;
  expecteds[U64][C64] = false;
  expecteds[U64][BF16] = false;
  expecteds[U64][C128] = false;
  expecteds[U64][F4E2M1FN] = false;
  expecteds[U64][F8E5M2] = false;
  expecteds[U64][F8E4M3] = false;
  expecteds[U64][F8E4M3FN] = false;
  expecteds[U64][F8E4M3B11FNUZ] = false;
  expecteds[U64][F8E5M2FNUZ] = false;
  expecteds[U64][F8E4M3FNUZ] = false;
  expecteds[U64][F8E3M4] = false;
  expecteds[U64][F8E8M0FNU] = false;
  expecteds[F16][PRED] = false;
  expecteds[F16][S1] = false;
  expecteds[F16][S2] = false;
  expecteds[F16][S4] = false;
  expecteds[F16][S8] = false;
  expecteds[F16][S16] = false;
  expecteds[F16][S32] = false;
  expecteds[F16][S64] = false;
  expecteds[F16][U1] = false;
  expecteds[F16][U2] = false;
  expecteds[F16][U4] = false;
  expecteds[F16][U8] = false;
  expecteds[F16][U16] = false;
  expecteds[F16][U32] = false;
  expecteds[F16][U64] = false;
  expecteds[F16][F16] = true;
  expecteds[F16][F32] = true;
  expecteds[F16][F64] = true;
  expecteds[F16][C64] = true;
  expecteds[F16][BF16] = false;
  expecteds[F16][C128] = true;
  expecteds[F16][F4E2M1FN] = false;
  expecteds[F16][F8E5M2] = false;
  expecteds[F16][F8E4M3] = false;
  expecteds[F16][F8E4M3FN] = false;
  expecteds[F16][F8E4M3B11FNUZ] = false;
  expecteds[F16][F8E5M2FNUZ] = false;
  expecteds[F16][F8E4M3FNUZ] = false;
  expecteds[F16][F8E3M4] = false;
  expecteds[F16][F8E8M0FNU] = false;
  expecteds[F32][PRED] = false;
  expecteds[F32][S1] = false;
  expecteds[F32][S2] = false;
  expecteds[F32][S4] = false;
  expecteds[F32][S8] = false;
  expecteds[F32][S16] = false;
  expecteds[F32][S32] = false;
  expecteds[F32][S64] = false;
  expecteds[F32][U1] = false;
  expecteds[F32][U2] = false;
  expecteds[F32][U4] = false;
  expecteds[F32][U8] = false;
  expecteds[F32][U16] = false;
  expecteds[F32][U32] = false;
  expecteds[F32][U64] = false;
  expecteds[F32][F16] = false;
  expecteds[F32][F32] = true;
  expecteds[F32][F64] = true;
  expecteds[F32][C64] = true;
  expecteds[F32][BF16] = false;
  expecteds[F32][C128] = true;
  expecteds[F32][F4E2M1FN] = false;
  expecteds[F32][F8E5M2] = false;
  expecteds[F32][F8E4M3] = false;
  expecteds[F32][F8E4M3FN] = false;
  expecteds[F32][F8E4M3B11FNUZ] = false;
  expecteds[F32][F8E5M2FNUZ] = false;
  expecteds[F32][F8E4M3FNUZ] = false;
  expecteds[F32][F8E3M4] = false;
  expecteds[F32][F8E8M0FNU] = false;
  expecteds[F64][PRED] = false;
  expecteds[F64][S1] = false;
  expecteds[F64][S2] = false;
  expecteds[F64][S4] = false;
  expecteds[F64][S8] = false;
  expecteds[F64][S16] = false;
  expecteds[F64][S32] = false;
  expecteds[F64][S64] = false;
  expecteds[F64][U1] = false;
  expecteds[F64][U2] = false;
  expecteds[F64][U4] = false;
  expecteds[F64][U8] = false;
  expecteds[F64][U16] = false;
  expecteds[F64][U32] = false;
  expecteds[F64][U64] = false;
  expecteds[F64][F16] = false;
  expecteds[F64][F32] = false;
  expecteds[F64][F64] = true;
  expecteds[F64][C64] = false;
  expecteds[F64][BF16] = false;
  expecteds[F64][C128] = true;
  expecteds[F64][F4E2M1FN] = false;
  expecteds[F64][F8E5M2] = false;
  expecteds[F64][F8E4M3] = false;
  expecteds[F64][F8E4M3FN] = false;
  expecteds[F64][F8E4M3B11FNUZ] = false;
  expecteds[F64][F8E5M2FNUZ] = false;
  expecteds[F64][F8E4M3FNUZ] = false;
  expecteds[F64][F8E3M4] = false;
  expecteds[F64][F8E8M0FNU] = false;
  expecteds[C64][PRED] = false;
  expecteds[C64][S1] = false;
  expecteds[C64][S2] = false;
  expecteds[C64][S4] = false;
  expecteds[C64][S8] = false;
  expecteds[C64][S16] = false;
  expecteds[C64][S32] = false;
  expecteds[C64][S64] = false;
  expecteds[C64][U1] = false;
  expecteds[C64][U2] = false;
  expecteds[C64][U4] = false;
  expecteds[C64][U8] = false;
  expecteds[C64][U16] = false;
  expecteds[C64][U32] = false;
  expecteds[C64][U64] = false;
  expecteds[C64][F16] = false;
  expecteds[C64][F32] = false;
  expecteds[C64][F64] = false;
  expecteds[C64][C64] = true;
  expecteds[C64][BF16] = false;
  expecteds[C64][C128] = true;
  expecteds[C64][F4E2M1FN] = false;
  expecteds[C64][F8E5M2] = false;
  expecteds[C64][F8E4M3] = false;
  expecteds[C64][F8E4M3FN] = false;
  expecteds[C64][F8E4M3B11FNUZ] = false;
  expecteds[C64][F8E5M2FNUZ] = false;
  expecteds[C64][F8E4M3FNUZ] = false;
  expecteds[C64][F8E3M4] = false;
  expecteds[C64][F8E8M0FNU] = false;
  expecteds[BF16][PRED] = false;
  expecteds[BF16][S1] = false;
  expecteds[BF16][S2] = false;
  expecteds[BF16][S4] = false;
  expecteds[BF16][S8] = false;
  expecteds[BF16][S16] = false;
  expecteds[BF16][S32] = false;
  expecteds[BF16][S64] = false;
  expecteds[BF16][U1] = false;
  expecteds[BF16][U2] = false;
  expecteds[BF16][U4] = false;
  expecteds[BF16][U8] = false;
  expecteds[BF16][U16] = false;
  expecteds[BF16][U32] = false;
  expecteds[BF16][U64] = false;
  expecteds[BF16][F16] = false;
  expecteds[BF16][F32] = true;
  expecteds[BF16][F64] = true;
  expecteds[BF16][C64] = true;
  expecteds[BF16][BF16] = true;
  expecteds[BF16][C128] = true;
  expecteds[BF16][F4E2M1FN] = false;
  expecteds[BF16][F8E5M2] = false;
  expecteds[BF16][F8E4M3] = false;
  expecteds[BF16][F8E4M3FN] = false;
  expecteds[BF16][F8E4M3B11FNUZ] = false;
  expecteds[BF16][F8E5M2FNUZ] = false;
  expecteds[BF16][F8E4M3FNUZ] = false;
  expecteds[BF16][F8E3M4] = false;
  expecteds[BF16][F8E8M0FNU] = false;
  expecteds[C128][PRED] = false;
  expecteds[C128][S1] = false;
  expecteds[C128][S2] = false;
  expecteds[C128][S4] = false;
  expecteds[C128][S8] = false;
  expecteds[C128][S16] = false;
  expecteds[C128][S32] = false;
  expecteds[C128][S64] = false;
  expecteds[C128][U1] = false;
  expecteds[C128][U2] = false;
  expecteds[C128][U4] = false;
  expecteds[C128][U8] = false;
  expecteds[C128][U16] = false;
  expecteds[C128][U32] = false;
  expecteds[C128][U64] = false;
  expecteds[C128][F16] = false;
  expecteds[C128][F32] = false;
  expecteds[C128][F64] = false;
  expecteds[C128][C64] = false;
  expecteds[C128][BF16] = false;
  expecteds[C128][C128] = true;
  expecteds[C128][F4E2M1FN] = false;
  expecteds[C128][F8E5M2] = false;
  expecteds[C128][F8E4M3] = false;
  expecteds[C128][F8E4M3FN] = false;
  expecteds[C128][F8E4M3B11FNUZ] = false;
  expecteds[C128][F8E5M2FNUZ] = false;
  expecteds[C128][F8E4M3FNUZ] = false;
  expecteds[C128][F8E3M4] = false;
  expecteds[C128][F8E8M0FNU] = false;
  expecteds[F4E2M1FN][PRED] = false;
  expecteds[F4E2M1FN][S1] = false;
  expecteds[F4E2M1FN][S2] = false;
  expecteds[F4E2M1FN][S4] = false;
  expecteds[F4E2M1FN][S8] = false;
  expecteds[F4E2M1FN][S16] = false;
  expecteds[F4E2M1FN][S32] = false;
  expecteds[F4E2M1FN][S64] = false;
  expecteds[F4E2M1FN][U1] = false;
  expecteds[F4E2M1FN][U2] = false;
  expecteds[F4E2M1FN][U4] = false;
  expecteds[F4E2M1FN][U8] = false;
  expecteds[F4E2M1FN][U16] = false;
  expecteds[F4E2M1FN][U32] = false;
  expecteds[F4E2M1FN][U64] = false;
  expecteds[F4E2M1FN][F16] = true;
  expecteds[F4E2M1FN][F32] = true;
  expecteds[F4E2M1FN][F64] = true;
  expecteds[F4E2M1FN][C64] = true;
  expecteds[F4E2M1FN][BF16] = true;
  expecteds[F4E2M1FN][C128] = true;
  expecteds[F4E2M1FN][F4E2M1FN] = true;
  expecteds[F4E2M1FN][F8E5M2] = true;
  expecteds[F4E2M1FN][F8E4M3] = true;
  expecteds[F4E2M1FN][F8E4M3FN] = true;
  expecteds[F4E2M1FN][F8E4M3B11FNUZ] = false;
  expecteds[F4E2M1FN][F8E4M3FNUZ] = false;
  expecteds[F4E2M1FN][F8E5M2FNUZ] = false;
  expecteds[F4E2M1FN][F8E3M4] = true;
  expecteds[F4E2M1FN][F8E8M0FNU] = false;
  expecteds[F8E5M2][PRED] = false;
  expecteds[F8E5M2][S1] = false;
  expecteds[F8E5M2][S2] = false;
  expecteds[F8E5M2][S4] = false;
  expecteds[F8E5M2][S8] = false;
  expecteds[F8E5M2][S16] = false;
  expecteds[F8E5M2][S32] = false;
  expecteds[F8E5M2][S64] = false;
  expecteds[F8E5M2][U1] = false;
  expecteds[F8E5M2][U2] = false;
  expecteds[F8E5M2][U4] = false;
  expecteds[F8E5M2][U8] = false;
  expecteds[F8E5M2][U16] = false;
  expecteds[F8E5M2][U32] = false;
  expecteds[F8E5M2][U64] = false;
  expecteds[F8E5M2][F16] = true;
  expecteds[F8E5M2][F32] = true;
  expecteds[F8E5M2][F64] = true;
  expecteds[F8E5M2][C64] = true;
  expecteds[F8E5M2][BF16] = true;
  expecteds[F8E5M2][C128] = true;
  expecteds[F8E5M2][F4E2M1FN] = false;
  expecteds[F8E5M2][F8E5M2] = true;
  expecteds[F8E5M2][F8E4M3] = false;
  expecteds[F8E5M2][F8E4M3FN] = false;
  expecteds[F8E5M2][F8E4M3B11FNUZ] = false;
  expecteds[F8E5M2][F8E5M2FNUZ] = false;
  expecteds[F8E5M2][F8E4M3FNUZ] = false;
  expecteds[F8E5M2][F8E3M4] = false;
  expecteds[F8E5M2][F8E8M0FNU] = false;
  expecteds[F8E4M3][PRED] = false;
  expecteds[F8E4M3][S1] = false;
  expecteds[F8E4M3][S2] = false;
  expecteds[F8E4M3][S4] = false;
  expecteds[F8E4M3][S8] = false;
  expecteds[F8E4M3][S16] = false;
  expecteds[F8E4M3][S32] = false;
  expecteds[F8E4M3][S64] = false;
  expecteds[F8E4M3][U1] = false;
  expecteds[F8E4M3][U2] = false;
  expecteds[F8E4M3][U4] = false;
  expecteds[F8E4M3][U8] = false;
  expecteds[F8E4M3][U16] = false;
  expecteds[F8E4M3][U32] = false;
  expecteds[F8E4M3][U64] = false;
  expecteds[F8E4M3][F16] = true;
  expecteds[F8E4M3][F32] = true;
  expecteds[F8E4M3][F64] = true;
  expecteds[F8E4M3][C64] = true;
  expecteds[F8E4M3][BF16] = true;
  expecteds[F8E4M3][C128] = true;
  expecteds[F8E4M3][F4E2M1FN] = false;
  expecteds[F8E4M3][F8E5M2] = false;
  expecteds[F8E4M3][F8E5M2FNUZ] = false;
  expecteds[F8E4M3][F8E4M3] = true;
  expecteds[F8E4M3][F8E4M3FN] = false;
  expecteds[F8E4M3][F8E4M3FNUZ] = false;
  expecteds[F8E4M3][F8E4M3B11FNUZ] = false;
  expecteds[F8E4M3][F8E3M4] = false;
  expecteds[F8E4M3][F8E8M0FNU] = false;
  expecteds[F8E4M3FN][PRED] = false;
  expecteds[F8E4M3FN][S1] = false;
  expecteds[F8E4M3FN][S2] = false;
  expecteds[F8E4M3FN][S4] = false;
  expecteds[F8E4M3FN][S8] = false;
  expecteds[F8E4M3FN][S16] = false;
  expecteds[F8E4M3FN][S32] = false;
  expecteds[F8E4M3FN][S64] = false;
  expecteds[F8E4M3FN][U1] = false;
  expecteds[F8E4M3FN][U2] = false;
  expecteds[F8E4M3FN][U4] = false;
  expecteds[F8E4M3FN][U8] = false;
  expecteds[F8E4M3FN][U16] = false;
  expecteds[F8E4M3FN][U32] = false;
  expecteds[F8E4M3FN][U64] = false;
  expecteds[F8E4M3FN][F16] = true;
  expecteds[F8E4M3FN][F32] = true;
  expecteds[F8E4M3FN][F64] = true;
  expecteds[F8E4M3FN][C64] = true;
  expecteds[F8E4M3FN][BF16] = true;
  expecteds[F8E4M3FN][C128] = true;
  expecteds[F8E4M3FN][F4E2M1FN] = false;
  expecteds[F8E4M3FN][F8E5M2] = false;
  expecteds[F8E4M3FN][F8E5M2FNUZ] = false;
  expecteds[F8E4M3FN][F8E4M3] = false;
  expecteds[F8E4M3FN][F8E4M3FN] = true;
  expecteds[F8E4M3FN][F8E4M3FNUZ] = false;
  expecteds[F8E4M3FN][F8E4M3B11FNUZ] = false;
  expecteds[F8E4M3FN][F8E3M4] = false;
  expecteds[F8E4M3FN][F8E8M0FNU] = false;
  expecteds[F8E4M3B11FNUZ][PRED] = false;
  expecteds[F8E4M3B11FNUZ][S1] = false;
  expecteds[F8E4M3B11FNUZ][S2] = false;
  expecteds[F8E4M3B11FNUZ][S4] = false;
  expecteds[F8E4M3B11FNUZ][S8] = false;
  expecteds[F8E4M3B11FNUZ][S16] = false;
  expecteds[F8E4M3B11FNUZ][S32] = false;
  expecteds[F8E4M3B11FNUZ][S64] = false;
  expecteds[F8E4M3B11FNUZ][U1] = false;
  expecteds[F8E4M3B11FNUZ][U2] = false;
  expecteds[F8E4M3B11FNUZ][U4] = false;
  expecteds[F8E4M3B11FNUZ][U8] = false;
  expecteds[F8E4M3B11FNUZ][U16] = false;
  expecteds[F8E4M3B11FNUZ][U32] = false;
  expecteds[F8E4M3B11FNUZ][U64] = false;
  expecteds[F8E4M3B11FNUZ][F16] = true;
  expecteds[F8E4M3B11FNUZ][F32] = true;
  expecteds[F8E4M3B11FNUZ][F64] = true;
  expecteds[F8E4M3B11FNUZ][C64] = true;
  expecteds[F8E4M3B11FNUZ][BF16] = true;
  expecteds[F8E4M3B11FNUZ][C128] = true;
  expecteds[F8E4M3B11FNUZ][F4E2M1FN] = false;
  expecteds[F8E4M3B11FNUZ][F8E5M2] = false;
  expecteds[F8E4M3B11FNUZ][F8E4M3] = false;
  expecteds[F8E4M3B11FNUZ][F8E4M3FN] = false;
  expecteds[F8E4M3B11FNUZ][F8E4M3B11FNUZ] = true;
  expecteds[F8E4M3B11FNUZ][F8E4M3FNUZ] = false;
  expecteds[F8E4M3B11FNUZ][F8E5M2FNUZ] = false;
  expecteds[F8E4M3B11FNUZ][F8E3M4] = false;
  expecteds[F8E4M3B11FNUZ][F8E8M0FNU] = false;
  expecteds[F8E5M2FNUZ][PRED] = false;
  expecteds[F8E5M2FNUZ][S1] = false;
  expecteds[F8E5M2FNUZ][S2] = false;
  expecteds[F8E5M2FNUZ][S4] = false;
  expecteds[F8E5M2FNUZ][S8] = false;
  expecteds[F8E5M2FNUZ][S16] = false;
  expecteds[F8E5M2FNUZ][S32] = false;
  expecteds[F8E5M2FNUZ][S64] = false;
  expecteds[F8E5M2FNUZ][U1] = false;
  expecteds[F8E5M2FNUZ][U2] = false;
  expecteds[F8E5M2FNUZ][U4] = false;
  expecteds[F8E5M2FNUZ][U8] = false;
  expecteds[F8E5M2FNUZ][U16] = false;
  expecteds[F8E5M2FNUZ][U32] = false;
  expecteds[F8E5M2FNUZ][U64] = false;
  expecteds[F8E5M2FNUZ][F16] = true;
  expecteds[F8E5M2FNUZ][F32] = true;
  expecteds[F8E5M2FNUZ][F64] = true;
  expecteds[F8E5M2FNUZ][C64] = true;
  expecteds[F8E5M2FNUZ][BF16] = true;
  expecteds[F8E5M2FNUZ][C128] = true;
  expecteds[F8E5M2FNUZ][F4E2M1FN] = false;
  expecteds[F8E5M2FNUZ][F8E5M2] = false;
  expecteds[F8E5M2FNUZ][F8E4M3] = false;
  expecteds[F8E5M2FNUZ][F8E4M3FN] = false;
  expecteds[F8E5M2FNUZ][F8E4M3B11FNUZ] = false;
  expecteds[F8E5M2FNUZ][F8E5M2FNUZ] = true;
  expecteds[F8E5M2FNUZ][F8E4M3FNUZ] = false;
  expecteds[F8E5M2FNUZ][F8E3M4] = false;
  expecteds[F8E5M2FNUZ][F8E8M0FNU] = false;
  expecteds[F8E4M3FNUZ][PRED] = false;
  expecteds[F8E4M3FNUZ][S1] = false;
  expecteds[F8E4M3FNUZ][S2] = false;
  expecteds[F8E4M3FNUZ][S4] = false;
  expecteds[F8E4M3FNUZ][S8] = false;
  expecteds[F8E4M3FNUZ][S16] = false;
  expecteds[F8E4M3FNUZ][S32] = false;
  expecteds[F8E4M3FNUZ][S64] = false;
  expecteds[F8E4M3FNUZ][U1] = false;
  expecteds[F8E4M3FNUZ][U2] = false;
  expecteds[F8E4M3FNUZ][U4] = false;
  expecteds[F8E4M3FNUZ][U8] = false;
  expecteds[F8E4M3FNUZ][U16] = false;
  expecteds[F8E4M3FNUZ][U32] = false;
  expecteds[F8E4M3FNUZ][U64] = false;
  expecteds[F8E4M3FNUZ][F16] = true;
  expecteds[F8E4M3FNUZ][F32] = true;
  expecteds[F8E4M3FNUZ][F64] = true;
  expecteds[F8E4M3FNUZ][C64] = true;
  expecteds[F8E4M3FNUZ][BF16] = true;
  expecteds[F8E4M3FNUZ][C128] = true;
  expecteds[F8E4M3FNUZ][F4E2M1FN] = false;
  expecteds[F8E4M3FNUZ][F8E5M2] = false;
  expecteds[F8E4M3FNUZ][F8E4M3] = false;
  expecteds[F8E4M3FNUZ][F8E4M3FN] = false;
  expecteds[F8E4M3FNUZ][F8E4M3B11FNUZ] = false;
  expecteds[F8E4M3FNUZ][F8E5M2FNUZ] = false;
  expecteds[F8E4M3FNUZ][F8E4M3FNUZ] = true;
  expecteds[F8E4M3FNUZ][F8E3M4] = false;
  expecteds[F8E4M3FNUZ][F8E8M0FNU] = false;
  expecteds[F8E3M4][PRED] = false;
  expecteds[F8E3M4][S1] = false;
  expecteds[F8E3M4][S2] = false;
  expecteds[F8E3M4][S4] = false;
  expecteds[F8E3M4][S8] = false;
  expecteds[F8E3M4][S16] = false;
  expecteds[F8E3M4][S32] = false;
  expecteds[F8E3M4][S64] = false;
  expecteds[F8E3M4][U1] = false;
  expecteds[F8E3M4][U2] = false;
  expecteds[F8E3M4][U4] = false;
  expecteds[F8E3M4][U8] = false;
  expecteds[F8E3M4][U16] = false;
  expecteds[F8E3M4][U32] = false;
  expecteds[F8E3M4][U64] = false;
  expecteds[F8E3M4][F16] = true;
  expecteds[F8E3M4][F32] = true;
  expecteds[F8E3M4][F64] = true;
  expecteds[F8E3M4][C64] = true;
  expecteds[F8E3M4][BF16] = true;
  expecteds[F8E3M4][C128] = true;
  expecteds[F8E3M4][F4E2M1FN] = false;
  expecteds[F8E3M4][F8E5M2] = false;
  expecteds[F8E3M4][F8E5M2FNUZ] = false;
  expecteds[F8E3M4][F8E4M3] = false;
  expecteds[F8E3M4][F8E4M3FN] = false;
  expecteds[F8E3M4][F8E4M3FNUZ] = false;
  expecteds[F8E3M4][F8E4M3B11FNUZ] = false;
  expecteds[F8E3M4][F8E3M4] = true;
  expecteds[F8E3M4][F8E8M0FNU] = false;
  expecteds[F8E8M0FNU][PRED] = false;
  expecteds[F8E8M0FNU][S1] = false;
  expecteds[F8E8M0FNU][S2] = false;
  expecteds[F8E8M0FNU][S4] = false;
  expecteds[F8E8M0FNU][S8] = false;
  expecteds[F8E8M0FNU][S16] = false;
  expecteds[F8E8M0FNU][S32] = false;
  expecteds[F8E8M0FNU][S64] = false;
  expecteds[F8E8M0FNU][U1] = false;
  expecteds[F8E8M0FNU][U2] = false;
  expecteds[F8E8M0FNU][U4] = false;
  expecteds[F8E8M0FNU][U8] = false;
  expecteds[F8E8M0FNU][U16] = false;
  expecteds[F8E8M0FNU][U32] = false;
  expecteds[F8E8M0FNU][U64] = false;
  expecteds[F8E8M0FNU][F16] = false;
  expecteds[F8E8M0FNU][F32] = true;
  expecteds[F8E8M0FNU][F64] = true;
  expecteds[F8E8M0FNU][C64] = true;
  expecteds[F8E8M0FNU][BF16] = true;
  expecteds[F8E8M0FNU][C128] = true;
  expecteds[F8E8M0FNU][F4E2M1FN] = false;
  expecteds[F8E8M0FNU][F8E5M2] = false;
  expecteds[F8E8M0FNU][F8E4M3] = false;
  expecteds[F8E8M0FNU][F8E4M3FN] = false;
  expecteds[F8E8M0FNU][F8E4M3B11FNUZ] = false;
  expecteds[F8E8M0FNU][F8E4M3FNUZ] = false;
  expecteds[F8E8M0FNU][F8E5M2FNUZ] = false;
  expecteds[F8E8M0FNU][F8E3M4] = false;
  expecteds[F8E8M0FNU][F8E8M0FNU] = true;

  for (int from_type_int = PrimitiveType_MIN;
       from_type_int < PrimitiveType_ARRAYSIZE; ++from_type_int) {
    auto from_type = static_cast<PrimitiveType>(from_type_int);
    if (!primitive_util::IsArrayType(from_type)) {
      continue;
    }
    for (int to_type_int = PrimitiveType_MIN;
         to_type_int < PrimitiveType_ARRAYSIZE; ++to_type_int) {
      auto to_type = static_cast<PrimitiveType>(to_type_int);
      if (!primitive_util::IsArrayType(to_type)) {
        continue;
      }
      bool expected = expecteds[from_type][to_type];
      bool actual = primitive_util::CastPreservesValues(from_type, to_type);
      EXPECT_EQ(expected, actual)
          << primitive_util::LowercasePrimitiveTypeName(from_type) << " -> "
          << primitive_util::LowercasePrimitiveTypeName(to_type);
    }
  }
}  // NOLINT(readability/fn_size)

}  // namespace
}  // namespace xla
