#include "Lec4VM/VM.hpp"
#include "project.h"
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>

using namespace Lec4VM;
namespace po = boost::program_options;

int
main(int argc, char* argv[])
try {
  po::options_description od("可用选项");
  od.add_options()                                             //
    ("help,h", "打印帮助信息")                                 //
    ("version,v", "打印版本信息")                              //
    ("debug,d", "单步调试模式执行")                            //
    ("print,p", "只打印代码而不执行")                          //
    ("source", po::value<std::string>(), "指定程序源代码文件") //
    ;

  po::positional_options_description pod;
  pod.add("source", 1);

  po::variables_map vmap;
  po::store(
    po::command_line_parser(argc, argv).options(od).positional(pod).run(),
    vmap);
  po::notify(vmap);

  if (vmap.count("help")) {
    std::cout << od << std::endl;
    return 0;
  }

  if (vmap.count("version")) {
    std::cout << __TIME__ " (" __DATE__ ") - CppLec4VM " CppLec4VM_VERSION
              << std::endl;
    return 0;
  }

  std::shared_ptr<VM::Code> code;
  if (vmap.count("source") == 0) {
    code = std::make_shared<VM::Code>(VM::Code::parse(std::cin));
  } else {
    std::string source;
    source = vmap["source"].as<std::string>();
    std::ifstream fin(source, std::ios::binary);
    if (!fin) {
      std::cerr << "无法读取文件 '" << source << "'" << std::endl;
      return 1;
    }
    code = std::make_shared<VM::Code>(VM::Code::parse(fin));
  }

  if (vmap.count("print")) {
    code->print(std::cout);
    return 0;
  }

  VM vm;
  vm.set_code(code);
  if (vmap.count("debug")) {
    do {
      vm.debug(std::cout);
      while (std::cin.get() != '\n')
        ;
    } while (vm.step());
    return 0;
  }

  while (vm.step())
    ;
  vm.debug(std::cout);
}

catch (Lec4VM::Err& e) {
  std::cerr << e.what() << std::endl;
  std::cerr << e.info() << std::endl;
  return -3;
}

catch (std::exception& e) {
  std::cerr << e.what() << std::endl;
  return -2;
}

catch (...) {
  return -1;
}
