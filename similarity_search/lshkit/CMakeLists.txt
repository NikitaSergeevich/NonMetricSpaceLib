include_directories(${PROJECT_SOURCE_DIR}/lshkit/include ${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

set(lshkit_SRCS
  src/mplsh.cpp
  src/mplsh-model.cpp
  src/apost.cpp
  src/char_bit_cnt.cpp
  src/vq.cpp
  src/kdtree.c)

add_library(lshkit ${lshkit_SRCS})
target_link_libraries(lshkit ${CMAKE_REQUIRED_LIBRARIES})

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set (LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/release/")
    set (EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/release/")
else ()
    set (LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/debug/")
    set (EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/debug/")
endif ()

#ADD_SUBDIRECTORY("tools")

