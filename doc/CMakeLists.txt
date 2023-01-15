# Doxygen
find_package(Doxygen COMPONENTS dot OPTIONAL_COMPONENTS mscgen dia)

if(DOXYGEN_FOUND)
  # 给 doxygen_add_docs 传参
  set(DOXYGEN_IMAGE_PATH .)
  set(DOXYGEN_USE_MATHJAX YES)
  set(DOXYGEN_CREATE_SUBDIRS YES)
  set(DOXYGEN_GENERATE_TREEVIEW YES)

  doxygen_add_docs(doxygen
    # 主页
    README.md

    # 提取代码中的注释
    ../lib
  )

  set_target_properties(doxygen PROPERTIES EXCLUDE_FROM_ALL FALSE)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION doc)
else()
  message(WARNING "Doxygen 未找到，文档生成已禁用。")
endif()
