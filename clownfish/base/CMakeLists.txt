aux_source_directory(. base_SRCS)
#set(base_SRCS
#  Timestamp.cc
#  )

add_library(clownfish_base ${base_SRCS})
target_link_libraries(clownfish_base pthread)

install(TARGETS clownfish_base DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/clownfish/base)

if(NOT CMAKE_BUILD_NO_EXAMPLES)
  add_subdirectory(tests)
endif()
