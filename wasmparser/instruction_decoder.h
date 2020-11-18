//
// Created by shikugawa on 2020/11/17.
//

#ifndef WASMPARSER_CPP_INSTRUCTION_DECODER_H
#define WASMPARSER_CPP_INSTRUCTION_DECODER_H

#include "instructions.h"
#include "leb128.h"
#include "module.h"

namespace wasmparser {

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
  int32_t decodeExpr(std::vector<Instruction>* iseq);
  int32_t decodeFunc(Func* f);
  int32_t decodeLocals(Func::Local* l);

  // Instruction decoder
  int32_t decodeInstruction(Instruction* i);
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

InstructionDecoder::InstructionDecoder(Module* m) {
  if (!decodeGlobalSection(&m->global_sec)) {
    throw std::runtime_error("Failed to decode global section.");
  }
  if (!decodeDataSection(&m->data_sec)) {
    throw std::runtime_error("Failed to decode data section");
  }
  if (!decodeCodeSection(&m->code_sec)) {
    throw std::runtime_error("Failed to decode code section");
  }
  if (!decodeElementSection(&m->element_sec)) {
    throw std::runtime_error("Failed to decode element section");
  }
}

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
  return idx_ - start_idx;
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
  auto res = decodeSLEB128(fetchByte(), fetchByte(4), idx);
  if (res == 0) {
    return -1;
  }
  idx_ += res;
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeI64Integer(int64_t* idx) {
  size_t start_idx = idx_;
  auto res = decodeSLEB128(fetchByte(), fetchByte(8), idx);
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

int32_t InstructionDecoder::decodeExpr(std::vector<Instruction>* iseq) {
  size_t start_idx = idx_;
  while (*fetchByte() != 0x0B) {
    Instruction i;
    if (decodeInstruction(&i) < 0) {
      return -1;
    }
    iseq->emplace_back(i);
  }
  ++idx_;
  return idx_ - start_idx;
}

int32_t InstructionDecoder::decodeInstruction(Instruction* i) {
  size_t start_idx = idx_;
  if (0x28 <= *fetchByte() && *fetchByte() <= 0x3E) {
    BasicMemoryInstruction bmi;
    if (decodeBasicMemoryInstruction(&bmi) < 0) {
      return -1;
    }
    i->type = InstructionType::BasicMemory;
    i->basic_memory_instruction = bmi;
  } else if (0x3F <= *fetchByte() && *fetchByte() <= 0x40) {
    MemorySizeInstruction msi;
    if (decodeMemorySizeInstruction(&msi) < 0) {
      return -1;
    }
    i->type = InstructionType::MemorySize;
    i->memory_size_instruction = msi;
  } else if (0x45 <= *fetchByte() && *fetchByte() <= 0xC4) {
    NumericInstruction ni;
    if (decodeNumericInstruction(&ni) < 0) {
      return -1;
    }
    i->type = InstructionType::Numeric;
    i->numeric_instruction = ni;
  } else if (0x41 <= *fetchByte() && *fetchByte() <= 0x44) {
    NumericConstInstruction nci;
    if (decodeNumericConstInstruction(&nci) < 0) {
      return -1;
    }
    i->type = InstructionType::NumericConst;
    i->numeric_const_instruction = nci;
  } else if (0x20 <= *fetchByte() && *fetchByte() <= 0x24) {
    VariableInstruction vi;
    if (decodeVariableInstruction(&vi) < 0) {
      return -1;
    }
    i->type = InstructionType::Variable;
    i->variable_instruction = vi;
  } else if (0x02 <= *fetchByte() && *fetchByte() <= 0x04) {
    BlockInstruction bi;
    if (decodeBlockInstruction(&bi) < 0) {
      return -1;
    }
    i->type = InstructionType::Block;
    i->block_instruction = bi;
  } else if (0x0C <= *fetchByte() && *fetchByte() <= 0x0E) {
    BranchInstruction bi;
    if (decodeBranchInstruction(&bi) < 0) {
      return -1;
    }
    i->type = InstructionType::Branch;
    i->branch_instruction = bi;
  } else if (0x0E == *fetchByte()) {
    TableBranchInstruction tbi;
    if (decodeTableBranchInstruction(&tbi) < 0) {
      return -1;
    }
    i->type = InstructionType::TableBranch;
    i->table_branch_instruction = tbi;
  } else if (0x10 <= *fetchByte() && *fetchByte() <= 0x11) {
    CallInstruction ci;
    if (decodeCallInstruction(&ci) < 0) {
      return -1;
    }
    i->type = InstructionType::Call;
    i->call_instruction = ci;
  } else if (0x1A <= *fetchByte() && *fetchByte() <= 0x1B) {
    ParametricInstruction pi;
    if (decodeParametricInstruction(&pi) < 0) {
      return -1;
    }
    i->type = InstructionType::Parametric;
    i->parametric_instruction = pi;
  } else if (0x0F == *fetchByte()) {
    SingleOperandControlInstruction s;
    s.type = SingleOperandControlInstruction::Type::RETURN;
    i->type = InstructionType::SingleOperandControl;
    i->single_operand_control_instruction = s;
    ++idx_;
  } else if (0x00 == *fetchByte()) {
    SingleOperandControlInstruction s;
    s.type = SingleOperandControlInstruction::Type::UNREACHABLE;
    i->type = InstructionType::SingleOperandControl;
    i->single_operand_control_instruction = s;
    ++idx_;
  } else if (0x01 == *fetchByte()) {
    SingleOperandControlInstruction s;
    s.type = SingleOperandControlInstruction::Type::NOP;
    i->type = InstructionType::SingleOperandControl;
    i->single_operand_control_instruction = s;
    ++idx_;
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
  size_t start_idx = idx_;
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
  return idx_ - start_idx;
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
  std::vector<Instruction> iseq;
  bool is_else = false;
else_block:
  while (true) {
    if (*fetchByte() == 0x0B) {
      if (is_else) {
        bi->else_instructions = iseq;
      } else {
        bi->instructions = iseq;
      }
      break;
    }
    if (*fetchByte() == 0x05) {
      bi->instructions = iseq;
      iseq.clear();
      is_else = true;
      goto else_block;
    }
    Instruction i;
    if (decodeInstruction(&i) < 0) {
      return -1;
    }
    iseq.emplace_back(i);
  }
  ++idx_;
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
