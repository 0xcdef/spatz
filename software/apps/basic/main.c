// Copyright 2021 ETH Zurich and University of Bologna.
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Author: Domenic Wüthrich, ETH Zurich

// clang-format off

#include <stdint.h>

#define UNALIGNED 1
#define VEC_LENGTH 33+UNALIGNED

uint8_t vector1[VEC_LENGTH] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23};
uint8_t vector2[VEC_LENGTH] = {0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 0xfe, 0xdc};
uint8_t vector_res[VEC_LENGTH];

uint8_t vector_sld[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
uint8_t vector_sld_res[16];

int main() {
  // Stripmining loop
  int32_t remaining_elem = VEC_LENGTH-UNALIGNED;
  uint8_t *ptr_vec1 = vector1+UNALIGNED;
  uint8_t *ptr_vec2 = vector2+UNALIGNED;
  uint8_t *ptr_vec_res = vector_res+UNALIGNED;
  while (remaining_elem > 0) {
    uint32_t actual_elem;
    asm volatile("vsetvli %0, %1, e8, m1, ta, ma" : "=r"(actual_elem) : "r"(remaining_elem));
    asm volatile("vle8.v v12, (%[vector1])" :: [vector1]"r"(ptr_vec1));
    asm volatile("vle8.v v13, (%[vector2])" :: [vector2]"r"(ptr_vec2));
    asm volatile("vadd.vv v14, v12, v13");
    asm volatile("vse8.v v14, (%[vector_res])" :: [vector_res]"r"(ptr_vec_res));
    remaining_elem -= actual_elem;
    ptr_vec1 += actual_elem;
    ptr_vec2 += actual_elem;
    ptr_vec_res += actual_elem;
  }
  for (int i = UNALIGNED; i < VEC_LENGTH; i++) {
    if (vector1[i] + vector2[i] != vector_res[i]) return i+1;
  }

  // Vector slide1up
  uint8_t elem_insert = 0xff;
  asm volatile("vsetivli zero, 16, e8, m1, ta, ma");
  asm volatile("vle8.v v8, (%0)" :: "r"(vector_sld));
  asm volatile("vslide1up.vx v9, v8, %0" :: "r"(elem_insert));
  asm volatile("vse8.v v9, (%0)" :: "r"(vector_sld_res));

  if (vector_sld_res[0] != elem_insert) return 1;
  for (int i = 1; i < 16; i++) {
    if (vector_sld_res[i] != vector_sld[i-1]) return i+1;
  }

  // Vector slide1down
  asm volatile("vsetivli zero, 16, e8, m1, ta, ma");
  asm volatile("vle8.v v6, (%0)" :: "r"(vector_sld));
  asm volatile("vslide1down.vx v7, v6, %0" :: "r"(elem_insert));
  asm volatile("vse8.v v7, (%0)" :: "r"(vector_sld_res));

  for (int i = 0; i < 15; i++) {
    if (vector_sld_res[i] != vector_sld[i-1]) return i+1;
  }
  if (vector_sld_res[15] != elem_insert) return 16;

  return 0;
}

// clang-format on
