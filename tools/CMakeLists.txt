# =================================
# rufus
#
# tools/CMakeLists.txt
# =================================

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)

# set header and source files
set(RUFUS_TOOLS_SOURCES
	rufus.cpp
)

INCLUDE_DIRECTORIES(
  ${SCI_BOOST_INCLUDE}
  ${BAMTOOLS_INCLUDE}
  ${SPARSEHASH_INCLUDE_DIR}
  ${CONCURRENTQUEUE_INCLUDE_DIR}
  ${MINTOMIC_INCLUDE}
  )


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # clang Doesnt use pthread
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

#set(CMAKE_BUILD_TYPE Debug) # uncomment this when you need a debug build

#create binary
add_executable(rufus
  ${RUFUS_TOOLS_SOURCES}
)

add_dependencies(rufus ${BAMTOOLS_PROJECT} ${JELLYFISH_PROJECT} ${BWA_PROJECT} Boost_external ${SPARSEHASH_PROJECT} ${CONCURRENTQUEUE} ${MINTOMIC_PROJECT})

# link libraries to executables
target_link_libraries(rufus
    ${BAMTOOLS_LIB}
   	${BAMTOOLS_UTIL_LIB}
   	${BWA_LIB}
	${ZLIB_LIB}
	${CORE_LIB}
	#${FASTAHACK_LIB}
	${SCI_BOOST_LIBRARY}
	${CONCURRENTQUEUE_INCLUDE_DIR}
)
