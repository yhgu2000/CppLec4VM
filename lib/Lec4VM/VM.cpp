#include "VM.hpp"
#include <cassert>
#include <iomanip>
#include <unordered_map>

namespace Lec4VM {

VM::Code
VM::Code::parse(std::istream& in)
{
  Code code;
  Instr instr;
  while (in.read(reinterpret_cast<char*>(&instr), sizeof(instr)))
    code.push_back(instr);
  return code;
}

void
VM::Code::print(std::ostream& out,
                std::size_t secBegin,
                std::size_t secEnd) const noexcept
{
  struct InstrInfo
  {
    const char* name;
    unsigned size;
  };

  static const std::unordered_map<std::int32_t, InstrInfo> kInstrInfos = {
    { Instr::kCst, { "Cst", 2 } },       { Instr::kAdd, { "Add", 1 } },
    { Instr::kMul, { "Mul", 1 } },       { Instr::kVar, { "Var", 2 } },
    { Instr::kPop, { "Pop", 1 } },       { Instr::kSwap, { "Swap", 1 } },
    { Instr::kCall, { "Call", 3 } },     { Instr::kRet, { "Ret", 2 } },
    { Instr::kIfZero, { "IfZero", 2 } }, { Instr::kGoto, { "Goto", 2 } },
    { Instr::kExit, { "Exit", 1 } },
  };

  secBegin = std::min(secBegin, size());
  secEnd = std::min(secEnd, size());
  auto width = std::to_string(secEnd - secBegin).size() + 4;

  auto begin = &front();
  auto end = begin + secEnd;
  auto ptr = begin;

  out << std::left;
  while (ptr < end) {
    auto info = kInstrInfos.find(*ptr);

    if (ptr - begin < secBegin) {
      if (info == kInstrInfos.end())
        ptr += 1;
      else
        ptr += info->second.size;
      continue;
    }

    out << std::setw(width) << ptr - begin;

    if (info == kInstrInfos.end()) {
      out << '!' << *ptr << '\n';
      ptr += 1;
      continue;
    }

    out << std::setw(8) << info->second.name;
    for (unsigned i = 1; i < info->second.size; ++i) {
      out << ' ';
      if (ptr + i >= end) {
        out << '*';
        break;
      }
      out << ptr[i];
    }
    out << '\n';

    ptr += info->second.size;
  }
}

bool
VM::step() noexcept(false)
{
  if (_pc == nullptr)
    return false;
  ensure_pc(_pc);

  /**
   * 这些和 pc 相关的检查其实都可以在加载期一遍完成，然后在运行期就不需要
   * 再检查了，应该可以很大程度上提升性能。
   */

  switch (*_pc) {
    case Instr::kCst: {
      ensure_pc_offset(2);

      stack_push(_pc[1]);
      _pc += 2;
    } break;

    case Instr::kAdd: {
      ensure_stack_size(2);

      stack_top(2) = stack_top(1) + stack_top(2);
      stack_pop(1);
      _pc += 1;
    } break;

    case Instr::kMul: {
      ensure_stack_size(2);

      stack_top(2) = stack_top(1) * stack_top(2);
      stack_pop(1);
      _pc += 1;
    } break;

    case Instr::kVar: {
      ensure_pc_offset(2);
      auto index = _pc[1] + 1;
      ensure_stack_size(index);

      stack_push(stack_top(index));
      _pc += 2;
    } break;

    case Instr::kPop: {
      ensure_stack_size(1);

      stack_pop(1);
      _pc += 1;
    } break;

    case Instr::kSwap: {
      ensure_stack_size(2);

      auto temp = stack_top(1);
      stack_top(1) = stack_top(2);
      stack_top(2) = temp;
      _pc += 1;
    } break;

    case Instr::kCall: {
      ensure_pc_offset(3);
      auto addr = _pc + _pc[1];
      ensure_pc(addr);
      auto argn = _pc[2];
      ensure_stack_size(argn);

      _callers.push_back(_pc + 3); // 把下一条指令的地址压栈
      _pc = addr;
    } break;

    case Instr::kRet: {
      ensure_pc_offset(2);
      auto argn = _pc[1];
      if (_callers.empty())
        throw err::CallerUnderflow();
      ensure_stack_size(argn + 1);

      stack_top(argn + 1) = stack_top(1);
      stack_pop(argn);
      _pc = _callers.back();
      _callers.pop_back();
    } break;

    case Instr::kIfZero: {
      ensure_pc_offset(2);
      auto addr = _pc + _pc[1];
      ensure_pc(addr);
      ensure_stack_size(1);

      if (_stack.back() == 0)
        _pc = addr;
      else
        _pc += 2;
      stack_pop(1);
    } break;

    case Instr::kGoto: {
      ensure_pc_offset(2);
      auto addr = _pc + _pc[1];
      ensure_pc(addr);

      _pc = addr;
    } break;

    case Instr::kExit: {
      _pc = nullptr;
      return false;
    } break;

    default:
      throw err::InvalidInstr(*_pc);
  }

  return true;
}

void
VM::set_code(std::shared_ptr<const Code> code) noexcept
{
  if (code) {
    _pc = _codeBegin = &code->front();
    _codeEnd = &code->back() + 1;
  } else {
    _pc = _codeBegin = _codeEnd = nullptr;
  }
  _code = std::move(code);
  _callers.clear();
}

void
VM::debug(std::ostream& out)
{
  out << "===== STATE ====\n";
  _code->print(out,
               std::max(_pc - _codeBegin - 10, std::ptrdiff_t(0)),
               std::max(_pc - _codeBegin + 10, std::ptrdiff_t(0)));
  out << "\n> " << _pc - _codeBegin << " | ";
  for (auto i = _callers.rbegin(); i != _callers.rend(); ++i)
    out << *i - _codeBegin << " | ";
  out << "\n\n";

  out << "===== STACK ====\n";
  auto width = std::to_string(_stack.size()).size() + 2;
  for (auto i = _stack.size(); --i != SIZE_MAX;)
    std::cout << std::setw(width) << i << "| " << _stack[i] << '\n';
  out << "================\n";
}

}
