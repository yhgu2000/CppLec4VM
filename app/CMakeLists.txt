# app 可执行目标名统一采用小写



#
# 命令行解释器
#
add_executable(lec4vm_app lec4vm.cpp)

set_target_properties(lec4vm_app PROPERTIES OUTPUT_NAME lec4vm)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS program_options)

target_link_libraries(lec4vm_app PUBLIC Lec4VM
  Boost::boost Boost::program_options
)

install(TARGETS lec4vm_app EXPORT ${EXPORT_TARGETS})
