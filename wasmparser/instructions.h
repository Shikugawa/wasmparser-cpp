//
// Created by shikugawa on 2020/11/18.
//

#ifndef WASMPARSER_CPP_INSTRUCTIONS_H
#define WASMPARSER_CPP_INSTRUCTIONS_H

#include "types.h"

namespace wasmparser {
enum class InstructionType {
  SingleOperandControl,
  Block,
  Branch,
  TableBranch,
  Call,
  Parametric,
  Variable,
  BasicMemory,
  MemorySize,
  Numeric,
  NumericConst,
};

struct Instruction;

struct SingleOperandControlInstruction {
  enum class Type : Byte {
    UNREACHABLE = 0x00,
    NOP = 0x01,
    RETURN = 0x0F,
  };
  Type type;
};

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
//  std::vector<Instruction> instructions;
//  std::vector<Instruction> else_instructions;
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
//  std::vector<uint32_t> l;
  uint32_t ln;
};

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

struct Instruction {
  InstructionType type;
  union {
    SingleOperandControlInstruction single_operand_control_instruction;
    BlockInstruction block_instruction;
    BranchInstruction branch_instruction;
    TableBranchInstruction table_branch_instruction;
    CallInstruction call_instruction;
    ParametricInstruction parametric_instruction;
    VariableInstruction variable_instruction;
    BasicMemoryInstruction basic_memory_instruction;
    MemorySizeInstruction memory_size_instruction;
    NumericConstInstruction numeric_const_instruction;
    NumericInstruction numeric_instruction;
  };
};
}  // namespace wasmparser

#endif  // WASMPARSER_CPP_INSTRUCTIONS_H
