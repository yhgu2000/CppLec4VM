# test 目标名前统一添加 test_ 前缀

if(NOT BUILD_TESTING)
  return()
endif()

# 本项目使用 Boost 的单元测试框架
find_package(Boost COMPONENTS unit_test_framework)
if(NOT Boost_UNIT_TEST_FRAMEWORK_FOUND)
  message(WARNING "Boost.Test 未找到，单元测试已禁用。")
  return()
endif()

if(NOT Boost_USE_STATIC_LIBS)
  add_compile_definitions(BOOST_TEST_DYN_LINK)
endif()



#
# 测试实用工具
#
add_library(test_util OBJECT util.hpp util.cpp)

target_link_libraries(test_util
  PUBLIC Boost::unit_test_framework Boost::boost
)



#
# 测试 VM.hpp 和 VM.cpp
#
add_executable(test_VM test_VM.cpp)

target_link_libraries(test_VM PRIVATE test_util Lec4VM)

target_compile_definitions(test_VM PRIVATE BOOST_TEST_MODULE=VM)

add_test(NAME test_VM COMMAND test_vm)
