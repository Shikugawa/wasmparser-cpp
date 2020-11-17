//
// Created by shikugawa on 2020/11/17.
//

#ifndef WASMPARSER_CPP_INSTRUCTION_DECODER_H
#define WASMPARSER_CPP_INSTRUCTION_DECODER_H

#include "leb128.h"
#include "module.h"

namespace wasmparser {

struct UnreachableInstruction;  // 0x00
struct NopInstruction;          // 0x01

struct BlockInstruction {
  enum class Type : Byte {
    BLOCK = 0x02,
    LOOP = 0x03,
    IF = 0x04,
  };
  Type type;
  enum class BlockType {
    Empty,
    ValueType,
    TypeIndex,
  };
  BlockType block_type;
  union {
    ValueType value_type;
    int64_t type_idx;  // >= 0
  };
  // TODO: add instructions space
};

struct BranchInstruction {
  enum class Type : Byte {
    BR = 0x0C,
    BR_IF = 0x0D,
  };
  Type type;
  uint32_t index;
};

struct TableBranchInstruction {
  std::vector<uint32_t> l;
  uint32_t ln;
};

struct ReturnInstruction;  // 0x0F

struct CallInstruction {
  enum class Type : Byte {
    CALL = 0x10,
    CALL_INDIRECT = 0x11,
  };
  Type type;
  uint32_t index;
};

struct ParametricInstruction {
  enum class Type : Byte {
    DROP = 0x1A,
    SELECT = 0x1B,
  };
  Type type;
};

struct VariableInstruction {
  enum class Type : Byte {
    LOCAL_GET = 0x20,
    LOCAL_SET = 0x21,
    LOCAL_TEE = 0x22,
    GLOBAL_GET = 0x23,
    GLOBAL_SET = 0x24,
  };
  Type type;
  uint32_t idx;
};

struct BasicMemoryInstruction {
  enum class Type : Byte {
    I32_LOAD = 0x28,
    I64_LOAD = 0x29,
    F32_LOAD = 0x2A,
    F64_LOAD = 0x2B,
    I32_LOAD8_S = 0x2C,
    I32_LOAD8_U = 0x2D,
    I32_LOAD16_S = 0x2E,
    I32_LOAD16_U = 0x2F,
    I64_LOAD8_S = 0x30,
    I64_LOAD8_U = 0x31,
    I64_LOAD16_S = 0x32,
    I64_LOAD16_U = 0x33,
    I64_LOAD32_S = 0x34,
    I64_LOAD32_U = 0x35,
    I32_STORE = 0x36,
    I64_STORE = 0x37,
    F32_STORE = 0x38,
    F64_STORE = 0x39,
    I32_STORE_8 = 0x3A,
    I32_STORE_16 = 0x3B,
    I64_STORE_8 = 0x3C,
    I64_STORE_16 = 0x3D,
    I64_STORE_32 = 0x3E,
  };
  Type type;
  struct MemoryArgument {
    uint32_t align;
    uint32_t offset;
  };
  MemoryArgument arg;
};

struct MemorySizeInstruction {
  enum class Type : Byte {
    MEMORY_SIZE = 0x3F,
    MEMORY_GLOW = 0x40,
  };
  Type type;
};

struct NumericConstInstruction {
  enum class Type : Byte {
    I32_CONST = 0x41,
    I64_CONST = 0x42,
    F32_CONST = 0x43,
    F64_CONST = 0x44,
  };
  Type type;
  union {
    int32_t i32_value;
    int64_t i64_value;
    float f32_value;
    double f64_value;
  };
};

struct NumericInstruction {
  enum class Type : Byte {
    I32_EQZ = 0x45,
    I32_EQ = 0x46,
    I32_NE = 0x47,
    I32_LT_S = 0x48,
    I32_LT_U = 0x49,
    I32_GT_S = 0x4A,
    I32_GT_U = 0x4B,
    I32_LE_S = 0x4C,
    I32_LE_U = 0x4D,
    I32_GE_S = 0x4E,
    I32_GE_U = 0x4F,
    I64_EQZ = 0x50,
    I64_EQ = 0x51,
    I64_NE = 0x52,
    I64_LT_S = 0x53,
    I64_LT_U = 0x54,
    I64_GT_S = 0x55,
    I64_GT_U = 0x56,
    I64_LE_S = 0x57,
    I64_LE_U = 0x58,
    I64_GE_S = 0x59,
    I64_GE_U = 0x5A,
    F32_EQ = 0x5B,
    F32_NE = 0x5C,
    F32_LT = 0x5D,
    F32_GT = 0x5E,
    F32_LE = 0x5F,
    F32_GE = 0x60,
    F64_EQ = 0x61,
    F64_NE = 0x62,
    F64_LT = 0x63,
    F64_GT = 0x64,
    F64_LE = 0x65,
    F64_GE = 0x66,
    I32_CLZ = 0x67,
    I32_CTZ = 0x68,
    I32_POPCNT = 0x69,
    I32_ADD = 0x6A,
    I32_SUB = 0x6B,
    I32_MUL = 0x6C,
    I32_DIV_S = 0x6D,
    I32_DIV_U = 0x6E,
    I32_REM_S = 0x6F,
    I32_REM_U = 0x70,
    I32_AND = 0x71,
    I32_OR = 0x72,
    I32_XOR = 0x73,
    I32_SHL = 0x74,
    I32_SHR_S = 0x75,
    I32_SHR_U = 0x76,
    I32_ROTL = 0x77,
    I32_ROTR = 0x78,
    I64_CLZ = 0x79,
    I64_CTZ = 0x7A,
    I64_POPCNT = 0x7B,
    I64_ADD = 0x7C,
    I64_SUB = 0x7D,
    I64_MUL = 0x7E,
    I64_DIV_S = 0x7F,
    I64_DIV_U = 0x80,
    I64_REM_S = 0x81,
    I64_REM_U = 0x82,
    I64_AND = 0x83,
    I64_OR = 0x84,
    I64_XOR = 0x85,
    I64_SHL = 0x86,
    I64_SHR_S = 0x87,
    I64_SHR_U = 0x88,
    I64_ROTL = 0x89,
    I64_ROTR = 0x8A,
    F32_ABS = 0x8B,
    F32_NEG = 0x8C,
    F32_CEIL = 0x8D,
    F32_FLOOR = 0x8E,
    F32_TRUNC = 0x8F,
    F32_NEAREST = 0x90,
    F32_SQRT = 0x91,
    F32_ADD = 0x92,
    F32_SUB = 0x93,
    F32_MUL = 0x94,
    F32_DIV = 0x95,
    F32_MIN = 0x96,
    F32_MAX = 0x97,
    F32_COPYSIGN = 0x98,
    F64_ABS = 0x99,
    F64_NEG = 0x9A,
    F64_CEIL = 0x9B,
    F64_FLOOR = 0x9C,
    F64_TRUNC = 0x9D,
    F64_NEAREST = 0x9E,
    F64_SQRT = 0x9F,
    F64_ADD = 0xA0,
    F64_SUB = 0xA1,
    F64_MUL = 0xA2,
    F64_DIV = 0xA3,
    F64_MIN = 0xA4,
    F64_MAX = 0xA5,
    F64_COPYSIGN = 0xA6,
    I32_WRAP_I64 = 0xA7,
    I32_TRUNC_F32_S = 0xA8,
    I32_TRUNC_F32_U = 0xA9,
    I32_TRUNC_F64_S = 0xAA,
    I32_TRUNC_F64_U = 0xAB,
    I64_EXTEND_I32_S = 0xAC,
    I64_EXTEND_I32_U = 0xAD,
    I64_TRUNC_F32_S = 0xAE,
    I64_TRUNC_F32_U = 0xAF,
    I64_TRUNC_F64_S = 0xB0,
    I64_TRUNC_F64_U = 0xB1,
    F32_CONVERT_I32_S = 0xB2,
    F32_CONVERT_I32_U = 0xB3,
    F32_CONVERT_I64_S = 0xB4,
    F32_CONVERT_I64_U = 0xB5,
    F32_DEMOTE_F64 = 0xB6,
    F64_CONVERT_I32_S = 0xB7,
    F64_CONVERT_I32_U = 0xB8,
    F64_CONVERT_I64_S = 0xB9,
    F64_CONVERT_I64_U = 0xBA,
    F64_PROMOTE_F32 = 0xBB,
    I32_REINTERPRET_F64 = 0xBC,
    I64_REINTERPRET_F64 = 0xBD,
    F32_REINTERPRET_I32 = 0xBE,
    F64_REINTERPRET_I64 = 0xBF,
    I32_EXTEND8_S = 0xC0,
    I32_EXTEND16_S = 0xC1,
    I64_EXTEND8_S = 0xC2,
    I64_EXTEND16_S = 0xC3,
    I64_EXTEND32_S = 0xC4,
  };
  Type type;
};

class InstructionDecoder {
 public:
  InstructionDecoder(Module* m);

