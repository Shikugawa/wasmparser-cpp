// MIT License
//
//
// Copyright (c) Rei Shimizu 2020
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
//        of this software and associated documentation files (the "Software"),
//        to deal
// in the Software without restriction, including without limitation the rights
//        to use, copy, modify, merge, publish, distribute, sublicense, and/or
//        sell copies of the Software, and to permit persons to whom the
//        Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all
//        copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef WASMPARSER_CPP_LEB128_H
#define WASMPARSER_CPP_LEB128_H

#include "types.h"

namespace wasmparser {

// TODO: buf_end is not needed anymore.
size_t decodeULEB128(Byte *buf, Byte *buf_end, uint32_t *r) {
  Byte *p = buf;
  unsigned int shift = 0;
  uint32_t result = 0;
  unsigned char byte;

  while (true) {
    if (p >= buf_end) return 0;

    byte = *p++;
    result |= ((uint32_t)(byte & 0x7f)) << shift;
    if ((byte & 0x80) == 0) break;
    shift += 7;
  }

  *r = result;
  return p - buf;
}

size_t decodeULEB128(Byte *buf, Byte *buf_end, uint64_t *r) {
  Byte *p = buf;
  unsigned int shift = 0;
  uint64_t result = 0;
  unsigned char byte;

  while (true) {
    if (p >= buf_end) return 0;

    byte = *p++;
    result |= ((uint64_t)(byte & 0x7f)) << shift;
    if ((byte & 0x80) == 0) break;
    shift += 7;
  }

  *r = result;
  return p - buf;
}

size_t decodeSLEB128(Byte *buf, Byte *buf_end, int32_t *r) {
  Byte *p = buf;
  unsigned int shift = 0;
  int32_t result = 0;
  unsigned char byte;

  while (true) {
    if (p >= buf_end) return 0;

    byte = *p++;
    result |= ((int32_t)(byte & 0x7f)) << shift;
    shift += 7;
    if ((byte & 0x80) == 0) break;
  }

  if (shift < (sizeof(*r) * 8) && (byte & 0x40) != 0)
    result |= -(((int32_t)1) << shift);

  *r = result;
  return p - buf;
}

size_t decodeSLEB128(Byte *buf, Byte *buf_end, int64_t *r) {
  Byte *p = buf;
  unsigned int shift = 0;
  int64_t result = 0;
  unsigned char byte;

  while (true) {
    if (p >= buf_end) return 0;

    byte = *p++;
    result |= ((int64_t)(byte & 0x7f)) << shift;
    shift += 7;
    if ((byte & 0x80) == 0) break;
  }

  if (shift < (sizeof(*r) * 8) && (byte & 0x40) != 0)
    result |= -(((int64_t)1) << shift);

  *r = result;
  return p - buf;
}

// TODO: Correct?
size_t decodeS33LEB128(Byte *buf, Byte *buf_end, int64_t *r) {
  Byte *p = buf;
  unsigned int shift = 0;
  int64_t result = 0;
  unsigned char byte;

  while (true) {
    if (p >= buf_end) return 0;

    byte = *p++;
    result |= ((int64_t)(byte & 0x7f)) << shift;
    shift += 7;
    if ((byte & 0x80) == 0) break;
  }

  if (shift < (sizeof(*r) * 8) && (byte & 0x40) != 0)
    result |= 0x1FFFFFFFF << shift;

  if (result & 0x100000000 > 0) result = result - 8589934592;

  *r = result;
  return p - buf;
}

}  // namespace wasmparser
#endif  // WASMPARSER_CPP_LEB128_H
