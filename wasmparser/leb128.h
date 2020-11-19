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

size_t decodeULEB128(Byte *buf, uint32_t *r) {
  uint32_t result = 0;
  int shift = 0;
  Byte byte;
  int i = 0;

  while (true) {
    byte = buf[i];
    result |= static_cast<uint32_t>(byte & 0x7f) << shift;
    shift += 7;
    ++i;
    if ((byte & 0x80) == 0) {
      break;
    }
  }
  *r = result;
  return i;
}

size_t decodeULEB128(Byte *buf, uint64_t *r) {
  uint64_t result = 0;
  int shift = 0;
  Byte byte;
  int i = 0;

  while (true) {
    byte = buf[i];
    result |= static_cast<uint64_t>(byte & 0x7f) << shift;
    shift += 7;
    ++i;
    if ((byte & 0x80) == 0) {
      break;
    }
  }
  *r = result;
  return i;
}

size_t decodeSLEB128(Byte *buf, int32_t *r) {
  int32_t result = 0;
  int shift = 0;
  Byte byte;
  int i = 0;

  while (true) {
    byte = buf[i];
    result |= static_cast<int32_t>(byte & 0x7f) << shift;
    shift += 7;
    ++i;
    if ((byte & 0x80) == 0) {
      break;
    }
  }

  if (shift < 32 && (byte & 0x40) != 0) {
    result |= static_cast<int32_t>(~0 << shift);
  }

  *r = result;
  return i;
}

size_t decodeSLEB128(Byte *buf, int64_t *r) {
  int64_t result = 0;
  int shift = 0;
  Byte byte;
  int i = 0;

  while (true) {
    byte = buf[i];
    result |= static_cast<int64_t>(byte & 0x7f) << shift;
    shift += 7;
    ++i;
    if ((byte & 0x80) == 0) {
      break;
    }
  }

  if (shift < 64 && (byte & 0x40) != 0) {
    result |= static_cast<int64_t>(~0 << shift);
  }

  *r = result;
  return i;
}

// TODO: Correct?
size_t decodeS33LEB128(Byte *buf, int64_t *r) {
  int64_t result = 0;
  int shift = 0;
  Byte byte;
  int i = 0;

  while (true) {
    byte = buf[i];
    result |= static_cast<int64_t>(byte & 0x7f) << shift;
    shift += 7;
    ++i;
    if ((byte & 0x80) == 0) {
      break;
    }
  }

  if (shift < 64 && (byte & 0x40) != 0) {
    result |= static_cast<int64_t>(~0 << shift);
  }

  *r = result;
  return i;
}

}  // namespace wasmparser
#endif  // WASMPARSER_CPP_LEB128_H