  bool decodeGlobalSection(RawBufferGlobalSection* gs);
  bool decodeElementSection(RawBufferElementSection* es);
  bool decodeDataSection(RawBufferDataSection* ds);
  bool decodeCodeSection(RawBufferCodeSection* cs);

  int32_t decodeValueType(ValueType* vt);
  int32_t decodeU32Integer(uint32_t* idx);
  int32_t decodeI32Integer(int32_t* idx);
  int32_t decodeI64Integer(int64_t* idx);
  int32_t decodeS33AsI64(int64_t* idx);
  int32_t decodeGlobalType(GlobalType* gt);
  int32_t decodeExpr(Expr* e);
  int32_t decodeFunc(Func* f);
  int32_t decodeLocals(Func::Local* l);

  // Instruction decoder
  int32_t decodeInstruction(/* Generic instruction pointer */);
  int32_t decodeNumericInstruction(NumericInstruction* ni);
  int32_t decodeNumericConstInstruction(NumericConstInstruction* nci);
  int32_t decodeBasicMemoryInstruction(BasicMemoryInstruction* bmi);
  int32_t decodeMemorySizeInstruction(MemorySizeInstruction* msi);
  int32_t decodeMemoryArgument(BasicMemoryInstruction::MemoryArgument* arg);
  int32_t decodeVariableInstruction(VariableInstruction* vi);
  int32_t decodeParametricInstruction(ParametricInstruction* pi);
  int32_t decodeBlockType(BlockInstruction* bi);
  int32_t decodeBlockInstruction(BlockInstruction* bi);
  int32_t decodeBranchInstruction(BranchInstruction* bi);
  int32_t decodeTableBranchInstruction(TableBranchInstruction* tbi);
  int32_t decodeCallInstruction(CallInstruction* ci);

