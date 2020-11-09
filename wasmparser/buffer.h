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

#ifndef WASMPARSER_CPP_BUFFER_H
#define WASMPARSER_CPP_BUFFER_H

#include <sys/stat.h>

#include <fstream>
#include <memory>
#include <string_view>
#include <vector>

#include "types.h"

namespace wasmparser {
class ZeroCopyBuffer {
 public:
  static std::unique_ptr<ZeroCopyBuffer> createBuffer(
      std::string_view filename);
  ZeroCopyBuffer(const ZeroCopyBuffer& buf) = delete;
  ZeroCopyBuffer(char* buf, size_t size);

  Byte* at(size_t idx) const;
  size_t size() const { return buf_.size(); }

 private:
  std::vector<Byte> buf_;
};

std::unique_ptr<ZeroCopyBuffer> ZeroCopyBuffer::createBuffer(
    std::string_view filename) {
  struct stat result;
  if (stat(filename.data(), &result) != 0) {
    throw std::runtime_error("Failed to check file stats.");
  }
  auto size = result.st_size;
  std::ifstream stream(filename, std::ios::in | std::ios::binary);
  char buf[size];
  stream.read(buf, size);
  return std::make_unique<ZeroCopyBuffer>(&buf[0], size);
}

Byte* ZeroCopyBuffer::at(size_t idx) const {
  if (idx >= buf_.size()) {
    throw std::runtime_error("Invalid buffer access");
  }
  return &buf_[idx];
}

ZeroCopyBuffer::ZeroCopyBuffer(char* buf, size_t size) {
  for (auto i = 0; i < size; ++i) {
    buf_.emplace_back(static_cast<Byte>(buf[i]));
  }
}

using ZeroCopyBufferPtr = std::unique_ptr<ZeroCopyBuffer>;

}  // namespace wasmparser

#endif  // WASMPARSER_CPP_BUFFER_H
