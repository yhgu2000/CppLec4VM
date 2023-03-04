# library 目标名统一采用大写



#
# 第四课虚拟机
#
file(GLOB Lec4VM_src Lec4VM/*)

add_library(Lec4VM ${Lec4VM_src})

target_include_directories(Lec4VM
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS Lec4VM EXPORT ${EXPORT_TARGETS})

install(DIRECTORY Lec4VM TYPE INCLUDE PATTERN "*.cpp" EXCLUDE)