  Byte* fetchByte(size_t offset = 0) {
    return &target_section_->value.elem[idx_ + offset];
  }

  uint32_t fetchVecSize() {
    uint32_t size;
    if (decodeU32Integer(&size) < 0) {
      return 0;
    }
    return size;
  }

  size_t idx_;
  Section<Bytes>* target_section_;

  DataSection ds_;
  CodeSection cs_;
  GlobalSection gs_;
  ElementSection es_;
};

bool InstructionDecoder::decodeGlobalSection(RawBufferGlobalSection* gs) {
  idx_ = 0;
  target_section_ = gs;
  while (idx_ < target_section_->value.size) {
    Global g;
    if (decodeGlobalType(&g.type) < 0) {
      return false;
    }
    if (decodeExpr(&g.init) < 0) {
      return false;
    }
    gs_.emplace_back(g);
  }
  target_section_ = nullptr;
  return true;
}

bool InstructionDecoder::decodeElementSection(RawBufferElementSection* es) {
  idx_ = 0;
  target_section_ = es;
  while (idx_ < target_section_->value.size) {
    ElementSegment eseg;
    if (decodeU32Integer(&eseg.table) < 0) {
      return false;
    }
    if (decodeExpr(&eseg.offset) < 0) {
      return false;
    }
    uint32_t vec_size = fetchVecSize();
    std::vector<uint32_t> init;
    while (vec_size > 0) {
      uint32_t num;
      if (decodeU32Integer(&num) < 0) {
        return false;
      }
      init.emplace_back(num);
      --vec_size;
    }
    eseg.init = init;
    es_.emplace_back(eseg);
  }
  target_section_ = nullptr;
  return true;
}

bool InstructionDecoder::decodeDataSection(RawBufferDataSection* ds) {
  idx_ = 0;
  target_section_ = ds;
  while (idx_ < target_section_->value.size) {
    DataSegment dseg;
    if (decodeU32Integer(&dseg.data) < 0) {
      return false;
    }
    if (decodeExpr(&dseg.offset) < 0) {
      return false;
    }
    uint32_t vec_size = fetchVecSize();
    std::vector<Byte> init;
    while (vec_size > 0) {
      init.emplace_back(*fetchByte());
      ++idx_;
      --vec_size;
    }
    dseg.init = init;
    ds_.emplace_back(dseg);
  }
  target_section_ = nullptr;
  return true;
}

bool InstructionDecoder::decodeCodeSection(RawBufferCodeSection* cs) {
  idx_ = 0;
  target_section_ = cs;
  while (idx_ < target_section_->value.size) {
    Code c;
    if (decodeU32Integer(&c.size) < 0) {
      return false;
    }
    if (decodeFunc(&c.code) < 0) {
      return false;
    }
    cs_.emplace_back(c);
  }
  target_section_ = nullptr;
  return true;
}

int32_t InstructionDecoder::decodeValueType(ValueType* vt) {
  size_t start_idx = idx_;
  if (*fetchByte() == 0x7F) {
    *vt = ValueType::I32;
  } else if (*fetchByte() == 0x7E) {
    *vt = ValueType::I64;
  } else if (*fetchByte() == 0x7D) {
    *vt = ValueType::F32;
  } else if (*fetchByte() == 0x7C) {
    *vt = ValueType::F64;
  } else {
    return -1;
  }
  ++idx_;
  return start_idx - idx_;
}

int32_t InstructionDecoder::decodeFunc(Func* f) {
  size_t start_idx = idx_;
  auto vec_size = fetchVecSize();
  while (vec_size > 0) {
    Func::Local l;
    if (decodeLocals(&l) < 0) {
      return -1;
    }
    f->locals.emplace_back(l);
    --vec_size;
  }
  if (decodeExpr(&f->expr) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeLocals(Func::Local* l) {
  size_t start_idx = idx_;
  if (decodeU32Integer(&l->n) < 0) {
    return -1;
  }
  if (decodeValueType(&l->t) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeU32Integer(uint32_t* idx) {
  size_t start_idx = idx_;
  auto res = decodeULEB128(fetchByte(), fetchByte(3), idx);
  if (res == 0) {
    return -1;
  }
  idx_ += res;
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeI32Integer(int32_t* idx) {
  size_t start_idx = idx_;
  auto res = decodeSLEB128(fetchByte(), fetchByte(3), idx);
  if (res == 0) {
    return -1;
  }
  idx_ += res;
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeI64Integer(int64_t* idx) {
  size_t start_idx = idx_;
  auto res = decodeSLEB128(fetchByte(), fetchByte(7), idx);
  if (res == 0) {
    return -1;
  }
  idx_ += res;
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeS33AsI64(int64_t* idx) {
  size_t start_idx = idx_;
  auto res = decodeS33LEB128(fetchByte(), fetchByte(4), idx);
  if (res == 0) {
    return -1;
  }
  idx += res;
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeGlobalType(GlobalType* gt) {
  size_t start_idx = idx_;
  if (decodeValueType(&gt->val_type) < 0) {
    return -1;
  }
  if (*fetchByte() == 0x00) {
    gt->mut = GlobalType::Mutability::Const;
  } else if (*fetchByte() == 0x01) {
    gt->mut = GlobalType::Mutability::Var;
  } else {
    return -1;
  }
  ++idx_;
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeExpr(Expr* e) {
  size_t start_idx = idx_;
  while (*fetchByte() != 0x0B) {
    // TODO: let instruction
    if (decodeInstruction() < 0) {
      return -1;
    }
    // TODO: push instruction
  }
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeInstruction() {
  size_t start_idx = idx_;
  if (0x45 <= *fetchByte() && *fetchByte() <= 0xC4) {
    NumericInstruction ni;
    if (decodeNumericInstruction(&ni) < 0) {
      return -1;
    }
  } else if (0x41 <= *fetchByte() && *fetchByte() <= 0x44) {
    NumericConstInstruction nci;
    if (decodeNumericConstInstruction(&nci) < 0) {
      return -1;
    }
  } else if (0x20 <= *fetchByte() && *fetchByte() <= 0x24) {
    VariableInstruction vi;
    if (decodeVariableInstruction(&vi) < 0) {
      return -1;
    }
  } else if (0x00 <= *fetchByte() && *fetchByte() <= 0x01) {

  } else if (0x02 <= *fetchByte() && *fetchByte() <= 0x04) {

  } else if (0x0C <= *fetchByte() && *fetchByte() <= 0x0E) {

  } else if (0x0E == *fetchByte()) {

  } else if (0x0F == *fetchByte()) {

  } else if (0x10 <= *fetchByte() && *fetchByte() <= 0x11) {

  } else {
    assert(false);
  }
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeNumericInstruction(NumericInstruction* ni) {
  size_t start_idx = idx_;
  ni->type = static_cast<NumericInstruction::Type>(*fetchByte());
  ++idx_;
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeNumericConstInstruction(
    NumericConstInstruction* nci) {
  nci->type = static_cast<NumericConstInstruction::Type>(*fetchByte());
  ++idx_;
  switch (nci->type) {
    case NumericConstInstruction::Type::I32_CONST:
      if (decodeI32Integer(&nci->i32_value) < 0) {
        return -1;
      }
      break;
    case NumericConstInstruction::Type::I64_CONST:
      if (decodeI64Integer(&nci->i64_value) < 0) {
        return -1;
      }
      break;
    case NumericConstInstruction::Type::F32_CONST:
    case NumericConstInstruction::Type::F64_CONST:
    default:
      throw std::runtime_error("Not supported instruction has appeared");
  }
}

int32_t InstructionDecoder::decodeBasicMemoryInstruction(
    BasicMemoryInstruction* bmi) {
  size_t start_idx = idx_;
  bmi->type = static_cast<BasicMemoryInstruction::Type>(*fetchByte());
  ++idx_;
  if (decodeMemoryArgument(&bmi->arg) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeMemorySizeInstruction(
    MemorySizeInstruction* msi) {
  size_t start_idx = idx_;
  msi->type = static_cast<MemorySizeInstruction::Type>(*fetchByte());
  ++idx_;
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeMemoryArgument(
    BasicMemoryInstruction::MemoryArgument* arg) {
  size_t start_idx = idx_;
  if (decodeU32Integer(&arg->align) < 0) {
    return -1;
  }
  if (decodeU32Integer(&arg->offset) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeVariableInstruction(VariableInstruction* vi) {
  size_t start_idx = idx_;
  vi->type = static_cast<VariableInstruction::Type>(*fetchByte());
  ++idx_;
  if (decodeU32Integer(&vi->idx) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeParametricInstruction(
    ParametricInstruction* pi) {
  size_t start_idx = idx_;
  pi->type = static_cast<ParametricInstruction::Type>(*fetchByte());
  ++idx_;
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeBlockType(BlockInstruction* bi) {
  size_t start_idx = idx_;
  if (*fetchByte() == 0x40) {
    bi->block_type = BlockInstruction::BlockType::Empty;
    ++idx_;
    return idx_ - start_idx;
  }
  ValueType vt;
  if (decodeValueType(&vt) > 0) {
    bi->block_type = BlockInstruction::BlockType::ValueType;
    bi->value_type = vt;
    return idx_ - start_idx;
  }
  // TODO: it it work correctly?
  if (decodeS33AsI64(&bi->type_idx) < 0) {
    return -1;
  }
  bi->block_type = BlockInstruction::BlockType::TypeIndex;
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeBlockInstruction(BlockInstruction* bi) {
  size_t start_idx = idx_;
  bi->type = static_cast<BlockInstruction::Type>(*fetchByte());
  ++idx_;
  if (decodeBlockType(bi) < 0) {
    return -1;
  }
else_block:
  while (true) {
    if (*fetchByte() == 0x0B) {
      break;
    }
    if (*fetchByte() == 0x05) {
      goto else_block;
    }
    if (decodeInstruction() < 0) {
      return -1;
    }
  }
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeBranchInstruction(BranchInstruction* bi) {
  size_t start_idx = idx_;
  bi->type = static_cast<BranchInstruction::Type>(*fetchByte());
  ++idx_;
  if (decodeU32Integer(&bi->index) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeTableBranchInstruction(
    TableBranchInstruction* tbi) {
  size_t start_idx = idx_;
  ++idx_;
  auto vec_size = fetchVecSize();
  std::vector<uint32_t> label_idxs;
  while (vec_size > 0) {
    uint32_t label_idx;
    if (decodeU32Integer(&label_idx) < 0) {
      return -1;
    }
    label_idxs.emplace_back(label_idx);
    --vec_size;
  }
  if (decodeU32Integer(&tbi->ln) < 0) {
    return -1;
  }
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeCallInstruction(CallInstruction* ci) {
  size_t start_idx = idx_;
  ci->type = static_cast<CallInstruction::Type>(*fetchByte());
  ++idx_;
  if (decodeU32Integer(&ci->index) < 0) {
    return -1;
  }
  if (ci->type == CallInstruction::Type::CALL_INDIRECT) {
    if (0x00 != *fetchByte()) {
      return -1;
    }
    ++idx_;
  }
  return idx_ - start_idx;
}

}  // namespace wasmparser

#endif  // WASMPARSER_CPP_INSTRUCTION_DECODER_H
