#pragma once

#include "Err.hpp"
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace Lec4VM {

/**
 * @brief 字节码解释器/虚拟机
 */
class VM
{
public:
  enum Instr
  {
    kCst = 0,
    kAdd = 1,
    kMul = 2,
    kVar = 3,
    kPop = 4,
    kSwap = 5,
    kCall = 6,
    kRet = 7,
    kIfZero = 8,
    kGoto = 9,
    kExit = 10,
  };

  class Code : public std::vector<std::int32_t>
  {
    using _S = std::vector<std::int32_t>;

  public:
    /**
     * @brief 从输入流读取代码
     * @param in 输入流
     * @return 代码向量
     */
    static Code parse(std::istream& in);

  public:
    using _S::_S;

  public:
    /**
     * @brief 执行代码检查，如果发现错误则抛出异常，检查内容包括指令的操作数
     * 是否完整、跳转地址是否有效。
     */
    //    void check() noexcept(false);

    /**
     * @brief 打印成人类可读形式，可通过 begin 和 end 只打印代码片段
     * @param out 输出流
     * @param secBegin 片段起始字
     * @param secEnd 片段结束字
     */
    void print(std::ostream& out,
               std::size_t secBegin = 0,
               std::size_t secEnd = SIZE_MAX) const noexcept;
  };

public:
  /**
   * @brief 执行一步
   * @return 停机返回 false，否则返回 true
   */
  bool step() noexcept(false);

  /**
   * @brief 获取当前程序指针
   * @return 返回 SIZE_MAX 表示已停机
   */
  std::size_t get_pc() const noexcept { return _pc - _codeBegin; }

  /**
   * @brief 设置程序指针
   * @param pc 在程序代码中的偏移量
   * @return 成功返回 true，失败返回 false
   */
  bool set_pc(std::size_t pc) noexcept
  {
    return pc >= _code->size() ? false : (_pc = _codeBegin + pc, true);
  }

  /**
   * @brief 获取当前程序代码
   * @return 共享的程序代码
   */
  std::shared_ptr<const Code> get_code() const noexcept { return _code; }

  /**
   * @brief 设置当前程序代码，重置 pc 为 0，若 code 为空，则置 pc 为停机，
   * 同时调用栈将被清空。
   * @param code 共享的程序代码
   */
  void set_code(std::shared_ptr<const Code> code) noexcept;

  /**
   * @brief 操作虚拟机的运行时栈
   * @return 运行时栈的可变引用
   */
  auto& stack() noexcept { return _stack; }

  /**
   * @brief 操作虚拟机的调用栈
   * @return 调用栈的可变引用
   */
  auto& callers() { return _callers; }

  /**
   * @brief 以人类可读的文本形式打印虚拟机的状态以调试
   * @param out 输出流
   */
  void debug(std::ostream& out);

private:
  const std::int32_t* _pc{ nullptr };
  std::vector<const std::int32_t*> _callers;

  std::shared_ptr<const Code> _code;
  const std::int32_t* _codeBegin{ nullptr };
  const std::int32_t* _codeEnd{ nullptr };

  std::vector<std::int32_t> _stack;

private:
  void ensure_pc(const std::int32_t* pc)
  {
    if (pc < _codeBegin || _codeEnd < pc)
      throw err::CodeOutOfBound(pc - _codeBegin, _code->size());
  }

  void ensure_pc_offset(std::ptrdiff_t offset)
  {
    auto pc = _pc + offset;
    if (pc < _codeBegin || _codeEnd < pc)
      throw err::CodeOutOfBound(pc + offset - _codeBegin, _code->size());
  }

  void ensure_stack_size(std::size_t size)
  {
    if (_stack.size() < size)
      throw err::StackOutOfBound(size, _stack.size());
  }

  std::int32_t& stack_top(std::size_t index)
  {
    return _stack[_stack.size() - index]; // 索引从 1 开始
  }

  void stack_pop(std::size_t n) { _stack.resize(_stack.size() - n); }

  void stack_push(std::int32_t val) { _stack.push_back(val); }
};

}
