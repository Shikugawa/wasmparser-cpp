//
// Created by shikugawa on 2020/11/16.
//

#ifndef WASMPARSER_CPP_VALUE_H
#define WASMPARSER_CPP_VALUE_H

#include <vector>

namespace wasmparser {

template <class T>
struct Vec {
  uint32_t size;
  std::vector<T> elem;
};

using Byte = unsigned char;
using Name = Vec<Byte>;
using Bytes = Vec<Byte>;

// Instruction symbols and raw bytes which means operand of them.
// This value is completely raw bytes extracted from wasm binary.
using Expr = std::vector<Byte>;

}  // namespace wasmparser

#endif  // WASMPARSER_CPP_VALUE_H
