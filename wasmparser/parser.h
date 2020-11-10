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

#ifndef WASMPARSER_CPP_PARSER_H
#define WASMPARSER_CPP_PARSER_H

#include <array>
#include <cassert>
#include <string_view>

#include "buffer.h"
#include "leb128.h"
#include "module.h"

namespace wasmparser {
namespace {
static constexpr std::array<Byte, 4> MAGIC = {0x00, 0x61, 0x73, 0x6D};
static constexpr std::array<Byte, 4> VERSION = {0x01, 0x00, 0x00, 0x00};
}  // namespace

class Parser {
 public:
  Parser(ZeroCopyBufferPtr buf) : buf_(std::move(buf)) {}
  static Module doParse(std::string_view filename);

 private:
  bool isEnd();
  bool checkMagicField();
  bool checkVersionField();
  bool doParseSection(Module* m);
  int32_t doParseCustomSection(CustomSection* cs);
  int32_t doParseTypeSection(TypeSection* ts);
  int32_t doParseFuncType(FuncType* ft);
  int32_t doParseName(Name* name);
  int32_t doParseValueTypes(ValType* val);
  int32_t doParseResultTypes(ResultType* rt);
  int32_t doParseU32Integer(uint32_t* size);
  int32_t doParseImportDesc(ImportDescVariant* vd);
  int32_t doParseImport(Import* ip);
  int32_t doParseImportSection(ImportSection* is);
  int32_t doParseFunctionSection(FuncSection* fc);
  int32_t doParseTypeIdx(TypeIdx* idx);
  int32_t doParseTableTypes(TableType* tt);
  int32_t doParseLimits(Limit* l);
  int32_t doParseMemoryTypes(MemoryType* mt);
  int32_t doParseGlobalTypes(GlobalType* gt);
  int32_t doParseTableSection(TableSection* ts);
  int32_t doParseMemorySection(MemorySection* ms);
  int32_t doParseGlobalSection(GlobalSection* gs);
  int32_t doParseFuncIdx(FuncIdx* idx);
  int32_t doParseTableIdx(TableIdx* idx);
  int32_t doParseMemIdx(MemIdx* idx);
  int32_t doParseGlobalIdx(GlobalIdx* idx);
  int32_t doParseExportSection(ExportSection* es);
  int32_t doParseExportDesc(ExportDesc* ed);
  int32_t doParseExport(Export* e);
  int32_t doParseStartSection(StartSection* ss);
  int32_t doParseElementSection(ElementSection* es);
  int32_t doParseCodeSection(CodeSection* cs);
  int32_t doParseDataSection(DataSection* ds);

