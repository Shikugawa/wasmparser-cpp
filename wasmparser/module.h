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

#include "instruction.h"
#include "types.h"

namespace wasmparser {

using TypeIdx = uint32_t;
using FuncIdx = uint32_t;
using TableIdx = uint32_t;
using MemIdx = uint32_t;
using GlobalIdx = uint32_t;
using LocalIdx = uint32_t;
using LabelIdx = uint32_t;

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
  Code = 0x10,
  Data = 0x11,
};

struct Custom {
  Name name_;
  Bytes bytes_;
};

template <class T>
struct ImportDesc {
  T value_;
};

using TypeIdxImportDesc = ImportDesc<TypeIdx>;
using TableTypeImportDesc = ImportDesc<TableType>;
using MemTypeImportDesc = ImportDesc<MemoryType>;
using GlobalTypeImportDesc = ImportDesc<GlobalType>;

using ImportDescVariant = std::variant<TypeIdxImportDesc, TableTypeImportDesc,
                                       MemTypeImportDesc, GlobalTypeImportDesc>;

struct Import {
  Name module_name_;
  Name name_;
  ImportDescVariant desc_;
};

struct Global {
  GlobalType type_;
  Expr expr_;
};

enum class ExportDescType : Byte {
  FuncIdx = 0x00,
  TableIdx = 0x01,
  MemIdx = 0x02,
  GlobalIdx = 0x03
};

struct ExportDesc {
  ExportDescType type_;
  uint32_t idx_;
};

struct Export {
  Name name_;
  ExportDesc desc_;
};

struct Element {
  TableIdx table_;
  Expr offset_;
  Vec<FuncIdx> init_;
};

struct Local {
  uint32_t nums_;  // denoting count locals of the same value type.
  ValType type_;
};

struct Func {
  Vec<Local> locals_;
  Expr body_;
};

struct Code {
  uint32_t size_;  // the function code in bytes,
  Func code_;
};

struct Data {
 public:
  MemIdx idx_;
  Expr offset_;
  Bytes bytes_;
};

template <class T>
struct Section {
  uint32_t size_;
  T value_;
};

using TypeSection = Section<Vec<FuncType>>;
using ImportSection = Section<Vec<Import>>;
using FuncSection = Section<Vec<FuncIdx>>;
using TableSection = Section<Vec<TableType>>;
using MemorySection = Section<Vec<MemoryType>>;
using GlobalSection = Section<Vec<Global>>;
using ExportSection = Section<Vec<Export>>;
using StartSection = Section<FuncIdx>;
using ElementSection = Section<Vec<Element>>;
using CodeSection = Section<Vec<Code>>;
using DataSection = Section<Vec<Data>>;
using CustomSection = Section<Custom>;

struct Module {
 public:
  TypeSection type_sec_;
  ImportSection import_sec_;
  FuncSection func_sec_;
  TableSection table_sec_;
  MemorySection mem_sec_;
  GlobalSection global_sec_;
  ExportSection export_sec_;
  StartSection start_sec_;
  ElementSection element_sec_;
  CodeSection code_sec_;
  DataSection data_sec_;
  std::vector<CustomSection> custom_sec_;
};

}  // namespace wasmparser

#endif  // WASMPARSER_CPP_MODULE_H
