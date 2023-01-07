# app 可执行目标名统一采用小写



#
# 命令行解释器
#
add_executable(lec4vm lec4vm.cpp)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS program_options)

target_link_libraries(lec4vm PUBLIC Lec4VM
  Boost::boost Boost::program_options
)

install(TARGETS lec4vm EXPORT ${EXPORT_TARGETS})
