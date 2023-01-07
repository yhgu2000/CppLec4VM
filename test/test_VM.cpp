#pragma once

#include "Lec4VM/VM.hpp"
#include "project.h"
#include "util.hpp"
#include <fstream>

using namespace Lec4VM;

BOOST_AUTO_TEST_CASE(code1)
{
  const char* kCode1Path = CppLec4VM_SOURCE_DIR "/test/code1";
  std::ifstream fin(kCode1Path, std::ios::binary);
  BOOST_ASSERT(fin);

  auto code = std::make_shared<VM::Code>(VM::Code::parse(fin));
  VM vm;
  vm.set_code(code);
  while (vm.step())
    ;
  BOOST_ASSERT(!vm.stack().empty());
  BOOST_ASSERT(vm.stack().back() == 120);
}
