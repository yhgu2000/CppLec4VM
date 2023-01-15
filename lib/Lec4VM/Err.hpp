#pragma once

#include <exception>
#include <string>

namespace Lec4VM {

#define ERR_WHAT(cls) "class ::Lec4VM::" #cls ";"

class Err : public std::exception
{
public:
  /**
   * @brief 构造并返回人类可读的错误信息字符串
   */
  virtual std::string info() const { return what(); }

  // exception interface
public:
  virtual const char* what() const noexcept override { return ERR_WHAT(Err); }
};

namespace err {

class Str : public Err
{
public:
  std::string _what;

public:
  Str(std::string what)
    : _what(std::move(what))
  {
  }

  // Err interface
public:
  virtual std::string info() const override { return _what; }

  // exception interface
public:
  virtual const char* what() const noexcept override { return _what.c_str(); }
};

class InvalidInstr : public Err
{
public:
  std::uint32_t _instr;

public:
  InvalidInstr(std::uint32_t instr)
    : _instr(instr)
  {
  }

  // exception interface
public:
  virtual const char* what() const noexcept override
  {
    return ERR_WHAT(InvalidInstr);
  }

  // Err interface
public:
  virtual std::string info() const override;
};

class CodeOutOfBound : public Err
{
public:
  std::size_t _addr;
  std::size_t _bound;

public:
  CodeOutOfBound(std::size_t addr, std::size_t bound)
    : _addr(addr)
    , _bound(bound)
  {
  }

  // exception interface
public:
  virtual const char* what() const noexcept override
  {
    return ERR_WHAT(CodeOutOfBound);
  }

  // Err interface
public:
  virtual std::string info() const override;
};

class StackOutOfBound : public Err
{
public:
  std::size_t _addr;
  std::size_t _bound;

public:
  StackOutOfBound(std::size_t addr, std::size_t bound)
    : _addr(addr)
    , _bound(bound)
  {
  }

  // exception interface
public:
  virtual const char* what() const noexcept override
  {
    return ERR_WHAT(StackOutOfBound);
  }

  // Err interface
public:
  virtual std::string info() const override;
};

class CallerUnderflow : public Err
{
  // exception interface
public:
  virtual const char* what() const noexcept override
  {
    return ERR_WHAT(CallerUnderflow);
  }
};

}

}
