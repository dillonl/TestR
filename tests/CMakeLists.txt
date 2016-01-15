# =================================
# rufus - test
#
# text/CMakeLists.txt
# =================================

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/config/DataPaths.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/config/DataPaths.h"
)

# set header and source files
set(RUFUS_TEST_SOURCES
  gtest_main.cpp
)

# Where Google Test's .h files can be found.
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${GTEST_INCLUDE}
  ${GTEST_INCLUDE}/include
  ${SCI_BOOST_INCLUDE}
  ${BAMTOOLS_INCLUDE}
  ${SPARSEHASH_INCLUDE_DIR}
  ${CONCURRENTQUEUE_INCLUDE_DIR})

set(Boost_USE_MULTITHREADED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # clang Doesnt use pthread
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()
#set(CMAKE_BUILD_TYPE Debug)


#create binary
add_executable(rufus_tests
  ${RUFUS_TEST_SOURCES}
  )

add_dependencies(rufus_tests ${BAMTOOLS_PROJECT} ${JELLYFISH_PROJECT} ${BWA_PROJECT} Boost_external ${GTEST_PROJECT} ${SPARSEHASH_PROJECT} ${CONCURRENTQUEUE})


target_link_libraries(rufus_tests
  ${CORE_LIB}
  ${BAMTOOLS_LIB}
  ${BAMTOOLS_UTIL_LIB}
  ${BWA_LIB}
  ${ZLIB_LIB}
  
  #${FASTAHACK_LIB}
  ${SCI_BOOST_LIBRARY}
  ${CONCURRENTQUEUE_INCLUDE_DIR}
  ${GTEST_LIB}
  )


