// MIT License
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

#ifndef WASMPARSER_CPP_TYPES_H
#define WASMPARSER_CPP_TYPES_H

#include <optional>
#include <vector>

namespace wasmparser {

enum class ValType : uint8_t {
  I32 = 0x7F,
  I64 = 0x7E,
  F32 = 0x7D,
  F64 = 0x7C,
};

template <class T>
struct Vec {
  uint32_t size;
  std::vector<T> elem;
};

using ResultType = Vec<ValType>;

struct FuncType {
  ResultType param_type;
  ResultType return_type;
};

struct Limit {
  uint32_t min_;
  std::optional<uint32_t> max_;
};

struct MemoryType {
  Limit limit;
};

enum class Mutability : uint8_t {
  Const = 0x00,
  Var = 0x01,
};

struct GlobalType {
  ValType val_type;
  Mutability mut;
};

using Byte = unsigned char;
using Name = Vec<Byte>;
using Bytes = Vec<Byte>;
using ElemType = Byte;

struct TableType {
  unsigned char et_ = 0x70;
  Limit limit;
};

}  // namespace wasmparser

#endif  // WASMPARSER_CPP_TYPES_H
