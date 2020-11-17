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

#ifndef WASMPARSER_CPP_MODULE_H
#define WASMPARSER_CPP_MODULE_H

#include <variant>

#include "types.h"

namespace wasmparser {

enum class SectionId : Byte {
  Custom = 0x00,
  Type = 0x01,
  Import = 0x02,
  Function = 0x03,
  Table = 0x04,
  Memory = 0x05,
  Global = 0x06,
  Export = 0x07,
  Start = 0x08,
  Element = 0x09,
  Code = 0x0a,
  Data = 0x0b,
};

struct Custom {
  Name name;
  std::vector<Byte> bytes;
};

struct Import {
  template <class T>
  struct ImportDesc {
    T value;
  };
  using TypeIdxImportDesc = ImportDesc<uint32_t>;
  using TableTypeImportDesc = ImportDesc<TableType>;
  using MemTypeImportDesc = ImportDesc<MemoryType>;
  using GlobalTypeImportDesc = ImportDesc<GlobalType>;
  using ImportDescVariant =
      std::variant<TypeIdxImportDesc, TableTypeImportDesc, MemTypeImportDesc,
                   GlobalTypeImportDesc>;

  Name module_name;
  Name name;
  ImportDescVariant desc;
};

struct Export {
  struct ExportDesc {
    enum class ExportDescType : Byte {
      FuncIdx = 0x00,
      TableIdx = 0x01,
      MemIdx = 0x02,
      GlobalIdx = 0x03
    };
    ExportDescType type;
    uint32_t idx;
  };
  Name name;
  ExportDesc desc;
};

struct Func {
  uint32_t type;
  std::vector<ValueType> locals;
  std::vector<Byte> body;
};

struct Code {
  uint32_t size;
  Func code;
};

struct Global {
  GlobalType type;
  Expr init;
};

struct ElementSegment {
  uint32_t table;
  Expr offset;
  std::vector<uint32_t> init;
};

struct DataSegment {
  uint32_t data;
  Expr offset;
  std::vector<Byte> init;
};

template <class T>
struct Section {
  uint32_t size;
  T value;
};

using TypeSection = Section<Vec<FuncType>>;
using ImportSection = Section<Vec<Import>>;
using FuncSection = Section<Vec<uint32_t>>;
using TableSection = Section<Vec<TableType>>;
using MemorySection = Section<Vec<MemoryType>>;
using ExportSection = Section<Vec<Export>>;
using StartSection = Section<uint32_t>;
using CustomSection = Section<Custom>;

// These sections have the value which can't be distinguished on runtime.
// In detail, we can't determine the length of internal structures because it
// has expressions.
using RawBufferDataSection = Section<Bytes>;
using RawBufferCodeSection = Section<Bytes>;
using RawBufferElementSection = Section<Bytes>;
using RawBufferGlobalSection = Section<Bytes>;

using DataSection = Section<Vec<DataSegment>>;
using CodeSection = Section<Vec<Code>>;
using ElementSection = Section<Vec<ElementSegment>>;
using GlobalSection = std::vector<Global>;

struct Module {
 public:
  TypeSection type_sec;
  ImportSection import_sec;
  FuncSection func_sec;
  TableSection table_sec;
  MemorySection mem_sec;
  RawBufferGlobalSection global_sec;
  ExportSection export_sec;
  StartSection start_sec;
  RawBufferElementSection element_sec;
  RawBufferCodeSection code_sec;
  RawBufferDataSection data_sec;
  std::vector<CustomSection> custom_sec;
};

}  // namespace wasmparser

#endif  // WASMPARSER_CPP_MODULE_H
