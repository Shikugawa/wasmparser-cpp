//
// Created by shikugawa on 2020/11/17.
//

#ifndef WASMPARSER_CPP_INSTRUCTION_DECODER_H
#define WASMPARSER_CPP_INSTRUCTION_DECODER_H

#include "module.h"

namespace wasmparser {

struct NumericConstInstruction {
  enum class Type : Byte {
    I32_CONST = 0x41,
    I64_CONST = 0x42,
    F32_CONST = 0x43,
    F64_CONST = 0x44,
  };
  Type type;
  // The length of operand is 4 or 8 bytes.
  std::vector<Byte> operand;
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

  bool decodeGlobalSection(RawBufferGlobalSection * ds);
  bool decodeElementSection(RawBufferElementSection* es);

  int32_t decodeIndex(uint32_t* idx);
  int32_t decodeGlobalType(GlobalType* gt);
  int32_t decodeExpr(Expr* e);
  int32_t decodeNumericInstruction(NumericInstruction* ni);

  Byte* currentByte() {
    return &target_section_->value.elem[idx_];
  }

  size_t idx_;
  Section<Bytes>* target_section_;

 DataSection ds_;
 CodeSection cs_;
 GlobalSection gs_;
 ElementSection es_;
};

bool InstructionDecoder::decodeGlobalSection(RawBufferGlobalSection* ds) {
  idx_ = 0;
  target_section_ = ds;
  while (idx_ < target_section_->value.size) {
    Global g;
    if (decodeGlobalType(&g.type) < 0) {
      return -1;
    }
    if (decodeExpr(&g.init) < 0) {
      return -1;
    }
    gs_.emplace_back(g);
  }
}

bool InstructionDecoder::decodeElementSection(RawBufferElementSection* es) {
  idx_ = 0;
  target_section_ = es;
  while (idx_ < target_section_->value.size) {
    ElementSegment eseg;
//    if ()
  }
}

//InstructionDecoder::

int32_t InstructionDecoder::decodeGlobalType(GlobalType* gt) {
  size_t start_idx = idx_;
  if (*currentByte() == 0x7F) {
    gt->val_type = ValueType::I32;
  } else if (*currentByte() == 0x7E) {
    gt->val_type = ValueType::I64;
  } else if (*currentByte() == 0x7D) {
    gt->val_type = ValueType::F32;
  } else if (*currentByte() == 0x7C) {
    gt->val_type = ValueType::F64;
  } else {
    return -1;
  }
  ++idx_;
  if (*currentByte() == 0x00) {
    gt->mut = GlobalType::Mutability::Const;
  } else if (*currentByte() == 0x01) {
    gt->mut = GlobalType::Mutability::Var;
  } else {
    return -1;
  }
  ++idx_;
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeExpr(Expr* e) {
  size_t start_idx = idx_;
  if (0x45 <= *currentByte() && *currentByte() <= 0xC4) {
    NumericInstruction ni;
    if (decodeNumericInstruction(&ni) < 0) {
      return -1;
    }
    // TODO: push instruction
  }
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeNumericInstruction(NumericInstruction* ni) {
  ni->type = static_cast<NumericInstruction::Type>(target_section_.value.elem[idx_]);
  ++idx_;
}

}

#endif  // WASMPARSER_CPP_INSTRUCTION_DECODER_H