  size_t idx_{8};
  ZeroCopyBufferPtr buf_;
};

bool Parser::isEnd() { return buf_->size() <= idx_; }

Module Parser::doParse(std::string_view filename) {
  auto buf = ZeroCopyBuffer::createBuffer(filename);
  Parser p(std::move(buf));

  if (!p.checkMagicField()) {
    throw std::runtime_error("Invalid magic number");
  }

  if (!p.checkVersionField()) {
    throw std::runtime_error("Invalid version number");
  }

  Module m;
  if (!p.doParseSection(&m)) {
    throw std::runtime_error("Failed to parse sections");
  }
  return m;
}

bool Parser::doParseSection(Module* m) {
  while (!isEnd()) {
    auto section_id = static_cast<SectionId>(*buf_->at(idx_));
    ++idx_;
    switch (section_id) {
      case SectionId::Custom: {
        CustomSection cs;
        if (doParseCustomSection(&cs) < 0) {
          return false;
        }
        m->custom_sec_.emplace_back(cs);
        break;
      }
      case SectionId::Type: {
        TypeSection ts;
        if (doParseTypeSection(&ts) < 0) {
          return false;
        }
        m->type_sec_ = ts;
        break;
      }
      case SectionId::Import: {
        ImportSection is;
        if (doParseImportSection(&is) < 0) {
          return false;
        }
        m->import_sec_ = is;
        break;
      }
      case SectionId::Function: {
        FuncSection fs;
        if (doParseFunctionSection(&fs) < 0) {
          return false;
        }
        m->func_sec_ = fs;
        break;
      }
      case SectionId::Table: {
        TableSection ts;
        if (doParseTableSection(&ts) < 0) {
          return false;
        }
        m->table_sec_ = ts;
        break;
      }
      case SectionId::Start: {
        StartSection ss;
        if (doParseStartSection(&ss) < 0) {
          return false;
        }
        m->start_sec_ = ss;
        break;
      }
      case SectionId::Element: {
        ElementSection es;
        if (doParseElementSection(&es) < 0) {
          return false;
        }
        m->element_sec_ = es;
        break;
      }
      case SectionId::Code: {
        CodeSection cs;
        if (doParseCodeSection(&cs) < 0) {
          return false;
        }
        m->code_sec_ = cs;
        break;
      }
      case SectionId::Data: {
        DataSection ds;
        if (doParseDataSection(&ds) < 0) {
          return false;
        }
        m->data_sec_ = ds;
        break;
      }
      case SectionId::Memory: {
        MemorySection ms;
        if (doParseMemorySection(&ms) < 0) {
          return false;
        }
        m->mem_sec_ = ms;
        break;
      }
      case SectionId::Export: {
        ExportSection es;
        if (doParseExportSection(&es) < 0) {
          return false;
        }
        m->export_sec_ = es;
        break;
      }
      case SectionId::Global: {
        GlobalSection gs;
        if (doParseGlobalSection(&gs) < 0) {
          return false;
        }
        m->global_sec_ = gs;
        break;
      }
      default:
        break;
    }
  }
  return true;
}

int32_t Parser::doParseValueTypes(ValType* val) {
  if (*buf_->at(idx_) == 0x7F) {
    *val = ValType::I32;
  } else if (*buf_->at(idx_) == 0x7E) {
    *val = ValType::I64;
  } else if (*buf_->at(idx_) == 0x7D) {
    *val = ValType::F32;
  } else if (*buf_->at(idx_) == 0x7C) {
    *val = ValType::F64;
  } else {
    return -1;
  }
  ++idx_;
  return 1;
}

int32_t Parser::doParseImportDesc(ImportDescVariant* vd) {
  size_t start_idx = idx_;
  if (*buf_->at(idx_) == 0x00) {
    ++idx_;
    TypeIdxImportDesc ti;
    if (doParseTypeIdx(&ti.value_) < 0) {
      return -1;
    }
    *vd = ti;
  } else if (*buf_->at(idx_) == 0x01) {
    ++idx_;
    TableTypeImportDesc tt;
    if (doParseTableTypes(&tt.value_) < 0) {
      return -1;
    }
    *vd = tt;
  } else if (*buf_->at(idx_) == 0x02) {
    ++idx_;
    MemTypeImportDesc mt;
    if (doParseMemoryTypes(&mt.value_) < 0) {
      return -1;
    }
    *vd = mt;
  } else if (*buf_->at(idx_) == 0x03) {
    ++idx_;
    GlobalTypeImportDesc gt;
    if (doParseGlobalTypes(&gt.value_) < 0) {
      return -1;
    }
    *vd = gt;
  } else {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseExportDesc(ExportDesc* ed) {
  size_t start_idx = idx_;
  if (*buf_->at(idx_) == 0x00) {
    ++idx_;
    ed->type_ = ExportDescType::FuncIdx;
    if (doParseFuncIdx(&ed->idx_) < 0) {
      return -1;
    }
  } else if (*buf_->at(idx_) == 0x01) {
    ++idx_;
    ed->type_ = ExportDescType::TableIdx;
    if (doParseTableIdx(&ed->idx_) < 0) {
      return -1;
    }
  } else if (*buf_->at(idx_) == 0x02) {
    ++idx_;
    ed->type_ = ExportDescType::MemIdx;
    if (doParseMemIdx(&ed->idx_) < 0) {
      return -1;
    }
  } else if (*buf_->at(idx_) == 0x03) {
    ++idx_;
    ed->type_ = ExportDescType::GlobalIdx;
    if (doParseGlobalIdx(&ed->idx_) < 0) {
      return -1;
    }
  } else {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseExport(Export* e) {
  size_t start_idx = idx_;
  if (doParseName(&e->name_) < 0) {
    return -1;
  }
  if (doParseExportDesc(&e->desc_) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseExportSection(ExportSection* es) {
  size_t start_idx = idx_;
  ++idx_;
  if (doParseU32Integer(&es->size_) < 0) {
    return -1;
  }
  if (doParseU32Integer(&es->value_.size_) < 0) {
    return -1;
  }
  size_t val_count = 0;
  while (val_count < es->value_.size_) {
    Export e;
    if (doParseExport(&e) < 0) {
      return -1;
    }
    es->value_.elem_.emplace_back(e);
    ++val_count;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseImport(Import* ip) {
  size_t start_idx = idx_;
  if (doParseName(&ip->module_name_) < 0) {
    return -1;
  }
  if (doParseName(&ip->name_) < 0) {
    return -1;
  }
  if (doParseImportDesc(&ip->desc_) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseStartSection(StartSection* ss) {
  size_t start_idx = idx_;
  if (doParseU32Integer(&ss->size_) < 0) {
    return -1;
  }
  if (doParseFuncIdx(&ss->value_) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseElementSection(ElementSection* es) {
  size_t start_idx = idx_;
  if (doParseU32Integer(&es->size_) < 0) {
    return -1;
  }
  if (doParseU32Integer(&es->value_.size_) < 0) {
    return -1;
  }
  auto u32_byte_len = doParseU32Integer(&es->value_.size_);
  if (u32_byte_len < 0) {
    return -1;
  }
  size_t vec_bytes = es->size_ - u32_byte_len;
  while (vec_bytes > 0) {
    es->value_.elem_.emplace_back(*buf_->at(idx_));
    --vec_bytes;
    ++idx_;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseFunctionSection(FuncSection* fc) {
  size_t start_idx = idx_;
  if (doParseU32Integer(&fc->size_) < 0) {
    return -1;
  }
  if (doParseU32Integer(&fc->value_.size_) < 0) {
    return -1;
  }
  size_t val_count = 0;
  while (val_count < fc->value_.size_) {
    TypeIdx idx;
    if (doParseTypeIdx(&idx) < 0) {
      return -1;
    }
    fc->value_.elem_.emplace_back(idx);
    ++val_count;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseImportSection(ImportSection* is) {
  size_t start_idx = idx_;
  if (doParseU32Integer(&is->size_) < 0) {
    return -1;
  }
  if (doParseU32Integer(&is->value_.size_) < 0) {
    return -1;
  }
  size_t val_count = 0;
  while (val_count < is->value_.size_) {
    Import ip;
    if (doParseImport(&ip) < 0) {
      return -1;
    }
    is->value_.elem_.emplace_back(ip);
    ++val_count;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseTableSection(TableSection* ts) {
  size_t start_idx = idx_;
  if (doParseU32Integer(&ts->size_) < 0) {
    return -1;
  }
  if (doParseU32Integer(&ts->value_.size_) < 0) {
    return -1;
  }
  size_t val_count = 0;
  while (val_count < ts->value_.size_) {
    TableType tt;
    if (doParseTableTypes(&tt) < 0) {
      return -1;
    }
    ts->value_.elem_.emplace_back(tt);
    ++val_count;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseGlobalSection(GlobalSection* gs) {
  size_t start_idx = idx_;
  if (doParseU32Integer(&gs->size_) < 0) {
    return -1;
  }
  auto u32_byte_len = doParseU32Integer(&gs->value_.size_);
  if (u32_byte_len < 0) {
    return -1;
  }
  size_t vec_bytes = gs->size_ - u32_byte_len;
  while (vec_bytes > 0) {
    gs->value_.elem_.emplace_back(*buf_->at(idx_));
    --vec_bytes;
    ++idx_;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseCodeSection(CodeSection* cs) {
  size_t start_idx = idx_;
  if (doParseU32Integer(&cs->size_) < 0) {
    return -1;
  }
  auto u32_byte_len = doParseU32Integer(&cs->value_.size_);
  if (u32_byte_len < 0) {
    return -1;
  }
  size_t vec_bytes = cs->size_ - u32_byte_len;
  while (vec_bytes > 0) {
    cs->value_.elem_.emplace_back(*buf_->at(idx_));
    --vec_bytes;
    ++idx_;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseLimits(Limit* l) {
  size_t start_idx = idx_;
  if (*buf_->at(idx_) == 0x00) {
    ++idx_;
    if (doParseU32Integer(&l->min_) < 0) {
      return -1;
    }
    l->max_ = std::nullopt;
    return idx_ - start_idx;
  } else if (*buf_->at(idx_) == 0x01) {
    ++idx_;
    if (doParseU32Integer(&l->min_) < 0) {
      return -1;
    }
    uint32_t max;
    if (doParseU32Integer(&max) < 0) {
      return -1;
    }
    l->max_ = max;
    return idx_ - start_idx;
  }
  return -1;
}

int32_t Parser::doParseTableTypes(TableType* tt) {
  size_t start_idx = idx_;
  // ElemType is only support 0x70 in current spec.
  if (*buf_->at(idx_) != 0x70) {
    return -1;
  }
  ++idx_;
  if (doParseLimits(&tt->limit_) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseDataSection(DataSection* ds) {
  size_t start_idx = idx_;
  if (doParseU32Integer(&ds->size_) < 0) {
    return -1;
  }
  auto u32_byte_len = doParseU32Integer(&ds->value_.size_);
  if (u32_byte_len < 0) {
    return -1;
  }
  size_t vec_bytes = ds->size_ - u32_byte_len;
  while (vec_bytes > 0) {
    ds->value_.elem_.emplace_back(*buf_->at(idx_));
    --vec_bytes;
    ++idx_;
  }

  return idx_ - start_idx;
}

int32_t Parser::doParseResultTypes(ResultType* rt) {
  size_t start_idx = idx_;
  if (doParseU32Integer(&rt->size_) < 0) {
    return -1;
  }
  size_t val_count = 0;
  while (val_count < rt->size_) {
    ValType val;
    if (doParseValueTypes(&val) < 0) {
      return -1;
    }
    rt->elem_.emplace_back(val);
    ++val_count;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseMemoryTypes(MemoryType* mt) {
  size_t start_idx = idx_;
  if (doParseLimits(&mt->limit_) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseFuncType(FuncType* ft) {
  size_t start_idx = idx_;
  if (*buf_->at(idx_) != 0x60) {
    return -1;
  }
  ++idx_;
  if (doParseResultTypes(&ft->param_) < 0) {
    return -1;
  }
  if (doParseResultTypes(&ft->return_) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseTypeSection(TypeSection* ts) {
  size_t start_idx = idx_;
  if (doParseU32Integer(&ts->size_) < 0) {
    return -1;
  }
  if (doParseU32Integer(&ts->value_.size_) < 0) {
    return -1;
  }
  size_t val_count = 0;
  while (val_count < ts->value_.size_) {
    FuncType ft;
    if (doParseFuncType(&ft) < 0) {
      return -1;
    }
    ts->value_.elem_.emplace_back(ft);
    ++val_count;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseMemorySection(MemorySection* ms) {
  size_t start_idx = idx_;
  if (doParseU32Integer(&ms->size_) < 0) {
    return -1;
  }
  if (doParseU32Integer(&ms->value_.size_) < 0) {
    return -1;
  }
  size_t val_count = 0;
  while (val_count < ms->value_.size_) {
    MemoryType mt;
    if (doParseMemoryTypes(&mt) < 0) {
      return -1;
    }
    ms->value_.elem_.emplace_back(mt);
    ++val_count;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseTypeIdx(TypeIdx* idx) {
  size_t start_idx = idx_;
  if (doParseU32Integer(idx) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseFuncIdx(FuncIdx* idx) {
  size_t start_idx = idx_;
  if (doParseU32Integer(idx) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseTableIdx(TableIdx* idx) {
  size_t start_idx = idx_;
  if (doParseU32Integer(idx) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseMemIdx(MemIdx* idx) {
  size_t start_idx = idx_;
  if (doParseU32Integer(idx) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseGlobalIdx(GlobalIdx* idx) {
  size_t start_idx = idx_;
  if (doParseU32Integer(idx) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseGlobalTypes(GlobalType* gt) {
  size_t start_idx = idx_;
  if (doParseValueTypes(&gt->val_type_) < 0) {
    return -1;
  }
  if (*buf_->at(idx_) == 0x00) {
    gt->mut_ = Mutability::Const;
  } else if (*buf_->at(idx_) == 0x01) {
    gt->mut_ = Mutability::Var;
  } else {
    return -1;
  }
  ++idx_;
  return idx_ - start_idx;
}

int32_t Parser::doParseCustomSection(CustomSection* cs) {
  size_t start_idx = idx_;
  if (doParseU32Integer(&cs->size_) < 0) {
    return -1;
  }
  auto name_size = doParseName(&cs->value_.name_);
  if (name_size < 0) {
    return -1;
  }
  auto bytes_rem = cs->size_ - name_size;
  while (bytes_rem > 0) {
    cs->value_.bytes_.emplace_back(*buf_->at(idx_));
    ++idx_;
    --bytes_rem;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseName(Name* name) {
  size_t start_idx = idx_;
  if (doParseU32Integer(&name->size_) < 0) {
    return -1;
  }
  size_t val_count = 0;
  while (val_count < name->size_) {
    name->elem_.emplace_back(*buf_->at(idx_));
    ++idx_;
    ++val_count;
  }
  return idx_ - start_idx;
}

int32_t Parser::doParseU32Integer(uint32_t* size) {
  size_t start_idx = idx_;
  auto res = decodeULEB128(buf_->at(idx_), buf_->at(idx_ + 3), size);
  if (res == 0) {
    return -1;
  }
  idx_ += res;
  return idx_ - start_idx;
}

bool Parser::checkMagicField() {
  if (buf_->size() < 4) {
    return false;
  }
  for (size_t i = 0; i < MAGIC.size(); ++i) {
    if (*buf_->at(i) != MAGIC[i]) {
      return false;
    }
  }
  return true;
}

bool Parser::checkVersionField() {
  if (buf_->size() < 8) {
    return false;
  }
  for (size_t i = 4; i < 4 + VERSION.size(); ++i) {
    if (*buf_->at(i) != VERSION[i - 4]) {
      return false;
    }
  }
  return true;
}
}  // namespace wasmparser

#endif  // WASMPARSER_CPP_PARSER_H
