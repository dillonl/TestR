INCLUDE_DIRECTORIES(
		${SCI_BOOST_INCLUDE}
		${SPARSEHASH_INCLUDE_DIR}
		${BAMTOOLS_INCLUDE}
		${CONCURRENTQUEUE_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # clang Doesnt use pthread
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

set(RUFUS_UTIL_SOURCES
  utils/Parameters.cpp
  )

set(RUFUS_ALIGNMENTS_SOURCES
  alignments/BamAlignmentReader.cpp
  )

SET(CORE_LIB rufus_core CACHE STRING "THE CORE LIBRARY") #add rufus_core and rufus_utils to the list of core libs

add_library(${CORE_LIB}
  ${RUFUS_UTIL_SOURCES}
  ${RUFUS_ALIGNMENTS_SOURCES}
  )

add_dependencies(${CORE_LIB} ${BAMTOOLS_PROJECT} ${JELLYFISH_PROJECT} ${BWA_PROJECT} Boost_external ${GTEST_PROJECT} ${SPARSEHASH_PROJECT} ${CONCURRENTQUEUE_PROJECT})
