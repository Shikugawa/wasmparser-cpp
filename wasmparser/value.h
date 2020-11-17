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

}  // namespace wasmparser

#endif  // WASMPARSER_CPP_VALUE_H
