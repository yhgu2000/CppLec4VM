#include "Err.hpp"

namespace Lec4VM {

using namespace std::string_literals;

std::string
err::InvalidInstr::info() const
{
  return "无效的指令码："s + std::to_string(_instr);
}

std::string
err::CodeOutOfBound::info() const
{
  return "代码访问越界："s + std::to_string(_addr) +
         " >= " + std::to_string(_bound);
}

std::string
err::StackOutOfBound::info() const
{
  return "栈访问越界："s + std::to_string(_addr) +
         " >= " + std::to_string(_bound);
}

}
